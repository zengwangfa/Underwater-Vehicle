/*
 * servo.c
 *
 *  Created on: 2019年3月30日
 *      Author: zengwangfa
 *      Notes:  舵机设备
 */
#include "servo.h"
#include "propeller.h"
#include "flash.h"
#include <rtthread.h>
#include <elog.h>
#include <stdlib.h>
#include "sys.h"
#include "Return_Data.h"

#define RoboticArm_MedValue  1500
#define YunTai_MedValue  		 2000

uint8 RoboticArm_Speed = 5;
uint8 Yuntai_Speed = 5;

ServoType RoboticArm;  //机械臂
ServoType YunTai;      //云台

//YunTai.OpenValue = 1500;   //向上
//YunTai.CloseValue = 2500;  //向下
//YunTai.CurrentValue = 2000;

uint16 propeller_power = 1500;

extern struct rt_event init_event;/* ALL_init 事件控制块. */
/*******************************************
* 函 数 名：Servo_Output_Limit
* 功    能：舵机输出限制
* 输入参数：输入值：Value   最大限幅：max  反向最大限幅:min
* 返 回 值：限幅后的 Value
* 注    意：
********************************************/
uint16 Servo_Output_Limit(uint16* Value,uint16 max,uint16 min)
{
		*Value = *Value > max ? max: *Value;//正向限幅
		*Value = *Value < min ? min: *Value;//反向限幅
	
		return *Value;
}



void RoboticArm_Control(uint8 *action)
{
		switch(*action)
		{
				case 0x01:RoboticArm.CurrentValue += RoboticArm.Speed;
									if(RoboticArm.CurrentValue >= RoboticArm.MaxValue){device_hint |= 0x01;}//机械臂到头标志
									else {device_hint &= 0xFE;}; //清除机械臂到头标志
									RoboticArm.CurrentValue = Servo_Output_Limit(&RoboticArm.CurrentValue,RoboticArm.MaxValue,RoboticArm.MinValue);
									break;
				case 0x02:RoboticArm.CurrentValue -= RoboticArm.Speed;
									if(RoboticArm.CurrentValue <= RoboticArm.MinValue){device_hint |= 0x01;}//机械臂到头标志
									else {device_hint &= 0xFE;}; //清除机械臂到头标志
									RoboticArm.CurrentValue = Servo_Output_Limit(&RoboticArm.CurrentValue,RoboticArm.MaxValue,RoboticArm.MinValue);
									break;
				default:break;
		}
		TIM_SetCompare3(TIM4,RoboticArm.CurrentValue);
		*action = 0x00; //清除控制字
}

void YunTai_Control(uint8 *action)
{
		switch(*action)
		{
				case 0x01:YunTai.CurrentValue -= YunTai.Speed;  //向上
						if(YunTai.CurrentValue <= YunTai.MinValue){device_hint |= 0x02;}//云台到头标志
						else {device_hint &= 0xFD;}; //清除云台到头标志
						YunTai.CurrentValue = Servo_Output_Limit(&YunTai.CurrentValue,YunTai.MaxValue,YunTai.MinValue);
						break;  
						
				case 0x02:YunTai.CurrentValue += YunTai.Speed; //向下
						if(YunTai.CurrentValue >= YunTai.MaxValue){device_hint |= 0x02;}//云台到头标志
						else {device_hint &= 0xFD;}; //清除云台到头标志
						YunTai.CurrentValue = Servo_Output_Limit(&YunTai.CurrentValue,YunTai.MaxValue,YunTai.MinValue);
						break;  

				case 0x03:YunTai.CurrentValue = YunTai_MedValue;break;   //归中
				default: break;
		}
		TIM_SetCompare4(TIM4,YunTai.CurrentValue); 
		*action = 0x00; //清除控制字
}
/**
  * @brief  servo_thread_entry(舵机任务函数)
  * @param  void* parameter
  * @retval None
  * @notice 
  */
void servo_thread_entry(void *parameter)
{
		TIM_Cmd(TIM1, ENABLE);  //使能TIM1
		TIM_Cmd(TIM4, ENABLE);  //使能TIM4
	
		Propeller_Init();

		while(1)
		{
				
				TIM_SetCompare1(TIM1,propeller_power);  		//高电平1.5ms 总周期20ms  占空比7.5%
				TIM_SetCompare2(TIM1,propeller_power);  		
				TIM_SetCompare3(TIM1,propeller_power); 		
				TIM_SetCompare4(TIM1,propeller_power);  
			
				TIM_SetCompare1(TIM4,propeller_power);  //垂推
				TIM_SetCompare2(TIM4,propeller_power);  //垂推

			
				rt_thread_mdelay(10);

		}
	
}


