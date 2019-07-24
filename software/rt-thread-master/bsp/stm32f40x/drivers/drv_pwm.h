#ifndef __PWM_H_
#define __PWM_H_

#include "DataType.h"


void TIM1_PWM_Init(uint32 arr,uint32 psc);
void TIM4_PWM_Init(uint32 arr,uint32 psc);

void TIM10_PWM_Init(uint32 arr,uint32 psc);
void TIM11_PWM_Init(uint32 arr,uint32 psc);

int Light_Output_Limit(int *value);

#endif

