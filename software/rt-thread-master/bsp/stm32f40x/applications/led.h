#ifndef __LED_H
#define __LED_H

#include "init.h"

#define LEDR PEout(15)
#define LEDG PBout(10)
#define LEDB PBout(11)

//RGB灯共阳极接到+3.3V，电平 0亮 1灭
#define LED_ON(led_pin) 						rt_pin_write(led_pin ,PIN_LOW )
#define LED_OFF(led_pin) 						rt_pin_write(led_pin ,PIN_HIGH)
#define LED_Turn(led_pin,status) 		rt_pin_write(led_pin ,status =! status) //取反


/* 指示灯 全熄灭 */
void All_LED_OFF(void);
void Error_LED(void);
void led_blink_task(void);
void system_led_blink(rt_uint8_t InputData);



#endif


