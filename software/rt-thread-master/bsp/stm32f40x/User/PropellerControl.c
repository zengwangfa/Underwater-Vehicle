/*
 * PropellerControl.c
 *
 *  Created on: 2019年3月20日
 *      Author: 219
 *      Notes:  推进器控制
 */
#include <math.h>
#include "propeller.h"
#include "PropellerControl.h"
#include "rc_data.h"
#include "drv_pwm.h"
#include <rtthread.h>
#include "PID.h"
#include "ret_data.h"
#include "Control.h"

float Expect_Depth = 0.0f;

extern int16 PowerPercent;

 /*******************************************
* 函 数 名：AUV_Depth_Control
* 功    能：AUV深度控制
* 输入参数：摇杆结构体
* 返 回 值：none
* 注    意：none
********************************************/

void AUV_Depth_Control(Rocker_Type *rc)
{
		
		if(SIX_AXIS == VehicleMode){	 //AUV深度控制位数字量
				switch(ControlCmd.Vertical){//有控制数据不定深度
						case RiseUp: 
								 Expect_Depth-=3 ; 
								 if(Expect_Depth < 0) {Expect_Depth= 0;}//超过空气中的深度值，期望值不再上升
								 break;  //上升
					
						case Dive:   
									if(Total_Controller.High_Position_Control.Control_OutPut < 450){ //超过输出范围 停止累积
											Expect_Depth+=3 ;
									}
									
								 break;  //下潜
						default:break/*定深度PID*/;
				}
		}

	
		Depth_PID_Control(Expect_Depth,Sensor.DepthSensor.Depth);//深度控制 不受【解锁】字节控制
}


/*******************************************
* 函 数 名：ROV_Depth_Control
* 功    能：ROV深度控制
* 输入参数：摇杆结构体
* 返 回 值：none
* 注    意：none
********************************************/
void ROV_Depth_Control(Rocker_Type *rc){

			/* 垂直控制 */
		if(FOUR_AXIS == VehicleMode)	 //ROV由 摇杆模拟量控制
		{
				if(rc->Z > 5){
					 Expect_Depth -=( (float)rc->Z /100); 
					 if(Expect_Depth < 0) {//超过空气中的深度值，期望值不再上升
							Expect_Depth= 0;
						}
				}
				else if(rc->Z < -5){
						if(Total_Controller.High_Position_Control.Control_OutPut < 450){ //超过输出范围 停止累积
								Expect_Depth += (fabs((float)rc->Z)/100);
						}
				}
		}
		Depth_PID_Control(Expect_Depth,Sensor.DepthSensor.Depth);//深度控制 不受【解锁】字节控制
}


 /*******************************************
* 函 数 名：ROV_Rotate_Control
* 功    能：ROV水平旋转控制
* 输入参数：摇杆结构体
* 返 回 值：none
* 注    意：none
********************************************/
void ROV_Rotate_Control(Rocker_Type *rc){

			/* 垂直控制 */
		if(FOUR_AXIS == VehicleMode)	 //ROV由 摇杆模拟量控制
		{
				if((rc->Force)< 5){ //右摇杆优先级大于 左摇杆 优先级
						if(rc->Yaw > 5){
							 turnRight(rc->Yaw);
						}
						else if(rc->Yaw < -5){
							 turnLeft(-rc->Yaw);
						}
				}
		}
		Depth_PID_Control(Expect_Depth,Sensor.DepthSensor.Depth);//深度控制 不受【解锁】字节控制
}


/*******************************************
* 函 数 名：Output_Limit
* 功    能：推进器输出限制
* 输入参数：PowerValue
* 返 回 值：限幅后的 PowerValue
* 注    意：最大值为Propeller.PowerMax 初始化为1800
						最小值为Propeller.PowerMin 初始化为1300
********************************************/
//uint16 Output_Limit(int16 *PowerValue)
//{
//		//不超过+500   不超过-500
//		*PowerValue = (*PowerValue) > (16000 - PropellerParameter.PowerMed ) ? (PropellerParameter.PowerMax - PropellerParameter.PowerMed ): *PowerValue ;//正向限幅
//		*PowerValue = (*PowerValue) < (PropellerParameter.PowerMin - PropellerParameter.PowerMed ) ? (PropellerParameter.PowerMin - PropellerParameter.PowerMed ): *PowerValue ;//反向限幅
//	
//		return *PowerValue ;
//}
uint16 Propeller_Output_Limit(int16 value)
{
		//不超过+500   不超过-500
		value = (value) > 150  ? 150  : value ;//正向限幅
		value = (value) < -150 ? -150 : value;//反向限幅
	
		return value ;
}



