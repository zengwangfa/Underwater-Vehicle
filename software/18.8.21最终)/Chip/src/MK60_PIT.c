/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_pit.c
 * @brief      pit定时器函数库
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-23
 */


#include "common.h"
#include  "MK60_PIT.h"     //周期中断计时器

/*!
 *  @brief      PITn定时中断
 *  @param      PITn        模块号（PIT0~PIT3）
 *  @param      cnt         定时中断时间(单位为bus时钟周期)
 *  @since      v5.0
 *  Sample usage:
                    pit_init(PIT0, 1000);                          //定时 1000 个bus时钟 后中断
                    set_vector_handler(PIT0_VECTORn,pit_hander);   // 设置中断复位函数到中断向量表里
                    enable_irq(PIT0_IRQn);                         // 使能LPTMR中断
 */
void pit_init(PITn pitn, uint32 cnt)
{
    //PIT 用的是 Bus Clock 总线频率
    //溢出计数 = 总线频率 * 时间

    ASSERT( cnt > 0 );                          //用断言检测 时间必须不能为 0

    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;          //使能PIT时钟

    PIT_MCR         = (0
                       //| PIT_MCR_MDIS_MASK       //禁用PIT定时器时钟选择（0表示使能PIT，1表示禁止PIT）
                       //| PIT_MCR_FRZ_MASK        //调试模式下停止运行（0表示继续运行，1表示停止运行）
                      );

    PIT_LDVAL(pitn)  = cnt - 1 ;                    //设置溢出中断时间

    PIT_Flag_Clear(pitn);                           //清中断标志位

    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;        //禁止PITn定时器（用于清空计数值）
    PIT_TCTRL(pitn)  = ( 0
                         | PIT_TCTRL_TEN_MASK        //使能 PITn定时器
                         | PIT_TCTRL_TIE_MASK        //开PITn中断
                       );

    //enable_irq((int)pitn + PIT0_IRQn);            //开中断
}

/*!
 *  @brief      PITn延时
 *  @param      PITn        模块号（PIT0~PIT3）
 *  @param      cnt         延时时间(单位为bus时钟周期)
 *  @since      v5.0
 *  Sample usage:
                    pit_delay(PIT0, 1000);                         //延时 1000 个bus时钟
 */
void pit_delay(PITn pitn, uint32 cnt)
{
    //PIT 用的是 Bus Clock 总线频率
    //溢出计数 = 总线频率 * 时间

    ASSERT( cnt > 0 );              //用断言检测 时间必须不能为 0

    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;                          //使能PIT时钟

    PIT_MCR         &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );    //使能PIT定时器时钟 ，调试模式下继续运行

    PIT_TCTRL(pitn) &= ~( PIT_TCTRL_TEN_MASK );                     //禁用PIT ，以便设置加载值生效

    PIT_LDVAL(pitn)  = cnt - 1;                                     //设置溢出中断时间

    PIT_Flag_Clear(pitn);                                           //清中断标志位

    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                        //禁止PITn定时器（用于清空计数值）
    PIT_TCTRL(pitn)  = ( 0
                         | PIT_TCTRL_TEN_MASK                        //使能 PITn定时器
                         //| PIT_TCTRL_TIE_MASK                      //开PITn中断
                       );

    while( !(PIT_TFLG(pitn)& PIT_TFLG_TIF_MASK));

    PIT_Flag_Clear(pitn);                                           //清中断标志位
}

/*!
 *  @brief      PITn计时开始
 *  @param      PITn        模块号（PIT0~PIT3）
 *  @since      v5.0
 *  Sample usage:
                    pit_time_start(PIT0);                          //PIT0计时开始
 */
void pit_time_start(PITn pitn)
{
    //PIT 用的是 Bus Clock 总线频率
    //溢出计数 = 总线频率 * 时间

    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;                          //使能PIT时钟

    PIT_MCR         &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );    //使能PIT定时器时钟 ，调试模式下继续运行

    PIT_TCTRL(pitn) &= ~( PIT_TCTRL_TEN_MASK );                     //禁用PIT ，以便设置加载值生效

    PIT_LDVAL(pitn)  = ~0;                                          //设置溢出中断时间

    PIT_Flag_Clear(pitn);                                           //清中断标志位

    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                        //禁止PITn定时器（用于清空计数值）
    PIT_TCTRL(pitn)  = ( 0
                         | PIT_TCTRL_TEN_MASK                        //使能 PITn定时器
                         //| PIT_TCTRL_TIE_MASK                      //开PITn中断
                       );
}

/*!
 *  @brief      获取 PITn计时时间(超时时会关闭 定时器)
 *  @param      PITn        模块号（PIT0~PIT3）
 *  @since      v5.0
 *  Sample usage:
                        pit_time_get(PIT0);                         //获取 PITn计时时间
 */
uint32 pit_time_get(PITn pitn)
{
    uint32 val;

    val = (~0) - PIT_CVAL(pitn);

    if(PIT_TFLG(pitn)& PIT_TFLG_TIF_MASK)                           //判断是否时间超时
    {
        PIT_Flag_Clear(pitn);                                       //清中断标志位
        PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                    //禁止PITn定时器（用于清空计数值）
        return ~0;
    }

    if(val == (~0))
    {
        val--;              //确保 不等于 ~0
    }
    return val;
}

/*!
 *  @brief      关闭 pit 计时
 *  @param      PITn        模块号（PIT0~PIT3）
 *  @since      v5.0
 *  Sample usage:
                        pit_time_get(PIT0);                         //获取 PITn计时时间
 */
void pit_time_close(PITn pitn)
{
    PIT_Flag_Clear(pitn);                                       //清中断标志位
    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                    //禁止PITn定时器（用于清空计数值）
}