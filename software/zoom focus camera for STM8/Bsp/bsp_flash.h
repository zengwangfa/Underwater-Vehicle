/**
  **********************************  STM8S  ***********************************
  * @文件名     ： bsp_flash.h
  * @作者       ： strongerHuang
  * @库版本     ： V2.2.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年05月18日
  * @摘要       ： 内部FLASH头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _BSP_FLASH_H
#define _BSP_FLASH_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm8s.h"


/* 宏定义 --------------------------------------------------------------------*/

/* 函数申明 ------------------------------------------------------------------*/
void FLASH_Initializes(void);
void FLASH_WriteNByte(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t nByte);
void FLASH_ReadNByte(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t nByte);


#endif /* _BSP_FLASH_H */

/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
