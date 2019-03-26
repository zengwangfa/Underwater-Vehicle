#ifndef __PWM_H_
#define __PWM_H_

#include "init.h"



typedef struct  /*  */
{
		u32 OpenValue;		//机械臂 打开的PWM值 
		u32 CloseValue;	 //机械臂 关闭的PWM值
		u32 CurrentValue; //机械臂当前值
}ServoType;

void TIM1_PWM_Init(u32 arr,u32 psc);
void TIM4_PWM_Init(u32 arr,u32 psc);


#endif

