/*
 * servo.c
 *
 *  Created on: 2019年2月30日
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

#define RoboticArm_MedValue  1500
#define YunTai_MedValue  		 1500

extern struct rt_event init_event;/* ALL_init 事件控制块. */

ServoType RoboticArm = { //机械臂
												 .OpenValue = 1800,
												 .CloseValue = 1300,
											   .CurrentValue = 1500
};
ServoType YunTai ={  //云台
										.OpenValue = 1800,
										.CloseValue = 1300,
									  .CurrentValue = 1500
}; 



void servo_thread_entry(void *parameter)
{
		TIM_Cmd(TIM1, ENABLE);  //使能TIM1
		TIM_Cmd(TIM4, ENABLE);  //使能TIM4
	
		Propeller_Init();
		while(1)
		{
				TIM_SetCompare1(TIM1,1500);  		//高电平1.5ms 总周期20ms  占空比7.5%
				TIM_SetCompare2(TIM1,1500);  		
				TIM_SetCompare3(TIM1,1500); 		
				TIM_SetCompare4(TIM1,1500);  
			
				TIM_SetCompare1(TIM4,1500);  //垂推
				TIM_SetCompare2(TIM4,1500);  //垂推
				TIM_SetCompare3(TIM4,RoboticArm.CurrentValue);  //机械臂
				TIM_SetCompare4(TIM4,1500);  //云台
			
				rt_thread_mdelay(1);

		}
	
}


int servo_thread_init(void)
{
    rt_thread_t servo_tid;
		/*创建动态线程*/
    servo_tid = rt_thread_create("pwm",//线程名称
                    servo_thread_entry,				 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    512,										 //线程栈大小，单位是字节【byte】
                    10,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (servo_tid != RT_NULL){
				TIM1_PWM_Init(20000-1,168-1);	//168M/168=1Mhz的计数频率,重装载值(即PWM精度)20000，所以PWM频率为 1M/20000=50Hz.  
				TIM4_PWM_Init(20000-1,84-1);	//84M/84=1Mhz的计数频率,重装载值(即PWM精度)20000，所以PWM频率为 1M/20000=50Hz.  
				log_i("Servo_init()");
			
				rt_thread_startup(servo_tid);
				rt_event_send(&init_event, PWM_EVENT); //发送事件  表示初始化完成
		}

		return 0;
}
INIT_APP_EXPORT(servo_thread_init);













/*【机械臂】舵机 修改 【正向最大值】MSH方法 */
static int robotic_arm_openvalue_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: RoboticArm_openvalue_set 1600");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 5000){
				RoboticArm.OpenValue = atoi(argv[1]);
				Flash_Update();
				log_d("Write_Successed! Current RoboticArm_OpenValue:  %d",RoboticArm.OpenValue);
		}
		
		else {
				log_e("Error! The value is out of range!");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(robotic_arm_openvalue_set,ag: robotic_arm_openvalue_set 160);




/*【机械臂】舵机 修改 【反向最大值】 MSH方法 */
static int robotic_arm_closevalue_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: RoboticArm_closevalue_set 1150");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 3000){
				RoboticArm.CloseValue = atoi(argv[1]);
				Flash_Update();
				log_d("Write_Successed! Current RoboticArm_CloseValue:  %d",RoboticArm.CloseValue);
		}
		else {
				log_e("Error! The value is out of range!");
		}

		
		
_exit:
    return result;
}
MSH_CMD_EXPORT(robotic_arm_closevalue_set,ag: robotic_arm_closevalue_set 115);





/*【云台】舵机 修改 【正向最大值】MSH方法 */
static int yuntai_openvalue_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: YunTai_openvalue_set 1600");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 5000){
				YunTai.OpenValue = atoi(argv[1]);
				Flash_Update();
				log_d("Write_Successed! Current YunTai_OpenValue:  %d",RoboticArm.OpenValue);
		}
		
		else {
				log_e("Error! The value is out of range!");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(yuntai_openvalue_set,ag: yuntai_openvalue_set 160);




/*【云台】舵机 修改 【反向最大值】 MSH方法 */
static int yuntai_arm_closevalue_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: YunTai_closevalue_set 1150");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 3000){
				YunTai.CloseValue = atoi(argv[1]);
				Flash_Update();
				log_d("Write_Successed! Current YunTai_CloseValue:  %d",RoboticArm.CloseValue);
		}
		else {
				log_e("Error! The value is out of range!");
		}

_exit:
    return result;
}
MSH_CMD_EXPORT(yuntai_arm_closevalue_set,ag: yuntai_arm_closevalue_set 115);





