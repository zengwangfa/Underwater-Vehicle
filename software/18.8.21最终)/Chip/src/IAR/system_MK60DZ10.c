/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       system_MK60DZ10.c
 * @brief      系统启动相关函数
 * @author     山外科技
 * @version    v5.0
 * @date       2013-07-02
 */
 
#include "common.h"
#include "system_MK60DZ10.h"
#include "MK60_uart.h"
#include "MK60_wdog.h"



int core_clk_khz;
int core_clk_mhz;
int bus_clk_khz;

extern void main(void);

/*!
 *  @brief      kinetis 启动函数
 *  @since      v5.0
 *  @author     飞思卡尔公司
 *  @note       此函数是系统启动初期由汇编函数调用，然后执行main函数
 */
void start(void)
{
    wdog_disable();     // 关闭看门狗

    common_startup();   // 复制中断向量表 和 必要的数据到 RAM里

    sysinit();          // 系统初始化，设置系统频率，初始化printf端口

    //为了防止 main 函数 里 复用了 下载口，导致 下载失败，此处增加 按下 KEY_A 就进入死循环
//    SRART_CHECK();

    main();             // 执行用户主函数

    while(1);           // 死循环
}

/*!
 *  @brief      系统初始化，设置系统频率，初始化printf端口
 *  @since      v5.0
 *  @note       此函数是系统启动初期由汇编函数调用，然后执行main函数
 */
void sysinit (void)
{
    core_clk_mhz = pll_init(CORE_CLK);     //设置系统时钟

    core_clk_khz = core_clk_mhz * 1000;     //把配置好的时钟都保存在全局变量里，以便后续使用
    bus_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24) + 1);

    uart_init (VCAN_PORT, VCAN_BAUD);   //初始化printf用到的管脚
}

/*!
 *  @brief      trace 时钟输出 初始化
 *  @since      v5.0
 *  @note       可用来校验时钟输出是否正确
 *  @author     飞思卡尔公司
 */
void trace_clk_init(void)
{
    /* Set the trace clock to the core clock frequency */
    SIM_SOPT2 |= SIM_SOPT2_TRACECLKSEL_MASK;

    /* Enable the TRACE_CLKOUT pin function on PTA6 (alt7 function) */
    PORTA_PCR6 = ( PORT_PCR_MUX(0x7));
}

/*!
 *  @brief      fb 时钟输出 初始化
 *  @since      v5.0
 *  @note       可用来校验时钟输出是否正确
 *  @author     飞思卡尔公司
 */
void fb_clk_init(void)
{
    /* Enable the clock to the FlexBus module */
    SIM_SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;

    /* Enable the FB_CLKOUT function on PTC3 (alt5 function) */
    PORTC_PCR3 = ( PORT_PCR_MUX(0x5));
}


/*!
 *  @brief      复制中断向量表 和 必要的数据到 RAM里
 *  @since      v5.0
 *  @author     飞思卡尔公司
 */
#pragma section = ".data"
#pragma section = ".data_init"
#pragma section = ".bss"
#pragma section = "CodeRelocate"
#pragma section = "CodeRelocateRam"

void common_startup(void)
{
    /* Declare a counter we'll use in all of the copy loops */
    uint32 n;

    /* Declare pointers for various data sections. These pointers
     * are initialized using values pulled in from the linker file
     */
    uint8 *data_ram, * data_rom, * data_rom_end;
    uint8 *bss_start, * bss_end;


    /*  VECTOR_TABLE 和 VECTOR_RAM 的地址从  linker 文件里获得 （*.icf） */
    extern uint32 __VECTOR_TABLE[];
    extern uint32 __VECTOR_RAM[];

    /* 复制ROM里的中断向量表到RAM里 */
    if (__VECTOR_RAM != __VECTOR_TABLE)             //如果不是RAM启动，则需要复制中断向量表
    {
        for (n = 0; n < 0x410; n++)                 //逐个复制
            __VECTOR_RAM[n] = __VECTOR_TABLE[n];
    }
    /* 指定新的中断向量表地址为 __VECTOR_RAM */
    write_vtor((uint32)__VECTOR_RAM);

    /* 把已赋初值的变量从ROM里复制数据到RAM里 */
    data_ram = __section_begin(".data");            //已赋初值的变量的地址在RAM里
    data_rom = __section_begin(".data_init");       //已赋初值的变量的数据存放在ROM里，需要赋值到RAM里
    data_rom_end = __section_end(".data_init");
    n = data_rom_end - data_rom;

    /* 复制初始化数据到RAM里 */
    while (n--)
        *data_ram++ = *data_rom++;

    /* 没赋初值或者初值为0的变量，需要清除其RAM里的数据，确保值为0 */
    bss_start = __section_begin(".bss");
    bss_end = __section_end(".bss");

    /* 清除没赋初值或者初值为0的变量数据值 */
    n = bss_end - bss_start;
    while(n--)
        *bss_start++ = 0;

    /* 赋值用 __ramfunc 声明的函数的的代码段到 RAM，可以加快代码的运行        */
    uint8 *code_relocate_ram = __section_begin("CodeRelocateRam");
    uint8 *code_relocate = __section_begin("CodeRelocate");
    uint8 *code_relocate_end = __section_end("CodeRelocate");

    /* 从ROM里复制函数代码到RAM里 */
    n = code_relocate_end - code_relocate;
    while (n--)
        *code_relocate_ram++ = *code_relocate++;

}
