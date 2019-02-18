/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       common.h
 * @brief      山外K60 平台常用类型声明和宏定义
 * @author     山外科技
 * @version    v5.1
 * @date       2014-04-25
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/*
 * 数据类型声明
 */
typedef unsigned char       uint8;  /*  8 bits */
typedef unsigned short int  uint16; /* 16 bits */
typedef unsigned long int   uint32; /* 32 bits */
typedef unsigned long long  uint64; /* 64 bits */

typedef char                int8;   /*  8 bits */
typedef short int           int16;  /* 16 bits */
typedef long  int           int32;  /* 32 bits */
typedef long  long          int64;  /* 64 bits */

typedef volatile int8       vint8;  /*  8 bits */
typedef volatile int16      vint16; /* 16 bits */
typedef volatile int32      vint32; /* 32 bits */
typedef volatile int64      vint64; /* 64 bits */

typedef volatile uint8      vuint8;  /*  8 bits */
typedef volatile uint16     vuint16; /* 16 bits */
typedef volatile uint32     vuint32; /* 32 bits */
typedef volatile uint64     vuint64; /* 64 bits */

/*
 * 定义带位域的联合体类型
 */
typedef union
{
    uint32  DW;
    uint16  W[2];
    uint8   B[4];
    struct
    {
        uint32 b0: 1;
        uint32 b1: 1;
        uint32 b2: 1;
        uint32 b3: 1;
        uint32 b4: 1;
        uint32 b5: 1;
        uint32 b6: 1;
        uint32 b7: 1;
        uint32 b8: 1;
        uint32 b9: 1;
        uint32 b10: 1;
        uint32 b11: 1;
        uint32 b12: 1;
        uint32 b13: 1;
        uint32 b14: 1;
        uint32 b15: 1;
        uint32 b16: 1;
        uint32 b17: 1;
        uint32 b18: 1;
        uint32 b19: 1;
        uint32 b20: 1;
        uint32 b21: 1;
        uint32 b22: 1;
        uint32 b23: 1;
        uint32 b24: 1;
        uint32 b25: 1;
        uint32 b26: 1;
        uint32 b27: 1;
        uint32 b28: 1;
        uint32 b29: 1;
        uint32 b30: 1;
        uint32 b31: 1;
    };
} Dtype;    //sizeof(Dtype) 为 4

/*
 * 定义坐标结构体
 */
typedef struct
{
    uint16 x;
    uint16 y;
} Site_t;

/*
 * 定义矩形大小结构体
 */
typedef struct
{
    uint16 W;       //宽
    uint16 H;       //高
} Size_t;

#define TRUE    1
#define FALSE   0


/*
 * 中断号类型声明
 */
