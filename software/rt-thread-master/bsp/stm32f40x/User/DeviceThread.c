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
#include "focus.h"
#include "debug.h"


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
				else{
						Propller_Stop();
				}

				if(FOUR_AXIS == VehicleMode && UNLOCK == ControlCmd.All_Lock ){ //安全保护措施
						FourtAxis_Control(&Rocker);
						ROV_Depth_Control(&Rocker);
				}
				else if(SIX_AXIS == VehicleMode && UNLOCK == ControlCmd.All_Lock){
						//SixAxis_Control(&Rocker);
						ROV_Depth_Control(&Rocker);
				}

				Propeller_Output(); //推进器真实PWM输出		
				rt_thread_mdelay(1); //5ms
		}
	
}












/**
  * @brief  servo_thread_entry(舵机控制任务函数)
  * @param  void* parameter
  * @retval None
  * @notice 
  */
void devices_thread_entry(void *parameter)//高电平1.5ms 总周期20ms  占空比7.5% volatil
{



		while(1)
		{
			
				if(WORK == WorkMode){//工作模式
					
						//Extractor_Control(&ControlCmd.Arm); //吸取器控制
						RoboticArm_Control(&ControlCmd.Arm);//机械臂控制
						Search_Light_Control(&ControlCmd.Light);  //探照灯控制
						YunTai_Control(&ControlCmd.Yuntai); //云台控制				
						//Focus_Zoom_Camera_Control(&ControlCmd.Focus);//变焦摄像头控制					
				}
				else if(DEBUG == WorkMode)//调试模式
				{	
						Debug_Mode(get_button_value(&ControlCmd));
				}
				rt_thread_mdelay(20);
		}
	
}


int propeller_thread_init(void)
{
    rt_thread_t propeller_tid;
		/*创建动态线程*/
    propeller_tid = rt_thread_create("propoller",//线程名称
                    propeller_thread_entry,			 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    2048,										 //线程栈大小，单位是字节【byte】
                    8,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (propeller_tid != RT_NULL){
			
				PWM_Init(); //推进器、舵机类PWM初始化
			
				rt_thread_startup(propeller_tid);
		}

		return 0;
}
INIT_APP_EXPORT(propeller_thread_init);

int devices_thread_init(void)
{
    rt_thread_t devices_tid;
		/*创建动态线程*/
    devices_tid = rt_thread_create("devices",//线程名称
                    devices_thread_entry,			 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    1024,										 //线程栈大小，单位是字节【byte】
                    12,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (devices_tid != RT_NULL){
				Light_PWM_Init(); //探照灯PWM初始化
				log_i("Light_init()");

				rt_thread_startup(devices_tid);
				//rt_event_send(&init_event, PWM_EVENT); //发送事件  表示初始化完成
		}

		return 0;
}
INIT_APP_EXPORT(devices_thread_init);






