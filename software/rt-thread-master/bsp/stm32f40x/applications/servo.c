/*
 * servo.c
 *
 *  Created on: 2019年3月30日
 *      Author: zengwangfa
 *      Notes:  舵机设备
 */
#define  LOG_TAG    "servo"
#include <stdio.h>
#include <stdlib.h>
#include <rtthread.h>
#include <elog.h>

#include "sys.h"

#include "DataType.h"
#include "Control.h"
#include "ioDevices.h"
#include "servo.h"
#include "propeller.h"
#include "flash.h"
#include "Return_Data.h"
#include "RC_Data.h"
#include "focus.h"

#define RoboticArm_MedValue  1500
#define YunTai_MedValue  		 2000


ServoType RoboticArm = {
		 .MaxValue = 2000, 		//机械臂 正向最大值
		 .MinValue = 1500,	  //机械臂 反向
		 .MedValue = 1900,
	   .Speed  = 5//机械臂当前值
};  //机械臂
ServoType  YunTai = {
		 .MaxValue = 1700, 		//机械臂 正向最大值
		 .MinValue = 1000,	  //机械臂 反向
		 .MedValue = 1300,
	   .Speed  = 10//云台转动速度
};      //云台

uint16 propeller_power = 1500;

extern float Adjust1,Adjust2 ;
extern struct rt_event init_event;/* ALL_init 事件控制块. */
/*******************************************
* 函 数 名：Servo_Output_Limit
* 功    能：舵机输出限制
* 输入参数：输入值：舵机结构体地址 
* 返 回 值：None
* 注    意：
********************************************/
void Servo_Output_Limit(ServoType *Servo)
{
		Servo->CurrentValue = Servo->CurrentValue  > Servo->MaxValue ? Servo->MaxValue : Servo->CurrentValue ;//正向限幅
		Servo->CurrentValue = Servo->CurrentValue  < Servo->MinValue ? Servo->MinValue : Servo->CurrentValue ;//反向限幅
	
}


/**
  * @brief  RoboticArm_Control(机械臂控制)
  * @param  控制指令 0x00：不动作  0x01：张开  0x02：关闭
  * @retval None
  * @notice 
  */
void RoboticArm_Control(uint8 *action)
{
		switch(*action)
		{
				case 0x01:RoboticArm.CurrentValue += RoboticArm.Speed;
									if(RoboticArm.CurrentValue >= RoboticArm.MaxValue){device_hint_flag |= 0x01;}//机械臂到头标志
									else {device_hint_flag &= 0xFE;}; //清除机械臂到头标志

									break;
				case 0x02:RoboticArm.CurrentValue -= RoboticArm.Speed;
									if(RoboticArm.CurrentValue <= RoboticArm.MinValue){device_hint_flag |= 0x01;}//机械臂到头标志
									else {device_hint_flag &= 0xFE;}; //清除机械臂到头标志

									break;
				default:break;
		}
		Servo_Output_Limit(&RoboticArm);//机械臂舵机限幅
		TIM_SetCompare3(TIM4,RoboticArm.CurrentValue);
		*action = 0x00; //清除控制字
}


/**
  * @brief  YunTai_Control(云台控制)
  * @param  控制指令 0x00：不动作  0x01：向上  0x02：向下
  * @retval None
  * @notice 
  */

void YunTai_Control(uint8 *action)
{		
		static int DirectionMode = 1;
		
		switch(*action)
		{
				case 0x01:DirectionMode++;
						  DirectionMode = DirectionMode<=DirectionMode_MAX?DirectionMode:1;	
						  Buzzer_Set(&Beep,1,1);			
						break;  
						
				case 0x02:DirectionProportion(DirectionMode);
						break;  

				case 0x03:DirectionMode = 0;break;   //归中
				default: break;
		}
		Servo_Output_Limit(&YunTai);
		TIM_SetCompare4(TIM4,YunTai.CurrentValue); 
		*action = 0x00; //清除控制字
}

