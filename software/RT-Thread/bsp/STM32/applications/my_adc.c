#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"
#include "my_init.h"

extern rt_uint16_t voltage[3];

#define AD_CHANNEL 10






#define ADC_CHANNEL_MAX		(16) /* 0-15 */
#define __STM32_ADC(index, gpio, gpio_index) {index, GPIO##gpio##_CLK_ENABLE, GPIO##gpio, GPIO_PIN_##gpio_index}

static void GPIOA_CLK_ENABLE(void)
{
#ifdef __HAL_RCC_GPIOA_CLK_ENABLE
    __HAL_RCC_GPIOA_CLK_ENABLE();
#endif
}
static void GPIOB_CLK_ENABLE(void)
{
#ifdef __HAL_RCC_GPIOB_CLK_ENABLE
    __HAL_RCC_GPIOB_CLK_ENABLE();
#endif
}
static void GPIOC_CLK_ENABLE(void)
{
#ifdef __HAL_RCC_GPIOC_CLK_ENABLE
    __HAL_RCC_GPIOC_CLK_ENABLE();
#endif
}

struct adc_channel_index
{
    int index;
    void (*rcc)(void);
    GPIO_TypeDef *gpio;
    uint32_t pin;
};

static const struct adc_channel_index adc1_channels[] =
{
	__STM32_ADC(0, A, 0),
	__STM32_ADC(1, A, 1),
	__STM32_ADC(2, A, 2),
	__STM32_ADC(3, A, 3),
	__STM32_ADC(4, A, 4),
	__STM32_ADC(5, A, 5),
	__STM32_ADC(6, A, 6),
	__STM32_ADC(7, A, 7),
//	__STM32_ADC(8, B, 0),  //PWM
	__STM32_ADC(9, B, 1),

//	__STM32_ADC(11, C, 1), //HEAT2
//	__STM32_ADC(12, C, 2),
	

	__STM32_ADC(13, C, 3),
	__STM32_ADC(10, C, 0), //ADC
	__STM32_ADC(14, C, 4),
	__STM32_ADC(15, C, 5),
};


ADC_HandleTypeDef ADC1_Handler;	//ADC句柄

//初始化ADC
//ch: ADC_channels 
//通道值 0~16取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
void ADC_Init(ADC_HandleTypeDef ADCx_Handler)
{ 
    ADCx_Handler.Instance=ADCx_Handler.Instance;
    ADCx_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4分频，ADCCLK=PCLK2/4=90/4=22.5MHZ
    ADCx_Handler.Init.Resolution=ADC_RESOLUTION_12B;              //8位模式
    ADCx_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //右对齐
    ADCx_Handler.Init.ScanConvMode=DISABLE;                      //非扫描模式
    ADCx_Handler.Init.EOCSelection=DISABLE;                      //关闭EOC中断
    ADCx_Handler.Init.ContinuousConvMode=DISABLE;                //关闭连续转换
    ADCx_Handler.Init.NbrOfConversion=1;                         //1个转换在规则序列中 也就是只转换规则序列1 
    ADCx_Handler.Init.DiscontinuousConvMode=DISABLE;             //禁止不连续采样模式
    ADCx_Handler.Init.NbrOfDiscConversion=0;                     //不连续采样通道数为0
    ADCx_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //软件触发
    ADCx_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;	//使用软件触发
    ADCx_Handler.Init.DMAContinuousRequests=DISABLE;             //关闭DMA请求
    HAL_ADC_Init(&ADCx_Handler);                       //初始化 
}

//ADC底层驱动，引脚配置，时钟使能
//此函数会被HAL_ADC_Init()调用
//hadc:ADC句柄
void _ADC_Init(rt_uint8_t ch)
{
    GPIO_InitTypeDef GPIO_Initure;
    const struct adc_channel_index *index;
	
	
    __HAL_RCC_ADC1_CLK_ENABLE();            //使能ADC1时钟
    adc1_channels[ch].rcc();			//开启GPIOX时钟
	
    GPIO_Initure.Pin = adc1_channels[ch].pin;            //PA4
    GPIO_Initure.Mode = GPIO_MODE_ANALOG;     //模拟
    GPIO_Initure.Pull = GPIO_NOPULL;          //不带上下拉
    HAL_GPIO_Init(adc1_channels[ch].gpio,&GPIO_Initure);
}

