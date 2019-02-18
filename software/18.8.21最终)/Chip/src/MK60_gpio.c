/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_gpio.c
 * @brief      gpio驱动函数
 * @author     山外科技
 * @version    v5.0
 * @date       2013-06-26
 */

/*
 * 包含头文件
 */
#include "common.h"
#include "MK60_port.h"
#include "MK60_gpio.h"

/*
 * 定义数组
 */
GPIO_MemMapPtr GPIOX[PTX_MAX] = {PTA_BASE_PTR, PTB_BASE_PTR, PTC_BASE_PTR, PTD_BASE_PTR, PTE_BASE_PTR}; //定义五个指针数组保存 GPIOX 的地址



/*!
 *  @brief      初始化gpio
 *  @param      PTxn    端口
 *  @param      cfg     引脚方向,0=输入,1=输出
 *  @param      data    输出初始状态,0=低电平,1=高电平 （对输入无效）
 *  @since      v5.0
 *  Sample usage:       gpio_init (PTA8, GPI,0);    //初始化 PTA8 管脚为输入
 */
void gpio_init (PTXn_e ptxn, GPIO_CFG cfg, uint8 data)
{
    //复用管脚为GPIO功能
    port_init( ptxn, ALT1);

    //端口方向控制输入还是输出
    if(  cfg == GPI )
    {
        //设置端口方向为输入
        GPIO_PDDR_REG(GPIOX_BASE(ptxn)) &= ~(1 << PTn(ptxn));       // GPIO PDDR 管脚号 清0，即对应管脚配置为端口方向输入
    }
    else
    {
        //设置端口方向为输出
        GPIO_PDDR_REG(GPIOX_BASE(ptxn)) |= (1 << PTn(ptxn));        // GPIO PDDR 管脚号 置1，即对应管脚配置为端口方向输出

        //端口输出数据
        if(data == 0)
        {
            GPIO_PDOR_REG(GPIOX_BASE(ptxn)) &= ~(1 << PTn(ptxn));   // GPIO PDOR 管脚号 清0，即对应管脚配置为端口输出低电平
        }
        else
        {
            GPIO_PDOR_REG(GPIOX_BASE(ptxn))  |= (1 << PTn(ptxn));   // GPIO PDOR 管脚号 置1，即对应管脚配置为端口输出高电平
        }
    }
}

/*!
 *  @brief      设置引脚数据方向
 *  @param      PTxn    端口
 *  @param      cfg     引脚方向,0=输入,1=输出
 *  @since      v5.0
 *  Sample usage:       gpio_ddr (PTA8, GPI);    //设置 PTA8 管脚为输入
 */
void    gpio_ddr   (PTXn_e ptxn, GPIO_CFG cfg)
{
    //端口方向控制输入还是输出
    if(  cfg == GPI )
    {
        //设置端口方向为输入
        GPIO_PDDR_REG(GPIOX_BASE(ptxn)) &= ~(1 << PTn(ptxn));           // GPIO PDDR 管脚号 清0，即对应管脚配置为端口方向输入
    }
    else
    {
        //设置端口方向为输出
        GPIO_PDDR_REG(GPIOX_BASE(ptxn)) |= (1 << PTn(ptxn));            // GPIO PDDR 管脚号 置1，即对应管脚配置为端口方向输出
    }
}

/*!
 *  @brief      设置引脚状态
 *  @param      PTxn    端口
 *  @param      data    输出初始状态,0=低电平,1=高电平 （对输入无效）
 *  @since      v5.0
 *  @warning    务必保证数据方向为输出（DEBUG模式下，有断言进行检测）
 *  Sample usage:       gpio_set (PTA8, 1);    // PTA8 管脚 输出 1
 */
void gpio_set (PTXn_e ptxn, uint8 data)
{
    ASSERT( BIT_GET( GPIO_PDDR_REG(GPIOX_BASE(ptxn)) , PTn(ptxn)) == GPO ); // 断言，检测 输出方向是否为输出
                                                                            // 获取 GPIO PDDR 管脚号 ，比较是否为输出

    //端口输出数据
    if(data == 0)
    {
        GPIO_PDOR_REG(GPIOX_BASE(ptxn)) &= ~(1 << PTn(ptxn));   // GPIO PDOR 管脚号 清0，即对应管脚配置为端口输出低电平
    }
    else
    {
        GPIO_PDOR_REG(GPIOX_BASE(ptxn))  |= (1 << PTn(ptxn));   // GPIO PDOR 管脚号 置1，即对应管脚配置为端口输出高电平
    }
}


/*!
 *  @brief      反转引脚状态
 *  @param      PTxn    端口
 *  @since      v5.0
 *  @warning    务必保证数据方向为输出（DEBUG模式下，有断言进行检测）
 *  Sample usage:       gpio_turn (PTA8);    // PTA8 管脚 输出 反转
 */
void gpio_turn (PTXn_e ptxn)
{
    ASSERT( BIT_GET( GPIO_PDDR_REG(GPIOX_BASE(ptxn)) , PTn(ptxn)) == GPO ); // 断言，检测 输出方向是否为输出
                                                                            // 获取 GPIO PDDR 管脚号 ，比较是否为输出

    GPIO_PTOR_REG( GPIOX_BASE(ptxn))  =  1 << (PTn(ptxn ));                 // GPIO PTOR ptxn 置1，其他清0 ，即对应管脚配置为端口输出反转，其他位不变
                                                                            // 此处不能用 BIT_SET 这个宏来置1 ，因为必须保证其他位 不变，其他位直接清0即可
}

/*!
 *  @brief      读取引脚输入状态
 *  @param      PTxn    端口
 *  @return     管脚的状态，1为高电平，0为低电平
 *  @since      v5.0
 *  @warning    务必保证数据方向为输入（DEBUG模式下，有断言进行检测）
 *  Sample usage:       uint8 pta8_data = gpio_get (PTA8);    // 获取 PTA8 管脚 输入电平
 */
uint8 gpio_get(PTXn_e ptxn)
{
    ASSERT( BIT_GET( GPIO_PDDR_REG(GPIOX_BASE(ptxn)) , PTn(ptxn)) == GPI ); // 断言，检测 输出方向是否为输入
                                                                            // 获取 GPIO PDDR 管脚号 ，比较是否为输入

    return ((GPIO_PDIR_REG(GPIOX_BASE(ptxn)) >> PTn(ptxn )) & 0x01);        // 获取 GPIO PDIR ptxn 状态，即读取管脚输入电平
}
