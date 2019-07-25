/*
 * pwm.c
 *
 *  Created on: 2019年2月30日
 *      Author: zengwangfa
 *      Notes:  PWM设备
 */
#define LOG_TAG    "pwm"
#include "drv_pwm.h"
#include "sys.h"
#include "propeller.h"
#include "PropellerControl.h"
/*---------------------- Constant / Macro Definitions -----------------------*/		



/*----------------------- Variable Declarations -----------------------------*/




/*----------------------- Function Implement --------------------------------*/

/*------------- TIM1 推进器---------------*/

void TIM1_PWM_CH1_E9(short duty)
{
		TIM_SetCompare1(TIM1,duty);     //右上	 E9	
}

void TIM1_PWM_CH2_E11(short duty)
{
		TIM_SetCompare2(TIM1,duty);    //左下	 E11
}

void TIM1_PWM_CH3_E13(short duty)
{
		TIM_SetCompare3(TIM1,duty); 	    //左上   E13
}

void TIM1_PWM_CH4_E14(short duty)
{
		TIM_SetCompare4(TIM1,duty);   //右下   E14
}

/*------------- TIM3 吸取器---------------*/

void TIM3_PWM_CH3_B0(short duty)
{
		TIM_SetCompare3(TIM3,duty);
}

void TIM3_PWM_CH4_B1(short duty)
{
		TIM_SetCompare4(TIM3,duty);	
}


/*------------- TIM4 推进器、云台、机械臂 ---------------*/
void TIM4_PWM_CH1_D12(short duty)
{

		TIM_SetCompare1(TIM4,duty);  //左中   D12
}

void TIM4_PWM_CH2_D13(short duty)
{
		TIM_SetCompare2(TIM4,duty); //右中   D13
}

void TIM4_PWM_CH3_D14(short duty)
{
		TIM_SetCompare3(TIM4,duty);
}

void TIM4_PWM_CH4_D15(short duty)
{
		TIM_SetCompare4(TIM4,duty);
}

/*------------- TIM10 探照灯---------------*/
void TIM10_PWM_CH1_F6(short duty)
{
			TIM_SetCompare1(TIM10,duty);
		
}
/*------------- TIM11 ---------------*/
void TIM11_PWM_CH1_F7(short duty)
{
			TIM_SetCompare1(TIM11,duty);
}
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
		
		TIM_Cmd(TIM1, ENABLE);  //使能TIM1
				
}  

//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(uint32 arr,uint32 psc)//吸取器
{		 					 
		//此部分需手动修改IO口设置
		
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3时钟使能    
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //使能PORTF时钟	
		
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3);
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3);	
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;//GPIOF
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
		GPIO_Init(GPIOB,&GPIO_InitStructure);               //初始化PF7
			
		TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
		TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		
		TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//初始化定时器4
		
		//初始化TIM11 Channel PWM模式	 
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
		
		TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 3OC1
		TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 4OC1
				
		TIM_CtrlPWMOutputs(TIM3,ENABLE);		  
		
		TIM_Cmd(TIM3, ENABLE);  //使能TIM3
				
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
		
		TIM_Cmd(TIM4, ENABLE);  //使能TIM4						  
} 



//TIM10 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM10_PWM_Init(uint32 arr,uint32 psc)//探照灯90K Hz -> F6
{		 					 
		//此部分需手动修改IO口设置
		
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);  	//TIM10时钟使能    
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//使能PORTF时钟	
		
		GPIO_PinAFConfig(GPIOF,GPIO_PinSource6,GPIO_AF_TIM10);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//GPIOF
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
		GPIO_Init(GPIOF,&GPIO_InitStructure);              //初始化PF9
			
		TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
		TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		
		TIM_TimeBaseInit(TIM10,&TIM_TimeBaseStructure);//初始化定时器10
		
		//初始化TIM4 Channel PWM模式	 
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
		
		TIM_OC1Init(TIM10, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 4OC1
		
		TIM_CtrlPWMOutputs(TIM10,ENABLE);
		
		TIM_Cmd(TIM10, ENABLE);  //使能TIM10					  
} 



//TIM11 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM11_PWM_Init(uint32 arr,uint32 psc)//探照灯90K Hz -> F7
{		 					 
		//此部分需手动修改IO口设置
		
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE);  	//TIM11时钟使能    
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//使能PORTF时钟	
		
		GPIO_PinAFConfig(GPIOF,GPIO_PinSource7,GPIO_AF_TIM11);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//GPIOF
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
		GPIO_Init(GPIOF,&GPIO_InitStructure);               //初始化PF7
			
		TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
		TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		
		TIM_TimeBaseInit(TIM11,&TIM_TimeBaseStructure);//初始化定时器4
		
		//初始化TIM11 Channel PWM模式	 
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
		
		TIM_OC1Init(TIM11, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 4OC1
		
		TIM_CtrlPWMOutputs(TIM11,ENABLE);		  
		
		TIM_Cmd(TIM11, ENABLE);  //使能TIM10		
} 

