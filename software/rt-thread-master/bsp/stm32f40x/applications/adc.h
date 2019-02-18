#ifndef __ADC_CONVERT_H
#define __ADC_CONVERT_H

#include "init.h"


//初始化ADC															   
void  adc_init(void);

u16 get_adc(u8 ch);  //get adc通道值

double get_vol(void);//返回电压值

void get_voltage(void);




#endif


