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
#include "sys.h"


int propeller_thread_init(void)
{
    rt_thread_t propeller_tid;
		/*创建动态线程*/
    propeller_tid = rt_thread_create("propoller",//线程名称
                    propeller_thread_entry,			 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    2048,										 //线程栈大小，单位是字节【byte】
                    10,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (propeller_tid != RT_NULL){

				TIM1_PWM_Init(20000-1,168-1);	//168M/168=1Mhz的计数频率,重装载值(即PWM精度)20000，所以PWM频率为 1M/20000=50Hz.  【现在为500Hz】
				TIM3_PWM_Init(20000-1,84-1);  //吸取器
				TIM_Cmd(TIM1, ENABLE);  //使能TIM1
				TIM_Cmd(TIM4, ENABLE);  //使能TIM4
	
				rt_thread_startup(propeller_tid);
		}

		return 0;
}
INIT_APP_EXPORT(propeller_thread_init);
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
				
				Propeller_Output(); //推进器真实PWM输出		
				rt_thread_mdelay(10);
		}
	
}












int servo_thread_init(void)
{
    rt_thread_t servo_tid;
		/*创建动态线程*/
    servo_tid = rt_thread_create("servo",//线程名称
                    servo_thread_entry,			 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    1024,										 //线程栈大小，单位是字节【byte】
                    15,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (servo_tid != RT_NULL){

				TIM4_PWM_Init(20000-1,84-1);	//84M/84=1Mhz的计数频率,重装载值(即PWM精度)20000，所以PWM频率为 1M/20000=50Hz.  

				log_i("Servo_init()");
				rt_thread_startup(servo_tid);
				//rt_event_send(&init_event, PWM_EVENT); //发送事件  表示初始化完成
		}

		return 0;
}
INIT_APP_EXPORT(servo_thread_init);
/**
  * @brief  servo_thread_entry(舵机控制任务函数)
  * @param  void* parameter
  * @retval None
  * @notice 
  */
void servo_thread_entry(void *parameter)//高电平1.5ms 总周期20ms  占空比7.5% volatil
{


		//rt_thread_mdelay(5000);//等待外部设备初始化成功

		while(1)
		{
				//Light_Control(&ControlCmd.Light);  //探照灯控制
				YunTai_Control(&ControlCmd.Yuntai); //云台控制
				RoboticArm_Control(&ControlCmd.Arm);//机械臂控制
				
				rt_thread_mdelay(20);
		}
	
}











