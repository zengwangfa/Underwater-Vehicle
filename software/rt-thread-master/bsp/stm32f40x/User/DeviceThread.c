/*
 * DeviceThread.c
 *
 *  Created on: 2019年2月30日
 *      Author: zengwangfa
 *      Notes:  设备控制任务
 */

#include "DeviceThread.h"
#include <rtthread.h>

#include "propeller.h"
#include "servo.h"
#include "light.h"
#include "rc_data.h"

/**
  * @brief  propeller_thread_entry(推进器控制任务函数)
  * @param  void* parameter
  * @retval None
  * @notice 
  */
void propeller_thread_entry(void *parameter)
{

		Propeller_Init();       //推进器初始化
		while(1)
		{

				rt_thread_mdelay(100);
		}
	
}

/**
  * @brief  servo_thread_entry(舵机控制任务函数)
  * @param  void* parameter
  * @retval None
  * @notice 
  */
void servo_thread_entry(void *parameter)//高电平1.5ms 总周期20ms  占空比7.5% volatil
{


		rt_thread_mdelay(5000);//等待外部设备初始化成功
		
		while(1)
		{
				//Light_Control(&ControlCmd.Light);  //探照灯控制
				YunTai_Control(&ControlCmd.Yuntai); //云台控制
				RoboticArm_Control(&ControlCmd.Arm);//机械臂控制	
			
				rt_thread_mdelay(20);
		}
	
}











