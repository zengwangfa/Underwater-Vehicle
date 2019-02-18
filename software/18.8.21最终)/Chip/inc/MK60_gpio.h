/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_gpio.h
 * @brief      gpio驱动头文件
 * @author     山外科技
 * @version    v5.1
 * @date       2014-04-25
 */
#ifndef __MK60_GPIO_H__
#define __MK60_GPIO_H__

#include "MK60_gpio_cfg.h"

/*
 * 定义管脚方向
 */
typedef enum GPIO_CFG
{
    //这里的值不能改！！！
    GPI         = 0,                                //定义管脚输入方向      GPIOx_PDDRn里，0表示输入，1表示输出
    GPO         = 1,                                //定义管脚输出方向
} GPIO_CFG;

#define HIGH  1u
#define LOW   0u

extern  GPIO_MemMapPtr      GPIOX[PTX_MAX];
#define GPIOX_BASE(PTxn)    GPIOX[PTX(PTxn)]       //GPIO模块的地址


/****************************外部使用****************************/

extern void    gpio_init  (PTXn_e, GPIO_CFG, uint8 data);    //初始化gpio
extern void    gpio_ddr   (PTXn_e, GPIO_CFG);                //设置引脚数据方向
extern void    gpio_set   (PTXn_e,           uint8 data);    //设置引脚状态
extern void    gpio_turn  (PTXn_e);                          //反转引脚状态
extern uint8   gpio_get   (PTXn_e);                          //读取引脚状态

//如下 4个 函数 的 PTxn 只能是 宏定义，不能是 变量
#define GPIO_SET(PTxn,data)       (PTXn_T(PTxn,OUT)= (data))    //设置输出电平
#define GPIO_TURN(PTxn)           (PTXn_T(PTxn,T)= 1)           //翻转输出电平
#define GPIO_GET(PTxn)            (PTXn_T(PTxn,IN))             //读取引脚输入状态
#define GPIO_DDR(PTxn,ddr)        (PTXn_T(PTxn,DDR) = ddr)      //输入输出状态


//如下  函数 的 PTxn 可以是  宏定义，也可以是 变量


//n位操作
#define GPIO_SET_NBIT(NBIT,PTxn,data)   GPIO_PDOR_REG(GPIOX_BASE(PTxn)) =   (                                                   \
                                                                                (                                               \
                                                                                    GPIO_PDOR_REG(GPIOX_BASE(PTxn))             \
                                                                                    &                                           \
                                                                                    ((uint32)( ~(((1<<NBIT)-1)<<PTn(PTxn))))    \
                                                                                )                                               \
                                                                                |   ( ((data)&( (1<<(NBIT))-1))<<PTn(PTxn) )    \
                                                                            )


#define GPIO_DDR_NBIT(NBIT,PTxn,ddr)   GPIO_PDDR_REG(GPIOX_BASE(PTxn))  =   (                                                   \
                                                                                (                                               \
                                                                                    GPIO_PDDR_REG(GPIOX_BASE(PTxn))             \
                                                                                    &                                           \
                                                                                    ((uint32)( ~(((1<<(NBIT))-1)<<PTn(PTxn))))  \
                                                                                )                                               \
                                                                                |   ( ( (ddr) &  ( (1<<(NBIT))-1))<<PTn(PTxn) ) \
                                                                            )

#define GPIO_T_NBIT(NBIT,PTxn,data)   GPIO_PTOR_REG(GPIOX_BASE(PTxn))  =   (                                                    \
                                                                                (                                               \
                                                                                    GPIO_PTOR_REG(GPIOX_BASE(PTxn))             \
                                                                                    &                                           \
                                                                                    ((uint32)( ~(((1<<NBIT)-1)<<PTn(PTxn))))    \
                                                                                )                                               \
                                                                                |   ( ((data)&( (1<<(NBIT))-1))<<PTn(PTxn) )    \
                                                                            )


#define GPIO_GET_NBIT(NBIT,PTxn)    (( GPIO_PDIR_REG(GPIOX_BASE(PTxn))>>PTn(PTxn) ) & ((1<<NBIT)-1))


#endif      //__MK60_GPIO_H__
