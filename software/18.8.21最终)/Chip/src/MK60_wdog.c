/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_wdog.c
 * @brief      看门狗驱动函数
 * @author     山外科技
 * @version    v5.1
 * @date       2014-04-25 
 */

#include "common.h"
#include "MK60_wdog.h"

//内部函数声明
static void wdog_unlock(void);          //看门狗解锁

/*!
 *  @brief      初始化看门狗，设置喂狗时间
 *  @param      cnt     喂狗时间（单位为 ms）
 *  @since      v5.0
 */
void wdog_init_ms(uint32 ms)
{
    ASSERT(ms >= 4);                                //断言，计数时间最小为4个时钟周期（WDOG_TOVALL里说明）

    wdog_unlock();                                  //解锁看门狗，这样才能配置看门狗

    WDOG_PRESC = WDOG_PRESC_PRESCVAL(0);            //设置分频系数 = PRESCVAL +1(PRESCVAL取值范围为0~7)

    WDOG_TOVALH = ms >> 16;                         //设置喂狗时间
    WDOG_TOVALL = (uint16)ms;

    WDOG_STCTRLH = ( 0
                    | WDOG_STCTRLH_WDOGEN_MASK     //WDOGEN置位，使能 看门狗
                    //| WDOG_STCTRLH_CLKSRC_MASK   //看门狗时钟选择（0为 LDO ，1为bus 时钟）
                    | WDOG_STCTRLH_ALLOWUPDATE_MASK
                    | WDOG_STCTRLH_STOPEN_MASK
                    | WDOG_STCTRLH_WAITEN_MASK
                    | WDOG_STCTRLH_STNDBYEN_MASK
                    //|
                    );
}


/*!
 *  @brief      启用看门狗
 *  @since      v5.0
 */
void wdog_enable(void)
{
    wdog_unlock();                                  //解锁看门狗，这样才能配置看门狗

    WDOG_STCTRLH |= WDOG_STCTRLH_WDOGEN_MASK;       //WDOGEN置位，使能 看门狗
}


/*!
 *  @brief      禁用看门狗
 *  @since      v5.0
 */
void wdog_disable(void)
{
    wdog_unlock();                                  //解锁看门狗，这样才能配置看门狗

    WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;      //WDOGEN清0，禁用 看门狗
}

/*!
 *  @brief      喂狗
 *  @since      v5.0
 */
void wdog_feed(void)
{
    //此函数不能单步执行

    //WDOG_REFRESH 寄存器里描述，连续向此寄存器写入0xA602 、0xB480即可解锁，
    //中间不得超过20个时钟周期，因此需要先关总中断

    uint8 tmp = __get_BASEPRI();    //用于返回寄存器 PRIMASK 的值(1bit)
                                    //1表示关中断，0表示开中断

    //关闭总中断，否则有可能没法在 20个周期内连续写入 WDOG_UNLOCK
    DisableInterrupts;

    //更新 看门狗（喂狗）
    WDOG_REFRESH = 0xA602;
    WDOG_REFRESH = 0xB480;

    if(tmp == 0)
    {
        EnableInterrupts;
    }
}

/*!
 *  @brief      解锁看门狗
 *  @since      v5.0
 */
void wdog_unlock(void)
{
    //此函数不能单步执行

    //WDOG_UNLOCK 寄存器里描述，连续向此寄存器写入0xC520、0xD928即可解锁，
    //中间不得超过20个时钟周期，因此需要先关总中断
    uint8 tmp = __get_BASEPRI();        //用于返回寄存器 PRIMASK 的值(1bit)
                                        //1表示关中断，0表示开中断

    //关闭总中断，否则有可能没法在 20个周期内连续写入 WDOG_UNLOCK
    DisableInterrupts;

    //解锁 看门狗
    WDOG_UNLOCK = 0xC520;
    WDOG_UNLOCK = 0xD928;

    if(tmp == 0)
    {
        EnableInterrupts;
    }
}

