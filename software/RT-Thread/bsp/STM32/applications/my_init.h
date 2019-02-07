#ifndef __MY_INIT_H
#define __MY_INIT_H

#include <rtthread.h>

#include "my_variable.h"  //用户变量
#include "my_adc.h"
#include "my_led.h"
#include "my_timer.h"
#include "my_key.h"
#include "my_uart.h"
#include "my_HX711.h"
#include "my_ds18b20.h"
#include "my_oled.h"


#define Heat1_PIN    4
#define Heat2_PIN    9

#define LED2 56
#define LED3 57

#define BEEP_PIN 59
#define Feeding_PIN 61
#define Watering_PIN 62



//-------------------------------定义蜂箱指令信息-------------------------------//

#define SET_WASHING        0x01                         //清洗蜂箱
#define SET_FEED           0x02                         //喂食蜜蜂
#define SET_TEMPTURE       0x03                         //设置温度，加热丝加热
#define RETRUN_MESSAGE     0x04                         //返回蜂箱的信息
#define RETURN_TEMPTURE    0x05                          //返回蜂箱温度值
#define CONTR_BEE_STATE1   0x06                         //开口打开最大全部蜜蜂都可通过
#define CONTR_BEE_STATE2   0x07                         //蜂王禁止通行
#define CONTR_BEE_STATE3   0x08                         //蜂王禁止通行且开口最小



void my_delay_us(unsigned int us);
void my_delay_ms(unsigned int ms);

void buzzer_once(void);
void buzzer_bibi(rt_uint8_t t,rt_uint8_t l);
void buzzer_ring(void);

rt_int32_t Bubble(rt_int32_t *v);
rt_uint16_t Bubble_filter(rt_uint16_t *v,rt_uint8_t NO); //NO为 1是为中值滤波   NO为其他时为 取最小
char *myitoa(int value, char *string, int radix);

#endif



