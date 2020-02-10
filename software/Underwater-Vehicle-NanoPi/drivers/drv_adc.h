/*
 * @Description: 获取电压、电流
 * @Author: chenxi
 * @Date: 2020-02-09 17:25:35
 * @LastEditTime : 2020-02-10 17:09:56
 * @LastEditors  : chenxi
 */

#ifndef __ADC_CONVERT_H_
#define __ADC_CONVERT_H_

#include "../user/DataType.h"

//初始化ADC
int adc_init(void);

uint16 get_adc0(void);         // 获取 adcin0 通道值
uint16 get_adc1(void);         // 获取 adcin1 通道值
float get_voltage_value(void); // 获取电压值
float get_current_value(void); // 获取电流值

#endif
