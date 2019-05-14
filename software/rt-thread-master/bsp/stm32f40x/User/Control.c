/*
 * Control.c
 *
 *  Created on: 2019年3月20日
 *      Author: zengwangfa
 *      Notes:  方位角控制、深度控制
 */
#include "Control.h"
#include "PID.h"
#include <rtthread.h>
#include <stdlib.h>
#include "RC_Data.h"
#include "focus.h"
#include "led.h"
#include "servo.h"
#include "PropellerControl.h"
#include "propeller.h"
#include <elog.h>

float Yaw_Control = 0.0f;//Yaw—— 偏航控制 
float Yaw = 0.0f;

/* 原先
 *  N—— 0°/-0°
 *  W—— 90°
 *  S—— 180°/-180
 *  E—— -90°
*/
/** 规定   逆时针
 *  N—— 0°
 *  W—— 90°
 *  S—— 180°
 *  E—— 270°
 */

void control_thread_entry(void *parameter)
{

		rt_thread_mdelay(1200);//等待串口设备初始化成功
		while(1)
		{
				Devices_Control();
				rt_thread_mdelay(30);
		}
}



/**
  * @brief  Devices_Control(设备控制)
  * @param  None
  * @retval None
  * @notice 
  */
void Devices_Control(void)
{
	
		//Light_Control(&Control.Light);  //探照灯控制



		Depth_Control();
		Propeller_Control();

			/*
				Control.Depth_Lock     = RC_Control_Data[3]; //深度锁定
				Control.Direction_Lock = RC_Control_Data[4]; //方向锁定
				Control.Move					 = RC_Control_Data[5]; //前后运动
				Control.Translation		 = RC_Control_Data[6]; //左右云顶
				Control.Vertical 			 = RC_Control_Data[7]; //垂直运动
				Control.Rotate 				 = RC_Control_Data[8]; //旋转运动
				
				Control.Power 				 = RC_Control_Data[9];  //动力控制
				Control.Light 				 = RC_Control_Data[10]; //灯光控制
				
				Control.Focus 				 = RC_Control_Data[11]; //变焦摄像头控制
	
				Control.Yuntai 				 = RC_Control_Data[12]; //云台控制
				Control.Arm						 = RC_Control_Data[13]; //机械臂控制*/
	
}



int control_thread_init(void)
{
		rt_thread_t control_tid;
		/*创建动态线程*/
    control_tid = rt_thread_create("control",//线程名称
                    control_thread_entry,				 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    2048,										 //线程栈大小，单位是字节【byte】
                    8,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (control_tid != RT_NULL){
				rt_thread_startup(control_tid);
		}
		return 0;
}
INIT_APP_EXPORT(control_thread_init);


void Angle_Control(void)
{
	
		if(Sensor.JY901.Euler.Yaw < 0) Yaw = (180+Sensor.JY901.Euler.Yaw) + 180;//角度补偿
		if(Sensor.JY901.Euler.Yaw > 0) Yaw = (float)Sensor.JY901.Euler.Yaw;            //角度补偿
		Total_Controller.Yaw_Angle_Control.Expect = (float)Yaw_Control;//偏航角速度环期望，直接来源于遥控器打杆量
		Total_Controller.Yaw_Angle_Control.FeedBack = (float)Yaw;//偏航角反馈
	
		PID_Control_Yaw(&Total_Controller.Yaw_Angle_Control);//偏航角度控制
	

		//偏航角速度环期望，来源于偏航角度控制器输出
		//Total_Controller.Yaw_Gyro_Control.Expect = Total_Controller.Yaw_Angle_Control.Control_OutPut;
}



void Depth_Control(void)
{
		
		Total_Controller.High_Position_Control.Expect = (float)Expect_Depth*10; //期望深度由遥控器给定
		Total_Controller.High_Position_Control.FeedBack = (float)Sensor.Depth;  //当前深度反馈
		PID_Control(&Total_Controller.High_Position_Control);//高度位置控制器
	
		robot_upDown(Total_Controller.High_Position_Control.Control_OutPut);		//竖直推进器控制
}



void Gyro_Control(void)//角速度环
{

//  	偏航角前馈控制
//  	Total_Controller.Yaw_Gyro_Control.FeedBack=Yaw_Gyro;


//		PID_Control_Div_LPF(&Total_Controller.Yaw_Gyro_Control);
//		Yaw_Gyro_Control_Expect_Delta=1000*(Total_Controller.Yaw_Gyro_Control.Expect-Last_Yaw_Gyro_Control_Expect)
//			/Total_Controller.Yaw_Gyro_Control.PID_Controller_Dt.Time_Delta;
//		//**************************偏航角前馈控制**********************************
//		Total_Controller.Yaw_Gyro_Control.Control_OutPut+=Yaw_Feedforward_Kp*Total_Controller.Yaw_Gyro_Control.Expect
//			+Yaw_Feedforward_Kd*Yaw_Gyro_Control_Expect_Delta;//偏航角前馈控制
//		Total_Controller.Yaw_Gyro_Control.Control_OutPut=constrain_float(Total_Controller.Yaw_Gyro_Control.Control_OutPut,
//																																		 -Total_Controller.Yaw_Gyro_Control.Control_OutPut_Limit,
//																																		 Total_Controller.Yaw_Gyro_Control.Control_OutPut_Limit);
//		Last_Yaw_Gyro_Control_Expect=Total_Controller.Yaw_Gyro_Control.Expect;
//		

}

/*【深度 】期望yaw MSH方法 */
static int depth(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        rt_kprintf("Error! Proper Usage: RoboticArm_openvalue_set 1600");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1])<100){
				Expect_Depth = atoi(argv[1]);
		}
		else {
				log_e("Error! The  value is out of range!");
		}

		
_exit:
    return result;
}
MSH_CMD_EXPORT(depth,ag: depth 10);



/*【机械臂】舵机 期望yaw MSH方法 */
static int yaw(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        rt_kprintf("Error! Proper Usage: RoboticArm_openvalue_set 1600");
				result = -RT_ERROR;
        goto _exit;
    }

		Yaw_Control = atoi(argv[1]);

		
_exit:
    return result;
}
MSH_CMD_EXPORT(yaw,ag: yaw 100);

