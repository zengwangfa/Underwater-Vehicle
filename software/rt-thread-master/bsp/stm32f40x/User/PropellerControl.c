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
#include "RC_Data.h"
#include "drv_pwm.h"
#include <rtthread.h>
#include "PID.h"
#include "Return_Data.h"
#include "Control.h"

float Expect_Depth = 0.0f;

extern int16 PowerPercent;


/*******************************************
* 函 数 名：Output_Limit
* 功    能：推进器输出限制
* 输入参数：PowerValue
* 返 回 值：限幅后的 PowerValue
* 注    意：最大值为Propeller.PowerMax 初始化为1800
						最小值为Propeller.PowerMin 初始化为1300
********************************************/
uint16 Output_Limit(int16 *PowerValue)
{
		//不超过+500   不超过-500
		*PowerValue = (*PowerValue) > (PropellerParameter.PowerMax - PropellerParameter.PowerMed ) ? (PropellerParameter.PowerMax - PropellerParameter.PowerMed ): *PowerValue ;//正向限幅
		*PowerValue = (*PowerValue) < (PropellerParameter.PowerMin - PropellerParameter.PowerMed ) ? (PropellerParameter.PowerMin - PropellerParameter.PowerMed ): *PowerValue ;//反向限幅
	
		return *PowerValue ;
}



/*******************************************
* 函 数 名：propeller_control
* 功    能：水平轴推进器的控制
* 输入参数：none
* 返 回 值：none
* 注    意：none
********************************************/
void Propeller_Control(void)
{

		if(UNLOCK == ControlCmd.All_Lock){ //解锁

		}
		else {
				Propller_stop();		 //推进器数值清零
		}
		Propeller_Output();  //推进器限幅输出
		


		

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
		
		PropellerPower.rightUp = Output_Limit(&PropellerPower.rightUp); //PWM限幅
	
		PropellerPower.leftUp = Output_Limit(&PropellerPower.leftUp);
	
		PropellerPower.rightDown = Output_Limit(&PropellerPower.rightDown);
	
		PropellerPower.leftDown = Output_Limit(&PropellerPower.leftDown);
	
		PropellerPower.leftMiddle = Output_Limit(&PropellerPower.leftMiddle);
	
		PropellerPower.rightMiddle = Output_Limit(&PropellerPower.rightMiddle);
		
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
MSH_CMD_EXPORT(turnRight,ag: turnRight);


void turnLeft(uint16 power)  //左旋
{
		power = ((PowerPercent) * ( power) )/70;
		PropellerPower.leftUp =    0 + PropellerError.leftUp;
		PropellerPower.rightUp =   PropellerDir.rightUp*(power) + PropellerError.rightUp;
		PropellerPower.leftDown =  0 + PropellerError.leftDown;
		PropellerPower.rightDown = PropellerDir.rightDown*(power) + PropellerError.rightDown;
}
MSH_CMD_EXPORT(turnLeft,ag: turnLeft);


void Propller_stop(void)  //推进器停转
{
		PropellerPower.leftUp =    0 + PropellerError.leftUp;
		PropellerPower.rightUp =   0 + PropellerError.rightUp;
		PropellerPower.leftDown =  0 + PropellerError.leftDown;
		PropellerPower.rightDown = 0 + PropellerError.rightDown;
	
//		PropellerPower.leftMiddle = 0 + PropellerError.leftMiddle;
//		PropellerPower.rightMiddle = 0+ PropellerError.rightMiddle; 
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
		
		if(ROV_Mode == VehicleMode){ //这个是为了平衡两边推力(以为正反推进器，其特有推力不一致)

				
		}
		
		else if(AUV_Mode == VehicleMode) //这个是为了补偿推进器死区值
		{
//			  PropellerPower.leftMiddle  -= (PropellerDir.leftMiddle  * 20);//死区值 20
//				PropellerPower.rightMiddle -= (PropellerDir.rightMiddle * 20);	
		}
}

