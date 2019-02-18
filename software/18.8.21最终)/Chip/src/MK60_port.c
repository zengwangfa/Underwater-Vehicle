/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_port.c
 * @brief      port复用配置和各种功能配置
 * @author     山外科技
 * @version    v5.1
 * @date       2014-04-25
 */

#include "common.h"
#include "MK60_port.h"

PORT_MemMapPtr PORTX[PTX_MAX] = {PORTA_BASE_PTR, PORTB_BASE_PTR, PORTC_BASE_PTR, PORTD_BASE_PTR, PORTE_BASE_PTR};

/*!
 *  @brief      PORT初始化
 *  @param      PTxn    端口
 *  @param      cfg     端口属性配置，如触发选项和上拉下拉选项
 *  @since      v5.0
 *  @note       与port_init_NoALT不同的是，此函数需要配置 MUX 复用功能，
                否则 MUX = ALT0
 *  Sample usage:       port_init (PTA8, IRQ_RISING | PF | ALT1 | PULLUP );    //初始化 PTA8 管脚，上升沿触发中断，带无源滤波器，复用功能为GPIO ，上拉电阻
 */
void  port_init(PTXn_e ptxn, uint32 cfg )
{
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK << PTX(ptxn));                           //开启PORTx端口

    PORT_ISFR_REG(PORTX_BASE(ptxn)) = (1<<PTn(ptxn));                           // 清空标志位

    PORT_PCR_REG(PORTX_BASE(ptxn), PTn(ptxn)) = cfg;                            // 复用功能 , 确定触发模式 ,开启上拉或下拉电阻
}

/*!
 *  @brief      PORT初始化
 *  @param      PTxn    端口
 *  @param      cfg     端口属性配置，如触发选项和上拉下拉选项
 *  @since      v5.0
 *  @note       与port_init不同的是，此函数不需要配置 MUX 复用功能（即使配置了也不生效），
                MUX 保留 为原先寄存器配置的值
 *  Sample usage:       port_init_NoALT (PTA8, IRQ_RISING | PF | PULLUP );    //初始化 PTA8 管脚，上升沿触发中断，带无源滤波器，保留原先复用功能，上拉电阻
 */
void  port_init_NoALT(PTXn_e ptxn, uint32 cfg)
{
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK << PTX(ptxn));                           //开启PORTx端口

    PORT_ISFR_REG(PORTX_BASE(ptxn)) = (1<<PTn(ptxn));                           // 清空标志位

    //清空cfg里的MUX，加载寄存器里的MUX
    cfg &= ~PORT_PCR_MUX_MASK;                      //清了MUX 字段（即不需要配置ALT，保持原来的ALT）
    cfg |=  (PORT_PCR_REG(PORTX_BASE(ptxn), PTn(ptxn)) & PORT_PCR_MUX_MASK);    //读取寄存器里配置的 MUX

    PORT_PCR_REG(PORTX_BASE(ptxn), PTn(ptxn)) = cfg;            // 复用功能 , 确定触发模式 ,开启上拉或下拉电阻
}


/*!
 *  @brief      PORTA的参考中断服务函数
 *  @since      v5.0
 *  @warning    此函数需要用户根据自己需求完成，这里仅仅是提供一个模版
 *  Sample usage:       set_vector_handler(PORTA_VECTORn , porta_handler);    //把 porta_handler 函数添加到中断向量表，不需要我们手动调用
 */
void porta_handler(void)
{
    uint8  n = 0;    //引脚号

    //PTA6
    n = 6;
    if(PORTA_ISFR & (1 << n))           //PTA6触发中断
    {
        PORTA_ISFR  = (1 << n);        //写1清中断标志位

        /*  以下为用户任务  */


        /*  以上为用户任务  */
    }

    //假设 PTA7 需要执行的用户任务函数为 func() ，则可以直接调用如下宏定义：
    //PORT_FUNC(PORTA,7,func);
    //这个宏的作用与上面 PTA6 的效果是一样的，只不过是简化了用户的输入
}