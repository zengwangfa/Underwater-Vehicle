#ifndef __ADC_CONVERT_H_
#define __ADC_CONVERT_H_

#include "init.h"

extern float  volatge;


//初始化ADC															   
void  adc_init(void);

u16 get_adc(u8 ch);  //get adc通道值

float get_vol(void); //get 电压值

void get_voltage(void);




#endif


