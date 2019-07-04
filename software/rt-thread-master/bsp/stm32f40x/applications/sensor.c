/*
 * servo.c
 *
 *  Created on: 2019年2月30日
 *      Author: zengwangfa
 *      Notes:  传感器设备【深度传感器、CPU参数、ADC电压】
 */
 
 #define LOG_TAG    "sensor"
#include "sensor.h"
#include <rtthread.h>
#include <elog.h>
#include "drv_MS5837.h"
#include "gyroscope.h"
#include "stdio.h"
#include "drv_i2c.h"
#include "drv_adc.h"
#include "drv_cpu_temp.h"
#include "drv_cpuusage.h"
#include "filter.h"
#include "drv_spl1301.h"

//#define Using_SPL1301 

extern struct rt_event init_event; /* ALL_init 事件控制块 */

Sensor_Type Sensor;//传感器参数
/**
  * @brief  sensor_lowSpeed_thread_entry(低速获取传感器任务函数)
  * @param  void* parameter
  * @retval None
  * @notice 
  */
void sensor_lowSpeed_thread_entry(void* parameter)
{
		uint8 cpu_usage_major, cpu_usage_minor; //整数位、小数位
		rt_thread_mdelay(1500);//等待1.5s系统稳定再获取数据

		while(1)
		{
			
				Sensor.CPU.Temperature = get_cpu_temp(); //获取CPU温度
				Sensor.PowerSource.Voltage = get_voltage_value();//获取电源电压值
			
				cpu_usage_get(&cpu_usage_major, &cpu_usage_minor); //获取CPU使用率
				Sensor.CPU.Usage = cpu_usage_major + (float)cpu_usage_minor/100;
			

				rt_thread_mdelay(1000);
		}
}

/**
  * @brief  sensor_highSpeed_thread_entry(高速获取传感器任务函数)
  * @param  void* parameter
  * @retval None
  * @notice 
  */
float spl_pressure = 0.0f;
float spl_init_pressure = 0.0f;
float spl_res_pressure = 0.0f;
float spl_init_temp = 0.0f;
float spl_temp = 0.0f;

void sensor_highSpeed_thread_entry(void* parameter)
{
		static uint8 ON_OFF = 0; //自锁开关
		rt_thread_mdelay(1500);//等待1.5s系统稳定再获取数据

		while(1)
		{
				JY901_Convert(&Sensor.JY901); //JY901数据转换
#ifdef Using_SPL1301		
			
				spl1301_get_raw_temp();
				spl1301_get_raw_pressure();
				if(0 == ON_OFF){
						Sensor.MS5837.Init_PessureValue = spl1301_get_pressure(); //获取压力初值 
						ON_OFF = 1; //自锁
				}
				Sensor.MS5837.PessureValue = spl1301_get_pressure();
				Sensor.MS5837.Temperature = spl1301_get_temperature();
#else
				MS5837_Convert();   //MS5837设备数据转换
				if(0 == ON_OFF){
						Sensor.MS5837.Init_PessureValue = get_ms5837_init_pressure(); //获取压力初值 
						ON_OFF = 1; //自锁
				}

#endif
			
				Sensor.Depth = (int)((int)(Sensor.MS5837.PessureValue - Sensor.MS5837.Init_PessureValue)/10);		
				rt_thread_mdelay(20);
		}
}



/*******************************线程初始化*******************************************/
int sensor_thread_init(void)
{
    rt_thread_t sensor_lowSpeed_tid; //低速获取的传感器放入接口
	  rt_thread_t sensor_highSpeed_tid;//高速获取的传感器放入接口
		/*创建动态线程*/
    sensor_lowSpeed_tid = rt_thread_create("sensor",  //线程名称
                    sensor_lowSpeed_thread_entry,		 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    1024,										 //线程栈大小，单位是字节【byte】
                    30,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

	  sensor_highSpeed_tid = rt_thread_create("sensor",  //线程名称
                    sensor_highSpeed_thread_entry,		 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    1024,										 //线程栈大小，单位是字节【byte】
                    15,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (sensor_lowSpeed_tid != RT_NULL && sensor_highSpeed_tid != RT_NULL){
			
#ifdef Using_SPL1301
				spl1301_init();
#else
			  if(MS5837_Init()){
						log_i("MS5837_Init()");

				}
				else {
						log_e("MS5837_Init_Failed!");
				}
				
#endif
				if(adc_init()){
						log_i("Adc_Init()");
				}
				
				rt_thread_startup(sensor_lowSpeed_tid);
				rt_thread_startup(sensor_highSpeed_tid);
		}
		return 0;
}
INIT_APP_EXPORT(sensor_thread_init);


void MS5837_Convert(void)//MS5837数据转换
{
		static uint32 res_value[10] = {0};
		static uint8 i = 0;
		MS583703BA_getTemperature();//获取外部温度
		MS583703BA_getPressure();   //获取水压

		if(i >= 9){i = 0;}
		res_value[i++] = get_ms5837_pressure();
		
		Sensor.MS5837.Temperature  = get_ms5837_temperature();
		Sensor.MS5837.PessureValue = Bubble_Filter(res_value);
}



/* 打印传感器信息 */
void print_sensor_info(void)
{
		log_i("    variable        |  value");
		log_i("--------------------|-----------");
	
		print_JY901_info();  //打印角度
				


		log_i("--------------------|-----------");
		log_i("     Voltage        |   %0.3f",Sensor.PowerSource.Voltage); //电压
		log_i("     Current        |   %d",Sensor.PowerSource.Current); //电流
		log_i("--------------------|-----------");
		log_i(" Water Temperature  |   %0.3f",Sensor.MS5837.Temperature);  //水温
		log_i("sensor_Init_Pressure|   %d",Sensor.MS5837.Init_PessureValue); //深度传感器初始压力值	
		log_i("   sensor_Pressure  |   %d",Sensor.MS5837.PessureValue); //深度传感器当前压力值	
		log_i("     Depth          |   %d",Sensor.Depth); //深度值
	

		

}
MSH_CMD_EXPORT(print_sensor_info, printf gysocope & PowerSource & pressure);












