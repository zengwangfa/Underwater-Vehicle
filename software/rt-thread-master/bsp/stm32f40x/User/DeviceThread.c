/*
 * DeviceThread.c
 *
 *  Created on: 2019年2月30日
 *      Author: zengwangfa
 *      Notes:  设备控制任务
 */

#include "DeviceThread.h"
#include <rtthread.h>


/**
  * @brief  servo_thread_entry(舵机控制任务函数)
  * @param  void* parameter
  * @retval None
  * @notice 
  */
void servo_thread_entry(void *parameter)//高电平1.5ms 总周期20ms  占空比7.5% volatil
{



		while(1)
		{

				rt_thread_mdelay(100);
		}
	
}




/**
  * @brief  propeller_thread_entry(推进器控制任务函数)
  * @param  void* parameter
  * @retval None
  * @notice 
  */
void propeller_thread_entry(void *parameter)
{


		while(1)
		{

				rt_thread_mdelay(100);
		}
	
}