rt_uint16_t Get_Adc(rt_uint8_t ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //通道
    ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
    ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //通道配置
	
    HAL_ADC_Start(&ADC1_Handler);                               //开启ADC
	
    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //轮询转换
 
	return (rt_uint16_t)HAL_ADC_GetValue(&ADC1_Handler);	    //返回最近一次ADC1规则组的转换结果
}

rt_err_t convert(struct rt_device *device, int channel, rt_uint16_t *value)
{
	static int old_channel = 0xFF;
	rt_err_t result = RT_EOK;
//	rt_uint16_t temp;

	if(channel > (ADC_CHANNEL_MAX - 1))
	{
		result = -RT_EIO;
		goto _exit;
	}
	if(old_channel != channel)
	{
		_ADC_Init(channel);
		old_channel = channel;
	}
//	temp = Get_Adc(channel);
	*value = Get_Adc(channel); 

_exit:
	return result;
}

static const struct rt_adc_ops adc_ops =
{
	convert,
};


int rt_hw_adc_init(void)
{
    int ret = RT_EOK;
	ADC1_Handler.Instance = ADC1;
	
	ADC_Init(ADC1_Handler);
	
	
	/* add ADC initial. */ 

    ret = rt_device_adc_create("adc1", &adc_ops, RT_NULL);

    return ret;
}
INIT_DEVICE_EXPORT(rt_hw_adc_init);




static rt_size_t _adc_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
	rt_err_t result = RT_EOK;
	rt_size_t i;
    struct rt_device_adc *adc = (struct rt_device_adc *)dev;
	rt_uint16_t *value = (rt_uint16_t *)buffer;
	
	if(!adc->ops->convert)
	{
		return 0;
	}

	for(i=0; i<size; i+=sizeof(rt_uint16_t)/2)
	{
		result = adc->ops->convert(dev, pos + i, value);
		if(result != RT_EOK)
		{
			return 0;
		}
		value++;
	}

    return i;
}

rt_err_t rt_device_adc_register(struct rt_device_adc *device, const char *name, const struct rt_adc_ops *ops, const void *user_data)
{
	rt_err_t result = RT_EOK;
	
	memset(device, 0, sizeof(struct rt_device_adc));

    device->parent.type         = RT_Device_Class_Miscellaneous;

    device->parent.init         = RT_NULL;
    device->parent.open         = RT_NULL;
    device->parent.close        = RT_NULL;
    device->parent.read         = _adc_read;
    device->parent.write        = RT_NULL;
    device->parent.control      = RT_NULL;

    device->ops                 = ops;
    device->parent.user_data    = (void *)user_data;

    result = rt_device_register(&device->parent, name, RT_DEVICE_FLAG_RDWR);

    return result;
}

rt_err_t rt_device_adc_create(const char *name, const struct rt_adc_ops *ops, const void *user_data)
{
	rt_err_t result = RT_EOK;
	struct rt_device_adc *device;
	
	device = rt_malloc(sizeof(struct rt_device_adc));
	if(!device)
	{
		result = -RT_ENOMEM;
		goto _exit;
	}
	
	result = rt_device_adc_register(device, name, ops, user_data);
	
_exit:	
    return result;
}





void voltage_get_val(void* parameter)
{
		rt_device_t dev;
		rt_uint8_t i;
		rt_uint16_t temp = 0;
		rt_uint16_t var[10] = {0};

		
		dev = rt_device_find("adc1");
		rt_device_open(dev, RT_DEVICE_FLAG_RDONLY);
		while(1){
			for(i = 0;i < 10;i++){
				if( rt_device_read(dev, AD_CHANNEL, &temp, sizeof(temp) ) != sizeof(temp) ){  //12位精度
						rt_kprintf("channel %d: faild! \n", AD_CHANNEL);
				}
						var[i] = temp;
						rt_thread_delay(1);             
			}
			temp = Bubble_filter(var,1); //中值滤波

			voltage[0] = (temp*100+44800)/227;  //线性拟合
			voltage[1] = voltage[0]/100;  //整数位
			voltage[2] = voltage[0]%100;  //小数位
			
			rt_thread_delay(10);
		}
        
}

int voltage_collect(void)
{
    rt_thread_t tid;
        tid = rt_thread_create("Voltage_Collect",voltage_get_val, RT_NULL,1024,15, 20);
        if (tid != RT_NULL)
                rt_thread_startup(tid);
        
        return 0;
}
INIT_APP_EXPORT(voltage_collect);

