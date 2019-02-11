#ifndef __ADC_CONVERT_H
#define __ADC_CONVERT_H

#include "init.h"


//≥ı ºªØADC															   
void  adc_init(void);

u16 get_adc(u8 ch);

u16 get_vol(u8 ch,u8 times);





#endif


