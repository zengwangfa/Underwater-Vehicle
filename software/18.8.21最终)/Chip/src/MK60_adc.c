/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_adc.c
 * @brief      ADC函数
 * @author     山外科技
 * @version    v5.1
 * @date       2014-01-16
 */

#include "common.h"
#include "MK60_adc.h"

ADC_MemMapPtr ADCN[2] = {ADC0_BASE_PTR, ADC1_BASE_PTR}; //定义两个指针数组保存 ADCN 的地址

static void     adc_start   (ADCn_Ch_e, ADC_nbit);    //开始adc转换

/*!
 *  @brief      ADC初始化
 *  @param      ADCn_Ch_e    ADC通道
 *  @since      v5.0
 *  @note       此初始化仅支持软件触发，不是每个通道都支持ADC 软件触发，
                具体说明见 ADCn_Ch_e 的注释说明
 *  Sample usage:       adc_init (ADC0_SE10 );    //初始化 ADC0_SE10 ，使用 PTA7 管脚
 */
void adc_init(ADCn_Ch_e adcn_ch)
{

    uint8 adcn = adcn_ch >> 5 ;
    //uint8 ch = adcn_ch & 0x1F;

    switch(adcn)
    {
    case ADC0:       /*   ADC0  */
        SIM_SCGC6 |= (SIM_SCGC6_ADC0_MASK );        //开启ADC0时钟
        SIM_SOPT7 &= ~(SIM_SOPT7_ADC0ALTTRGEN_MASK  | SIM_SOPT7_ADC0PRETRGSEL_MASK);
        SIM_SOPT7 |= SIM_SOPT7_ADC0TRGSEL(0);
        break;
    case ADC1:       /*   ADC1    */
        SIM_SCGC3 |= (SIM_SCGC3_ADC1_MASK );
        SIM_SOPT7 &= ~(SIM_SOPT7_ADC1ALTTRGEN_MASK  | SIM_SOPT7_ADC1PRETRGSEL_MASK) ;
        SIM_SOPT7 |= SIM_SOPT7_ADC1TRGSEL(0);
        break;
    default:
        ASSERT(0);
    }

    switch(adcn_ch)
    {
    case ADC0_SE8:       // PTB0
        port_init(PTB0, ALT0);
        break;
    case ADC0_SE9:       // PTB1
        port_init(PTB1, ALT0);
        break;
    case ADC0_SE10:      // PTA7
        port_init(PTA7, ALT0);
        break;
    case ADC0_SE11:      // PTA8
        port_init(PTA8, ALT0);
        break;
    case ADC0_SE12:      // PTB2
        port_init(PTB2, ALT0);
        break;
    case ADC0_SE13:      // PTB3
        port_init(PTB3, ALT0);
        break;
    case ADC0_SE14:      // PTC0
        port_init(PTC0, ALT0);
        break;
    case ADC0_SE15:      // PTC1
        port_init(PTC1, ALT0);
        break;
    case ADC0_SE17:      // PTE24
        port_init(PTE24, ALT0);
        break;
    case ADC0_SE18:      // PTE25
        port_init(PTE25, ALT0);
        break;

    case ADC0_DP0:
    case ADC0_DP1:
    case ADC0_DP3:
    case ADC0_DM0:       // ADC0_DM0
    case ADC0_DM1:       // ADC0_DM1
    case ADC0_SE16:      // ADC0_SE16
    case Temp0_Sensor:   // Temperature Sensor,内部温度测量，可用ADC函数
    case VREFH0:         // 参考高电压,可用ADC函数 ,结果恒为 2^n-1
    case VREFL0:         // 参考低电压,可用ADC函数 ,结果恒为 0
        break;          //这部分管脚不用配置复用

        // ---------------------------------ADC1-------------------------
    case ADC1_DP0:
    case ADC1_DP1:
    case ADC1_DP3:
        break;
    case ADC1_SE4a:      // PTE0
        port_init(PTE0, ALT0);
        break;
    case ADC1_SE5a:      // PTE1
        port_init(PTE1, ALT0);
        break;
    case ADC1_SE6a:      // PTE2
        port_init(PTE2, ALT0);
        break;
    case ADC1_SE7a:      // PTE3
        port_init(PTE3, ALT0);
        break;

    case ADC1_SE8:       // PTB0
        port_init(PTB0, ALT0);
        break;
    case ADC1_SE9:       // PTB1
        port_init(PTB1, ALT0);
        break;
    case ADC1_SE10:      // PTB4
        port_init(PTB4, ALT0);
        break;
    case ADC1_SE11:      // PTB5
        port_init(PTB5, ALT0);
        break;
    case ADC1_SE12:      // PTB6
        port_init(PTB6, ALT0);
        break;
    case ADC1_SE13:      // PTB7
        port_init(PTB7, ALT0);
        break;
    case ADC1_SE14:      // PTB10
        port_init(PTB10, ALT0);
        break;
    case ADC1_SE15:      // PTB11
        port_init(PTB11, ALT0);
        break;
    case ADC1_SE17:      // PTA17
        port_init(PTA17, ALT0);
        break;

    case ADC1_SE16:      // ADC1_SE16
    case VREF_OUTPUT:    // VREF Output
    case ADC1_DM0:       // ADC1_DM0
    case ADC1_DM1:       // ADC1_DM1
    case Temp1_Sensor:
    case VREFH1:         // 参考高电压,可用ADC函数 ,结果恒为 2^n-1
    case VREFL1:         // 参考低电压,可用ADC函数 ,结果恒为 0
        break;

    default:
        ASSERT(0);      //断言，传递的管脚不支持 ADC 单端软件触发，请换 其他管脚
        break;
    }
}

