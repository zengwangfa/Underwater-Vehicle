/*
 * @Description: 开启各个传感器线程函数、打印传感器信息
 * @Author: chenxi
 * @Date: 2020-02-09 12:30:19
 * @LastEditTime : 2020-02-10 18:35:47
 * @LastEditors  : chenxi
 */

#define LOG_TAG "sensor"

#include "../easylogger/inc/elog.h"

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "sensor.h"
#include "gyroscope.h"
#include "../drivers/drv_cpu_status.h"
#include "../drivers/drv_spl1301.h"
#include "../drivers/drv_adc.h"
#include "../Tools/filter.h"

#include <wiringPi.h>
#include <wiringSerial.h>

/*----------------------- Variable Declarations -----------------------------*/
char *Depth_Sensor_Name[3] = {"MS5837", "SPL1301", "null"};

// extern struct rt_event init_event; /* ALL_init 事件控制块 */

Sensor_Type Sensor; //传感器参数
float temp_current = 0.0f;

/*----------------------- Function Implement --------------------------------*/
/**
  * @brief  sensor_lowSpeed_callback_fun(低速获取传感器任务函数)
  * @param  void* arg
  * @retval None
  * @notice 
  */
void *sensor_lowSpeed_callback_fun(void *arg)
{
    // TODO

    adc_init(); //ADC电压采集初始化
    while (1)
    {
        // 不必再延时，get_voltage_value() 内已有延时
        Sensor.CPU.Temperature = get_cpu_temp();          //获取CPU温度
        Sensor.PowerSource.Voltage = get_voltage_value(); //获取电源电压值

        // if (Sensor.PowerSource.Voltage > 6.0f) //当未接入电源时，不检测电流值
        // {

        Sensor.PowerSource.Current = get_current_value();
        temp_current = Sensor.PowerSource.Current;
        Sensor.PowerSource.Current = KalmanFilter(&Sensor.PowerSource.Current);
        // 电流值 进行卡尔曼滤波【该卡尔曼滤波调节r的值，滞后性相对较大】

        // }
    }
    return NULL;
}

//深度传感器数据转换
void Depth_Sensor_Data_Convert(void)
{
    static uint32 value[10] = {0};
    static uint8 ON_OFF = 0; //自锁开关
    static uint8 i = 0;

    if (SPL1301 == Sensor.DepthSensor.Type) //歌尔 SPL1301
    {
        spl1301_get_raw_temp();
        spl1301_get_raw_pressure(); //传感器数据转换

        if (ON_OFF == 0)
        {
            ON_OFF = 1;
            Sensor.DepthSensor.Init_PessureValue = get_spl1301_pressure(); //获取初始化数据
        }
        for (i = 0; i < 10; i++)
        {
            value[i++] = get_spl1301_pressure(); //获取1次数据
        }
        Sensor.DepthSensor.Temperature = get_spl1301_temperature();
        Sensor.DepthSensor.PessureValue = Bubble_Filter(value);
        /* 深度数值 单位为cm   定标系数为 1.3 单位/cm */
        Sensor.DepthSensor.Depth = ((Sensor.DepthSensor.PessureValue - Sensor.DepthSensor.Init_PessureValue) / 20);
    }
    else if (MS5837 == Sensor.DepthSensor.Type) //使用MS5837
    {
    }
}

void *DepthSensor_callback_fun(void *arg)
{
    if (MS5837 == Sensor.DepthSensor.Type) // 深度传感器类型判定
    {
    }
    else if (SPL1301 == Sensor.DepthSensor.Type)
    {
        spl1301_init();
        while (1)
        {
            Depth_Sensor_Data_Convert(); // 深度数据转换
            delay(20);
        }
    }
    else if (DS_NULL == Sensor.DepthSensor.Type)
    {
        log_e("not set Depth Senor");
    }

    return NULL;
}

void *JY901_callback_fun(void *arg)
{
    int fd = JY901_Init();
    while (1)
    {
        delay(20);
        while (serialDataAvail(fd))
        {
            CopeSerial2Data((uint8)serialGetchar(fd));
        }
    }
    return NULL;
}

