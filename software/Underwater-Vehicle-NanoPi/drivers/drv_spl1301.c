/*
 * @Description: 深度传感器相关函数
 * @Author: chenxi
 * @Date: 2020-01-14 16:52:11
 * @LastEditTime : 2020-02-10 17:10:51
 * @LastEditors  : chenxi
 */

#define LOG_TAG "spl1301"

#include "../easylogger/inc/elog.h"
#include "drv_spl1301.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <errno.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>

static struct spl1301_t spl1301;
static struct spl1301_t *p_spl1301;
static int spl1301_fd;

void spl1301_get_calib_param(void);

/*****************************************************************************
 Function: spl1301_init
 Description: initialization
 Input: void   
 Output: 
 Return: fd 初始化成功,0初始化失败
 Calls: 
 Called By: 
*****************************************************************************/
int spl1301_init(void)
{
    spl1301_fd = 0;
    spl1301_fd = wiringPiI2CSetupInterface(spl1301_I2C, HW_ADR);

    log_i("[%s %s] [%s: %s: %d]", __DATE__, __TIME__, __FILE__, __func__, __LINE__);
    log_d("spl1301_fd:%d", spl1301_fd);

    if (spl1301_fd < 0)
    {
        log_e("spl1301 init failed");
        return -1;
    }

    p_spl1301 = &spl1301; /* read Chip Id */
    p_spl1301->i32rawPressure = 0;
    p_spl1301->i32rawTemperature = 0;
    p_spl1301->chip_id = 0x10;
    spl1301_get_calib_param();
    // sampling rate = 1Hz; Pressure oversample = 2;
    spl1301_rateset(PRESSURE_SENSOR, 32, 8);
    // sampling rate = 1Hz; Temperature oversample = 1;
    spl1301_rateset(TEMPERATURE_SENSOR, 32, 8);
    //Start background measurement
    spl1301_start_continuous(CONTINUOUS_P_AND_T); //后台模式(开启转换 气压 及 温度)

    if (-1 == spl1301.calib_param.c0)
    { //当为-1时，初始化失败(为接入SPL1301 or 接入不是SPL1301)
        return 0;
    }
    return spl1301_fd;
}

/*****************************************************************************
 Function: spl1301_rateset
 Description: set sample rate and over sample rate per second for specific sensor
 Input:   uint8 u8OverSmpl  oversample rate   Maximal = 128
  uint8 u8SmplRate  sample rate(Hz) Maximal = 128
  uint8 iSensor   0: Pressure; 1: Temperature 
 Output: 
 Return: void
 Calls: 
 Called By: 
*****************************************************************************/
void spl1301_rateset(uint8 iSensor, uint8 u8SmplRate, uint8 u8OverSmpl)
{
    uint8 reg = 0;
    int32 i32kPkT = 0;
    switch (u8SmplRate)
    {
    case 2:
        reg |= (1 << 4);
        break;
    case 4:
        reg |= (2 << 4);
        break;
    case 8:
        reg |= (3 << 4);
        break;
    case 16:
        reg |= (4 << 4);
        break;
    case 32:
        reg |= (5 << 4);
        break;
    case 64:
        reg |= (6 << 4);
        break;
    case 128:
        reg |= (7 << 4);
        break;
    case 1:
    default:
        break;
    }
    switch (u8OverSmpl)
    {
    case 2:
        reg |= 1;
        i32kPkT = 1572864;
        break;
    case 4:
        reg |= 2;
        i32kPkT = 3670016;
        break;
    case 8:
        reg |= 3;
        i32kPkT = 7864320;
        break;
    case 16:
        i32kPkT = 253952;
        reg |= 4;
        break;
    case 32:
        i32kPkT = 516096;
        reg |= 5;
        break;
    case 64:
        i32kPkT = 1040384;
        reg |= 6;
        break;
    case 128:
        i32kPkT = 2088960;
        reg |= 7;
        break;
    case 1:
    default:
        i32kPkT = 524288;
        break;
    }

    if (iSensor == PRESSURE_SENSOR)
    {
        p_spl1301->i32kP = i32kPkT;
        wiringPiI2CWriteReg8(spl1301_fd, 0x06, reg);
        if (u8OverSmpl > 8)
        {
            reg = wiringPiI2CReadReg8(spl1301_fd, 0x09);
            wiringPiI2CWriteReg8(spl1301_fd, 0x09, reg | 0x04);
        }
        else
        {
            reg = wiringPiI2CReadReg8(spl1301_fd, 0x09);
            wiringPiI2CWriteReg8(spl1301_fd, 0x09, reg & (~0x04));
        }
    }
    if (iSensor == TEMPERATURE_SENSOR)
    {
        p_spl1301->i32kT = i32kPkT;
        wiringPiI2CWriteReg8(spl1301_fd, 0x07, reg | 0x80); //Using mems temperature
        if (u8OverSmpl > 8)
        {
            reg = wiringPiI2CReadReg8(spl1301_fd, 0x09);
            wiringPiI2CWriteReg8(spl1301_fd, 0x09, reg | 0x08);
        }
        else
        {
            reg = wiringPiI2CReadReg8(spl1301_fd, 0x09);
            wiringPiI2CWriteReg8(spl1301_fd, 0x09, reg & (~0x08));
        }
    }
}