/*!
 *  @brief      获取ADC采样值(不支持B通道)
 *  @param      ADCn_Ch_e    ADC通道
 *  @param      ADC_nbit     ADC精度（ ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ）
 *  @return     采样值
 *  @since      v5.0
 *  Sample usage:       uint16 var = adc_once(ADC0_SE10, ADC_8bit);
 */
uint16 adc_once(ADCn_Ch_e adcn_ch, ADC_nbit bit) //采集某路模拟量的AD值
{
    ADCn_e adcn = (ADCn_e)(adcn_ch >> 5) ;

    uint16 result = 0;

    adc_start(adcn_ch, bit);      //启动ADC转换

    while (( ADC_SC1_REG(ADCN[adcn], 0 ) & ADC_SC1_COCO_MASK ) != ADC_SC1_COCO_MASK);   //只支持 A通道
    result = ADC_R_REG(ADCN[adcn], 0);
    ADC_SC1_REG(ADCN[adcn], 0) &= ~ADC_SC1_COCO_MASK;
    return result;
}

/*!
 *  @brief      启动ADC软件采样(不支持B通道)
 *  @param      ADCn_Ch_e    ADC通道
 *  @param      ADC_nbit     ADC精度（ ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ）
 *  @since      v5.0
 *  @note       此函数内部调用，启动后即可等待数据采集完成
 *  Sample usage:       adc_start(ADC0_SE10, ADC_8bit);
 */
void adc_start(ADCn_Ch_e adcn_ch, ADC_nbit bit)
{
    ADCn_e adcn = (ADCn_e)(adcn_ch >> 5) ;
    uint8 ch = (uint8)(adcn_ch & 0x1F);

    //初始化ADC默认配置
    ADC_CFG1_REG(ADCN[adcn]) = (0
                                //| ADC_CFG1_ADLPC_MASK         //ADC功耗配置，0为正常功耗，1为低功耗
                                | ADC_CFG1_ADIV(2)              //时钟分频选择,分频系数为 2^n,2bit
                                | ADC_CFG1_ADLSMP_MASK          //采样时间配置，0为短采样时间，1 为长采样时间
                                | ADC_CFG1_MODE(bit)
                                | ADC_CFG1_ADICLK(0)            //0为总线时钟,1为总线时钟/2,2为交替时钟（ALTCLK），3为 异步时钟（ADACK）。
                               );


    ADC_CFG2_REG(ADCN[adcn])  = (0
                                 //| ADC_CFG2_MUXSEL_MASK       //ADC复用选择,0为a通道，1为b通道。
                                 //| ADC_CFG2_ADACKEN_MASK      //异步时钟输出使能,0为禁止，1为使能。
                                 | ADC_CFG2_ADHSC_MASK          //高速配置,0为正常转换序列，1为高速转换序列
                                 | ADC_CFG2_ADLSTS(0)           //长采样时间选择，ADCK为4+n个额外循环，额外循环，0为20，1为12，2为6，3为2
                                );

    //写入 SC1A 启动转换
    ADC_SC1_REG(ADCN[adcn], 0 ) = (0
                                   | ADC_SC1_AIEN_MASK          // 转换完成中断,0为禁止，1为使能
                                   //| ADC_SC1_DIFF_MASK        // 差分模式使能,0为单端，1为差分
                                   | ADC_SC1_ADCH( ch )
                                  );

}

/*!
 *  @brief      停止ADC软件采样
 *  @param      ADCn_e       ADC模块号（ ADC0、 ADC1）
 *  @since      v5.0
 *  Sample usage:       adc_stop(ADC0);
 */
void adc_stop(ADCn_e adcn)
{
    ADC_SC1_REG(ADCN[adcn], 0) = (0
                                  | ADC_SC1_AIEN_MASK                       // 转换完成中断,0为禁止，1为使能
                                  //| ADC_SC1_DIFF_MASK                     // 差分模式使能,0为单端，1为差分
                                  | ADC_SC1_ADCH(Module0_Dis)               //输入通道选择,此处选择禁止通道
                                 );
}

