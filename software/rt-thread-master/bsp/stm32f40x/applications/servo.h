#ifndef __SERVO_H
#define __SERVO_H

#include "drv_pwm.h"

typedef struct  /*  */
{
		uint16 MaxValue;		//机械臂 正向最大值
		uint16 MinValue;	  //机械臂 反向
		uint16 MedValue;
		uint16 CurrentValue; //机械臂当前值
		uint8  Speed; //机械臂当前值
}ServoType;

extern ServoType RoboticArm ;//机械臂

extern ServoType YunTai; //云台

extern int DirectionMode;

void RoboticArm_Control(uint8 *action);

void YunTai_Control(uint8 *action);
void DirectionProportion(int Mode);





#endif
