/*
 * @Description: 通过 I2C 控制 PCA9685 来使用 PWM 功能
 * @Author: chenxi
 * @Date: 2020-02-10 12:15:34
 * @LastEditTime : 2020-02-10 17:04:47
 * @LastEditors  : chenxi
 */

#define LOG_TAG "I2C_PWM"

#include "../easylogger/inc/elog.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <errno.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "I2C_PWM.h"

static int i2c_pwm_fd;

/**
 * @description: 初始化 I2C 和 PWM
 * @param : 
 * @return: 文件描述符 i2c_pwm_fd
 */
int I2C_PWM_Init(void)
{
  log_i("[%s %s] [%s: %s: %d]", __DATE__, __TIME__, __FILE__, __func__, __LINE__);
  // I2C0_OE，即 GPIOG11 低电平使能
  pinMode(I2C0_OE, OUTPUT);
  digitalWrite(I2C0_OE, LOW);

  i2c_pwm_fd = wiringPiI2CSetupInterface(I2C_PWM_Device, I2C_PWM_Addr);
  log_d("i2c_pwm_fd:%d", i2c_pwm_fd);

  if (i2c_pwm_fd < 0)
  {
    log_e("I2C_PWM_Init failed");
  }

  I2C_PWM_Reset();
  return i2c_pwm_fd;
}

void I2C_PWM_Reset(void)
{
  wiringPiI2CWriteReg8(i2c_pwm_fd, 0x0, 0x0);
}

void I2C_PWM_SetPWMFreq(float freq)
{
  freq *= 0.9; // Correct for overshoot in the frequency setting (see issue #11).
  float prescaleval = 25000000;
  prescaleval /= 4096;
  prescaleval /= freq;
  prescaleval -= 1;
  uint16 prescale = floor(prescaleval + 0.5);

  log_i("[%s %s] %s: %s: %d", __DATE__, __TIME__, __FILE__, __func__, __LINE__);
  log_d("Estimated pre-scale: %f", prescaleval);
  log_d("Final pre-scale: %d", prescale);

  uint16 oldmode = wiringPiI2CReadReg8(i2c_pwm_fd, PCA9685_MODE1);
  uint16 newmode = (oldmode & 0x7F) | 0x10;                     // sleep
  wiringPiI2CWriteReg8(i2c_pwm_fd, PCA9685_MODE1, newmode);     // go to sleep
  wiringPiI2CWriteReg8(i2c_pwm_fd, PCA9685_PRESCALE, prescale); // set the prescaler
  wiringPiI2CWriteReg8(i2c_pwm_fd, PCA9685_MODE1, oldmode);
  delay(5);
  wiringPiI2CWriteReg8(i2c_pwm_fd, PCA9685_MODE1, oldmode | 0xa1); //  This sets the MODE1 register to turn on auto increment.
  // This is why the beginTransmission below was not working.
}

void I2C_PWM_SetPWM(uint16 num, uint32 on, uint32 off)
{
  int reg = LED0_ON_L + 4 * num;

  wiringPiI2CWriteReg8(i2c_pwm_fd, reg, on);
  wiringPiI2CWriteReg8(i2c_pwm_fd, reg + 1, on >> 8);
  wiringPiI2CWriteReg8(i2c_pwm_fd, reg + 2, off);
  wiringPiI2CWriteReg8(i2c_pwm_fd, reg + 3, off >> 8);

  log_d("SetPWM %2d, on %4d, off %4d", num, on, off);
}

// Sets pin without having to deal with on/off tick placement and properly handles
// a zero value as completely off.  Optional invert parameter supports inverting
// the pulse for sinking to ground.  Val should be a value from 0 to 4095 inclusive.

// unsigned char invert bool值
void I2C_PWM_SetPin(uint16 num, uint16 val, uint16 invert)
{
  // TODO

  /*
  // Clamp value between 0 and 4095 inclusive.
  val = min(val, 4095);
  if (invert)
  {
    if (val == 0)
    {
      // Special value for signal fully on.
      setPWM(num, 4096, 0);
    }
    else if (val == 4095)
    {
      // Special value for signal fully off.
      setPWM(num, 0, 4096);
    }
    else
    {
      setPWM(num, 0, 4095 - val);
    }
  }
  else
  {
    if (val == 4095)
    {
      // Special value for signal fully on.
      setPWM(num, 4096, 0);
    }
    else if (val == 0)
    {
      // Special value for signal fully off.
      setPWM(num, 0, 4096);
    }
    else
    {
      setPWM(num, 0, val);
    }
  }
  */
}
