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

uint8 turnAngle = 45;    //转向角度

int32 Expect_Depth = 0;
uint16 clear_count = 0;
extern uint8 Frame_EndFlag;
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
			
				case  RiseUp: Expect_Depth-- ; break;  //上升
				case  Dive:   Expect_Depth++ ; break;  //下潜
				
				default:break/*定深度PID*/;
		}
		Control.Vertical = 0x00;
		
		switch(Control.Rotate){
				case  TurnLeft : turnLeft(); break;  //左转
				case  TurnRight: turnRight();break;  //右转			
				default:break;
		}
		
		Propeller_Output();  //推进器限幅输出
		
		Control.Move = 0x00;
		Control.Translation = 0x00;
		Control.Rotate = 0x00;

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
		
		PWM_Update(&PropellerPower);//PWM上传
	
		if(Frame_EndFlag == 1 && \
			 Control.Move ==0x00 && Control.Translation == 0x00 && Control.Rotate == 0x00){//接收到一帧 并且 无控制字 
					Horizontal_Propeller_Power_Clear();//10次更新PWM后清空
		}
		

		
}

void Horizontal_Propeller_Power_Clear(void)//水平方向推力清零 10次后清空
{
		clear_count ++;
		if(Control.Move ==0x00 && Control.Translation == 0x00 && Control.Rotate == 0x00){
				if(clear_count >= 15 ){ //10次都无控制字清除 推进器动力
						PropellerPower.rightUp = 0;
						PropellerPower.leftDown = 0;
						PropellerPower.leftUp = 0;
						PropellerPower.rightDown= 0;
					  
						clear_count = 0;
						Frame_EndFlag = 0;
				}
		}
		else {
			clear_count = 0;
		}
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
		PropellerPower.Power = Control.Power * 2; //油门大小
	
		PropellerPower.leftUp =    - PropellerPower.Power +PropellerError.leftUpError;
		PropellerPower.rightUp =     PropellerPower.Power +PropellerError.rightUpError;   
		PropellerPower.leftDown =  - PropellerPower.Power +PropellerError.leftDownError;  //√
		PropellerPower.rightDown = - PropellerPower.Power +PropellerError.rightDownError;
}
MSH_CMD_EXPORT(robotForward,ag: robotForward);

void robotBackAway(void)  //后退
{

		PropellerPower.Power = Control.Power * 2;
	
		PropellerPower.leftUp =    PropellerPower.Power +PropellerError.leftUpError;
		PropellerPower.rightUp = - PropellerPower.Power +PropellerError.rightUpError;
		PropellerPower.leftDown =  PropellerPower.Power +PropellerError.leftDownError;
		PropellerPower.rightDown = PropellerPower.Power +PropellerError.rightDownError;

}
MSH_CMD_EXPORT(robotBackAway,ag: robotBackAway);


void turnRight(void)  //右转
{

		PropellerPower.Power = Control.Power * 2;
	
		PropellerPower.leftUp =     PropellerPower.Power  +PropellerError.leftUpError;
		PropellerPower.rightUp =    0 +PropellerError.rightUpError;
		PropellerPower.leftDown =   0 +PropellerError.leftDownError;
		PropellerPower.rightDown =  -PropellerPower.Power +PropellerError.rightDownError;
}
MSH_CMD_EXPORT(turnRight,ag: turnRight);

void turnLeft(void)  //左转
{

		PropellerPower.Power = Control.Power * 2;
	
		PropellerPower.leftUp =     0 +PropellerError.leftUpError;
		PropellerPower.rightUp =   -PropellerPower.Power +PropellerError.rightUpError;
		PropellerPower.leftDown =  -PropellerPower.Power +PropellerError.leftDownError;
		PropellerPower.rightDown =  0 +PropellerError.rightDownError;
}
MSH_CMD_EXPORT(turnLeft,ag: turnLeft);

void moveLeft(void)  //左移
{

		PropellerPower.Power = Control.Power * 2;

		PropellerPower.leftUp =    PropellerPower.Power + PropellerError.leftUpError;
		PropellerPower.rightUp =   PropellerPower.Power + PropellerError.rightUpError;
		PropellerPower.leftDown = -PropellerPower.Power + PropellerError.leftDownError;
		PropellerPower.rightDown = PropellerPower.Power + PropellerError.rightDownError;

}

MSH_CMD_EXPORT(moveLeft,ag: moveLeft);

void moveRight(void)  //右移
{
		PropellerPower.Power = Control.Power * 2;
	
		PropellerPower.leftUp =    -PropellerPower.Power + PropellerError.leftUpError;
		PropellerPower.rightUp =   -PropellerPower.Power + PropellerError.rightUpError;
		PropellerPower.leftDown =   PropellerPower.Power + PropellerError.leftDownError;
		PropellerPower.rightDown = -PropellerPower.Power + PropellerError.rightDownError;
}
MSH_CMD_EXPORT(moveRight,ag: moveRight);

/*******************************************
* 函 数 名：Propeller_upDown
* 功    能：推进器上升或下降
* 输入参数：depth期望深度
* 返 回 值：none
* 注    意：none
********************************************/
void robot_upDown(float depth_output)  
{
		//限幅
		depth_output = depth_output < -500 ?-500:depth_output;
		depth_output = depth_output >  500 ? 500:depth_output;
		
		
		PropellerPower.leftMiddle   = - depth_output;//正反桨
		PropellerPower.rightMiddle  =   depth_output;
}

