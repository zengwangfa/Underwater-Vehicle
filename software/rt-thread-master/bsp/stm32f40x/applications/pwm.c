/*
 * pwm.c
 *
 *  Created on: 2019年2月30日
 *      Author: zengwangfa
 *      Notes:  PWM设备
 */
#define LOG_TAG    "pwm"

#include "sys.h"
#include <elog.h>
#include "propeller.h"
/*---------------------- Constant / Macro Definitions -----------------------*/		



/*----------------------- Variable Declarations -----------------------------*/




/*----------------------- Function Implement --------------------------------*/




/*
（1）当APB1和APB2分频数为1的时候，TIM1、TIM8~TIM11的时钟为APB2的时钟，TIM2~TIM7、TIM12~TIM14的时钟为APB1的时钟；

（2）而如果APB1和APB2分频数不为1，那么TIM1、TIM8~TIM11的时钟为APB2的时钟的两倍，TIM2~TIM7、TIM12~TIM14的时钟为APB1的时钟的两倍。
		 根据时钟分析，可知
		 因为系统初始化SystemInit函数里初始化APB1总线时钟为4分频即42M，APB2总线时钟为2分频即84M，
		 所以TIM1、TIM8~TIM11的时钟为APB2时钟的两倍即168M，TIM2~TIM7、TIM12~TIM14的时钟为APB1的时钟的两倍即84M。
*/


//TIM1 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数

void TIM1_PWM_Init(uint32 arr,uint32 psc)
{		 					 
		//此部分需手动修改IO口设置
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1时钟使能    
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//使能PORTE时钟	
		
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource9, GPIO_AF_TIM1);
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;//GPIOE
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
		GPIO_Init(GPIOE,&GPIO_InitStructure);              //初始化P
			
		TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
		TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		
		TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器1
		
		//初始化TIM14 Channel1 PWM模式	 
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
		
		TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
		TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
		TIM_OC3Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
		TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
		
		TIM_CtrlPWMOutputs(TIM1,ENABLE);

}  

//TIM4 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM4_PWM_Init(uint32 arr,uint32 psc)
{		 					 
		//此部分需手动修改IO口设置
		
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM1时钟使能    
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 	//使能PORTF时钟	
		
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_TIM4);
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_TIM4);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//GPIOD
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
		GPIO_Init(GPIOD,&GPIO_InitStructure);              //初始化PF9
			
		TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
		TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		
		TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//初始化定时器4
		
		//初始化TIM4 Channel PWM模式	 
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
		
		TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 4OC1
		TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 4OC1
		TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 4OC1		
		TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 4OC1
		
		TIM_CtrlPWMOutputs(TIM4,ENABLE);
		

 							  
} 