typedef enum
{
    /******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
    NonMaskableInt_IRQn             = -14,      /*!< 2 Non Maskable Interrupt                                                                                    */
    HardFault_IRQn                  = -13 ,     /*!< 3 Hard Fault                                                                                */
    MemoryManagement_IRQn           = -12,      /*!< 4 Cortex-M4 Memory Management Interrupt                                                     */
    BusFault_IRQn                   = -11,      /*!< 5 Cortex-M4 Bus Fault Interrupt                                                                     */
    UsageFault_IRQn                 = -10,      /*!< 6 Cortex-M4 Usage Fault Interrupt                                                               */
    SVCall_IRQn                     = -5,       /*!< 11 Cortex-M4 SV Call Interrupt                                                                      */
    DebugMonitor_IRQn               = -4,       /*!< 12 Cortex-M4 Debug Monitor Interrupt                                                            */
    PendSV_IRQn                     = -2,       /*!< 14 Cortex-M4 Pend SV Interrupt                                                                      */
    SysTick_IRQn                    = -1,       /*!< 15 Cortex-M4 System Tick Interrupt                                                              */
    /******  Kinetis 60 specific Interrupt Numbers **********************************************************************/
    DMA0_IRQn                       = 0,        // DMA Channel 0 Transfer Complete
    DMA1_IRQn                       = 1,        // DMA Channel 1 Transfer Complete
    DMA2_IRQn                       = 2,        // DMA Channel 2 Transfer Complete
    DMA3_IRQn                       = 3,        // DMA Channel 3 Transfer Complete
    DMA4_IRQn                       = 4,        // DMA Channel 4 Transfer Complete
    DMA5_IRQn                       = 5,        // DMA Channel 5 Transfer Complete
    DMA6_IRQn                       = 6,        // DMA Channel 6 Transfer Complete
    DMA7_IRQn                       = 7,        // DMA Channel 7 Transfer Complete
    DMA8_IRQn                       = 8,        // DMA Channel 8 Transfer Complete
    DMA9_IRQn                       = 9,        // DMA Channel 9 Transfer Complete
    DMA10_IRQn                      = 10,       // DMA Channel 10 Transfer Complete
    DMA11_IRQn                      = 11,       // DMA Channel 11 Transfer Complete
    DMA12_IRQn                      = 12,       // DMA Channel 12 Transfer Complete
    DMA13_IRQn                      = 13,       // DMA Channel 13 Transfer Complete
    DMA14_IRQn                      = 14,       // DMA Channel 14 Transfer Complete
    DMA15_IRQn                      = 15,       // DMA Channel 15 Transfer Complete
    DMA_Error_IRQn                  = 16,       // DMA Error Interrupt
    MCM_IRQn                        = 17,       // Normal Interrupt
    FTFL_IRQn                       = 18,       // FTFL Interrupt
    Read_Collision_IRQn             = 19,       // Read Collision Interrupt
    LVD_LVW_IRQn                    = 20,       // Low Voltage Detect, Low Voltage Warning
    LLW_IRQn                        = 21,       // Low Leakage Wakeup
    Watchdog_IRQn                   = 22,       // WDOG Interrupt
    RNG_IRQn                        = 23,       // RNGB Interrupt
    I2C0_IRQn                       = 24,       // I2C0 interrupt
    I2C1_IRQn                       = 25,       // I2C1 interrupt
    SPI0_IRQn                       = 26,       // SPI0 Interrupt
    SPI1_IRQn                       = 27,       // SPI1 Interrupt
    SPI2_IRQn                       = 28,       // SPI2 Interrupt
    CAN0_ORed_MB_IRQn               = 29,       // CAN0 OR'd Message Buffers Interrupt
    CAN0_Bus_Off_IRQn               = 30,       // CAN0 Bus Off Interrupt
    CAN0_Error_IRQn                 = 31,       // CAN0 Error Interrupt
    CAN0_Tx_Warning_IRQn            = 32,       // CAN0 Tx Warning Interrupt
    CAN0_Rx_Warning_IRQn            = 33,       // CAN0 Rx Warning Interrupt
    CAN0_Wake_Up_IRQn               = 34,       // CAN0 Wake Up Interrupt
    Reserved51_IRQn                 = 35,       // 保留 interrupt 51
    Reserved52_IRQn                 = 36,       // 保留 interrupt 52
    CAN1_ORed_MB_IRQn               = 37,       // CAN1 OR'd Message Buffers Interrupt
    CAN1_Bus_Off_IRQn               = 38,       // CAN1 Bus Off Interrupt
    CAN1_Error_IRQn                 = 39,       // CAN1 Error Interrupt
    CAN1_Tx_Warning_IRQn            = 40,       // CAN1 Tx Warning Interrupt
    CAN1_Rx_Warning_IRQn            = 41,       // CAN1 Rx Warning Interrupt
    CAN1_Wake_Up_IRQn               = 42,       // CAN1 Wake Up Interrupt
    Reserved59_IRQn                 = 43,       // 保留 interrupt 59
    Reserved60_IRQn                 = 44,       // 保留 interrupt 60
    UART0_RX_TX_IRQn                = 45,       // UART0 Receive/Transmit interrupt
    UART0_ERR_IRQn                  = 46,       // UART0 Error interrupt
    UART1_RX_TX_IRQn                = 47,       // UART1 Receive/Transmit interrupt
    UART1_ERR_IRQn                  = 48,       // UART1 Error interrupt
    UART2_RX_TX_IRQn                = 49,       // UART2 Receive/Transmit interrupt
    UART2_ERR_IRQn                  = 50,       // UART2 Error interrupt
    UART3_RX_TX_IRQn                = 51,       // UART3 Receive/Transmit interrupt
    UART3_ERR_IRQn                  = 52,       // UART3 Error interrupt
    UART4_RX_TX_IRQn                = 53,       // UART4 Receive/Transmit interrupt
    UART4_ERR_IRQn                  = 54,       // UART4 Error interrupt
    UART5_RX_TX_IRQn                = 55,       // UART5 Receive/Transmit interrupt
    UART5_ERR_IRQn                  = 56,       // UART5 Error interrupt
    ADC0_IRQn                       = 57,       // ADC0 interrupt
    ADC1_IRQn                       = 58,       // ADC1 interrupt
    CMP0_IRQn                       = 59,       // CMP0 interrupt
    CMP1_IRQn                       = 60,       // CMP1 interrupt
    CMP2_IRQn                       = 61,       // CMP2 interrupt
    FTM0_IRQn                       = 62,       // FTM0 fault, overflow and channels interrupt
    FTM1_IRQn                       = 63,       // FTM1 fault, overflow and channels interrupt
    FTM2_IRQn                       = 64,       // FTM2 fault, overflow and channels interrupt
    CMT_IRQn                        = 65,       // CMT interrupt
    RTC_IRQn                        = 66,       // RTC interrupt
    Reserved83_IRQn                 = 67,       // 保留 interrupt 83
    PIT0_IRQn                       = 68,       // PIT timer channel 0 interrupt
    PIT1_IRQn                       = 69,       // PIT timer channel 1 interrupt
    PIT2_IRQn                       = 70,       // PIT timer channel 2 interrupt
    PIT3_IRQn                       = 71,       // PIT timer channel 3 interrupt
    PDB0_IRQn                       = 72,       // PDB0 Interrupt
    USB0_IRQn                       = 73,       // USB0 interrupt
    USBDCD_IRQn                     = 74,       // USBDCD Interrupt
    ENET_1588_Timer_IRQn            = 75,       // Ethernet MAC IEEE 1588 Timer Interrupt
    ENET_Transmit_IRQn              = 76,       // Ethernet MAC Transmit Interrupt
    ENET_Receive_IRQn               = 77,       // Ethernet MAC Receive Interrupt
    ENET_Error_IRQn                 = 78,       // Ethernet MAC Error and miscelaneous Interrupt
    I2S0_IRQn                       = 79,       // I2S0 Interrupt
    SDHC_IRQn                       = 80,       // SDHC Interrupt
    DAC0_IRQn                       = 81,       // DAC0 interrupt
    DAC1_IRQn                       = 82,       // DAC1 interrupt
    TSI0_IRQn                       = 83,       // TSI0 Interrupt
    MCG_IRQn                        = 84,       // MCG Interrupt
    LPTMR_IRQn                      = 85,       // LPTimer interrupt
    Reserved102_IRQn                = 86,       // 保留 interrupt 102
    PORTA_IRQn                      = 87,       // Port A interrupt
    PORTB_IRQn                      = 88,       // Port B interrupt
    PORTC_IRQn                      = 89,       // Port C interrupt
    PORTD_IRQn                      = 90,       // Port D interrupt
    PORTE_IRQn                      = 91,       // Port E interrupt
    Reserved108_IRQn                = 92,       // 保留 interrupt 108
    Reserved109_IRQn                = 93,       // 保留 interrupt 109
    Reserved110_IRQn                = 94,       // 保留 interrupt 110
    Reserved111_IRQn                = 95,       // 保留 interrupt 111
    Reserved112_IRQn                = 96,       // 保留 interrupt 112
    Reserved113_IRQn                = 97,       // 保留 interrupt 113
    Reserved114_IRQn                = 98,       // 保留 interrupt 114
    Reserved115_IRQn                = 99,       // 保留 interrupt 115
    Reserved116_IRQn                = 100,      // 保留 interrupt 116
    Reserved117_IRQn                = 101,      // 保留 interrupt 117
    Reserved118_IRQn                = 102,      // 保留 interrupt 118
    Reserved119_IRQn                = 103,      // 保留 interrupt 119
} IRQn_t, IRQn_Type;


