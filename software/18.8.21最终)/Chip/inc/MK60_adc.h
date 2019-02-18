/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_adc.h
 * @brief      ADC函数
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-28
 */


#ifndef __MK60_ADC_H__
#define __MK60_ADC_H__ 1

typedef enum
{
    // ---------------------------------ADC0-------------------------
    ADC0_DP0,
    ADC0_DP1,
    PGA0_DP,        //不支持ADC，需要配置 ADC PGA register 设置放大增益
    ADC0_DP3,
    //软件触发不支持B通道
    ADC0_SE4b,      // PTC2     不支持软件ADC
    ADC0_SE5b,      // PTD1     不支持软件ADC
    ADC0_SE6b,      // PTD5     不支持软件ADC
    ADC0_SE7b,      // PTD6     不支持软件ADC

    ADC0_SE8,       // PTB0
    ADC0_SE9,       // PTB1
    ADC0_SE10,      // PTA7
    ADC0_SE11,      // PTA8
    ADC0_SE12,      // PTB2
    ADC0_SE13,      // PTB3
    ADC0_SE14,      // PTC0
    ADC0_SE15,      // PTC1
    ADC0_SE16,      // ADC0_SE16
    ADC0_SE17,      // PTE24
    ADC0_SE18,      // PTE25
    ADC0_DM0,       // ADC0_DM0
    ADC0_DM1,       // ADC0_DM1
    RES0,           // 保留
    RES1,           // 保留
    DAC0_OUT,       // DAC0输出 不支持ADC
    RES2,           // 保留
    RES3,           // 保留
    Temp0_Sensor,   // Temperature Sensor,内部温度测量，可用ADC函数
    Bandgap0,       // 温度补偿结构带隙基准源   不支持ADC
    RES4,           // 保留
    VREFH0,         // 参考高电压,可用ADC函数 ,结果恒为 2^n-1
    VREFL0,         // 参考低电压,可用ADC函数 ,结果恒为 0
    Module0_Dis,    // 不支持 ADC

    // ---------------------------------ADC1-------------------------
    ADC1_DP0,
    ADC1_DP1,
    PGA1_DP,        // 不支持 ADC
    ADC1_DP3,
    ADC1_SE4a,      // PTE0
    ADC1_SE5a,      // PTE1
    ADC1_SE6a,      // PTE2
    ADC1_SE7a,      // PTE3

    ADC1_SE4b = ADC1_SE4a,  // PTC8     不支持软件ADC ,传递 ADC1_SE4b 到软件ADC函数里，会当作 ADC1_SE4a 处理。
    ADC1_SE5b = ADC1_SE5a,  // PTC9     不支持软件ADC
    ADC1_SE6b = ADC1_SE6a,  // PTC10    不支持软件ADC
    ADC1_SE7b = ADC1_SE7a,  // PTC11    不支持软件ADC

    ADC1_SE8,       // PTB0
    ADC1_SE9,       // PTB1
    ADC1_SE10,      // PTB4
    ADC1_SE11,      // PTB5
    ADC1_SE12,      // PTB6
    ADC1_SE13,      // PTB7
    ADC1_SE14,      // PTB10
    ADC1_SE15,      // PTB11
    ADC1_SE16,      // ADC1_SE16
    ADC1_SE17,      // PTA17
    VREF_OUTPUT,    // VREF Output
    ADC1_DM0,       // ADC1_DM0
    ADC1_DM1,       // ADC1_DM1
    RES5,           //保留
    RES6,
    DAC1_OUT,
    RES7,           //保留
    RES8,
    Temp1_Sensor,
    Bandgap1,       // 温度补偿结构带隙基准源   不支持ADC
    RES9,
    VREFH1,         // 参考高电压,可用ADC函数 ,结果恒为 2^n-1
    VREFL1,         // 参考低电压,可用ADC函数 ,结果恒为 0
    Module1_Dis,    // 不支持 ADC

} ADCn_Ch_e;

typedef enum  //ADC模块
{
    ADC0,
    ADC1
} ADCn_e;

//精度位数
typedef enum ADC_nbit
{
    ADC_8bit   = 0x00,
    ADC_10bit  = 0x02,
    ADC_12bit  = 0x01,
    ADC_16bit  = 0x03
} ADC_nbit;


//外部函数接口声明
extern void     adc_init    (ADCn_Ch_e);                //ADC初始化
extern uint16   adc_once    (ADCn_Ch_e, ADC_nbit);      //采集一次一路模拟量的AD值


extern void     adc_stop    (ADCn_e);                   //停止ADC转换


#endif /* __MK60_ADC16_H__ */
