/**
  **********************************  STM8S  ***********************************
  * @文件名     ： bsp_i2c.h
  * @作者       ： strongerHuang
  * @库版本     ： V2.2.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年06月29日
  * @摘要       ： I2C驱动头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _BSP_I2C_H
#define _BSP_I2C_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm8s.h"


/* 宏定义 --------------------------------------------------------------------*/
#define PORT_I2C_SCL              GPIOB
#define PORT_I2C_SDA              GPIOB

#define PIN_I2C_SCL               GPIO_PIN_4
#define PIN_I2C_SDA               GPIO_PIN_5

#define I2C_SCL_LOW               (PORT_I2C_SCL->ODR &= (uint8_t)(~PIN_I2C_SCL))
#define I2C_SCL_HIGH              (PORT_I2C_SCL->ODR |= (uint8_t)PIN_I2C_SCL)
#define I2C_SDA_LOW               (PORT_I2C_SDA->ODR &= (uint8_t)(~PIN_I2C_SDA))
#define I2C_SDA_HIGH              (PORT_I2C_SDA->ODR |= (uint8_t)PIN_I2C_SDA)

#define I2C_SDA_READ              (PORT_I2C_SDA->IDR & (uint8_t)PIN_I2C_SDA)

#define I2C_ACK                   0                        //应答
#define I2C_NOACK                 1                        //非应答

/* 函数申明 ------------------------------------------------------------------*/
void I2C_Initializes(void);

void I2C_Start(void);
void I2C_Stop(void);
void I2C_PutAck(uint8_t Ack);
uint8_t I2C_GetAck(void);

void I2C_WriteByte(uint8_t Data);
uint8_t I2C_ReadByte(uint8_t ack);


#endif /* _BSP_I2C_H */

/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
