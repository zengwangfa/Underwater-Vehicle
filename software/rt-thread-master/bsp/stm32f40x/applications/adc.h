#ifndef __ADC_CONVERT_H_
#define __ADC_CONVERT_H_

#include "DataType.h"



//初始化ADC															   
void  adc_init(void);

uint16 get_adc(uint8 ch);  //get adc通道值

float get_vol(void); //get 电压值

void get_voltage(void);


extern float  volatge;

#endif


