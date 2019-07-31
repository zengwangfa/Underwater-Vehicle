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
#include "rc_data.h"

#include "focus.h"
#include "led.h"
#include "servo.h"
#include "PropellerControl.h"
#include "propeller.h"
#include "sensor.h"

float Yaw_Control = 0.0f;//Yaw—— 偏航控制 
float Yaw = 0.0f;

extern int16 PowerPercent;
extern uint8 Frame_EndFlag;



#define STEP_VLAUE  1

/**
  * @brief  Convert_RockerValue(遥控器数据转换为推进器动力值)
  * @param  摇杆数据结构体指针
  * @retval None
  * @notice 
  */
void Convert_RockerValue(Rocker_Type *rc) //获取摇杆值
{
		rt_enter_critical();/* 调度器上锁，上锁后，将不再切换到其他线程，仅响应中断 */
		static int16 last_rc_x = 0 ,last_rc_y = 0;
		if(Frame_EndFlag){	

				rc->X = ControlCmd.Move - 126; 			  //摇杆值变换：X轴摇杆值 -127 ~ +127
				rc->Y = ControlCmd.Translation - 128  ;//					  Y轴摇杆值 -127 ~ +127
				rc->Z = ControlCmd.Vertical - 127;    //当大于128时上浮,小于128时下潜，差值越大，速度越快
				rc->Yaw = ControlCmd.Rotate - 128;    //偏航
		}
		//Speed_Buffer(&rc->X,&last_rc_x, 4);	//输出摇杆缓冲
		//Speed_Buffer(&rc->Y,&last_rc_y, 4);	

		rt_exit_critical();		/* 调度器解锁 */		
		

//		if(SIX_AXIS == VehicleMode){
//				rc->Angle = Rad2Deg(atan2(rc->X,rc->Y));// 求取atan角度：180 ~ -180
//				if(rc->Angle < 0){rc->Angle += 360;}  /*角度变换 以极坐标定义 角度顺序 0~360°*/ 	
//																				
//				rc->Force = sqrt(rc->X*rc->X + rc->Y*rc->Y);	//求合力斜边
//				rc->Fx = (sqrt(2)/2)*(rc->X - rc->Y);//转换的 X轴分力	  因为四浆对置为45°角
//				rc->Fy = (sqrt(2)/2)*(rc->X + rc->Y);//转换的 Y轴分力	  因为四浆对置为45°角
//				   
//				/* 推力F = 推进器方向*推力系数*摇杆打杆程度 + 偏差值 */   //ControlCmd.Power
//				PropellerPower.leftUp =    (PropellerDir.leftUp    * (PowerPercent) * ( rc->Fy) )/70 + ACC1 + PropellerError.leftUp;  //Power为推进器系数 0~300%
//				PropellerPower.rightUp =   (PropellerDir.rightUp   * (PowerPercent) * ( rc->Fx) )/70 + ACC2 + PropellerError.rightUp;  //处于70为   128(摇杆打杆最大程度)*255(上位机的动力系数)/70 = 466≈500(推进器最大动力)
//				PropellerPower.leftDown =  (PropellerDir.leftDown  * (PowerPercent) * ( rc->Fx) )/70 + ACC3 + PropellerError.leftDown ; 
//				PropellerPower.rightDown = (PropellerDir.rightDown * (PowerPercent) * ( rc->Fy) )/70 + ACC4 + PropellerError.rightDown;
//				

//		}

}


uint16 diff_value = 0;
	
/*******************************************
* 函 数 名：void Speed_Buffer(int *BufferMember,int BufferRange)
* 功    能：速度缓冲器
* 输入参数：1.待缓冲成员地址（*BufferMenber） 2.触发值（BufferRange）     
* 返 回 值：none
* 注    意：步进值为 宏STEP_VALUE
********************************************/
void Speed_Buffer(short *now_value,short *last_value,short range)
{		

		diff_value = abs((*last_value) - (*now_value));//暂存差值的绝对值
		
		if(diff_value >= range)//微分大于预设值，启动缓冲
		{
				if(*now_value <= *last_value){
						*now_value = *last_value - STEP_VLAUE;
				}
				else{
						*now_value = *last_value + STEP_VLAUE;
				}
				*last_value = *now_value;	
		}
}


short last_left_speed  = 0;
short last_right_speed = 0;
short left_speed  = 0;
short right_speed = 0;
float speed = 0;			   //速度总和
float left_precent = 0;	 //左推进器数值百分比
float right_precent = 0; //右推进器数值百分比
float Angle_Rad = 0.0f;
/**
  * @brief  FourtAxis_Control(四推进器水平控制)
  * @param  摇杆数据结构体指针
  * @retval None
  * @notice 
  */	
