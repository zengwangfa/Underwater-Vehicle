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


void Angle_Control(void)
{
	
		if(Sensor.JY901.Euler.Yaw < 0) Yaw = (180+Sensor.JY901.Euler.Yaw) + 180;//角度补偿
		if(Sensor.JY901.Euler.Yaw > 0) Yaw = Sensor.JY901.Euler.Yaw;            //角度补偿
		Total_Controller.Yaw_Angle_Control.Expect = Yaw_Control;//偏航角速度环期望，直接来源于遥控器打杆量
		Total_Controller.Yaw_Angle_Control.FeedBack = Yaw;//偏航角反馈
		PID_Control_Yaw(&Total_Controller.Yaw_Angle_Control);//偏航角度控制
	

		//偏航角速度环期望，来源于偏航角度控制器输出
		//Total_Controller.Yaw_Gyro_Control.Expect = Total_Controller.Yaw_Angle_Control.Control_OutPut;
	
}


void Gyro_Control()//角速度环
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

