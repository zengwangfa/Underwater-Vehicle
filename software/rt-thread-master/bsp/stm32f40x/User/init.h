#ifndef __INIT_H_
#define __INIT_H_



#include "sys.h" 

#include "led.h"
#include "key.h"
#include "buzzer.h"
#include "drv_oled.h"
#include "oled.h"
#include "gyroscope.h"
#include "adc.h"
#include "pwm.h"

#include "DataType.h"


/*---------------------- Constant / Macro Definitions -----------------------*/		

/************重要定义****************/





//void delay_us(unsigned int us);
//void delay_ms(unsigned int ms);

//void delay_us(u32 nTimer);
//void delay_ms(u32 nTimer);
void rt_hw_us_delay(uint32 us);
void rt_hw_ms_delay(uint32 ms);
void delay_us(uint32 nTimer);

int sprintf(char *buf, const char *format, ...);


#endif



