/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_dwt.h
 * @brief      flash函数库
 * @author     山外科技
 * @version    v5.0
 * @date       2013-10-22
 */

#include "common.h"

#define  DWT_CLK_HHZ                        (core_clk_khz )
#define  DEM_CR_TRCENA_MASK                 (1 << 24)
#define  DWT_CTRL_CYCCNTENA                 (1 <<  0)

/*!
 *  @brief      延时(us)
 *  @param      us      延时时间
 *  @since      v5.0
 */
void dwt_delay_us(uint32 us)
{
    uint32 st,et,ts;
    DEMCR            |=  DEM_CR_TRCENA_MASK;        //使能 调试时钟 （默认已经打开）
    DWT_CTRL         |=  DWT_CTRL_CYCCNTENA;

    st = DWT_CYCCNT;
    ts =  us * (DWT_CLK_HHZ /(1000));
    et = st + ts;
    if(et < st)
    {
        //溢出，需要转动一周
        while(DWT_CYCCNT > et);      //等待 DWT_CYCCNT 溢出 返回0
    }

    while(DWT_CYCCNT < et);      //等待 DWT_CYCCNT 到底计数值
}

/*!
 *  @brief      延时(ms)
 *  @param      ms      延时时间
 *  @since      v5.0
 */
void dwt_delay_ms(uint32 ms)
{
    while(ms--)
    {
        dwt_delay_us(1000);
    }
}