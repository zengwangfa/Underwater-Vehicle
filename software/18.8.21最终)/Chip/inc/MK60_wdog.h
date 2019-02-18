/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_wdog.h
 * @brief      看门狗驱动函数
 * @author     山外科技
 * @version    v5.0
 * @date       2013-07-02
 */

#ifndef __MK60_WDOG_H__
#define __MK60_WDOG_H__

/********************************************************************/

//wdog 代码，采用 LDO 时钟，不分频。

extern void wdog_init_ms(uint32 ms);   //初始化看门狗，设置喂狗时间 ms
extern void wdog_feed(void);           //喂狗


extern void wdog_disable(void);        //禁用看门狗
extern void wdog_enable(void);         //启用看门狗

/********************************************************************/
#endif /* __MK60_WDOG_H__ */
