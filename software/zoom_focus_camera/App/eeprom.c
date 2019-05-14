/**
  **********************************  STM8S  ***********************************
  * @文件名     ： eeprom.c
  * @作者       ： strongerHuang
  * @库版本     ： V2.2.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年06月29日
  * @摘要       ： EEPROM应用源文件
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2017-06-29 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "eeprom.h"
#include "bsp_i2c.h"
#include "bsp_timer.h"


/**************************** 写读1Byte ****************************/
/************************************************
函数名称 ： EEPROM_WriteByte
功    能 ： EEPROM写一字节
参    数 ： Addr --- 地址
            Data --- 数据
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void EEPROM_WriteByte(uint16_t Addr, uint8_t Data)
{
  /* 1.开始 */
  I2C_Start();

  /* 2.设备地址/写 */
  I2C_WriteByte(EEPROM_DEV_ADDR | EEPROM_WR);

  /* 3.数据地址 */
#if (8 == EEPROM_WORD_ADDR_SIZE)
  I2C_WriteByte((uint8_t)(Addr&0x00FF));         //数据地址(8位)
#else
  I2C_WriteByte((uint8_t)(Addr>>8));             //数据地址(16位)
  I2C_WriteByte((uint8_t)(Addr&0x00FF));
#endif

  /* 4.写一字节数据 */
  I2C_WriteByte(Data);

  /* 5.停止 */
  I2C_Stop();
}

/************************************************
函数名称 ： EEPROM_ReadByte
功    能 ： EEPROM读一字节
参    数 ： Addr --- 地址
            data --- 数据
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void EEPROM_ReadByte(uint16_t Addr, uint8_t *Data)
{
  /* 1.开始 */
  I2C_Start();

  /* 2.设备地址/写 */
  I2C_WriteByte(EEPROM_DEV_ADDR | EEPROM_WR);

  /* 3.数据地址 */
#if (8 == EEPROM_WORD_ADDR_SIZE)
  I2C_WriteByte((uint8_t)(Addr&0x00FF));         //数据地址(8位)
#else
  I2C_WriteByte((uint8_t)(Addr>>8));             //数据地址(16位)
  I2C_WriteByte((uint8_t)(Addr&0x00FF));
#endif

  /* 4.重新开始 */
  I2C_Start();

  /* 5.设备地址/读 */
  I2C_WriteByte(EEPROM_DEV_ADDR | EEPROM_RD);

  /* 6.读一字节数据 */
  *Data = I2C_ReadByte(I2C_NOACK);               //只读取1字节(产生非应答)

  /* 7.停止 */
  I2C_Stop();
}

/**************************** 写读NByte ****************************/
/************************************************
函数名称 ： EEPROM_WritePage
功    能 ： EEPROM写页
参    数 ： Addr ---- 地址
            pData --- 数据
            Length --- 长度(<=EEPROM_PAGE_SIZE)
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void EEPROM_WritePage(uint16_t Addr, uint8_t *pData, uint8_t Length)
{
  uint16_t cnt;

  /* 1.开始 */
  I2C_Start();

  /* 2.设备地址/写 */
  I2C_WriteByte(EEPROM_DEV_ADDR | EEPROM_WR);

  /* 3.数据地址 */
#if (8 == EEPROM_WORD_ADDR_SIZE)
  I2C_WriteByte((uint8_t)(Addr&0x00FF));         //数据地址(8位)
#else
  I2C_WriteByte((uint8_t)(Addr>>8));             //数据地址(16位)
  I2C_WriteByte((uint8_t)(Addr&0x00FF));
#endif

  /* 4.连续写数据 */
  for(cnt=0; cnt<(Length-1); cnt++)
  {
    I2C_WriteByte(*pData);
    pData++;
  }
  I2C_WriteByte(*pData);

  /* 5.停止 */
  I2C_Stop();
}