/*****************************************************************************
 Function: spl1301_get_calib_param
 Description: obtain the calibrated coefficient
 Input: void   
 Output: 
 Return: void
 Calls: 
 Called By: 
*****************************************************************************/
void spl1301_get_calib_param(void)
{
    uint32 h;
    uint32 m;
    uint32 l;
    h = wiringPiI2CReadReg8(spl1301_fd, 0x10);
    l = wiringPiI2CReadReg8(spl1301_fd, 0x11);
    p_spl1301->calib_param.c0 = (int16)h << 4 | l >> 4;
    p_spl1301->calib_param.c0 = (p_spl1301->calib_param.c0 & 0x0800) ? (0xF000 | p_spl1301->calib_param.c0) : p_spl1301->calib_param.c0;
    h = wiringPiI2CReadReg8(spl1301_fd, 0x11);
    l = wiringPiI2CReadReg8(spl1301_fd, 0x12);
    p_spl1301->calib_param.c1 = (int16)(h & 0x0F) << 8 | l;
    p_spl1301->calib_param.c1 = (p_spl1301->calib_param.c1 & 0x0800) ? (0xF000 | p_spl1301->calib_param.c1) : p_spl1301->calib_param.c1;
    h = wiringPiI2CReadReg8(spl1301_fd, 0x13);
    m = wiringPiI2CReadReg8(spl1301_fd, 0x14);
    l = wiringPiI2CReadReg8(spl1301_fd, 0x15);
    p_spl1301->calib_param.c00 = (int32)h << 12 | (int32)m << 4 | (int32)l >> 4;
    p_spl1301->calib_param.c00 = (p_spl1301->calib_param.c00 & 0x080000) ? (0xFFF00000 | p_spl1301->calib_param.c00) : p_spl1301->calib_param.c00;
    h = wiringPiI2CReadReg8(spl1301_fd, 0x15);
    m = wiringPiI2CReadReg8(spl1301_fd, 0x16);
    l = wiringPiI2CReadReg8(spl1301_fd, 0x17);
    p_spl1301->calib_param.c10 = (int32)(h & 0x0F) << 16 | (int32)m << 8 | l;
    p_spl1301->calib_param.c10 = (p_spl1301->calib_param.c10 & 0x080000) ? (0xFFF00000 | p_spl1301->calib_param.c10) : p_spl1301->calib_param.c10;
    h = wiringPiI2CReadReg8(spl1301_fd, 0x18);
    l = wiringPiI2CReadReg8(spl1301_fd, 0x19);
    p_spl1301->calib_param.c01 = (int16)h << 8 | l;
    h = wiringPiI2CReadReg8(spl1301_fd, 0x1A);
    l = wiringPiI2CReadReg8(spl1301_fd, 0x1B);
    p_spl1301->calib_param.c11 = (int16)h << 8 | l;
    h = wiringPiI2CReadReg8(spl1301_fd, 0x1C);
    l = wiringPiI2CReadReg8(spl1301_fd, 0x1D);
    p_spl1301->calib_param.c20 = (int16)h << 8 | l;
    h = wiringPiI2CReadReg8(spl1301_fd, 0x1E);
    l = wiringPiI2CReadReg8(spl1301_fd, 0x1F);
    p_spl1301->calib_param.c21 = (int16)h << 8 | l;
    h = wiringPiI2CReadReg8(spl1301_fd, 0x20);
    l = wiringPiI2CReadReg8(spl1301_fd, 0x21);
    p_spl1301->calib_param.c30 = (int16)h << 8 | l;
}

