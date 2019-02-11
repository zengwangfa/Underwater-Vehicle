#ifndef __INIT_H
#define __INIT_H

#include <rtdevice.h>

#include "sys.h" 
#include "led.h"
#include "key.h"
#include "buzzer.h"
#include "drv_oled.h"
#include "oled.h"

#include "gyroscope.h"
#include "ulog.h"
#include "adc_convert.h"

int sprintf(char *buf, const char *format, ...);

enum VehicleStatus{
	System_NULL = 1,
	System_Er,
	System_OK

};  //枚举系统状态


enum VehicleMode{
	AUV_Mode = 1,
	ROV_Mode,
};  //枚举系统模式

//void delay_us(unsigned int us);
//void delay_ms(unsigned int ms);

void delay_us(u32 nTimer);
void delay_ms(u32 nTimer);
void rt_hw_us_delay(rt_uint32_t us);
#endif



