/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_SysTick.c
 * @brief      SysTick 驱动函数，包括延时
 * @author     山外科技
 * @version    v5.0
 * @date       2013-10-08
 */


#include "common.h"
#include "MK60_SysTick.h"
/*
Cortex-M4 的内核中包含一个 SysTick 时钟。SysTick?为一个?24?位递减计数器，
SysTick 设定初值并使能后，每经过 1 个系统时钟周期，计数值就减 1。
计数到 0 时，SysTick 计数器自动重装初值并继续计数，
同时内部的 COUNTFLAG 标志会置位，触发中断(如果中断使能情况下)。
*/


/*!
 *  @brief      SysTick延时函数
 *  @param      time          LPTMR延时时间(0~65535)
 *  @since      v5.0
 *  Sample usage:       systick_delay(32);     // systick 延时32 个系统周期
 */
void systick_delay(uint32 time)
{
    if(time == 0)
    {
        return;
    }

    ASSERT(time <= (SysTick_RVR_RELOAD_MASK >> SysTick_RVR_RELOAD_SHIFT));  //24位

    SYST_CSR = 0x00;                        //先关了 systick ,清标志位

    SYST_RVR = time;                        //设置延时时间

    SYST_CVR = 0x00;                        //清空计数器

    SYST_CSR = ( 0
                 | SysTick_CSR_ENABLE_MASK       //使能 systick
                 //| SysTick_CSR_TICKINT_MASK    //使能中断 (注释了表示关闭中断)
                 | SysTick_CSR_CLKSOURCE_MASK    //时钟源选择 ( core clk)
               );

    while( !(SYST_CSR & SysTick_CSR_COUNTFLAG_MASK));   //等待时间到
}

/*!
 *  @brief      SysTick延时函数
 *  @param      ms          延时时间
 *  @since      v5.0
 *  Sample usage:       systick_delay_ms(32);     // systick 延时32 ms
 */
void systick_delay_ms(uint32 ms)
{
    while(ms--)
    {
        systick_delay(SYSTICK_CLK_KHZ);
    }
}

/*!
 *  @brief      SysTick定时函数
 *  @param      time          定时时间(0~65535)
 *  @since      v5.0
 *  Sample usage:       systick_timing(32);     // systick 定时 32 个系统周期
 */
void systick_timing(uint32 time)
{

    ASSERT(time <= (SysTick_RVR_RELOAD_MASK >> SysTick_RVR_RELOAD_SHIFT));  //24位

    SYST_RVR = time;                        //设置延时时间

    //设置优先级
    NVIC_SetPriority (SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1); /* set Priority for Cortex-M4 System Interrupts */

    SYST_CVR = 0x00;                        //清空计数器

    SYST_CSR = ( 0
                 | SysTick_CSR_ENABLE_MASK       //使能 systick
                 | SysTick_CSR_TICKINT_MASK      //使能中断 (注释了表示关闭中断)
                 | SysTick_CSR_CLKSOURCE_MASK    //时钟源选择 ( core clk)
               );
}




