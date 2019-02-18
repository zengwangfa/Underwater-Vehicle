/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_conf.c
 * @brief      山外K60 平台配置功能实现文件
 * @author     山外科技
 * @version    v5.1
 * @date       2013-06-26
 */


#include    "common.h"
#include    "include.h"
#include    "stdio.h"
#include    "MK60_uart.h"
#include    "MK60_flash.h"


/*!
 *  @brief      断言失败所执行的函数
 *  @param      file    文件路径地址
 *  @param      line    行数
 *  @since      v5.0
 *  Sample usage:       assert_failed(__FILE__, __LINE__);
 */
const char ASSERT_FAILED_STR[] = "Assertion failed in %s at line %d\n";

void assert_failed(char *file, int line)
{
    while (1)
    {

        DEBUG_PRINTF(ASSERT_FAILED_STR, file, line);      //通过串口提示断言失败

        //死循环等待程序员检测为何断言失败
        DELAY_MS(1000);

    }
}

/*!
 *  @brief      重定义printf 到串口
 *  @param      ch      需要打印的字节
 *  @param      stream  数据流
 *  @since      v5.0
 *  @note       此函数由编译器自带库里的printf所调用
 */
int fputc(int ch, FILE *stream)
{
    uart_putchar(VCAN_PORT, (char)ch);
    return(ch);
}

/*!
 *  @brief      启动前进行检测（检测按键是否按下，按下则进入死循环，防止进入 main 函数,松开按键后再按下则进行解锁）
 *  @since      v5.0
 *  @note       此函数可避免 下载口在 main 前 复用了导致无法下载等问题
 */


#ifdef  DEBUG
// VECTOR_TABLE end
static char vector_str[][15];
#endif

/*!
 *  @brief      默认中断服务函数
 *  @since      v5.0
 *  @note       此函数写入中断向量表里，不需要用户执行
 */
void default_isr(void)
{


#ifdef  DEBUG
#define VECTORNUM    ((SCB_ICSR & SCB_ICSR_VECTACTIVE_MASK)>>SCB_ICSR_VECTACTIVE_SHIFT)
                            //等效于 (*(volatile uint8_t*)(0xE000ED04))
    uint8 vtr = VECTORNUM;

    while(1)
    {
        DEBUG_PRINTF("\n****default_isr entered on vector %d*****\n\n%s Interrupt", vtr, vector_str[vtr]);

        DELAY_MS(1000);
    }
#else
    return;
#endif
}

#ifdef DEBUG