/*****************************************************************************
 Function: spl1301_start_temperature
 Description: start one measurement for temperature
 Input: void  
 Output: 
 Return: void
 Calls: 
 Called By: 
*****************************************************************************/
void spl1301_start_temperature(void)
{
    wiringPiI2CWriteReg8(spl1301_fd, 0x08, 0x02);
}

/*****************************************************************************
 Function: spl1301_start_pressure
 Description: start one measurement for pressure
 Input: void   
 Output: 
 Return: void
 Calls: 
 Called By: 
*****************************************************************************/

void spl1301_start_pressure(void)
{
    wiringPiI2CWriteReg8(spl1301_fd, 0x08, 0x01);
}
/*****************************************************************************
 Function: spl1301_start_continuous
 Description: Select mode for the continuously measurement
 Input: uint8 mode  1: pressure; 2: temperature; 3: pressure and temperature  
 Output: 
 Return: void
 Calls: 
 Called By: 
*****************************************************************************/
void spl1301_start_continuous(uint8 mode)
{
    wiringPiI2CWriteReg8(spl1301_fd, 0x08, mode + 4);
}

void spl1301_stop(void)
{
    wiringPiI2CWriteReg8(spl1301_fd, 0x08, 0);
}

/*****************************************************************************
 Function: spl1301_get_raw_temp
 Description:obtain the original temperature value and turn them into 32bits-integer 
 获取原始温度值并将其转换为32位整数
 Input: void  
 Output: 
 Return: void
 Calls: 
 Called By: 
*****************************************************************************/
void spl1301_get_raw_temp(void)
{
    uint8 h, m, l;

    h = wiringPiI2CReadReg8(spl1301_fd, 0x03);
    m = wiringPiI2CReadReg8(spl1301_fd, 0x04);
    l = wiringPiI2CReadReg8(spl1301_fd, 0x05);

    p_spl1301->i32rawTemperature = (int32)h << 16 | (int32)m << 8 | (int32)l;
    p_spl1301->i32rawTemperature = (p_spl1301->i32rawTemperature & 0x800000) ? (0xFF000000 | p_spl1301->i32rawTemperature) : p_spl1301->i32rawTemperature;
}

/*****************************************************************************
 Function: spl1301_get_raw_pressure
 Description: obtain the original pressure value and turn them into 32bits-integer
 Input: void   
 Output: 
 Return: void
 Calls: 
 Called By: 
*****************************************************************************/
void spl1301_get_raw_pressure(void)
{
    uint8 h, m, l;

    h = wiringPiI2CReadReg8(spl1301_fd, 0x00);
    m = wiringPiI2CReadReg8(spl1301_fd, 0x01);
    l = wiringPiI2CReadReg8(spl1301_fd, 0x02);

    p_spl1301->i32rawPressure = (int32)h << 16 | (int32)m << 8 | (int32)l;
    p_spl1301->i32rawPressure = (p_spl1301->i32rawPressure & 0x800000) ? (0xFF000000 | p_spl1301->i32rawPressure) : p_spl1301->i32rawPressure;
}

/*****************************************************************************
 Function: spl1301_get_temperature
 Description:  return calibrated temperature value base on original value.
 Input: void  
 Output: 
 Return: void
 Calls: 
 Called By: 
*****************************************************************************/
float get_spl1301_temperature(void)
{
    float fTCompensate;
    float fTsc;

    fTsc = p_spl1301->i32rawTemperature / (float)p_spl1301->i32kT;
    fTCompensate = p_spl1301->calib_param.c0 * 0.5 + p_spl1301->calib_param.c1 * fTsc;

    return fTCompensate;
}

/*****************************************************************************
 Function: spl1301_get_pressure
 Description: return calibrated pressure value base on original value.
 Input: void  
 Output: 
 Return: void
 Calls: 
 Called By: 
*****************************************************************************/

float get_spl1301_pressure(void)
{
    float fTsc, fPsc;
    float qua2, qua3;
    float fPCompensate;

    fTsc = p_spl1301->i32rawTemperature / (float)p_spl1301->i32kT;
    fPsc = p_spl1301->i32rawPressure / (float)p_spl1301->i32kP;
    qua2 = p_spl1301->calib_param.c10 + fPsc * (p_spl1301->calib_param.c20 + fPsc * p_spl1301->calib_param.c30);
    qua3 = fTsc * fPsc * (p_spl1301->calib_param.c11 + fPsc * p_spl1301->calib_param.c21);

    fPCompensate = p_spl1301->calib_param.c00 + fPsc * qua2 + fTsc * p_spl1301->calib_param.c01 + qua3;

    return fPCompensate;
}
