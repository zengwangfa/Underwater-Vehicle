/*
 * Control.c
 *
 *  Created on: 2019年3月20日
 *      Author: zengwangfa
 *      Notes:  运动总控制
 */
 
#define LOG_TAG "Control"

#include <rtthread.h>
#include <elog.h>
#include <stdlib.h>
#include <math.h>

#include "Control.h"
#include "PID.h"
#include "RC_Data.h"

#include "focus.h"
#include "led.h"
#include "servo.h"
#include "PropellerControl.h"
#include "propeller.h"
#include "sensor.h"
#include "Depth.h"


float Yaw_Control = 0.0f;//Yaw—— 偏航控制 
float Yaw = 0.0f;
char ACC1 = 0,ACC2 = 0,ACC3 = 0,ACC4 = 0;
Direction_Type Direction = {1,1,1,1,1,1};


extern int16 PowerPercent;
extern uint8 Frame_EndFlag;



#define ShutDown 1

/**
  * @brief  highSpeed Devices_Control(高速设备控制)
  * @param  None
  * @retval None
  * @notice 
  */
void Convert_RockerValue(Rocker_Type *rc) //获取摇杆值
{

		static int16 LastRcX = 0 ,LastRcY = 0;
		if(Frame_EndFlag){	

				rc->X = ControlCmd.Move - 128; 			  //摇杆值变换：X轴摇杆值 -127 ~ +127
				rc->Y = ControlCmd.Translation - 128  ;//					  Y轴摇杆值 -127 ~ +127
				rc->Z = ControlCmd.Vertical - 128;    //当大于128时上浮,小于128时下潜，差值越大，速度越快
				rc->Yaw = ControlCmd.Rotate - 128;    //偏航
		}

		if(abs(abs(LastRcX) - abs(rc->X))>=ShutDown*4) //如果差值大于9，启动减速器
		{
				if(rc->X < LastRcX)														//判断前进还是后退
				{
					rc->X = LastRcX - ShutDown;									//减小加速度
				}
				else
				{
					rc->X = LastRcX + ShutDown;
				}
				LastRcX = rc->X;	
		}
		if(abs(abs(LastRcY) - abs(rc->Y))>=ShutDown*4)
		{
				if(rc->Y < LastRcY)
				{
					rc->Y = LastRcY - ShutDown;
				}
				else
				{
					rc->Y = LastRcY + ShutDown;
				}
				LastRcY = rc->Y;	
		}
		
		
<<<<<<< HEAD
		if(ROV_Mode == VehicleMode){
				rc->Angle = Rad2Deg(atan2(rc->X,rc->Y));// 求取atan角度：180 ~ -180
				if(rc->Angle < 0){rc->Angle += 360;}  /*角度变换 以极坐标定义 角度顺序 0~360°*/ 	
																				
				rc->Force = sqrt(rc->X*rc->X + rc->Y*rc->Y);	//求合力斜边
				rc->Fx = (sqrt(2)/2)*(rc->X - rc->Y);//转换的 X轴分力	  因为四浆对置为45°角
				rc->Fy = (sqrt(2)/2)*(rc->X + rc->Y);//转换的 Y轴分力	  因为四浆对置为45°角
				   
				/* 推力F = 推进器方向*推力系数*摇杆打杆程度 + 偏差值 */   //ControlCmd.Power
				PropellerPower.leftUp =    (PropellerDir.leftUp    * (PowerPercent) * ( rc->Fy) )/70 + ACC1 + PropellerError.leftUp;  //Power为推进器系数 0~300%
				PropellerPower.rightUp =   (PropellerDir.rightUp   * (PowerPercent) * ( rc->Fx) )/70 + ACC2 + PropellerError.rightUp;  //处于70为   128(摇杆打杆最大程度)*255(上位机的动力系数)/70 = 466≈500(推进器最大动力)
				PropellerPower.leftDown =  (PropellerDir.leftDown  * (PowerPercent) * ( rc->Fx) )/70 + ACC3 + PropellerError.leftDown ; 
				PropellerPower.rightDown = (PropellerDir.rightDown * (PowerPercent) * ( rc->Fy) )/70 + ACC4 + PropellerError.rightDown;
				

		}
=======
//		if(ROV_Mode == VehicleMode){
//				rc->Angle = Rad2Deg(atan2(rc->X,rc->Y));// 求取atan角度：180 ~ -180
//				if(rc->Angle < 0){rc->Angle += 360;}  /*角度变换 以极坐标定义 角度顺序 0~360°*/ 	
//																				
//				rc->Force = sqrt(rc->X*rc->X + rc->Y*rc->Y);	//求合力斜边
//				rc->Fx = (sqrt(2)/2)*(rc->X - rc->Y);//转换的 X轴分力	  因为四浆对置为45°角
//				rc->Fy = (sqrt(2)/2)*(rc->X + rc->Y);//转换的 Y轴分力	  因为四浆对置为45°角
//			
//				/* 推力F = 推进器方向*推力系数*摇杆打杆程度 + 偏差值 */   //ControlCmd.Power
//				PropellerPower.leftUp =    (PropellerDir.leftUp    * (PowerPercent) * ( rc->Fy) )/70  + PropellerError.leftUp;  //Power为推进器系数 0~300%
//				PropellerPower.rightUp =   (PropellerDir.rightUp   * (PowerPercent) * ( rc->Fx) )/70  + PropellerError.rightUp;  //处于70为   128(摇杆打杆最大程度)*255(上位机的动力系数)/70 = 466≈500(推进器最大动力)
//				PropellerPower.leftDown =  (PropellerDir.leftDown  * (PowerPercent) * ( rc->Fx) )/70  + PropellerError.leftDown ; 
//				PropellerPower.rightDown = (PropellerDir.rightDown * (PowerPercent) * ( rc->Fy) )/70  + PropellerError.rightDown;
//								
////				/* 推力F = 推进器方向*推力系数*摇杆打杆程度 + 偏差值 */   //ControlCmd.Power
////				PropellerPower.leftUp =    (PropellerDir.leftUp    * (PowerPercent) * ( rc->Fy) )/70 + ACC1 + PropellerError.leftUp;  //Power为推进器系数 0~300%
////				PropellerPower.rightUp =   (PropellerDir.rightUp   * (PowerPercent) * ( rc->Fx) )/70 + ACC2 + PropellerError.rightUp;  //处于70为   128(摇杆打杆最大程度)*255(上位机的动力系数)/70 = 466≈500(推进器最大动力)
////				PropellerPower.leftDown =  (PropellerDir.leftDown  * (PowerPercent) * ( rc->Fx) )/70 + ACC3 + PropellerError.leftDown ; 
////				PropellerPower.rightDown = (PropellerDir.rightDown * (PowerPercent) * ( rc->Fy) )/70 + ACC4 + PropellerError.rightDown;
////				

//		}
>>>>>>> d7be8108aa196336e709695979ebaca59562c13a
		
		else if(AUV_Mode == VehicleMode){
				/* 推力F = 推进器方向*推力系数*摇杆打杆程度 + 偏差值 */ 
				PropellerPower.leftUp =    (PropellerDir.leftUp    * ((PowerPercent) * ( rc->X ) /70 ))	+ ACC1 + PropellerError.leftUp  ;  //死区值为 10 Power为推进器系数0~100%
				PropellerPower.rightUp =   (PropellerDir.rightUp   * ((PowerPercent) * ( rc->Y ) /70 )) + ACC2 + PropellerError.rightUp ;  //处于70为   128(摇杆打杆最大程度)*255(上位机的动力系数)/70 = 466≈500(推进器最大动力)
				PropellerPower.leftDown =  (PropellerDir.leftDown  * ((PowerPercent) * ( rc->X ) /70 )) + ACC3 + PropellerError.leftDown ; 
				PropellerPower.rightDown = (PropellerDir.rightDown * ((PowerPercent) * ( rc->Y ) /70 )) + ACC4 + PropellerError.rightDown;
			
		}
}
int16 LeftFlag = 0,RightFlag = 0 ;
float X_FLAG1 = 0,X_FLAG2 = 0,Y_FLAG1 = 0 ,Y_FLAG2 = 0;
float TurnFlag = 0;
void FourtAxis_RovControl(Rocker_Type *rc)
{
	
						//	最终推进器的拟合系数
	//  转弯加速减速比例系数

	if(rc->X>=0)					//如果遥感X大于等于0，判断为前进,并将前进系数赋给推进器X方向的拟合系数
	{
		X_FLAG1 =1;//Direction.UP_P1;			
		X_FLAG2 = 1;//Direction.UP_P2;
	}
	else							//如果要搞X小于0，判断为后退，并将后退系数赋给推进器X方向的拟合系数
	{
		X_FLAG1 = 1;//Direction.DOWN_P1;
		X_FLAG2 =1;// Direction.DOWN_P2;
	}
	if(abs(abs(rc->Y)-abs(rc->X))>=50&&abs(rc->X <= 50))   //如果遥感X轴较小并且Y轴远大于X，则启用Y方向系数,不启用转向系数
	{
		if(rc->Y >= 0 )					
		{
			Y_FLAG1  = 1;//Direction.LEFT_P;		//如果遥感Y大于等于0，判断为右转,并将右转系数赋给推进器Y方向的拟合系数
			Y_FLAG2  = 0;
			TurnFlag = 0;
		}
		else						//如果遥感Y小于0，判断为左转,并将左转系数赋给推进器Y方向的拟合系数
		{
			Y_FLAG1  = 0;
			Y_FLAG2  = -1;//Direction.RIGHT_P;
			TurnFlag = 0;
		}
	}
	else							//X,Y差值较小或转向系数较大，启用转向系数，关闭方向系数。		
	{
		Y_FLAG1   = 0;
		Y_FLAG2   = 0;
		TurnFlag  = (float)rc->X / 128;
	}
	
	LeftFlag  = X_FLAG1 * rc->X + ( TurnFlag * rc->Y /1.3f ) + Y_FLAG1 * rc->Y;		//最终推进器拟合系数：X轴速度+转向系数*Y+Y轴方向系数*Y
	RightFlag = X_FLAG2 * rc->X - ( TurnFlag * rc->Y /1.3f ) + Y_FLAG2 * rc->Y;
	PropellerPower.leftDown = PropellerDir.leftDown*LeftFlag; 
	PropellerPower.rightDown = PropellerDir.rightDown*RightFlag;
	
}