/*******************************************
* 函 数 名：Propeller_Output
* 功    能：推进器输出限制
* 输入参数：运动值：MoveValue
* 返 回 值：none
* 注    意：最大值为Propeller.PowerMax 初始化为2000
						最小值为Propeller.PowerMin 初始化为1000
********************************************/
void Propeller_Output(void)
{
		
		PropellerPower.rightUp = Propeller_Output_Limit(PropellerPower.rightUp); //PWM限幅
	
		PropellerPower.leftUp = Propeller_Output_Limit(PropellerPower.leftUp);
	
		PropellerPower.rightDown = Propeller_Output_Limit(PropellerPower.rightDown);
	
		PropellerPower.leftDown = Propeller_Output_Limit(PropellerPower.leftDown);
	
//		PropellerPower.leftMiddle = Propeller_Output_Limit(PropellerPower.leftMiddle);
//	
//		PropellerPower.rightMiddle = Propeller_Output_Limit(PropellerPower.rightMiddle);
		
		PWM_Update(&PropellerPower);//PWM值更新
	
}


/*******************************************
* 函 数 名：turnRight
* 功    能：机器人右旋
* 输入参数：none
* 返 回 值：none
* 注    意：none
********************************************/
void turnRight(uint16 power)  //右旋
{
		power = ((PowerPercent) * ( power) )/70;
		PropellerPower.leftUp =     PropellerDir.leftUp*(power) + PropellerError.leftUp;
		PropellerPower.rightUp =    0 + PropellerError.rightUp;
		PropellerPower.leftDown =   PropellerDir.leftDown*(power) + PropellerError.leftDown;
		PropellerPower.rightDown =  0 + PropellerError.rightDown;
}



void turnLeft(uint16 power)  //左旋
{
		power = ((PowerPercent) * ( power) )/70;
		PropellerPower.leftUp =    0 + PropellerError.leftUp;
		PropellerPower.rightUp =   PropellerDir.rightUp*(power) + PropellerError.rightUp;
		PropellerPower.leftDown =  0 + PropellerError.leftDown;
		PropellerPower.rightDown = PropellerDir.rightDown*(power) + PropellerError.rightDown;
}



void Propller_stop(void)  //推进器停转
{
		PropellerPower.leftUp =    0 + PropellerError.leftUp;
		PropellerPower.rightUp =   0 + PropellerError.rightUp;
		PropellerPower.leftDown =  0 + PropellerError.leftDown;
		PropellerPower.rightDown = 0 + PropellerError.rightDown;
	
		PropellerPower.leftMiddle = 0 + PropellerError.leftMiddle;
		PropellerPower.rightMiddle = 0+ PropellerError.rightMiddle; 
}
MSH_CMD_EXPORT(Propller_stop,ag: propller_stop);


/*******************************************
* 函 数 名：Propeller_upDown
* 功    能：推进器上升或下降
* 输入参数：depth期望深度
* 返 回 值：none
* 注    意：none
********************************************/
void robot_upDown(float depth_output)  
{
		//限幅 限制在推进器 设定的最大油门值-停转值(中值)
	
		depth_output = depth_output < -(PropellerParameter.PowerMax - PropellerParameter.PowerMed ) ? -(PropellerParameter.PowerMax - PropellerParameter.PowerMed ):depth_output;
		depth_output = depth_output >  (PropellerParameter.PowerMax - PropellerParameter.PowerMed ) ?  (PropellerParameter.PowerMax - PropellerParameter.PowerMed ):depth_output;
		
		PropellerPower.leftMiddle   =  PropellerDir.leftMiddle  * ( -depth_output + PropellerError.leftMiddle);//正反桨
		PropellerPower.rightMiddle  =  PropellerDir.rightMiddle * ( -depth_output + PropellerError.rightMiddle);//输出为负值
		
		if(FOUR_AXIS == VehicleMode){ //这个是为了平衡两边推力(以为正反推进器，其特有推力不一致)

				
		}
		
		else if(SIX_AXIS == VehicleMode) //这个是为了补偿推进器死区值
		{
//			  PropellerPower.leftMiddle  -= (PropellerDir.leftMiddle  * 20);//死区值 20
//				PropellerPower.rightMiddle -= (PropellerDir.rightMiddle * 20);	
		}
}
