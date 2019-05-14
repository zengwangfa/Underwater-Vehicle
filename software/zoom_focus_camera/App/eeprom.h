/**
  **********************************  STM8S  ***********************************
  * @文件名     ： eeprom.h
  * @作者       ： strongerHuang
  * @库版本     ： V2.2.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年06月29日
  * @摘要       ： EEPROM应用头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _EEPROM_H
#define _EEPROM_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm8s.h"


/* 宏定义 --------------------------------------------------------------------*/
#define EEPROM_DEV_ADDR           0xA0                     //地址(设备地址:与A2、A1、A0有关)

#define EEPROM_WR                 0x00                     //写
#define EEPROM_RD                 0x01                     //读

#define AT24C64                                            //定义芯片型号(需根据芯片型号修改)
/*
1、"地址长度"根据芯片型号不同略有不同
    8位: AT24C01、AT24C02
   16位: AT24C04、AT24C08、AT24C16、AT24C32、AT24C64、AT24C128、AT24C256、AT24C512

2、"页长度"根据芯片型号不同略有不同
  8字节: AT24C01、AT24C02
 16字节: AT24C04、AT24C08、AT24C16
 32字节: AT24C32、AT24C64
 64字节: AT24C128、AT24C256
128字节: AT24C512
*/
#if defined (AT24C01) || defined (AT24C02)
  #define EEPROM_WORD_ADDR_SIZE   8                        //地址长度
  #define EEPROM_PAGE_SIZE        8                        //页长度
#endif

#if defined (AT24C04) || defined (AT24C08) || defined (AT24C16)
  #define EEPROM_WORD_ADDR_SIZE   16                       //地址长度
  #define EEPROM_PAGE_SIZE        16                       //页长度
#endif

#if defined (AT24C32) || defined (AT24C64)
  #define EEPROM_WORD_ADDR_SIZE   16                       //地址长度
  #define EEPROM_PAGE_SIZE        32                       //页长度
#endif

#if defined (AT24C128) || defined (AT24C256)
  #define EEPROM_WORD_ADDR_SIZE   16                       //地址长度
  #define EEPROM_PAGE_SIZE        64                       //页长度
#endif

#if defined (AT24C512)
  #define EEPROM_WORD_ADDR_SIZE   16                       //地址长度
  #define EEPROM_PAGE_SIZE        128                      //页长度
#endif



/* 函数申明 ------------------------------------------------------------------*/
void EEPROM_WriteByte(uint16_t Addr, uint8_t Data);
void EEPROM_ReadByte(uint16_t Addr, uint8_t *Data);

void EEPROM_WritePage(uint16_t Addr, uint8_t *pData, uint8_t Length);
void EEPROM_WriteNByte(uint16_t Addr, uint8_t *pData, uint16_t Length);
void EEPROM_ReadNByte(uint16_t Addr, uint8_t *pData, uint16_t Length);


#endif /* _EEPROM_H */

/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
