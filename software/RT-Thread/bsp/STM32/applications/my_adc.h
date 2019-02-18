#ifndef __DRV_ADC_H_INCLUDE__
#define __DRV_ADC_H_INCLUDE__

#include <rtthread.h>

struct rt_adc_ops
{
    rt_err_t (*convert)(struct rt_device *device, int channel, rt_uint16_t *value);
};

struct rt_device_adc
{
    struct rt_device parent;
    const struct rt_adc_ops *ops;
};

extern rt_err_t rt_device_adc_register(struct rt_device_adc *device, const char *name, const struct rt_adc_ops *ops, const void *user_data);

extern rt_err_t rt_device_adc_create(const char *name, const struct rt_adc_ops *ops, const void *user_data);

#endif /* __DRV_ADC_H_INCLUDE__ */










//#ifndef __MY_ADC_H
//#define __MY_ADC_H




//#include <rtthread.h>

//struct rt_adc_device;
//struct rt_adc_ops
//{
//    rt_err_t (*enabled)(struct rt_adc_device *device, rt_uint32_t channel, rt_bool_t enabled);
//    rt_err_t (*convert)(struct rt_adc_device *device, rt_uint32_t channel, rt_uint32_t *value);
//};

//struct rt_adc_device
//{
//    struct rt_device parent;
//    const struct rt_adc_ops *ops;
//};
//typedef struct rt_adc_device *rt_adc_device_t;

//typedef enum
//{
//    RT_ADC_CMD_ENABLE, //0
//    RT_ADC_CMD_DISABLE,
//} rt_adc_cmd_t;

//rt_err_t rt_hw_adc_register(rt_adc_device_t adc,const char *name, const struct rt_adc_ops *ops, const void *user_data);

//rt_uint32_t rt_adc_read(rt_adc_device_t dev, rt_uint32_t channel);
//rt_err_t rt_adc_enable(rt_adc_device_t dev, rt_uint32_t channel);
//rt_err_t rt_adc_disable(rt_adc_device_t dev, rt_uint32_t channel);




//#endif 
