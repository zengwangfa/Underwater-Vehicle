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
int32 Expect_Depth = 0;




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
				switch(ControlCmd.Vertical){//有控制数据不定深度
						case RiseUp: 
							   Expect_Depth-- ; 
								 if(Expect_Depth < 0) Expect_Depth= 0;//超过空气中的深度值，不再上升
								 break;  //上升
					
						case Dive:   
									if(Total_Controller.High_Position_Control.Control_OutPut < 450){ //超过输出范围 停止累积
											Expect_Depth++ ;
									}
									
								 break;  //下潜
						default:break/*定深度PID*/;
				}

				switch(ControlCmd.Rotate){
						case  TurnLeft : 
									if(Rocker.Force == 0){turnLeft();} //定义左摇杆优先级高：当左摇杆有数据时，右摇杆 左右旋转失效
									break;  //上升
									
						case  TurnRight: 
									if(Rocker.Force == 0){turnRight();} //定义左摇杆优先级高：当左摇杆有数据时，右摇杆 左右旋转失效
									break; //下潜
						default:break;
				}

		}
		else {
				Propeller_Stop();		 //推进器数值清零
		}
		//Propeller_Output();  //推进器限幅输出
		
		//ControlCmd.Vertical = 0x00;

		

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
* 函 数 名：robotForward
* 功    能：机器人前进
* 输入参数：none
* 返 回 值：none
* 注    意：none
********************************************/
void turnRight(void)  //右旋
{
		PropellerPower.leftUp =     PropellerDir.leftUp*(70) + PropellerError.leftUp;
		PropellerPower.rightUp =    0 + PropellerError.rightUp;
		PropellerPower.leftDown =   PropellerDir.leftDown*(70) + PropellerError.leftDown;
		PropellerPower.rightDown =  0 + PropellerError.rightDown;
}
MSH_CMD_EXPORT(turnRight,ag: turnRight);


void turnLeft(void)  //左旋
{

		PropellerPower.leftUp =    0 + PropellerError.leftUp;
		PropellerPower.rightUp =   PropellerDir.rightUp*(70) + PropellerError.rightUp;
		PropellerPower.leftDown =  0 + PropellerError.leftDown;
		PropellerPower.rightDown = PropellerDir.rightDown*(70) + PropellerError.rightDown;
}
MSH_CMD_EXPORT(turnLeft,ag: turnLeft);


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
		
		PropellerPower.leftMiddle   = - depth_output + PropellerError.leftMiddle;//正反桨
		PropellerPower.rightMiddle  =   depth_output + PropellerError.rightMiddle;
		
	
		if( PropellerPower.rightMiddle > 10){//当 正转时并推力超过10
				PropellerPower.leftMiddle = PropellerPower.leftMiddle -10; //右上推进器 由于反向  需要进行特殊补偿
		}
		else if( PropellerPower.leftMiddle < -10){//反转时
				PropellerPower.rightMiddle = PropellerPower.rightMiddle - 10;
		}
}

