/*
 * @Description: 通过 I2C 控制 PCA9685 来使用 PWM 功能
 * @Author: chenxi
 * @Date: 2020-02-10 12:15:34
 * @LastEditTime : 2020-02-10 16:36:03
 * @LastEditors  : chenxi
 */

#ifndef __I2C_PWM__
#define __I2C_PWM__

#include "..//user//DataType.h"

#define I2C_PWM_Device "/dev/i2c-0" // PCA9685 使用的 I2C
#define I2C_PWM_Addr 0x40   // 默认 PCA9685 地址
#define I2C0_OE 7   // I2C0_OE

#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD

int I2C_PWM_Init(void);
void I2C_PWM_Reset(void);
void I2C_PWM_SetPWMFreq(float freq);
void I2C_PWM_SetPWM(uint16 num, uint32 on, uint32 off);

#endif