/**
  * @brief  highSpeed Devices_Control(高速设备控制)
  * @param  * parameter
  * @retval None
  * @notice 
  */
void control_highSpeed_thread_entry(void *parameter)//高速控制线程
{
		
		rt_thread_mdelay(5000);//等待外部设备初始化成功
		print_sensor_info();   //打印传感器数据信息
		while(1)
		{
				Control_Cmd_Get(&ControlCmd); //控制命令获取 所有上位控制命令都来自于此【Important】

				if(UNLOCK == ControlCmd.All_Lock){ //如果解锁
						Convert_RockerValue(&Rocker); //遥控数据 转换 为推进器动力
						Focus_Zoom_Camera(&ControlCmd.Focus);//变焦聚焦摄像头控制
			
				}
<<<<<<< HEAD
				
				AUV_Depth_Control(&Rocker);  //AUV深度控制
				ROV_Depth_Control(&Rocker);  //ROV深度控制
=======
				FourtAxis_RovControl(&Rocker);
				AUV_Depth_Control(&Rocker);
				ROV_Depth_Control(&Rocker);
>>>>>>> d7be8108aa196336e709695979ebaca59562c13a
				ROV_Rotate_Control(&Rocker); //ROV旋转控制
				
				Propeller_Control(); //推进器真实PWM输出

				rt_thread_mdelay(10);
		}

}

