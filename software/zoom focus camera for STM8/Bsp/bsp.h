/**
  **********************************  STM8S  ***********************************
  * @文件名     ： bsp.h
  * @作者       ： strongerHuang
  * @库版本     ： V2.2.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年06月29日
  * @摘要       ： 板级支持包头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _BSP_H
#define _BSP_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm8s.h"


/* 宏定义 --------------------------------------------------------------------*/
#define LED_GPIO_PORT             GPIOD
#define LED_GPIO_PIN              GPIO_PIN_4

/* LED */
#define LED_ON                    GPIO_WriteHigh(LED_GPIO_PORT, LED_GPIO_PIN)
#define LED_OFF                   GPIO_WriteLow(LED_GPIO_PORT, LED_GPIO_PIN)
#define LED_REVERSE               GPIO_WriteReverse(LED_GPIO_PORT, LED_GPIO_PIN)


/* 函数申明 ------------------------------------------------------------------*/
void CLK_Configuration(void);
void GPIO_Configuration(void);
void BSP_Initializes(void);


#endif /* _BSP_H */

/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