/*
 * 中断向量表编号声明
 */
typedef enum
{
    /******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
    NonMaskableInt_VECTORn          = 2 ,       /*!< 2 Non Maskable Interrupt                                                                                    */
    HardFault_VECTORn               = 3 ,       /*!< 3 Hard Fault                                                                                */
    MemoryManagement_VECTORn        = 4 ,       /*!< 4 Cortex-M4 Memory Management Interrupt                                                     */
    BusFault_VECTORn                = 5 ,       /*!< 5 Cortex-M4 Bus Fault Interrupt                                                                     */
    UsageFault_VECTORn              = 6 ,       /*!< 6 Cortex-M4 Usage Fault Interrupt
    */
    SVCall_VECTORn                  = 11,       /*!< 11 Cortex-M4 SV Call Interrupt                                                                      */
    DebugMonitor_VECTORn            = 12,       /*!< 12 Cortex-M4 Debug Monitor Interrupt
    */
    PendSV_VECTORn                  = 14,       /*!< 14 Cortex-M4 Pend SV Interrupt                                                                      */
    SysTick_VECTORn                 = 15,       /*!< 15 Cortex-M4 System Tick Interrupt                                                              */
    /******  Kinetis 60 specific Interrupt Numbers **********************************************************************/
    DMA0_VECTORn                        ,       // DMA Channel 0 Transfer Complete
    DMA1_VECTORn                        ,       // DMA Channel 1 Transfer Complete
    DMA2_VECTORn                        ,       // DMA Channel 2 Transfer Complete
    DMA3_VECTORn                        ,       // DMA Channel 3 Transfer Complete
    DMA4_VECTORn                        ,       // DMA Channel 4 Transfer Complete
    DMA5_VECTORn                        ,       // DMA Channel 5 Transfer Complete
    DMA6_VECTORn                        ,       // DMA Channel 6 Transfer Complete
    DMA7_VECTORn                        ,       // DMA Channel 7 Transfer Complete
    DMA8_VECTORn                        ,       // DMA Channel 8 Transfer Complete
    DMA9_VECTORn                        ,       // DMA Channel 9 Transfer Complete
    DMA10_VECTORn                       ,       // DMA Channel 10 Transfer Complete
    DMA11_VECTORn                       ,       // DMA Channel 11 Transfer Complete
    DMA12_VECTORn                       ,       // DMA Channel 12 Transfer Complete
    DMA13_VECTORn                       ,       // DMA Channel 13 Transfer Complete
    DMA14_VECTORn                       ,       // DMA Channel 14 Transfer Complete
    DMA15_VECTORn                       ,       // DMA Channel 15 Transfer Complete
    DMA_Error_VECTORn                   ,       // DMA Error Interrupt
    MCM_VECTORn                         ,       // Normal Interrupt
    FTFL_VECTORn                        ,       // FTFL Interrupt
    Read_Collision_VECTORn              ,       // Read Collision Interrupt
    LVD_LVW_VECTORn                     ,       // Low Voltage Detect, Low Voltage Warning
    LLW_VECTORn                         ,       // Low Leakage Wakeup
    Watchdog_VECTORn                    ,       // WDOG Interrupt
    RNG_VECTORn                         ,       // RNGB Interrupt
    I2C0_VECTORn                        ,       // I2C0 interrupt
    I2C1_VECTORn                        ,       // I2C1 interrupt
    SPI0_VECTORn                        ,       // SPI0 Interrupt
    SPI1_VECTORn                        ,       // SPI1 Interrupt
    SPI2_VECTORn                        ,       // SPI2 Interrupt
    CAN0_ORed_MB_VECTORn                ,       // CAN0 OR'd Message Buffers Interrupt
    CAN0_Bus_Off_VECTORn                ,       // CAN0 Bus Off Interrupt
    CAN0_Error_VECTORn                  ,       // CAN0 Error Interrupt
    CAN0_Tx_Warning_VECTORn             ,       // CAN0 Tx Warning Interrupt
    CAN0_Rx_Warning_VECTORn             ,       // CAN0 Rx Warning Interrupt
    CAN0_Wake_Up_VECTORn                ,       // CAN0 Wake Up Interrupt
    Reserved51_VECTORn                  ,       // 保留 interrupt 51
    Reserved52_VECTORn                  ,       // 保留 interrupt 52
    CAN1_ORed_MB_VECTORn                ,       // CAN1 OR'd Message Buffers Interrupt
    CAN1_Bus_Off_VECTORn                ,       // CAN1 Bus Off Interrupt
    CAN1_Error_VECTORn                  ,       // CAN1 Error Interrupt
    CAN1_Tx_Warning_VECTORn             ,       // CAN1 Tx Warning Interrupt
    CAN1_Rx_Warning_VECTORn             ,       // CAN1 Rx Warning Interrupt
    CAN1_Wake_Up_VECTORn                ,       // CAN1 Wake Up Interrupt
    Reserved59_VECTORn                  ,       // 保留 interrupt 59
    Reserved60_VECTORn                  ,       // 保留 interrupt 60
    UART0_RX_TX_VECTORn                 ,       // UART0 Receive/Transmit interrupt
    UART0_ERR_VECTORn                   ,       // UART0 Error interrupt
    UART1_RX_TX_VECTORn                 ,       // UART1 Receive/Transmit interrupt
    UART1_ERR_VECTORn                   ,       // UART1 Error interrupt
    UART2_RX_TX_VECTORn                 ,       // UART2 Receive/Transmit interrupt
    UART2_ERR_VECTORn                   ,       // UART2 Error interrupt
    UART3_RX_TX_VECTORn                 ,       // UART3 Receive/Transmit interrupt
    UART3_ERR_VECTORn                   ,       // UART3 Error interrupt
    UART4_RX_TX_VECTORn                 ,       // UART4 Receive/Transmit interrupt
    UART4_ERR_VECTORn                   ,       // UART4 Error interrupt
    UART5_RX_TX_VECTORn                 ,       // UART5 Receive/Transmit interrupt
    UART5_ERR_VECTORn                   ,       // UART5 Error interrupt
    ADC0_VECTORn                        ,       // ADC0 interrupt
    ADC1_VECTORn                        ,       // ADC1 interrupt
    CMP0_VECTORn                        ,       // CMP0 interrupt
    CMP1_VECTORn                        ,       // CMP1 interrupt
    CMP2_VECTORn                        ,       // CMP2 interrupt
    FTM0_VECTORn                        ,       // FTM0 fault, overflow and channels interrupt
    FTM1_VECTORn                        ,       // FTM1 fault, overflow and channels interrupt
    FTM2_VECTORn                        ,       // FTM2 fault, overflow and channels interrupt
    CMT_VECTORn                         ,       // CMT interrupt
    RTC_VECTORn                         ,       // RTC interrupt
    Reserved83_VECTORn                  ,       // 保留 interrupt 83
    PIT0_VECTORn                        ,       // PIT timer channel 0 interrupt
    PIT1_VECTORn                        ,       // PIT timer channel 1 interrupt
    PIT2_VECTORn                        ,       // PIT timer channel 2 interrupt
    PIT3_VECTORn                        ,       // PIT timer channel 3 interrupt
    PDB0_VECTORn                        ,       // PDB0 Interrupt
    USB0_VECTORn                        ,       // USB0 interrupt
    USBDCD_VECTORn                      ,       // USBDCD Interrupt
    ENET_1588_Timer_VECTORn             ,       // Ethernet MAC IEEE 1588 Timer Interrupt
    ENET_Transmit_VECTORn               ,       // Ethernet MAC Transmit Interrupt
    ENET_Receive_VECTORn                ,       // Ethernet MAC Receive Interrupt
    ENET_Error_VECTORn                  ,       // Ethernet MAC Error and miscelaneous Interrupt
    I2S0_VECTORn                        ,       // I2S0 Interrupt
    SDHC_VECTORn                        ,       // SDHC Interrupt
    DAC0_VECTORn                        ,       // DAC0 interrupt
    DAC1_VECTORn                        ,       // DAC1 interrupt
    TSI0_VECTORn                        ,       // TSI0 Interrupt
    MCG_VECTORn                         ,       // MCG Interrupt
    LPTMR_VECTORn                       ,       // LPTimer interrupt
    Reserved102_VECTORn                 ,       // 保留 interrupt 102
    PORTA_VECTORn                       ,       // Port A interrupt
    PORTB_VECTORn                       ,       // Port B interrupt
    PORTC_VECTORn                       ,       // Port C interrupt
    PORTD_VECTORn                       ,       // Port D interrupt
    PORTE_VECTORn                       ,       // Port E interrupt
    Reserved108_VECTORn                 ,       // 保留 interrupt 108
    Reserved109_VECTORn                 ,       // 保留 interrupt 109
    Reserved110_VECTORn                 ,       // 保留 interrupt 110
    Reserved111_VECTORn                 ,       // 保留 interrupt 111
    Reserved112_VECTORn                 ,       // 保留 interrupt 112
    Reserved113_VECTORn                 ,       // 保留 interrupt 113
    Reserved114_VECTORn                 ,       // 保留 interrupt 114
    Reserved115_VECTORn                 ,       // 保留 interrupt 115
    Reserved116_VECTORn                 ,       // 保留 interrupt 116
    Reserved117_VECTORn                 ,       // 保留 interrupt 117
    Reserved118_VECTORn                 ,       // 保留 interrupt 118
    Reserved119_VECTORn                 ,       // 保留 interrupt 119
} VECTORn_t;