void *cpu_usage_callback_fun(void *arg)
{
    Sensor.CPU.Usage = get_cpu_usage();
    return NULL;
}

int sensor_thread_init(void)
{
    memset(&Sensor, 0, sizeof(Sensor_Type));
    Sensor.DepthSensor.Type = SPL1301;

    pthread_t cpu_usage_tid;
    if (pthread_create(&cpu_usage_tid, NULL, cpu_usage_callback_fun, NULL) == -1)
    {
        log_e("cpu_usage_thread create error!");
        return 1;
    }
    if (pthread_detach(cpu_usage_tid))
    {
        log_w("cpu_usage_thread detach failed...");
        return -2;
    }

    pthread_t JY901_tid;
    if (pthread_create(&JY901_tid, NULL, JY901_callback_fun, NULL) == -1)
    {
        log_e("JY901_thread create error!");
        return 1;
    }
    if (pthread_detach(JY901_tid))
    {
        log_w("JY901_thread detach failed...");
        return -2;
    }

    pthread_t sensor_lowSpeed_tid;
    if (pthread_create(&sensor_lowSpeed_tid, NULL, sensor_lowSpeed_callback_fun, NULL) == -1)
    {
        log_e("sensor_lowSpeed_thread create error!");
        return 1;
    }
    if (pthread_detach(sensor_lowSpeed_tid))
    {
        log_w("sensor_lowSpeed_thread detach failed...");
        return -2;
    }

    pthread_t DepthSensor_tid;
    if (pthread_create(&DepthSensor_tid, NULL, DepthSensor_callback_fun, NULL) == -1)
    {
        log_e("DepthSensor_thread create error!");
        return 1;
    }
    if (pthread_detach(DepthSensor_tid))
    {
        log_w("DepthSensor_thread detach failed...");
        return -2;
    }

    return 0;
}

// 打印传感器信息
void print_sensor_info(void)
{
    log_i("      variable      |   value");
    log_i("--------------------|------------");

    log_i("        Roll        |  %+0.3f", Sensor.JY901.Euler.Roll);
    log_i("        Pitch       |  %+0.3f", Sensor.JY901.Euler.Pitch);
    log_i("        Yaw         |  %+0.3f", Sensor.JY901.Euler.Yaw);
    log_i("--------------------|------------");
    log_i("        Acc.x       |  %+0.3f", Sensor.JY901.Acc.x);
    log_i("        Acc.y       |  %+0.3f", Sensor.JY901.Acc.y);
    log_i("        Acc.z       |  %+0.3f", Sensor.JY901.Acc.z);
    log_i("--------------------|------------");
    log_i("       Gyro.x       |  %+0.3f", Sensor.JY901.Gyro.x);
    log_i("       Gyro.y       |  %+0.3f", Sensor.JY901.Gyro.y);
    log_i("       Gyro.z       |  %+0.3f", Sensor.JY901.Gyro.z);
    log_i("  JY901_Temperature |  %+0.3f", Sensor.JY901.Temperature);
    log_i("--------------------|------------");
    log_i("       Voltage      |  %0.3f", Sensor.PowerSource.Voltage); // 电压
    log_i("       Current      |  %0.3f", Sensor.PowerSource.Current); // 电流
    log_i("--------------------|------------");
    log_i(" Depth Sensor Type  |  %s", Depth_Sensor_Name[Sensor.DepthSensor.Type]); // 深度传感器类型
    log_i(" Water Temperature  |  %0.3f", Sensor.DepthSensor.Temperature);          // 水温
    log_i("sensor_Init_Pressure|  %0.3f", Sensor.DepthSensor.Init_PessureValue);    // 深度传感器初始压力值
    log_i("   sensor_Pressure  |  %0.3f", Sensor.DepthSensor.PessureValue);         // 深度传感器当前压力值
    log_i("        Depth       |  %0.3f", Sensor.DepthSensor.Depth);                // 深度值
    log_i("--------------------|------------");
    log_i("   CPU.Temperature  |  %0.3f", Sensor.CPU.Temperature); // CPU温度
    log_i("      CPU.Usages    |  %0.3f", Sensor.CPU.Usage);       // CPU使用率
}
