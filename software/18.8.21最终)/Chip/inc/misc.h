/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       misc.h
 * @brief      山外K60 平台其他需要用到的函数的实现
 * @author     山外科技
 * @version    v5.1
 * @date       2014-04-25
 */

#ifndef __MISC_H__
#define __MISC_H__

#include "common.h"


void write_vtor (int);                                              //设置中断向量表的地址
void set_vector_handler(VECTORn_t , void pfunc_handler(void));      //设置中断函数到中断向量表里


//兼容旧代码
#define enable_irq(irq)                 NVIC_EnableIRQ(irq)         //使能IRQ
#define disable_irq(irq)                NVIC_DisableIRQ(irq)        //禁止IRQ
#define set_irq_priority(irq,pri0)      NVIC_SetPriority(irq,pri0)  //设置优先级

#if 1    //两种方法，一种是CMSIS自带，一种是直接汇编命令
#define EnableInterrupts                __enable_irq()              //使能全部中断
#define DisableInterrupts               __disable_irq()             //禁止全部中断
#else
#define EnableInterrupts                asm(" CPSIE i");            //使能全部中断
#define DisableInterrupts               asm(" CPSID i");            //禁止全部中断
#endif

void vcan_cpy( uint8 *dst, uint8 *src, uint32 count);

#endif  /* __MISC_H__ */

