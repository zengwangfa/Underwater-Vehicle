
#include "sensor.h"
#include <rtthread.h>
#include <elog.h>
#include "drv_MS5837.h"
#include "gyroscope.h"

extern struct rt_event init_event;/* ALL_init 事件控制块 */


void sensor_thread_entry(void* parameter)
{
	
		while(1)
		{
					/* 调度器上锁，上锁后，将不再切换到其他线程，仅响应中断 */
				rt_enter_critical();
				
				JY901_Convert(&JY901); //JY901数据转换
				/* 调度器解锁 */
				rt_exit_critical();
			
				MS583703BA_getTemperature();//获取温度
				rt_thread_mdelay(20);
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
                    8,										 	 //线程优先级【priority】
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



















