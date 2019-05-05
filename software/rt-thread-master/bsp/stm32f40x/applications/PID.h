#ifndef __PID_H_
#define __PID_H_

#include "DataType.h"
#include "filter.h"

typedef struct
{
		uint8 Err_Limit_Flag :1;//偏差限幅标志   位定义
		uint8 Integrate_Limit_Flag :1;//积分限幅标志
		uint8 Integrate_Separation_Flag :1;//积分分离标志
		float Expect;//期望
		float FeedBack;//反馈值
		float Err;//偏差
		float Last_Err;//上次偏差
		float Err_Max;//偏差限幅值
		float Integrate_Separation_Err;//积分分离偏差值
		float Integrate;//积分值
		float Integrate_Max;//积分限幅值
		float Kp;//控制参数Kp  12
		float Ki;//控制参数Ki  13
		float Kd;//控制参数Kd  14
		float Control_OutPut;//控制器总输出
		float Last_Control_OutPut;//上次控制器总输出
		float Control_OutPut_Limit;//输出限幅
		/***************************************/
		float Pre_Last_Err;//上上次偏差
		float Adaptable_Kd;//自适应微分参数
		float Last_FeedBack;//上次反馈值
		float Dis_Err;//微分量
		float Dis_Error_History[5];//历史微分量
		float Err_LPF;
		float Last_Err_LPF;
		float Dis_Err_LPF;
		float Last_Dis_Err_LPF;
		float Pre_Last_Dis_Err_LPF;
		Butter_BufferData Control_Device_LPF_Buffer;//控制器低通输入输出缓冲
		float Scale_Kp;
		float Scale_Ki;
		float Scale_Kd;
}PID_Controler;


typedef struct
{
		PID_Controler Roll_Gyro_Control;  //角速度
		PID_Controler Pitch_Gyro_Control;
		PID_Controler Yaw_Gyro_Control;
	
		PID_Controler Roll_Angle_Control; //自稳角
		PID_Controler Pitch_Angle_Control;
		PID_Controler Yaw_Angle_Control;
	
		PID_Controler High_Speed_Control;  //高度
		PID_Controler High_Position_Control;

}AllControler;

typedef enum
{

		Roll_Gyro_Controler=0,  //角速度
		Pitch_Gyro_Controler=1,
		Yaw_Gyro_Controler=2,
	
		Roll_Angle_Controler=3,  //角度
		Pitch_Angle_Controler=4, 
		Yaw_Angle_Controler=5,
	
		High_Speed_Controler=6,	
		High_Position_Controler=7

}Controler_Label;


typedef struct
{
  float Last_Time;
  float Now_Time;
  float Time_Delta;
  uint16 Time_Delta_INT;//单位ms
}Testime;

extern AllControler Total_Controller;


void Total_PID_Init(void);
float	PID_Control(PID_Controler *Controler);//海拔高度位置控制器
float PID_Control_Yaw(PID_Controler *Controler);
float PID_Control_Div_LPF(PID_Controler *Controler);
float Control_Device_LPF(float curr_inputer,Butter_BufferData *Buffer,Butter_Parameter *Parameter);

#endif




