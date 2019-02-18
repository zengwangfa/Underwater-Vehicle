/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_lptmr.h
 * @brief      lptmr驱动函数，包括延时，脉冲计数，定时功能待实现
 * @author     山外科技
 * @version    v5.0
 * @date       2013-06-26
 */

#ifndef __MK60_LPTMR_H__
#define __MK60_LPTMR_H__

/**
 *  @brief LPTMR脉冲计数输入管脚选项
 */
typedef enum
{
    //只有1、2管脚，并没有0、3管脚
    LPT0_ALT1 = 1,      // PTA19
    LPT0_ALT2 = 2       // PTC5
} LPT0_ALTn;

/**
 *  @brief LPTMR脉冲计数触发方式
 */
typedef enum LPT_CFG
{
    LPT_Rising  = 0,    //上升沿触发
    LPT_Falling = 1     //下降沿触发
} LPT_CFG;

#define LPTMR_Flag_Clear()  (LPTMR0_CSR |= LPTMR_CSR_TCF_MASK)         //清除LPT比较标志位

/*          用于延时         */
extern void     lptmr_delay_ms(uint16 ms);      //延时(ms)
extern void     lptmr_delay_us(uint16 us);      //延时(us)

/*       用于定时           */
extern void     lptmr_timing_ms(uint16 ms);     //定时(ms)
extern void     lptmr_timing_us(uint16 ms);     //定时(us)

/*        用于计时           */
extern void     lptmr_time_start_ms(void);      //开始计时(ms)
extern uint32   lptmr_time_get_ms(void);        //获取计时时间 

extern void     lptmr_time_start_us(void);      //开始计时(ns)
extern uint32   lptmr_time_get_us(void);        //获取计时时间 

extern void     lptmr_time_close();             //关闭计时器


/*       用于脉冲计数        */
extern void     lptmr_pulse_init  (LPT0_ALTn, uint16 count, LPT_CFG);   //计数器初始化设置
extern uint16   lptmr_pulse_get   (void);                               //获取计数值
extern void     lptmr_pulse_clean (void);                               //清空计数值


/*      中断复位函数模版    */
extern void lptmr_test_handler(void);                                       //中断复位函数，仅供参考（需用户自行实现）


#endif /* __MK60_LPTMR_H__ */
