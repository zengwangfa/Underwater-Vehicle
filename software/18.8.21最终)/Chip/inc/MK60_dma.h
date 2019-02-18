/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_dma.h
 * @brief      DMA函数库
 * @author     山外科技
 * @version    v5.1
 * @date       2014-04-25
 */


#ifndef _MK60_DMA_H_
#define _MK60_DMA_H_


typedef enum
{
    /*        禁用通道            */
    Channel_Disabled        = 0,

    /*        UART            */
    DMA_UART0_Rx            = 2,
    DMA_UART0_Tx            = 3,
    DMA_UART1_Rx            = 4,
    DMA_UART1_Tx            = 5,
    DMA_UART2_Rx            = 6,
    DMA_UART2_Tx            = 7,
    DMA_UART3_Rx            = 8,
    DMA_UART3_Tx            = 9,
    DMA_UART4_Rx            = 10,
    DMA_UART4_Tx            = 11,
    DMA_UART5_Rx            = 12,
    DMA_UART5_Tx            = 13,

    /*        I2S            */
    DMA_I2S0_Rx             = 14,
    DMA_I2S0_Tx             = 15,

    /*        SPI            */
    DMA_SPI0_Rx             = 16,
    DMA_SPI0_Tx             = 17,
    DMA_SPI1_Rx             = 18,
    DMA_SPI1_Tx             = 19,

    /*        I2C            */
    DMA_I2C0                = 22,
    DMA_I2C1                = 23,

    /*        FTM            */
    DMA_FTM0_CH0            = 24,
    DMA_FTM0_CH1            = 25,
    DMA_FTM0_CH2            = 26,
    DMA_FTM0_CH3            = 27,
    DMA_FTM0_CH4            = 28,
    DMA_FTM0_CH5            = 29,
    DMA_FTM0_CH6            = 30,
    DMA_FTM0_CH7            = 31,

    DMA_FTM1_CH0            = 32,
    DMA_FTM1_CH1            = 33,

    DMA_FTM2_CH0            = 34,
    DMA_FTM2_CH1            = 35,

    DMA_FTM3_CH0            = 36,
    DMA_FTM3_CH1            = 37,
    DMA_FTM3_CH2            = 38,

    DMA_FTM1_CH3            = 39,

    /*     ADC/DAC/CMP/CMT    */
    DMA_ADC0                = 40,
    DMA_ADC1                = 41,
    DMA_CMP0                = 42,
    DMA_CMP1                = 43,
    DMA_CMP2                = 44,
    DMA_DAC0                = 45,
    DMA_DAC1                = 46,
    DMA_CMT                 = 47,

    DMA_PDB                 = 48,

    DMA_PORTA              = 49,
    DMA_PORTB              = 50,
    DMA_PORTC              = 51,
    DMA_PORTD              = 52,
    DMA_PORTE              = 53,

    DMA_FTM3_CH4            = 54,
    DMA_FTM3_CH5            = 55,
    DMA_FTM3_CH6            = 56,
    DMA_FTM3_CH7            = 57,

    DMA_Always_EN1          = 58,
    DMA_Always_EN2          = 59,
    DMA_Always_EN3          = 60,
    DMA_Always_EN4          = 61,
    DMA_Always_EN5          = 62,
    DMA_Always_EN6          = 63,
} DMA_sources;



typedef enum
{
    DADDR_RECOVER = 0,        //恢复目的地址
    DADDR_KEEPON  = 1,        //目的地址保持不变

} DMA_cfg;

typedef enum      //DMA每次传输字节数
{
    DMA_BYTE1 = 0,
    DMA_BYTE2 = 1,
    DMA_BYTE4 = 2,
    DMA_BYTE16 = 4
} DMA_BYTEn;


typedef enum
{
    DMA_CH0,
    DMA_CH1,
    DMA_CH2,
    DMA_CH3,
    DMA_CH4,
    DMA_CH5,
    DMA_CH6,
    DMA_CH7,
    DMA_CH8,
    DMA_CH9,
    DMA_CH10,
    DMA_CH11,
    DMA_CH12,
    DMA_CH13,
    DMA_CH14,
    DMA_CH15
} DMA_CHn;


#define  DMA_IRQ_EN(DMA_CHn)    enable_irq((IRQn_t)((IRQn_t)DMA_CHn + DMA0_IRQn))               //允许DMA通道传输完成中断
#define  DMA_IRQ_DIS(DMA_CHn)   disable_irq((IRQn_t)((IRQn_t)DMA_CHn + DMA0_IRQn))              //禁止DMA通道传输完成中断

#define  DMA_IRQ_CLEAN(DMA_CHn) DMA_INT|=(DMA_INT_INT0_MASK<<DMA_CHn)           //清除通道传输中断标志位

#define  DMA_EN(DMA_CHn)        DMA_ERQ |= (DMA_ERQ_ERQ0_MASK<<(DMA_CHn))       //使能通道硬件DMA请求
#define  DMA_DIS(DMA_CHn)       DMA_ERQ &=~(DMA_ERQ_ERQ0_MASK<<(DMA_CHn))       //禁止通道硬件DMA请求

#if defined(MK60DZ10)
#define  DMA_CHn_DIS(DMA_CHn)   DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR,DMA_CHn) &= ~DMAMUX_CHCFG_SOURCE(Channel_Disabled)  //禁用通道
#elif defined(MK60F15)
#define  DMA_CHn_DIS(DMA_CHn)   DMAMUX_CHCFG_REG(DMAMUX0_BASE_PTR,DMA_CHn) &= ~DMAMUX_CHCFG_SOURCE(Channel_Disabled)  //禁用通道
#endif


//初始化DMA，使得PORT端口数据通过DMA传输到BUFF缓冲区
extern void dma_portx2buff_init(DMA_CHn, void *SADDR, void *DADDR, PTXn_e, DMA_BYTEn, uint32 count, uint32 cfg);



#endif  //_MK60_DMA_H_