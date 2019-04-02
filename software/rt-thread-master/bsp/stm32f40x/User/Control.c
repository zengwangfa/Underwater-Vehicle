#include "Control.h"
#include "PID.h"
#include "gyroscope.h"

short int Yaw_Control = 0;//Yaw——y轴偏航

void Angle_Control(void)
{
		if(0 == Yaw_Control)
		{
				Total_Controller.Yaw_Angle_Control.FeedBack = JY901.Euler.Yaw;//偏航角反馈
				PID_Control_Yaw(&Total_Controller.Yaw_Angle_Control);//偏航角度控制
				//偏航角速度环期望，来源于偏航角度控制器输出
				Total_Controller.Yaw_Gyro_Control.Expect = Total_Controller.Yaw_Angle_Control.Control_OutPut;
		}
	  else//波动偏航方向杆后，只进行内环角速度控制
		{
				Total_Controller.Yaw_Angle_Control.Expect = 0;//偏航角期望给0,不进行角度控制
				Total_Controller.Yaw_Gyro_Control.Expect = Yaw_Control;//偏航角速度环期望，直接来源于遥控器打杆量
		}
}


void Gyro_Control()//角速度环
{

  //偏航角前馈控制
  //Total_Controller.Yaw_Gyro_Control.FeedBack=Yaw_Gyro;


//  PID_Control_Div_LPF(&Total_Controller.Yaw_Gyro_Control);
//  Yaw_Gyro_Control_Expect_Delta=1000*(Total_Controller.Yaw_Gyro_Control.Expect-Last_Yaw_Gyro_Control_Expect)
//    /Total_Controller.Yaw_Gyro_Control.PID_Controller_Dt.Time_Delta;
//  //**************************偏航角前馈控制**********************************
//  Total_Controller.Yaw_Gyro_Control.Control_OutPut+=Yaw_Feedforward_Kp*Total_Controller.Yaw_Gyro_Control.Expect
//    +Yaw_Feedforward_Kd*Yaw_Gyro_Control_Expect_Delta;//偏航角前馈控制
//  Total_Controller.Yaw_Gyro_Control.Control_OutPut=constrain_float(Total_Controller.Yaw_Gyro_Control.Control_OutPut,
//                                                                   -Total_Controller.Yaw_Gyro_Control.Control_OutPut_Limit,
//                                                                   Total_Controller.Yaw_Gyro_Control.Control_OutPut_Limit);
//  Last_Yaw_Gyro_Control_Expect=Total_Controller.Yaw_Gyro_Control.Expect;
//  

}