/*
 * 定义运行到RAM的函数
 */
#if defined(__ICCARM__)
//IAR 用 __ramfunc 来声明
#define     __RAMFUNC __ramfunc
#else
#define     __RAMFUN
#endif


/*
 * 包含Cortex-M内核的通用头文件
 */
#include    <stdio.h>                       //printf
#include    <string.h>                      //memcpy
#include    <stdlib.h>                      //malloc
#include "misc.h"        //这两个顺序不能反，因为 arm_math.h 里需要  __NVIC_PRIO_BITS 的定义

#define   __NVIC_PRIO_BITS    4       /*ARM Cortex M4 implementation for interrupt priority shift*/
#include "arm_math.h"


/*
 * 包含山外K60平台的配置头文件
 */
#include "MK60_conf.h"

/*
 * 包含平台头文件
 */
#if defined(MK60DZ10)
#include "MK60DZ10.h"

#elif defined(MK60F15)
#include "MK60F15.h"

//SIM_SCGC4_IIC2_MASK 改为 SIM_SCGC4_IIC0_MASK
//SIM_SCGC4_IIC2_SHIFT  改为 SIM_SCGC4_IIC0_SHIFT
#else
#error "error"
#endif



/*
 * 包含常用头文件
 */
#include    "system_MK60DZ10.h"             //系统配置
#include    "PORT_cfg.h"                    //管脚复用配置
#include    "MK60_mcg.h"                    //K60 时钟模块

