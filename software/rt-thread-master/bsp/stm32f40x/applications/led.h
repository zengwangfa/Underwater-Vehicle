#ifndef __LED_H
#define __LED_H

#include <drivers/pin.h>

#define LEDR PEout(15)
#define LEDG PBout(10)
#define LEDB PBout(11)



void led_blink_task(void);




#endif