void FourtAxis_Control(Rocker_Type *rc)		//推进器控制函数
{

	
		rt_enter_critical();/* 调度器上锁，上锁后，将不再切换到其他线程，仅响应中断 */
	
		PropellerPower.PowerPercent = (float)ControlCmd.Power/128;//计算动力百分比 最大时为200%
	
		speed = sqrt(pow(rc->X,2)+pow(rc->Y,2));	//速度总和
	
		rc->Angle = Rad2Deg(atan2(rc->X,rc->Y));// 求取atan角度：180 ~ -180
		if(rc->Angle < 0){rc->Angle += 360;}     /*角度变换 以极坐标定义 角度顺序 0~360°*/ 

		Angle_Rad = Deg2Rad(rc->Angle);

		/* 左右推进器 运动控制公式 */
		left_speed   = abs(rc->X) * sin(Angle_Rad) + abs(rc->Y) * cos(Angle_Rad);//解算摇杆获取
		right_speed  = abs(rc->X) * sin(Angle_Rad) - abs(rc->Y) * cos(Angle_Rad);
		
		
		/* 直线前进/后退保护   90°±12°    270°±12°*/
		if( (rc->Angle >= 78 && rc->Angle <= 102) || (rc->Angle >= 258 && rc->Angle <= 282)   )//当摇杆 较为归中时，无视Y轴摇杆值【即仅为前进/后退】
		{
				left_speed   = abs(rc->X) * sin(Angle_Rad);
				right_speed  = abs(rc->X) * sin(Angle_Rad);
		}
		


		if(rc->X >= 0){ /* 推力公式 = 方向系数*(动力百分比*摇杆对应的推力值+偏差值) */
				PropellerPower.leftDown  =  PropellerDir.leftDown  * (PropellerPower.PowerPercent * left_speed );//推力公式 = 
				PropellerPower.rightDown =  PropellerDir.rightDown * (PropellerPower.PowerPercent * right_speed + PropellerError.rightDown);
		}
		else{
				PropellerPower.leftDown  =  PropellerDir.leftDown  * (PropellerPower.PowerPercent * left_speed  + PropellerError.leftDown) ;//推力公式 = 动力百分比*方向系数*(摇杆对应的推力值+偏差值)
				PropellerPower.rightDown =  PropellerDir.rightDown * (PropellerPower.PowerPercent * right_speed );		
		}
		
		Speed_Buffer(&PropellerPower.leftDown , &last_left_speed, 4);	//输出速度缓冲
		Speed_Buffer(&PropellerPower.rightDown, &last_right_speed,4);		

		rt_exit_critical();		/* 调度器解锁 */
		
		
		
		
		
//		if(rc->Y >= 0 )						//当Y轴 >= 0 时，左推进器速度 >= 右推进器
//		{
//				left_precent  = rc->X / abs(rc->X);		
//				right_precent = rc->X / speed;
//		}
//		else										  //当Y轴 < 0 时， 右推进器速度 >=左推进器
//		{
//				left_precent  = rc->X / speed;
//				right_precent = rc->X / abs(rc->X);
//		}
//		if(rc->X >= 0)
//		{
//				left_speed  = left_precent  * speed ;			//拟合速度
//				right_speed = right_precent * (speed +30);
//		}
//		else
//		{
//				left_speed  = left_precent  * (speed + 30);			//拟合速度
//				right_speed = right_precent * speed ;
//		}

}


void SixAxis_Control(Rocker_Type *rc)
{
		static int16 LeftUp,LeftDown,LeftMid,RightUp,RightDown,RightMid;
		static int16 LeftUpFlag,LeftDownFlag,LeftMidFlag,RightUpFlag,RightDownFlag,RightMidFlag;
		static int16 LeftUpCoe_X,LeftDownCoe_X,LeftMidCoe = 1,RightUpCoe_X,RightDonwCoe_X,RightMidCoe = 1;
		static int16 LeftUpCoe_Y,LeftDownCoe_Y,RightUpCoe_Y,RightDonwCoe_Y;
	
		LeftUpCoe_X   = 0; 						
		LeftDownCoe_X = 0;
		RightUpCoe_X  = 0;
		RightDonwCoe_X= 0;
		LeftUpCoe_Y   = 0;
		LeftDownCoe_Y = 0;
		RightUpCoe_Y  = 0;
		RightDonwCoe_Y= 0;
				
		if(rc->X != 0 && rc->Y != 0)
		{		
				LeftUpCoe_Y   =  1;
				LeftDownCoe_Y =  1;
				RightUpCoe_Y  = -1;
				RightDonwCoe_Y= -1;		
		}
		else
		{
				if(abs(rc->X) > 0)
				{
						LeftUpCoe_X   = 1;
						LeftDownCoe_X = 1;
						RightUpCoe_X  = 1;
						RightDonwCoe_X= 1;
				}
								
				if(abs(rc->Y) > 0)
				{
						LeftUpCoe_Y   =  1;
						LeftDownCoe_Y = -1;
						RightUpCoe_Y  = -1;
						RightDonwCoe_Y=  1;
				}
		}
		
		LeftUpFlag    = LeftUpCoe_X   * rc->X + LeftUpCoe_Y   * rc->Y ;
		LeftDownFlag  = LeftDownCoe_X * rc->X + LeftDownCoe_Y * rc->Y ;
		RightUpFlag   = RightUpCoe_X  * rc->X + RightUpCoe_Y  * rc->Y ;
		RightDownFlag = RightDonwCoe_X* rc->X + RightDonwCoe_Y* rc->Y ;
		


		
		PropellerPower.leftUp     = PropellerDir.leftUp     * (LeftUpFlag   + LeftUp)   ;
		PropellerPower.leftDown   = PropellerDir.leftDown   * (LeftDownFlag + LeftDown) ;
		PropellerPower.rightUp    = PropellerDir.rightUp    * (RightUpFlag  + RightUp)  ;
		PropellerPower.rightDown  = PropellerDir.rightDown  * (RightDownFlag+ RightDown);

	
}


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
	
		robot_upDown(&Total_Controller.High_Position_Control.Control_OutPut);		//竖直推进器控制
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

