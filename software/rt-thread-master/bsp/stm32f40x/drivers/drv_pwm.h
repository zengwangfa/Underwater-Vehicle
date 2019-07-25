#ifndef __PWM_H_
#define __PWM_H_

#include "DataType.h"


void TIM1_PWM_Init(uint32 arr,uint32 psc);//推进器
void TIM3_PWM_Init(uint32 arr,uint32 psc);//吸取器
void TIM4_PWM_Init(uint32 arr,uint32 psc);//推进器、机械臂、云台

void TIM10_PWM_Init(uint32 arr,uint32 psc);
void TIM11_PWM_Init(uint32 arr,uint32 psc);

void TIM1_PWM_CH1_E9(short duty); //右上	 E9	

void TIM1_PWM_CH2_E11(short duty);//左下	 E11

void TIM1_PWM_CH3_E13(short duty);//左上   E13

void TIM1_PWM_CH4_E14(short duty);//右下   E14


/*------------- TIM3 吸取器---------------*/

void TIM3_PWM_CH3_B0(short duty); //吸取器

void TIM3_PWM_CH4_B1(short duty);


/*------------- TIM4 推进器、云台、机械臂 ---------------*/
void TIM4_PWM_CH1_D12(short duty);//左中   D12

void TIM4_PWM_CH2_D13(short duty);//右中   D13

void TIM4_PWM_CH3_D14(short duty);//机械臂 D14

void TIM4_PWM_CH4_D15(short duty);//云台   D15



/*------------- TIM10 探照灯---------------*/
void TIM10_PWM_CH1_F6(short duty); //探照灯 F6

/*------------- TIM11 探照灯---------------*/
void TIM11_PWM_CH1_F7(short duty); //探照灯 F7

#endif

