#ifndef __INIT_H_
#define __INIT_H_


#include <rtdevice.h>

#include "sys.h" 
#include "led.h"
#include "key.h"
#include "buzzer.h"
#include "drv_oled.h"
#include "oled.h"
#include "gyroscope.h"
#include "adc.h"
#include "pwm.h"
#include "debug.h"


#include "w25qxx.h"



#include "ulog.h"

/*---------------------- Constant / Macro Definitions -----------------------*/		

/************重要定义****************/

#define MODE  ROV_Mode

/************重要定义***************/


#define LED_EVENT 			(1 << 0)  //LED事件标志位
#define KEY_EVENT 			(1 << 1)  //KEY事件标志位
#define BUZZ_EVENT 			(1 << 2)  //BUZZER事件标志位
#define OLED_EVENT 			(1 << 3)  //OLED事件标志位
#define GYRO_EVENT 			(1 << 4)  //Gyroscope事件标志位
#define ADC_EVENT 			(1 << 5)  //ADC事件标志位
#define PWM_EVENT 			(1 << 6)  //PWM事件标志位
#define W25Q128_EVENT   (1 << 7)  //W25Q128事件标志位








enum VehicleStatus{
	System_NULL = 1,
	System_Er,
	System_OK

};  //枚举系统状态


enum VehicleMode{
	AUV_Mode,
	ROV_Mode
};  //枚举系统模式


//void delay_us(unsigned int us);
//void delay_ms(unsigned int ms);

void delay_us(u32 nTimer);
void delay_ms(u32 nTimer);
void rt_hw_us_delay(rt_uint32_t us);

short bubble(short *adc_value);

int sprintf(char *buf, const char *format, ...);

#endif



