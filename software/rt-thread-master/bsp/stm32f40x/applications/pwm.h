#ifndef __PWM_H_
#define __PWM_H_

#include "DataType.h"



typedef struct  /*  */
{
		uint32 OpenValue;		//机械臂 打开的PWM值 
		uint32 CloseValue;	 //机械臂 关闭的PWM值
		uint32 CurrentValue; //机械臂当前值
}ServoType;

void TIM1_PWM_Init(uint32 arr,uint32 psc);
void TIM4_PWM_Init(uint32 arr,uint32 psc);


#endif

