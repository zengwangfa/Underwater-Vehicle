#ifndef __PROPELLER_H
#define __PROPELLER_H

#include "DataType.h"


typedef struct //推进器中值、最大值、最小值
{
		uint16 PowerMed;
		uint16 PowerMax;		
		uint16 PowerMin;	
	  uint16 PowerDeadband;	//死区值
	
}PropellerParamter_Type;//推进器参数结构体


typedef struct//调试时各个推进器的偏差量 
{
		int leftUpError;  //左上
		int rightUpError;
		int leftDownError; //左下
		int rightDownError;
		int leftMiddleError; //左中
		int rightMiddleError;

}PropellerError_Type;//推进器偏移值


typedef struct
{
	  uint16 Power; //基础推力
	
		uint16 leftUp;
		uint16 rightUp;
		uint16 leftDown;
		uint16 rightDown;
		uint16 leftMiddle;
		uint16 rightMiddle;

}PropellerPower_Type;//各个推进器推力

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


extern PropellerParamter_Type PropellerParamter; 
extern ActionTypeEnum       Posture_Flag; //机器人姿态标志位
extern PropellerPower_Type  PropellerPower; //推进器推理控制器
extern PropellerError_Type  PropellerError; //推进器偏差值


void Propeller_Init(void);





#endif




