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
#include "pwm.h"


uint8 turnAngle = 45;    //转向角度




/*******************************************
* 函 数 名：askResultant
* 功    能：求出合力大小及方向
* 输入参数：输入需要的角度和大小 forceSize 为需要的合力的大小  angle为转向的角度 
* 返 回 值：force 根据力和行进角度算出的单个推进器的推力大小
* 注    意：none
********************************************/
double askResultant(double angle,double forceSize)
{
		double force=0;
		force = forceSize*cos(Deg2Rad(angle));
		return force;
}


/*******************************************
* 函 数 名：Output_Limit
* 功    能：推进器输出限制
* 输入参数：PowerValue
* 返 回 值：限幅后的 PowerValue
* 注    意：最大值为Propeller.PowerMax 初始化为1800
						最小值为Propeller.PowerMin 初始化为1300


********************************************/
uint16 Output_Limit(uint16 *PowerValue)
{
		*PowerValue = (*PowerValue  + PropellerParameter.PowerMed) > PropellerParameter.PowerMax ? PropellerParameter.PowerMax : *PowerValue ;//正向限幅
		*PowerValue = (*PowerValue  + PropellerParameter.PowerMed) < PropellerParameter.PowerMin ? PropellerParameter.PowerMin : *PowerValue ;//反向限幅
	
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

		switch(Control.Move){
				case  Forward : robotForward();break;  //前进
				case  BackAway: robotBackAway();break;	 //后退
				default:break;
		}
		

		switch(Control.Translation){
				case  MoveLeft : moveLeft()	;break;  //左移
				case  MoveRight: moveRight();break;  //右移		
				default:break;
		}
		
		
		switch(Control.Vertical){//有控制数据不定深度
				case  RiseUp:  ;break;  //上升
				case  Dive:    ;break;  //下潜
				
				default:break/*定深度PID*/;
		}
	
		switch(Control.Rotate){
				case  TurnLeft : turnLeft(); break;  //左转
				case  TurnRight: turnRight();break;  //右转			
				default:break;
		}
		Propeller_Output();  //推进器输出限幅
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

		PropellerPower.rightUp = Output_Limit(&PropellerPower.rightUp);
	
		PropellerPower.leftUp = Output_Limit(&PropellerPower.leftUp);
	
		PropellerPower.rightDown = Output_Limit(&PropellerPower.rightDown);
	
		PropellerPower.leftDown = Output_Limit(&PropellerPower.leftDown);
	
		PropellerPower.leftMiddle = Output_Limit(&PropellerPower.leftMiddle);
	
		PropellerPower.rightMiddle = Output_Limit(&PropellerPower.rightMiddle);
		
		PWM_Update();//PWM上传
		
}



/*******************************************
* 函 数 名：robotForward
* 功    能：机器人前进
* 输入参数：none
* 返 回 值：none
* 注    意：none
********************************************/
void robotForward(void)  //前进
{
		Control.Power = Control.Power * 2; //油门大小
	
		PropellerPower.leftUp =   - Control.Power - 32;
		PropellerPower.rightUp =    Control.Power + 32;
		PropellerPower.leftDown = - Control.Power - 32;
		PropellerPower.rightDown =  Control.Power + 32;
}


void robotBackAway(void)  //后退
{

		PropellerPower.Power = Control.Power * 2;
	
		PropellerPower.leftUp =Control.Power+2+PropellerError.leftUpError;
		PropellerPower.rightUp = -Control.Power-30-PropellerError.rightUpError;
		PropellerPower.leftDown = Control.Power+2+PropellerError.leftDownError;
		PropellerPower.rightDown = -Control.Power-30-PropellerError.rightDownError;

}


void turnRight(void)  //右转
{

		PropellerPower.Power = Control.Power * 2;
	
		PropellerPower.leftUp = -PropellerPower.Power-30-PropellerError.leftUpError;
		PropellerPower.rightUp = -PropellerPower.Power-30-PropellerError.rightUpError;
		PropellerPower.leftDown = -PropellerPower.Power-30-PropellerError.leftDownError;
		PropellerPower.rightDown = -PropellerPower.Power-30-PropellerError.rightDownError;

}

void turnLeft(void)  //左转
{

		PropellerPower.Power = Control.Power * 2;
	
		PropellerPower.leftUp = PropellerPower.Power+0+PropellerError.leftUpError;
		PropellerPower.rightUp = PropellerPower.Power+0+PropellerError.rightUpError;
		PropellerPower.leftDown = PropellerPower.Power+0+PropellerError.leftDownError;
		PropellerPower.rightDown = PropellerPower.Power+0+PropellerError.rightDownError;

}


void moveLeft(void)  //左移
{

			PropellerPower.Power = PropellerPower.Power * 4;
			PropellerPower.leftUp = PropellerPower.Power+5+PropellerError.leftUpError;
			PropellerPower.rightUp = PropellerPower.Power+5+PropellerError.rightUpError;
			PropellerPower.leftDown = -PropellerPower.Power-32-PropellerError.leftDownError;
			PropellerPower.rightDown = -PropellerPower.Power-32-PropellerError.rightDownError;

}



void moveRight(void)  //右移
{


			PropellerPower.Power = PropellerPower.Power * 4;
			PropellerPower.leftUp = -PropellerPower.Power-32-PropellerError.leftUpError;
			PropellerPower.rightUp = -PropellerPower.Power-32-PropellerError.rightUpError;
			PropellerPower.leftDown = PropellerPower.Power+5+PropellerError.leftDownError;
			PropellerPower.rightDown = PropellerPower.Power+5+PropellerError.rightDownError;

}



/*******************************************
* 函 数 名：Propeller_upDown
* 功    能：推进器上升或下降
* 输入参数：depth期望深度
* 返 回 值：none
* 注    意：none
********************************************/
void Propeller_upDown(int depth)  
{
		static uint16 depth_RightDuty = 0;
		static uint16 depth_LeftDuty = 0;
		if(depth < -500) depth = (-500);
		if(depth >  500) depth =   500;

		depth_RightDuty = PropellerPower_Med - depth;//正反桨
		depth_LeftDuty  = PropellerPower_Med + depth;
	
	  depth_RightDuty = Output_Limit(&depth_RightDuty);
		depth_LeftDuty  = Output_Limit(&depth_LeftDuty );

}

