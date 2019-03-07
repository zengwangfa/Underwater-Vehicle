#ifndef __LED_H
#define __LED_H

#include "init.h"

#define LEDR PEout(15)
#define LEDG PBout(10)
#define LEDB PBout(11)

void LED_ON(u32 led_pin);
void LED_OFF(u32 led_pin);
void LED_Turn(u32 led_pin);


/* Ö¸Ê¾µÆ È«Ï¨Ãð */
void All_LED_OFF(void);
void Error_LED(void);
void led_blink_task(void);
void system_led_blink(rt_uint8_t InputData);



#endif


