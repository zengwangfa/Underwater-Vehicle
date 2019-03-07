/*
 * File      : board.h

 */
#define LOG_TAG "drv_cpu_temp"

#include "board.h"
#include <rtthread.h>
#include <elog.h>
#include <string.h>
#include <stdio.h>
#include "drv_cpu_temp.h"
/**
 * 获取ADC值函数
 *
 * @param ch 通道
 * @return ADC数值
 */
static uint16_t get_adc(u8 ch) {  //对于 STM32F40x 和 STM32F41x 器件，温度传感器内部连接到通道 ADC1_IN16
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_112Cycles );//手册第251页
    ADC_SoftwareStartConv(ADC1);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
    return ADC_GetConversionValue(ADC1);
}

/**
 * 获取 CPU 温度函数
 *
 * @return CPU温度值
 */
uint32_t get_cpu_temp(void) {
		char str[100] = {0}; 
    uint32_t adc[10] = { 0 }, adc_max = 0, adc_min = 0;
    uint32_t adc_value = 0, adc_sum = 0;
    double temperature = 0.0;

    for (uint8_t t = 0; t < 10; t++) {
        adc[t] = get_adc(ADC_Channel_16);
        adc_sum = adc_sum + adc[t];
        rt_thread_delay(rt_tick_from_millisecond(5));
    }
    adc_max = adc[0];
    adc_min = adc[0];
    for (uint8_t t = 1; t < 10; t++) {
        if (adc_max < adc[t]) {
            adc_max = adc[t];
        }
        if (adc_min > adc[t]) {
            adc_min = adc[t];
        }
    }

    adc_value = ((adc_sum - adc_max - adc_min) / 8);//减去最大最小值 取平均
    temperature = (float) adc_value * (3.3 / 4096);
    temperature = (temperature - 0.76) / 0.0025 + 25;

		sprintf(str,"STM32 CPU Temperature : %.2f C",temperature);
		log_v(str);
    return (uint32_t)(temperature);
}
MSH_CMD_EXPORT(get_cpu_temp,cpu temperature get);

/**
 * CPU 温度 ADC 硬件初始化
 */
int stm32_cpu_temperature_init(void) {
  
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);
    ADC_TempSensorVrefintCmd(ENABLE);

    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_112Cycles);
    ADC_Cmd(ADC1, ENABLE);

    return 0;
}
INIT_BOARD_EXPORT(stm32_cpu_temperature_init);
