/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_SysTick.h
 * @brief      SysTick 驱动函数，包括延时
 * @author     山外科技
 * @version    v5.0
 * @date       2013-10-08
 */

//SysTick 计数时间太短，不宜用于 计时，因而不加入 计时功能

#define     SYSTICK_CLK_KHZ                 core_clk_khz


/*          用于延时         */
extern void    systick_delay(uint32 time);
extern void    systick_delay_ms(uint32 ms);

#define     systick_delay_us(us)          systick_delay(us * SYSTICK_CLK_KHZ/1000);   //延时 us
#define     systick_delay_ns(ns)          systick_delay(ns * SYSTICK_CLK_KHZ/1000000);//延时 ns

/*       用于定时           */
extern void systick_timing(uint32 time);
#define systick_timing_ms(ms)               systick_timing(ms * SYSTICK_CLK_KHZ);        //延时 ms
#define systick_timing_us(us)               systick_timing(us * SYSTICK_CLK_KHZ/1000);   //延时 us
#define systick_timing_ns(ns)               systick_timing(ns * SYSTICK_CLK_KHZ/1000000);//延时 ns


