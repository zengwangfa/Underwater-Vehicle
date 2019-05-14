/**
  **********************************  STM8S  ***********************************
  * @文件名     ： bsp_timer.c
  * @作者       ： strongerHuang
  * @库版本     ： V2.2.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年06月29日
  * @摘要       ： TIM定时器源文件
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2017-06-29 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "bsp_timer.h"


/************************************************
函数名称 ： TIMER_Initializes
功    能 ： 定时器初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void TIMER_Initializes(void)
{
                                                 //延时10us (2分频=8M  计数80个脉冲)
  TIM4_TimeBaseInit(TIM4_PRESCALER_2, 79);
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
}

/************************************************
函数名称 ： TIMDelay_N10us
功    能 ： 定时器延时N个10us
参    数 ： Times --- N值
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void TIMDelay_N10us(uint16_t Times)
{
  TIM4_Cmd(ENABLE);                              //启动定时器
  while(Times--)
  {
    while(RESET == TIM4_GetFlagStatus(TIM4_FLAG_UPDATE));
    TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  }
  TIM4_Cmd(ENABLE);                              //关闭定时器
}

/************************************************
函数名称 ： TIMDelay_Nms
功    能 ： 定时器延时Nms
参    数 ： Times --- N值
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void TIMDelay_Nms(uint16_t Times)
{
  while(Times--)
  {
    TIMDelay_N10us(100);
  }
}

/************************************************
函数名称 ： TIMDelay_Ns
功    能 ： 定时器延时Ns
参    数 ： Times --- N值
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void TIMDelay_Ns(uint16_t Times)
{
  while(Times--)
  {
    TIMDelay_Nms(1000);
  }
}


/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
