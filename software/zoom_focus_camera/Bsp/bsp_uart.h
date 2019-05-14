/**
  **********************************  STM8S  ***********************************
  * @文件名     ： bsp_uart.h
  * @作者       ： strongerHuang
  * @库版本     ： V2.2.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年06月29日
  * @摘要       ： UART头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _BSP_UART_H
#define _BSP_UART_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm8s.h"
#include <stdio.h>


/* 宏定义 --------------------------------------------------------------------*/

/* 函数申明 ------------------------------------------------------------------*/
void UART_Initializes(void);
void UART1_SendByte(uint8_t Data);
void UART1_SendNByte(uint8_t *pData, uint16_t Length);
void UART1_Printf(uint8_t *String);


#endif /* _BSP_UART_H */

/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