#if (defined(IAR))
#include "intrinsics.h"
#endif




/**
 *  @brief 变量的位清0和置1
 */
#define BIT_CLEAN(var,n)        (var) &= ~(1<<(n))   //变量var 的n位（即第n+1位）清0
#define BIT_SET(var,n)          (var) |=  (1<<(n))   //变量var 的n位（即第n+1位）置1
#define BIT_GET(var,n)          (((var)>>(n))&0x01)  //读取变量var 的n位（即第n+1位）

/**
 *  @brief 仿二进制赋值
 */
#define  HEX__(n)   0x##n##UL
#define  B8__(x)   ( (x & 0x0000000FUL) ? 1:0 )\
    +( (x & 0x000000F0UL) ? 2:0 )\
    +( (x & 0x00000F00UL) ? 4:0 )\
    +( (x & 0x0000F000UL) ? 8:0 )\
    +( (x & 0x000F0000UL) ? 16:0 )\
    +( (x & 0x00F00000UL) ? 32:0 )\
    +( (x & 0x0F000000UL) ? 64:0 )\
    +( (x & 0xF0000000UL) ? 128:0 )
#define  B8(x)                                     ((unsigned char)B8__(HEX__(x)))
#define  B16(x_msb,x_lsb)                (((unsigned int)B8(x_msb)<<8) + B8(x_lsb))
#define  B32(x_msb,x_2,x_3,x_lsb)   (((unsigned long)B8(x_msb)<<24) + ((unsigned long)B8(x_2)<<16) + ((unsigned long)B8(x_3)<<8) + B8(x_lsb))
/* Sample usage:
B8(01010101) = 85
B16(10101010,01010101) = 43605
B32(10000000,11111111,10101010,01010101) = 2164238933
*/

