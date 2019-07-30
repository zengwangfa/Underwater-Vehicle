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
		int leftUp;  //左上
		int rightUp;
		int leftDown; //左下
		int rightDown;
		int leftMiddle; //左中
		int rightMiddle;		

}PropellerDir_Type;

typedef struct//调试时各个推进器的偏差量 
{
		int16 leftUp;
		int16 rightUp;
		int16 leftDown;
		int16 rightDown;
		int16 leftMiddle;
		int16 rightMiddle;

}PropellerError_Type;//推进器偏移值

typedef struct
{
		int16 Adjust1;
		int16 Adjust2;
		int16 Adjust3;
		int16 Adjust4;
}Adjust_Parameter; //调节参数

typedef struct
{
	  float PowerPercent; //推进器动力百分比
	
		short leftUp;
		short rightUp;
		short leftDown;
		short rightDown;
		short leftMiddle;
		short rightMiddle;

}PropellerPower_Type;//各个推进器推力

typedef enum
{
		RiseUp    = 1, //上升
		Dive      = 2, //下潜
		Stop      = 0
		
}ActionType_Enum; //动作指令枚举


void Propeller_Init(void);

void PWM_Update(PropellerPower_Type* power);//推进器PWM 更新
void Extractor_Control(uint8 *action);

extern PropellerDir_Type PropellerDir;
extern PropellerParameter_Type PropellerParameter; 
extern PropellerPower_Type  PropellerPower; //推进器推理控制器
extern PropellerError_Type  PropellerError; //推进器偏差值

extern PropellerError_Type Forward;
extern PropellerError_Type Retreat;
extern PropellerError_Type TurnLeft;
extern PropellerError_Type TurnRight;
extern Adjust_Parameter AdjustParameter;

#endif