int servo_thread_init(void)
{
    rt_thread_t servo_tid;
		/*创建动态线程*/
    servo_tid = rt_thread_create("pwm",//线程名称
                    servo_thread_entry,			 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    512,										 //线程栈大小，单位是字节【byte】
                    10,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (servo_tid != RT_NULL){
				TIM1_PWM_Init(20000-1,168-1);	//168M/168=1Mhz的计数频率,重装载值(即PWM精度)20000，所以PWM频率为 1M/20000=50Hz.  
				TIM4_PWM_Init(20000-1,84-1);	//84M/84=1Mhz的计数频率,重装载值(即PWM精度)20000，所以PWM频率为 1M/20000=50Hz.  
				log_i("Servo_init()");
			
				rt_thread_startup(servo_tid);
				//rt_event_send(&init_event, PWM_EVENT); //发送事件  表示初始化完成
		}

		return 0;
}
INIT_APP_EXPORT(servo_thread_init);








/*【机械臂】舵机 修改 【正向最大值】MSH方法 */
static int propeller_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: RoboticArm_openvalue_set 1600");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 2000){
				propeller_power = atoi(argv[1]);
				log_d("Current propeller_set:  %d",propeller_power);
		}
		
		else {
				log_e("Error! The value is out of range!");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(propeller_set,ag: propeller set 2000);




/*【机械臂】舵机 修改 速度值 */
static int robotic_arm_speed_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: RoboticArm_Speed 10");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 255){
				RoboticArm.Speed = atoi(argv[1]);
				Flash_Update();
				log_d("Write_Successed! Current RoboticArm_Speed:  %d",RoboticArm.Speed);
		}
		
		else {
				log_e("Error! The value is out of range!");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(robotic_arm_speed_set,ag: robotic_arm_speed_set 10);


/*【机械臂】舵机 修改 【正向最大值】MSH方法 */
static int robotic_arm_maxvalue_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: RoboticArm_Maxvalue_set 1600");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 5000){
				RoboticArm.MaxValue = atoi(argv[1]);
				Flash_Update();
				log_d("Write_Successed! Current RoboticArm_MaxValue:  %d",RoboticArm.MaxValue);
		}
		
		else {
				log_e("Error! The value is out of range!");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(robotic_arm_maxvalue_set,ag: robotic_arm_openvalue_set 2000);




/*【机械臂】舵机 修改 【反向最大值】 MSH方法 */
static int robotic_arm_minvalue_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: RoboticArm_minvalue_set 1150");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 3000){
				RoboticArm.MinValue = atoi(argv[1]);
				Flash_Update();
				log_d("Write_Successed! Current RoboticArm_minValue:  %d",RoboticArm.MinValue);
		}
		else {
				log_e("Error! The value is out of range!");
		}

		
		
_exit:
    return result;
}
MSH_CMD_EXPORT(robotic_arm_minvalue_set,ag: robotic_arm_closevalue_set 1500);


/*【机械臂】舵机 修改 速度值 */
static int yuntai_speed_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: yuntai_speed_set 5");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 255){
				YunTai.Speed = atoi(argv[1]);
				Flash_Update();
				log_d("Write_Successed! Current Yuntai_Speed:  %d",YunTai.Speed);
		}
		else {
				log_e("Error! The value is out of range!");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(yuntai_speed_set,ag: yuntai_speed_set 5);

/*【云台】舵机 修改 【正向最大值】MSH方法 */
static int yuntai_maxvalue_set(int argc, char **argv)
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
				log_d("Write_Successed! Current YunTai_MaxValue:  %d",YunTai.MaxValue);
		}
		
		else {
				log_e("Error! The value is out of range!");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(yuntai_maxvalue_set,ag: yuntai_maxvalue_set 2500);




/*【云台】舵机 修改 【反向最大值】 MSH方法 */
static int yuntai_minvalue_set(int argc, char **argv)
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
				log_d("Write_Successed! Current YunTai_minValue:  %d",YunTai.MinValue);
		}
		else {
				log_e("Error! The value is out of range!");
		}

_exit:
    return result;
}
MSH_CMD_EXPORT(yuntai_minvalue_set,ag: yuntai_arm_closevalue_set 1500);

/*【云台】舵机 修改 【反向最大值】 MSH方法 */
static int yuntai_medvalue_set(int argc, char **argv)
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
				log_d("Write_Successed! Current YunTai_MedValue:  %d",YunTai.MedValue);
		}
		else {
				log_e("Error! The value is out of range!");
		}

_exit:
    return result;
}
MSH_CMD_EXPORT(yuntai_medvalue_set,ag: yuntai_arm_medvalue_set 2000);