void DirectionProportion(int Mode)
{
	switch(Mode)
	{
		case DirectionUp   :Direction.UP_P1 = Adjust1;
							Direction.UP_P2 = Adjust2;	
							break;
		case DirectionDown :Direction.DOWN_P1 = Adjust1;
						    Direction.DOWN_P2 = Adjust2;
							break;
		case DirectionLeft :Direction.LEFT_P = Adjust1;
		case DirectionRight:Direction.RIGHT_P = Adjust1;
		default: break;
	}
}

//void YunTai_Control(uint8 *action)
//{
//		switch(*action)
//		{
//				case 0x01:YunTai.CurrentValue += YunTai.Speed;  //向上
//						if(YunTai.CurrentValue <= YunTai.MaxValue){device_hint_flag |= 0x02;}//云台到头标志
//						else {device_hint_flag &= 0xFD;}; //清除云台到头标志

//						break;  
//						
//				case 0x02:YunTai.CurrentValue -= YunTai.Speed; //向下
//						if(YunTai.CurrentValue >= YunTai.MinValue){device_hint_flag |= 0x02;}//云台到头标志
//						else {device_hint_flag &= 0xFD;}; //清除云台到头标志

//						break;  

//				case 0x03:YunTai.CurrentValue = YunTai.MedValue;break;   //归中
//				default: break;
//		}
//		Servo_Output_Limit(&YunTai);
//		TIM_SetCompare4(TIM4,YunTai.CurrentValue); 
//		*action = 0x00; //清除控制字
//}



/**
  * @brief  servo_thread_entry(舵机初始化任务函数)
  * @param  void* parameter
  * @retval None
  * @notice 
  */
void servo_thread_entry(void *parameter)//高电平1.5ms 总周期20ms  占空比7.5% volatil
{
		TIM1_PWM_Init(20000-1,168-1);	//168M/168=1Mhz的计数频率,重装载值(即PWM精度)20000，所以PWM频率为 1M/20000=50Hz.  【现在为500Hz】
		TIM4_PWM_Init(20000-1,84-1);	//84M/84=1Mhz的计数频率,重装载值(即PWM精度)20000，所以PWM频率为 1M/20000=50Hz.  
		TIM_Cmd(TIM1, ENABLE);  //使能TIM1
		TIM_Cmd(TIM4, ENABLE);  //使能TIM4
	
		Propeller_Init();       //推进器初始化
	
		rt_thread_mdelay(100);
	
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

				log_i("Servo_init()");
			
				rt_thread_startup(servo_tid);
				//rt_event_send(&init_event, PWM_EVENT); //发送事件  表示初始化完成
		}

		return 0;
}
INIT_APP_EXPORT(servo_thread_init);











/*【机械臂】舵机 修改 速度值 */
static int robotic_arm_speed_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: RoboticArm_Speed 10");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 255 && atoi(argv[1]) > 0){
				RoboticArm.Speed = atoi(argv[1]);
				Flash_Update();
				log_i("Write_Successed! RoboticArm.Speed:  %d",RoboticArm.Speed);
		}
		
		else {
				log_e("Error! The value is out of range!");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(robotic_arm_speed_set,ag: robotic_arm_speed_set 10);


/*【机械臂】舵机 修改 【正向最大值】MSH方法 */
static int robotic_arm_maxValue_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: RoboticArm_Maxvalue_set 1600");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 5000 && atoi(argv[1]) >= 1500){
				RoboticArm.MaxValue = atoi(argv[1]);
				Flash_Update();
				log_i("Write_Successed!  RoboticArm.MaxValue:  %d",RoboticArm.MaxValue);
		}
		
		else {
				log_e("Error! The value is out of range!");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(robotic_arm_maxValue_set,ag: robotic_arm_openvalue_set 2000);




/*【机械臂】舵机 修改 【反向最大值】 MSH方法 */
static int robotic_arm_minValue_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: RoboticArm_minvalue_set 1150");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 3000 &&  atoi(argv[1]) >= 500){
				RoboticArm.MinValue = atoi(argv[1]);
				Flash_Update();
				log_i("Write_Successed!  RoboticArm.MinValue:  %d",RoboticArm.MinValue);
		}
		else {
				log_e("Error! The value is out of range!");
		}

		
		
