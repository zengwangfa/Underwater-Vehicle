/**
  **********************************  STM8S  ***********************************
  * @文件名     ： bsp_uart.c
  * @作者       ： strongerHuang
  * @库版本     ： V2.2.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年06月29日
  * @摘要       ： UART源文件
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2017-06-29 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "bsp_uart.h"


/************************************************
函数名称 ： UART_Initializes
功    能 ： UART初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void UART_Initializes(void)
{
  UART1_Init((uint32_t)115200, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
  UART1_Cmd(ENABLE);
  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);

  enableInterrupts();
}

/************************************************
函数名称 ： UART1_SendByte
功    能 ： UART1发送一个字符
参    数 ： Data --- 数据
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void UART1_SendByte(uint8_t Data)
{
  while((UART1_GetFlagStatus(UART1_FLAG_TXE)==RESET));
  UART1_SendData8(Data);
  while((UART1_GetFlagStatus(UART1_FLAG_TC)==RESET));
}

/************************************************
函数名称 ： UART1_SendNByte
功    能 ： 串口1发送N个字符
参    数 ： pData ---- 字符串
            Length --- 长度
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void UART1_SendNByte(uint8_t *pData, uint16_t Length)
{
  while(Length--)
  {
    UART1_SendByte(*pData);
    pData++;
  }
}

/************************************************
函数名称 ： UART1_Printf
功    能 ： 串口1打印输出
参    数 ： String --- 字符串
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void UART1_Printf(uint8_t *String)
{
  while((*String) !=  '\0 ')
  {
    UART1_SendByte(*String);
    String++;
  }
}

/******************************************* 串口重定义 *******************************************/
/************************************************
函数名称 ： fputc
功    能 ： 重定义putc函数
参    数 ： ch --- 字符
            *f --- 文件指针
返 回 值 ： 字符
作    者 ： strongerHuang
*************************************************/
int fputc(int ch, FILE *f)
{
  UART1_SendByte((uint8_t)ch);

  return ch;
}

/************************************************
函数名称 ： fgetc
功    能 ： 重定义getc函数
参    数 ： *f --- 文件指针
返 回 值 ： 字符
作    者 ： strongerHuang
*************************************************/
int fgetc(FILE *f)
{
  while(UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET);

  return (int)UART1_ReceiveData8();
}


/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