char vector_str[][15] =
{
    /******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
    "",                         //0
    "",
    "NonMaskable"               , /*!< 2 Non Maskable Interrupt                              */
    "HardFault"                 , /*!< 3 Hard Fault                                          */
    "MemManagement"             , /*!< 4 Cortex-M4 Memory Management Interrupt               */
    "BusFault"                  , /*!< 5 Cortex-M4 Bus Fault Interrupt                       */
    "UsageFault"                , /*!< 6 Cortex-M4 Usage Fault Interrupt                     */
    "",                         //7
    "",
    "",                         //9
    "",
    "SVCall"                    , /*!< 11 Cortex-M4 SV Call Interrupt                        */
    "DebugMonitor"              , /*!< 12 Cortex-M4 Debug Monitor Interrupt                  */
    "",
    "PendSV"                    , /*!< 14 Cortex-M4 Pend SV Interrupt                        */
    "SysTick"                   , /*!< 15 Cortex-M4 System Tick Interrupt                    */
    /******  Kinetis 60 specific Interrupt Numbers **********************************************************************/
    "DMA0"                      , // DMA Channel 0 Transfer Complete
    "DMA1"                      , // DMA Channel 1 Transfer Complete
    "DMA2"                      , // DMA Channel 2 Transfer Complete
    "DMA3"                      , // DMA Channel 3 Transfer Complete
    "DMA4"                      , // DMA Channel 4 Transfer Complete
    "DMA5"                      , // DMA Channel 5 Transfer Complete
    "DMA6"                      , // DMA Channel 6 Transfer Complete
    "DMA7"                      , // DMA Channel 7 Transfer Complete
    "DMA8"                      , // DMA Channel 8 Transfer Complete
    "DMA9"                      , // DMA Channel 9 Transfer Complete
    "DMA10"                     , // DMA Channel 10 Transfer Complete
    "DMA11"                     , // DMA Channel 11 Transfer Complete
    "DMA12"                     , // DMA Channel 12 Transfer Complete
    "DMA13"                     , // DMA Channel 13 Transfer Complete
    "DMA14"                     , // DMA Channel 14 Transfer Complete
    "DMA15"                     , // DMA Channel 15 Transfer Complete
    "DMA_Error"                 , // DMA Error Interrupt
    "MCM"                       , // Normal Interrupt
    "FTFL"                      , // FTFL Interrupt
    "Read_Collision"            , // Read Collision Interrupt
    "LVD_LVW"                   , // Low Voltage Detect, Low Voltage Warning
    "LLW"                       , // Low Leakage Wakeup
    "Watchdog"                  , // WDOG Interrupt
    "RNG"                       , // RNGB Interrupt
    "I2C0"                      , // I2C0 interrupt
    "I2C1"                      , // I2C1 interrupt
    "SPI0"                      , // SPI0 Interrupt
    "SPI1"                      , // SPI1 Interrupt
    "SPI2"                      , // SPI2 Interrupt
    "CAN0_OR_MB_buf"            , // CAN0 OR'd Message Buffers Interrupt
    "CAN0_Bus_Off"              , // CAN0 Bus Off Interrupt
    "CAN0_Error"                , // CAN0 Error Interrupt
    "CAN0_Tx_Warning"           , // CAN0 Tx Warning Interrupt
    "CAN0_Rx_Warning"           , // CAN0 Rx Warning Interrupt
    "CAN0_Wake_Up"              , // CAN0 Wake Up Interrupt
    "Reserved51"                , // 保留 interrupt 51
    "Reserved52"                , // 保留 interrupt 52
    "CAN1_OR_MB_buf"            , // CAN1 OR'd Message Buffers Interrupt
    "CAN1_Bus_Off"              , // CAN1 Bus Off Interrupt
    "CAN1_Error"                , // CAN1 Error Interrupt
    "CAN1_Tx_Warning"           , // CAN1 Tx Warning Interrupt
    "CAN1_Rx_Warning"           , // CAN1 Rx Warning Interrupt
    "CAN1_Wake_Up"              , // CAN1 Wake Up Interrupt
    "Reserved59"                , // 保留 interrupt 59
    "Reserved60"                , // 保留 interrupt 60
    "UART0_RX_TX"               , // UART0 Receive/Transmit interrupt
    "UART0_ERR"                 , // UART0 Error interrupt
    "UART1_RX_TX"               , // UART1 Receive/Transmit interrupt
    "UART1_ERR"                 , // UART1 Error interrupt
    "UART2_RX_TX"               , // UART2 Receive/Transmit interrupt
    "UART2_ERR"                 , // UART2 Error interrupt
    "UART3_RX_TX"               , // UART3 Receive/Transmit interrupt
    "UART3_ERR"                 , // UART3 Error interrupt
    "UART4_RX_TX"               , // UART4 Receive/Transmit interrupt
    "UART4_ERR"                 , // UART4 Error interrupt
    "UART5_RX_TX"               , // UART5 Receive/Transmit interrupt
    "UART5_ERR"                 , // UART5 Error interrupt
    "ADC0"                      , // ADC0 interrupt
    "ADC1"                      , // ADC1 interrupt
    "CMP0"                      , // CMP0 interrupt
    "CMP1"                      , // CMP1 interrupt
    "CMP2"                      , // CMP2 interrupt
    "FTM0"                      , // FTM0 fault, overflow and channels interrupt
    "FTM1"                      , // FTM1 fault, overflow and channels interrupt
    "FTM2"                      , // FTM2 fault, overflow and channels interrupt
    "CMT"                       , // CMT interrupt
    "RTC"                       , // RTC interrupt
    "Reserved83"                , // 保留 interrupt 83
    "PIT0"                      , // PIT timer channel 0 interrupt
    "PIT1"                      , // PIT timer channel 1 interrupt
    "PIT2"                      , // PIT timer channel 2 interrupt
    "PIT3"                      , // PIT timer channel 3 interrupt
    "PDB0"                      , // PDB0 Interrupt
    "USB0"                      , // USB0 interrupt
    "USBDCD"                    , // USBDCD Interrupt
    "ENET_1588_Timer"           , // Ethernet MAC IEEE 1588 Timer Interrupt
    "ENET_Transmit"             , // Ethernet MAC Transmit Interrupt
    "ENET_Receive"              , // Ethernet MAC Receive Interrupt
    "ENET_Error"                , // Ethernet MAC Error and miscelaneous Interrupt
    "I2S0"                      , // I2S0 Interrupt
    "SDHC"                      , // SDHC Interrupt
    "DAC0"                      , // DAC0 interrupt
    "DAC1"                      , // DAC1 interrupt
    "TSI0"                      , // TSI0 Interrupt
    "MCG"                       , // MCG Interrupt
    "LPTimer"                   , // LPTimer interrupt
    "Reserved102"               , // 保留 interrupt 102
    "PORTA"                     , // Port A interrupt
    "PORTB"                     , // Port B interrupt
    "PORTC"                     , // Port C interrupt
    "PORTD"                     , // Port D interrupt
    "PORTE"                     , // Port E interrupt
    "Reserved108"               , // 保留 interrupt 108
    "Reserved109"               , // 保留 interrupt 109
    "Reserved110"               , // 保留 interrupt 110
    "Reserved111"               , // 保留 interrupt 111
    "Reserved112"               , // 保留 interrupt 112
    "Reserved113"               , // 保留 interrupt 113
    "Reserved114"               , // 保留 interrupt 114
    "Reserved115"               , // 保留 interrupt 115
    "Reserved116"               , // 保留 interrupt 116
    "Reserved117"               , // 保留 interrupt 117
    "Reserved118"               , // 保留 interrupt 118
    "Reserved119"               , // 保留 interrupt 119
};
#endif