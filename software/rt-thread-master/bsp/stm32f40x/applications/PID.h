#ifndef __PID_H_
#define __PID_H_


#include "sys.h"


typedef struct
{
		float p;
		float i;
		float d;
}Vector3f_pid;


typedef struct
{
  u8 Err_Limit_Flag :1;//偏差限幅标志
  u8 Integrate_Limit_Flag :1;//积分限幅标志
  u8 Integrate_Separation_Flag :1;//积分分离标志
  float Expect;//期望
  float FeedBack;//反馈值
  float Err;//偏差
  float Last_Err;//上次偏差
  float Err_Max;//偏差限幅值
  float Integrate_Separation_Err;//积分分离偏差值
  float Integrate;//积分值
  float Integrate_Max;//积分限幅值
  float Kp;//控制参数Kp
  float Ki;//控制参数Ki
  float Kd;//控制参数Kd
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
  float Scale_Kp;
  float Scale_Ki;
  float Scale_Kd;
}PID_Controler;


typedef struct
{
  PID_Controler Pitch_Angle_Control;
  PID_Controler Pitch_Gyro_Control;
  PID_Controler Roll_Angle_Control;
  PID_Controler Roll_Gyro_Control;
  PID_Controler Yaw_Angle_Control;
  PID_Controler Yaw_Gyro_Control;
  PID_Controler High_Position_Control;
  PID_Controler High_Speed_Control;


}AllControler;


extern AllControler Total_Controller;




#endif




