/*
 * adc.c
 *
 *  Created on: 2019年2月30日
 *      Author: zengwangfa
 *      Notes:  ADC电压采集   
 */
#define LOG_TAG    "adc"

#include "drv_adc.h"
#include <rtthread.h>
#include <elog.h>
#include <stdio.h>
#include "sys.h"
#include "filter.h"
/*---------------------- Constant / Macro Definitions -----------------------*/		

#define REFER_VOLTAGE       3.3         // 参考电压 3.3V,数据精度乘以100保留2位小数
#define CONVERT_BITS        (1 << 12)   // 转换位数为12位 4096
#define Voltge_Parameter    11				  // 分压系数 1/(200K/(200K+2M)) = 11

/*----------------------- Variable Declarations -----------------------------*/


/*----------------------- Function Implement --------------------------------*/



/* 冒泡 get电压 */
float get_voltage_value(void)
{
		static uint8 i = 0;
		static float voltage = 0.0f;
		static uint32 adc_value[10] = {0};
		
		for(i = 0;i < 10;i++){
				rt_thread_mdelay(10);
				adc_value[i] = get_adc(ADC_Channel_10);//采样
		}
	
		voltage = Bubble_Filter(adc_value) * REFER_VOLTAGE / CONVERT_BITS * Voltge_Parameter;	//电压计算公式：voltage = adc采样值 * 采样精度(3.3V/4096) *分压系数
		return voltage;
} 


/* 冒泡 get电流 */
float get_current_value(void)
{
		static uint8 i = 0;
		static uint32 adc_value[10] = {0};
		static float voltage = 0.0f,current = 0.0f;
		for(i = 0;i < 10;i++){
				rt_thread_mdelay(10);
				adc_value[i] = get_adc(ADC_Channel_11);//采样
		}
	
		voltage = Bubble_Filter(adc_value) * REFER_VOLTAGE / CONVERT_BITS ;	//电压计算公式：voltage = adc采样值 * 采样精度(3.3V/4096) 
		current = voltage * 10;   // Is = Vo*1K/(Rs*Rl) = (Vo*1K)/(0.01*10K) = (10*Vo)详细见INA16 Datasheet
		return current;
}


//获得ADC值
//ch: @ref ADC_channels 
//通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
//返回值:转换结果
u16 get_adc(u8 ch)   
{
		//设置指定ADC的规则组通道，一个序列，采样时间
		ADC_RegularChannelConfig(ADC2, ch, 1, ADC_SampleTime_480Cycles );	//ADC2,ADC通道,480个周期,提高采样时间可以提高精确度			    

		ADC_SoftwareStartConv(ADC2);		//使能指定的ADC2的软件转换启动功能	
		 
		while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ));//等待转换结束

		return ADC_GetConversionValue(ADC2);	//返回最近一次ADC2规则组的转换结果
}


//初始化ADC																	   
int  adc_init(void)
{    
		GPIO_InitTypeDef  GPIO_InitStructure;
		ADC_CommonInitTypeDef ADC_CommonInitStructure;
		ADC_InitTypeDef       ADC_InitStructure;
		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOC时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); //使能ADC2时钟

		 //初始化ADC2通道10 IO口
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;//PC0 通道10    PC1 通道11
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
		GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化  
	 
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	  //ADC2复位
	
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);	//复位结束
		
		ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
		ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
		ADC_CommonInit(&ADC_CommonInitStructure);//初始化
		
		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
		ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1
		ADC_Init(ADC2, &ADC_InitStructure);//ADC2初始化	
		
		ADC_Cmd(ADC2, ENABLE);//开启AD2转换器	
	
		return 1;
}