/*
 * 打印宏定义展开结果
 */
#define _MKSTR(str)  #str
#define MKSTR(str)  _MKSTR(str)                       //把宏定义展开结果转换为字符串

#define DEFINE_PRINTF(def)  printf(MKSTR(def)"\n")    //串口打印宏定义展开结果


/*
 * 求最大值和最小值
 */
#define MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#define MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )


/*
 * 返回数组元素的个数
 */
#define ARR_SIZE( a ) ( sizeof( (a) ) / sizeof( ((a)[0]) ) )

/*
 * 宏定义实现返回绝对值（x里不能有自加自减的语句，否则变量出错）
 */
#define ABS(x) (((x) > 0) ? (x) : (-(x)))

/*
 * 获取结构体某成员偏移
 */
#define OFFSET(type, member)    (uint32)(&(((type *)0)->member))

/*
 * 确保x的范围为 min~max
 */
#define RANGE(x,max,min)        ((uint8)((x)<(min) ? (min) : ( (x)>(max) ? (max):(x) )))

/*
 * 交换32位数据的4字节顺序
 */
#if 1         //两种方法，第一种是CMSIS库自带
#define SWAP32(data)    __REV(data)
#else
#define SWAP32(data)    (uint32)((((uint32)(data) & (0xFFu<<0 ))<<24)      \
                          | (((uint32)(data) & (0xFFu<<8 ))<<8)            \
                          | (((uint32)(data) & (0xFFu<<16))>>8)            \
                          | (((uint32)(data) & (0xFFu<<24))>>24)           \
                        )
#endif

/*
 * 交换16位数据的2字节顺序
 */
#if 1         //两种方法，第一种是CMSIS库自带
#define SWAP16(data)    __REVSH(data)
#else
#define SWAP16(data)    (uint32)((((uint16)(data) & (0xFF<<0 ))<<8)      \
                          | (((uint32)(data) & (0xFF<<8))>>8)            \
                        )
#endif

/*
 * 交换 x, y 的值
 */
#define SWAP(x,y)           do{x^=y;y^=x;x^=y;}while(0)


/********************************************************************/

#endif /* _COMMON_H_ */
