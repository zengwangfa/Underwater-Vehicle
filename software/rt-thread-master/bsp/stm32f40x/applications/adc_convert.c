#include "adc_convert.h"


/*----------------------- Variable Declarations -----------------------------*/




/*----------------------- Function Implement --------------------------------*/

void adc_thread_entry(void *parameter)
{

		adc_init();
		LOG_I("adc_init()");
	
		while(1)
		{
				//get_vol(ADC_Channel_10,1);
				//LOG_E("volage:%d",get_voltage(ADC_Channel_10,10));
				rt_thread_mdelay(1000);

		}
	
}


int adc_thread_init(void)
{
    rt_thread_t adc_tid;
		/*创建动态线程*/
    adc_tid = rt_thread_create("adc",				 //线程名称
                    adc_thread_entry,				 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    512,										 //线程栈大小，单位是字节【byte】
                    10,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (adc_tid != RT_NULL)
     rt_thread_startup(adc_tid);
		return 0;
}
INIT_APP_EXPORT(adc_thread_init);




u16 get_vol(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t=0;
	for(t=0;t<times;t++)
	{
		temp_val+=get_adc(ch);
	//	delay_ms(1);
	}
	return temp_val/times;
} 





void get_voltage(void)
{
	u32 temp_val=0;
	u8 t=0;
	char str[128];
	for(t=0;t<5;t++)
	{
		temp_val+=get_adc(ADC_Channel_10);

	}
	
	sprintf(str,"voltage:%.3f\r\n",(float)temp_val);
	rt_kprintf(str);
	
} 
MSH_CMD_EXPORT(get_voltage,get voltage[u]);


//获得ADC值
//ch: @ref ADC_channels 
//通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
//返回值:转换结果
u16 get_adc(u8 ch)   
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC2, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
  
	ADC_SoftwareStartConv(ADC2);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC2);	//返回最近一次ADC1规则组的转换结果
}



//初始化ADC															   
void  adc_init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); //使能ADC1时钟

	 //初始化ADC1通道10 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PC0 通道10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);	//复位结束	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
  ADC_Init(ADC2, &ADC_InitStructure);//ADC初始化
	
	ADC_Cmd(ADC2, ENABLE);//开启AD转换器	

}		  











