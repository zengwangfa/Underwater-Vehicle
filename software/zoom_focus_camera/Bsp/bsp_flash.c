/**
  **********************************  STM8S  ***********************************
  * @文件名     ： bsp_flash.c
  * @作者       ： strongerHuang
  * @库版本     ： V2.2.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年05月18日
  * @摘要       ： 内部FLASH源文件
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2017-05-18 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "bsp_flash.h"

/************************************************
函数名称 ： FLASH_Initializes
功    能 ： 内部FLASH初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void FLASH_Initializes(void)
{
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
}

/************************************************
函数名称 ： FLASH_WriteNByte
功    能 ： 写Flash
参    数 ： pBuffer ----- 写入数据区首地址
            WriteAddr --- 要写入Flash的地址
            nByte ------- 要写入的字节数
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void FLASH_WriteNByte(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t nByte)
{
  FLASH_Unlock(FLASH_MEMTYPE_PROG);              //解锁
  while(FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET);
  while(nByte--)
  {
    FLASH_ProgramByte(WriteAddr, *pBuffer);
    WriteAddr++;
    pBuffer++;
    FLASH_WaitForLastOperation(FLASH_MEMTYPE_PROG);
  }
  FLASH_Lock(FLASH_MEMTYPE_PROG);                //上锁
}


/************************************************
函数名称 ： FLASH_ReadNByte
功    能 ： 读Flash
参    数 ： pBuffer ---- 数据buf
            ReadAddr --- 要读取Flash的首地址
            nByte ------ 要读取的字节数
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void FLASH_ReadNByte(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t nByte)
{
  while(nByte--)
  {
    *pBuffer = FLASH_ReadByte(ReadAddr);
    ReadAddr++;
    pBuffer++;
  }
}

/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
