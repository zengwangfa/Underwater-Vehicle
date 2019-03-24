#ifndef __PWM_H_
#define __PWM_H_

#include "init.h"



typedef struct  /*  */
{
		u32 open_value;		//机械臂 打开的PWM值 
		u32 close_value;	 //机械臂 关闭的PWM值
}ServoType;

void TIM1_PWM_Init(u32 arr,u32 psc);



#endif