/**
  * @brief  lowSpeed Devices_Control(低速设备控制)
  * @param  None
  * @retval None
  * @notice 
  */
void control_lowSpeed_thread_entry(void *parameter)//低速控制线程
{

		rt_thread_mdelay(5000);//等待外部设备初始化成功
		
		while(1)
		{

				Light_Control(&ControlCmd.Light);  //探照灯控制
				YunTai_Control(&ControlCmd.Yuntai); //云台控制
				RoboticArm_Control(&ControlCmd.Arm);//机械臂控制	
			
				rt_thread_mdelay(20);
		}
}



int control_thread_init(void)
{
		rt_thread_t control_lowSpeed_tid;
		rt_thread_t control_highSpeed_tid;
		/*创建动态线程*/
    control_lowSpeed_tid = rt_thread_create("control_low",//线程名称
                    control_lowSpeed_thread_entry,				 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    2048,										 //线程栈大小，单位是字节【byte】
                    10,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

			/*创建动态线程*/
    control_highSpeed_tid = rt_thread_create("control_high",//线程名称
                    control_highSpeed_thread_entry,				 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    2048,										 //线程栈大小，单位是字节【byte】
                    10,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms
	
    if (control_lowSpeed_tid != RT_NULL && control_highSpeed_tid != RT_NULL  ){
				rt_thread_startup(control_lowSpeed_tid);
				rt_thread_startup(control_highSpeed_tid);
				log_i("Control_Init()");
		}
		else {
				log_e("Control Error!");
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



void Depth_PID_Control(float expect_depth,float sensor_depth)
{
		
		Total_Controller.High_Position_Control.Expect = expect_depth ; //期望深度由遥控器给定
		Total_Controller.High_Position_Control.FeedBack = sensor_depth;  //当前深度反馈
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
		Yaw_Control = atoi(argv[1]); //ASCII to Integer
		
_exit:
    return result;
}
MSH_CMD_EXPORT(yaw,ag: yaw 100);



/*【解锁】 修改MSH方法 */
static int unlock(int argc, char **argv) //只能是 0~3.0f
{
		ControlCmd.All_Lock = UNLOCK;
		return 0;
}
MSH_CMD_EXPORT(unlock,unlock);


/*【锁定】 修改MSH方法 */
static int lock(int argc, char **argv) //只能是 0~3.0f
{
		ControlCmd.All_Lock = LOCK;
		return 0;
}
MSH_CMD_EXPORT(lock,lock);