_exit:
    return result;
}
MSH_CMD_EXPORT(robotic_arm_minValue_set,ag: robotic_arm_closevalue_set 1500);


/*【机械臂】舵机 修改 速度值 */
static int yuntai_speed_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: yuntai_speed_set 5");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 255 && atoi(argv[1]) > 0){
				YunTai.Speed = atoi(argv[1]);
				Flash_Update();
				log_i("Write_Successed! YunTai.Speed:  %d",YunTai.Speed);
		}
		else {
				log_e("Error! The value is out of range!");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(yuntai_speed_set,ag: yuntai_speed_set 5);

/*【云台】舵机 修改 【正向最大值】MSH方法 */
static int yuntai_maxValue_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: YunTai_maxvalue_set 1600");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 5000){
				YunTai.MaxValue = atoi(argv[1]);
				Flash_Update();
				log_i("Write_Successed! YunTai.MaxValue:  %d",YunTai.MaxValue);
		}
		
		else {
				log_e("Error! The value is out of range!");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(yuntai_maxValue_set,ag: yuntai_maxvalue_set 2500);




/*【云台】舵机 修改 【反向最大值】 MSH方法 */
static int yuntai_minValue_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: YunTai_minvalue_set 1150");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 3000){
				YunTai.MinValue = atoi(argv[1]);
				Flash_Update();
				log_i("Write_Successed! YunTai.MinValue:  %d",YunTai.MinValue);
		}
		else {
				log_e("Error! The value is out of range!");
		}

_exit:
    return result;
}
MSH_CMD_EXPORT(yuntai_minValue_set,ag: yuntai_arm_closevalue_set 1500);

/*【云台】舵机 修改 【反向最大值】 MSH方法 */
static int yuntai_medValue_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: YunTai_medvalue_set 2000");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 3000){
				YunTai.MedValue = atoi(argv[1]);
				Flash_Update();
				log_i("Write_Successed! YunTai.MedValue):  %d",YunTai.MedValue);
		}
		else {
				log_e("Error! The value is out of range!");
		}

_exit:
    return result;
}
MSH_CMD_EXPORT(yuntai_medValue_set,ag: yuntai_arm_medvalue_set 2000);



/*【云台】舵机 修改 【当前】 MSH方法 */
static int yuntai_currentValue_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: YunTai_medvalue_set 2000");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 3000 && atoi(argv[1]) >= 500){
				YunTai.CurrentValue = atoi(argv[1]);
				log_i("Write_Successed! Current YunTai.CurrentValue:  %d",YunTai.CurrentValue);
		}
		else {
				log_e("Error! The value is out of range!");
		}

_exit:
    return result;
}
MSH_CMD_EXPORT(yuntai_currentValue_set,ag: yuntai_currentValue_set 1500);


/*【云台】舵机 修改 【当前】 MSH方法 */
static int yuntai(int argc, char **argv)
{
    int result = 0;
		ServoType servo;
    if (argc > 1){
        log_e("Error! Proper Usage: YunTai_medvalue_set 2000");
				result = -RT_ERROR;
        goto _exit;
    }
		rt_kprintf("Set Min Value:");
		scanf("%d",(int *)(&servo.MinValue));

		if(servo.MinValue <= 7000 ){		
				log_i("Write_Successed! servo.MinValue  %d",servo.MinValue);
		}
		else {
				log_e("Error! The value is out of range!");
		}

_exit:
    return result;
}
MSH_CMD_EXPORT(yuntai,ag: yuntai_currentValue_set 1500);





