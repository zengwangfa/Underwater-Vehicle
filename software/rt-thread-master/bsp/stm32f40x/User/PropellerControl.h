#ifndef __PROPELLERCONTROL_H
#define __PROPELLERCONTROL_H

#include "sys.h"



typedef struct //推进器中值、最大值、最小值
{
		u16 PowerMed;
		u16 PowerMax;		
		u16 PowerMin;	
	  u16 PowerDeadband;	//死区值
	
}PropellerParamter_Type;//推进器参数结构体


typedef struct//调试时各个推进器的偏差量 
{
		int leftUpError;
		int rightUpError;
		int leftDownError;
		int rightDownError;
		int leftMiddleError;
		int RightMiddleError;

}PropellerError_Type;//推进器偏移值


typedef struct//调试时各个推进器的偏差量 
{
	  u16 Power; //基础推力
	
		u16 leftUp;
		u16 rightUp;
		u16 leftDown;
		u16 rightDown;
		u16 leftMiddle;
		u16 RightMiddle;

}PropellerPower_Type;//推进器偏移值

typedef enum
{
		Forward = 1, //前进
		BackAway,    //后退
		TurnLeft,    //左转
		TurnRight,   //右转
		MoveLeft,    //左移
		MoveRight,   //右移
		RiseUp,
		Dive,
		Stop
		
}ActionTypeEnum; //动作指令枚举


void Propeller_Output_Limit(int MoveValue);

#endif



