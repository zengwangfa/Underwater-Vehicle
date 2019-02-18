/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_i2c.h
 * @brief      i2c驱动头文件
 * @author     山外科技
 * @version    v5.0
 * @date       2013-07-12
 * @note       目前仅实现主机读写寄存器功能，其他功能待实现
 */

#ifndef     __MK60_I2C_H__
#define     __MK60_I2C_H__
/**
 *  @brief I2C模块编号
 */
typedef enum I2Cn
{
    I2C0  = 0,
    I2C1  = 1
} I2Cn_e;

/**
 *  @brief 主机读写模式选择
 */
typedef enum MSmode
{
    MWSR =   0x00,  /* 主机写模式  */
    MRSW =   0x01   /* 主机读模式  */
} MSmode;


uint32  i2c_init(I2Cn_e i2cn, uint32 baud);                               //初始化I2C
void    i2c_write_reg(I2Cn_e, uint8 SlaveID, uint8 reg, uint8 Data);      //往地址里的写内容
uint8   i2c_read_reg (I2Cn_e, uint8 SlaveID, uint8 reg);                  //从地址里读取内容

#endif  //__MK60_I2C_H__
