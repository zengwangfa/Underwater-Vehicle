#ifndef __PROPELLER_H
#define __PROPELLER_H

#include "DataType.h"


typedef struct //推进器中值、最大值、最小值
{
		int16 PowerMax;		
		int16 PowerMed;
		int16 PowerMin;	
	  int16 PowerDeadband;	//死区值
	
}PropellerParameter_Type;//推进器参数结构体

typedef struct //推进器方向
{
		char leftUp;  //左上
		char rightUp;
		char leftDown; //左下
		char rightDown;
		char leftMiddle; //左中
		char rightMiddle;		

}PropellerDir_Type;

typedef struct//调试时各个推进器的偏差量 
{
		int16 leftUp;  //左上
		int16 rightUp;
		int16 leftDown; //左下
		int16 rightDown;
		int16 leftMiddle; //左中
		int16 rightMiddle;

}PropellerError_Type;//推进器偏移值


typedef struct
{
	  int16 Power; //推进器动力系数 【不能为负】
	
		int16 leftUp;
		int16 rightUp;
		int16 leftDown;
		int16 rightDown;
		int16 leftMiddle;
		int16 rightMiddle;

}PropellerPower_Type;//各个推进器推力

typedef enum
{
		Forward   = 1, //前进
		BackAway  = 2, //后退
	
		TurnLeft  = 1, //左转
		TurnRight = 2, //右转
	
		RiseUp    = 1, //上升
		Dive      = 2, //下潜
	
		MoveLeft  = 1, //左移
		MoveRight = 2, //右移

		Stop      = 0
		
}ActionType_Enum; //动作指令枚举

extern PropellerDir_Type PropellerDir;
extern PropellerParameter_Type PropellerParameter; 
extern PropellerPower_Type  PropellerPower; //推进器推理控制器
extern PropellerError_Type  PropellerError; //推进器偏差值

void Propeller_Init(void);

void PWM_Update(PropellerPower_Type* power);//推进器PWM 更新





#endif




