/*
 * DeviceThread.c
 *
 *  Created on: 2019年2月30日
 *      Author: zengwangfa
 *      Notes:  设备控制任务
 */

#include "DeviceThread.h"
#include <rtthread.h>
#include <elog.h>
#include "propeller.h"
#include "servo.h"
#include "light.h"
#include "rc_data.h"
#include "Control.h"
#include "PropellerControl.h"
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
			
				Control_Cmd_Get(&ControlCmd); //控制命令获取 所有上位控制命令都来自于此【Important】

				if(UNLOCK == ControlCmd.All_Lock){ //如果解锁
						Convert_RockerValue(&Rocker); //遥控数据 转换 为推进器动力
					
				}
				
				if(FOUR_AXIS == VehicleMode){
						FourtAxis_Control(&Rocker);
				}
				else if(SIX_AXIS == VehicleMode){
						SixAxis_Control(&Rocker);
				}
				else{
						log_e("not yet set vehicle mode !");
				}
				
				//Propeller_Output(); //推进器真实PWM输出		
				rt_thread_mdelay(10);
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











