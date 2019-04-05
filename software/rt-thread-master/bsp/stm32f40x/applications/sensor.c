/*
 * servo.c
 *
 *  Created on: 2019年2月30日
 *      Author: zengwangfa
 *      Notes:  深度传感器设备
 */
#include "sensor.h"
#include <rtthread.h>
#include <elog.h>
#include "drv_MS5837.h"
#include "gyroscope.h"
#include "stdio.h"
#include "drv_i2c.h"
extern struct rt_event init_event;/* ALL_init 事件控制块 */


void sensor_thread_entry(void* parameter)
{
	
		while(1)
		{


				MS583703BA_getTemperature();//获取温度
				MS583703BA_getPressure();   //获取大气压


				rt_thread_mdelay(10);
		}
}





int sensor_thread_init(void)
{
    rt_thread_t sensor_tid;
		/*创建动态线程*/
    sensor_tid = rt_thread_create("sensor",//线程名称
                    sensor_thread_entry,				 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    1024,										 //线程栈大小，单位是字节【byte】
                    20,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (sensor_tid != RT_NULL){


				if(MS5837_Init()){
						rt_event_send(&init_event, MS5837_EVENT);
						log_i("MS5837_Init()");
				}
				else {
						log_e("MS5837_Init_Failed!");
				}
				rt_thread_startup(sensor_tid);
		}
		return 0;
}
INIT_APP_EXPORT(sensor_thread_init);




/* Get Pressure */
void get_pressure(void)
{
		static char str[50] = {0};


		MS583703BA_getTemperature();//获取温度
		MS583703BA_getPressure();   //获取大气压
	
		sprintf(str,"MS_Temp2:%f\n",MS5837_Temp);
		rt_kprintf(str);
		rt_kprintf("MS_Pressure:%d\n",MS5837_Pressure);//MS5837_Pressure
}
MSH_CMD_EXPORT(get_pressure, get pressure[pa]);












