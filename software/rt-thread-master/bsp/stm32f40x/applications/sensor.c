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


void sensor_highSpeed_thread_entry(void* parameter)
{

		rt_thread_mdelay(3000);//等待3s系统稳定再获取数据

		while(1)
		{
				JY901_Convert(&Sensor.JY901); //JY901数据转换
				
				Depth_Sensor_Data_Convert();  //深度数据转换

				Sensor.Depth = (int) ((int)(Sensor.MS5837.PessureValue - Sensor.MS5837.Init_PessureValue));		
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
			
				if(AUV_Mode == VehicleMode){
						spl1301_init();        //AUV使用的传感器
						log_i("SPL1301_init()");
				}
				else{
						if(MS5837_Init()){
								log_i("MS5837_Init()");
						}
						else {
								log_e("MS5837_Init_Failed!");
						}
				}
				
				if(adc_init()){ //ADC电压采集初始化
						log_i("Adc_Init()");
				}
				
				rt_thread_startup(sensor_lowSpeed_tid);
				rt_thread_startup(sensor_highSpeed_tid);
		}
		return 0;
}
INIT_APP_EXPORT(sensor_thread_init);


void Depth_Sensor_Data_Convert(void)//深度传感器数据转换
{
		static uint32 value[10] = {0};
		static uint8 ON_OFF = 0; //自锁开关
		static uint8 i = 0;
		
		if(AUV_Mode == VehicleMode){  //AUV使用歌尔 SPL1301
				spl1301_get_raw_temp();
				spl1301_get_raw_pressure();//传感器数据转换
				
				if(ON_OFF == 0){
						ON_OFF = 1;
						Sensor.MS5837.Init_PessureValue = get_spl1301_pressure();//获取初始化数据
				}
				for(i = 0;i < 10;i++){
						value[i++] = get_spl1301_pressure();//获取1次数据
				}
				Sensor.MS5837.Temperature = get_spl1301_temperature();
				Sensor.MS5837.PessureValue = Bubble_Filter(value);
			
		}
		else{ //否则为ROV使用MS5837
				MS583703BA_getTemperature();//先获取外部温度,是为了给深度传感器进行温度曲线校准
				MS583703BA_getPressure();   //获取水压
				
				if(ON_OFF == 0){
						ON_OFF = 1;
						Sensor.MS5837.Init_PessureValue = get_ms5837_pressure();//获取初始化数据
				}
				for(i = 0;i < 10;i++){
						value[i++] = get_ms5837_pressure();//获取10次
				}
				Sensor.MS5837.Temperature  = get_ms5837_temperature();
				Sensor.MS5837.PessureValue = Bubble_Filter(value);
				
				
		}

		

		
		

}


/* 打印传感器信息 */
void print_sensor_info(void)
{
		log_i("    variable        |  value");
		log_i("--------------------|-----------");
	
		log_i("      Roll          |  %+0.3f",Sensor.JY901.Euler.Roll);
		log_i("      Pitch         |  %+0.3f",Sensor.JY901.Euler.Pitch);
		log_i("      Yaw           |  %+0.3f",Sensor.JY901.Euler.Yaw);
		log_i("--------------------|-----------");
		log_i("      Acc.x         |  %+0.3f",Sensor.JY901.Acc.x);
		log_i("      Acc.y         |  %+0.3f",Sensor.JY901.Acc.y);//
		log_i("      Acc.z         |  %+0.3f",Sensor.JY901.Acc.z);//
		log_i("--------------------|-----------");
		log_i("      Gyro.x        |  %+0.3f",Sensor.JY901.Gyro.x);
		log_i("      Gyro.y        |  %+0.3f",Sensor.JY901.Gyro.y);//	
		log_i("      Gyro.z        |  %+0.3f",Sensor.JY901.Gyro.z);//	
		log_i("  JY901_Temperature |  %+0.3f",Sensor.JY901.Temperature);//					

	
		log_i("--------------------|-----------");
		log_i("     Voltage        |  %0.3f",Sensor.PowerSource.Voltage); //电压
		log_i("     Current        |  %d",Sensor.PowerSource.Current);    //电流
		log_i("--------------------|-----------");
		log_i(" Water Temperature  |  %0.3f",Sensor.MS5837.Temperature);    //水温
		log_i("sensor_Init_Pressure|  %d",Sensor.MS5837.Init_PessureValue); //深度传感器初始压力值	
		log_i("   sensor_Pressure  |  %d",Sensor.MS5837.PessureValue); 		 //深度传感器当前压力值	
		log_i("     Depth          |  %d",Sensor.Depth); 									 //深度值
		log_i("--------------------|-----------");	
		log_i("    CPU.Usages      |  %0.3f",	Sensor.CPU.Temperature); //CPU温度
		log_i("   CPU.Temperature  |  %0.3f",	Sensor.CPU.Usage); 			 //CPU使用率

		

}
MSH_CMD_EXPORT(print_sensor_info, printf gysocope & PowerSource & pressure);