/************************************************
函数名称 ： EEPROM_WriteNByte
功    能 ： EEPROM写N字节
参    数 ： Addr ----- 地址
            pData ---- 数据
            Length --- 长度
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void EEPROM_WriteNByte(uint16_t Addr, uint8_t *pData, uint16_t Length)
{
  uint16_t addr_offset;                          //偏移地址
  uint8_t  num_page = 0;                         //页数(Length字节共多少页)
  uint8_t  num_single = 0;                       //"单"字节数(除了整页外的字节数)
  uint8_t  count = 0;                            //页剩余字节数量(偏移地址 -> 页末)

  addr_offset = Addr % EEPROM_PAGE_SIZE;         //"起始地址"偏移该页地址多少
  count = EEPROM_PAGE_SIZE - addr_offset;        //页剩余字节数量
  num_page = Length / EEPROM_PAGE_SIZE;          //页数
  num_single = Length % EEPROM_PAGE_SIZE;

  /* 1.起始地址未偏移(位于页首地址) */
  if(0 == addr_offset)
  {
    /* 数据量小于1页 */
    if(0 == num_page)
    {
      EEPROM_WritePage(Addr, pData, Length);     //页首地址,写入小于1页的数据
    }
    /* 数据量大于等于1页 */
    else
    {
      while(num_page--)
      {                                          //写num_page页数据
        EEPROM_WritePage(Addr, pData, EEPROM_PAGE_SIZE);
        Addr += EEPROM_PAGE_SIZE;
        pData += EEPROM_PAGE_SIZE;
      }
      if(0 != num_single)
      {                                          //写整页外剩下的字节数
        EEPROM_WritePage(Addr, pData, num_single);
      }
    }
  }

  /* 2.起始地址已偏移(不在页首地址) */
  else
  {
    /* 数据量小于1页 */
    if(0 == num_page)
    {
      /* 不超过该页 */
      if(Length < count)
      {
        EEPROM_WritePage(Addr, pData, Length);   //页偏移地址,写入小于该页的数据
      }
      /* 超过该页 */
      else
      {
        EEPROM_WritePage(Addr, pData, count);    //页偏移地址,写满该页的数据
        Addr += count;
        pData += count;
                                                 //下页首地址,写完剩下的数据
        EEPROM_WritePage(Addr, pData, Length - count);
      }
    }
    /* 数据量大于等于1页 */
    else
    {
      Length -= count;
      num_page = Length / EEPROM_PAGE_SIZE;      //剩下的页数(减去前面写的数)
      num_single = Length % EEPROM_PAGE_SIZE;    //最后一页需要写的字节数

      EEPROM_WritePage(Addr, pData, count);      //页偏移地址,写满该页的数据
      Addr += count;
      pData += count;

      while(num_page--)
      {                                          //写num_page页数据
        TIMDelay_Nms(5);                         //写周期延时5ms
        EEPROM_WritePage(Addr, pData, EEPROM_PAGE_SIZE);
        Addr += EEPROM_PAGE_SIZE;
        pData += EEPROM_PAGE_SIZE;
      }
      if(0 != num_single)
      {                                          //写整页外剩下的字节数
        TIMDelay_Nms(5);
        EEPROM_WritePage(Addr, pData, num_single);
      }
    }
  }
}

/************************************************
函数名称 ： EEPROM_ReadNByte
功    能 ： EEPROM读N字节
参    数 ： Addr ----- 地址
            pData ---- 数据
            Length --- 长度
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void EEPROM_ReadNByte(uint16_t Addr, uint8_t *pData, uint16_t Length)
{
  uint16_t cnt;

  /* 1.开始 */
  I2C_Start();

  /* 2.设备地址/写 */
  I2C_WriteByte(EEPROM_DEV_ADDR | EEPROM_WR);

  /* 3.数据地址 */
#if (8 == EEPROM_WORD_ADDR_SIZE)
  I2C_WriteByte((uint8_t)(Addr&0x00FF));         //数据地址(8位)
#else
  I2C_WriteByte((uint8_t)(Addr>>8));             //数据地址(16位)
  I2C_WriteByte((uint8_t)(Addr&0x00FF));
#endif

  /* 4.重新开始 */
  I2C_Start();

  /* 5.设备地址/读 */
  I2C_WriteByte(EEPROM_DEV_ADDR | EEPROM_RD);

  /* 6.读一字节数据 */
  for(cnt=0; cnt<(Length-1); cnt++)
  {
    *pData = I2C_ReadByte(I2C_ACK);              //连续读取(Length-1)字节(产生应答)
    pData++;
  }
  *pData = I2C_ReadByte(I2C_NOACK);              //读取最后1字节(产生非应答)

  /* 7.停止 */
  I2C_Stop();
}


/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
