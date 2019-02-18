/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_port.h
 * @brief      port复用配置和各种功能配置
 * @author     山外科技
 * @version    v5.0
 * @date       2013-06-29
 */

#ifndef __MK60_PORT_H__
#define __MK60_PORT_H__


/*! 枚举管脚编号 */
typedef enum
{
    /*  PTA端口    */ //0~31
    PTA0,  PTA1,  PTA2,  PTA3,  PTA4,  PTA5,  PTA6,  PTA7,  PTA8,  PTA9,  PTA10, PTA11, PTA12, PTA13, PTA14, PTA15,
    PTA16, PTA17, PTA18, PTA19, PTA20, PTA21, PTA22, PTA23, PTA24, PTA25, PTA26, PTA27, PTA28, PTA29, PTA30, PTA31,

    /*  PTB端口    */ //32~63
    PTB0,  PTB1,  PTB2,  PTB3,  PTB4,  PTB5,  PTB6,  PTB7,  PTB8,  PTB9,  PTB10, PTB11, PTB12, PTB13, PTB14, PTB15,
    PTB16, PTB17, PTB18, PTB19, PTB20, PTB21, PTB22, PTB23, PTB24, PTB25, PTB26, PTB27, PTB28, PTB29, PTB30, PTB31,

    /*  PTC端口    */
    PTC0,  PTC1,  PTC2,  PTC3,  PTC4,  PTC5,  PTC6,  PTC7,  PTC8,  PTC9,  PTC10, PTC11, PTC12, PTC13, PTC14, PTC15,
    PTC16, PTC17, PTC18, PTC19, PTC20, PTC21, PTC22, PTC23, PTC24, PTC25, PTC26, PTC27, PTC28, PTC29, PTC30, PTC31,

    /*  PTD端口    */
    PTD0,  PTD1,  PTD2,  PTD3,  PTD4,  PTD5,  PTD6,  PTD7,  PTD8,  PTD9,  PTD10, PTD11, PTD12, PTD13, PTD14, PTD15,
    PTD16, PTD17, PTD18, PTD19, PTD20, PTD21, PTD22, PTD23, PTD24, PTD25, PTD26, PTD27, PTD28, PTD29, PTD30, PTD31,

    /*  PTE端口    */
    PTE0,  PTE1,  PTE2,  PTE3,  PTE4,  PTE5,  PTE6,  PTE7,  PTE8,  PTE9,  PTE10, PTE11, PTE12, PTE13, PTE14, PTE15,
    PTE16, PTE17, PTE18, PTE19, PTE20, PTE21, PTE22, PTE23, PTE24, PTE25, PTE26, PTE27, PTE28, PTE29, PTE30, PTE31,
} PTXn_e;

/*! 枚举端口模块 */
typedef enum
{
    PTA, PTB, PTC, PTD, PTE,

    PTX_MAX,
} PTX_e;

/*! 枚举编号 */
typedef enum
{
    PT0 , PT1 , PT2 , PT3 , PT4 , PT5 , PT6 , PT7 ,
    PT8 , PT9 , PT10, PT11, PT12, PT13, PT14, PT15,
    PT16, PT17, PT18, PT19, PT20, PT21, PT22, PT23,
    PT24, PT25, PT26, PT27, PT28, PT29, PT30, PT31,
} PTn_e;

//根据以上的定义，可以得出：PTx = PTxn / 32 ; PTn = PTxn & 31
#define PTX(PTxn)           ((PTxn)>>5)
#define PTn(PTxn)           ((PTxn)&0x1f)
#define PORTX_BASE(PTxn)     PORTX[PTX(PTxn)]       //PORT模块的地址

/*! 枚举PORT 配置 */
typedef enum
{
    //中断方式和DMA请求方式，两者只能选其中一种（可以不选）
    //中断方式选择
    IRQ_ZERO     = 0x08 << PORT_PCR_IRQC_SHIFT,   //低电平触发
    IRQ_RISING   = 0x09 << PORT_PCR_IRQC_SHIFT,   //上升沿触发
    IRQ_FALLING  = 0x0A << PORT_PCR_IRQC_SHIFT,   //下降沿触发
    IRQ_EITHER   = 0x0B << PORT_PCR_IRQC_SHIFT,   //跳变沿触发
    IRQ_ONE      = 0x0C << PORT_PCR_IRQC_SHIFT,   //高电平触发

    //DMA请求选择
    DMA_RISING   = 0x01 << PORT_PCR_IRQC_SHIFT,   //上升沿触发
    DMA_FALLING  = 0x02 << PORT_PCR_IRQC_SHIFT,   //下降沿触发
    DMA_EITHER   = 0x03 << PORT_PCR_IRQC_SHIFT,   //跳变沿触发


    HDS          = 0x01 << PORT_PCR_DSE_SHIFT,    //输出高驱动能力
    ODO          = 0x01 << PORT_PCR_ODE_SHIFT,    //漏极输出
    PF           = 0x01 << PORT_PCR_PFE_SHIFT,    //带无源滤波器
    SSR          = 0x01 << PORT_PCR_SRE_SHIFT,    //输出慢变化率          Slow slew rate

    //下拉上拉选择
    PULLDOWN     = 0x02 << PORT_PCR_PS_SHIFT,     //下拉
    PULLUP       = 0x03 << PORT_PCR_PS_SHIFT,     //上拉

    //功能复用选择(如果不需要改变功能复用选择，保留原先的功能复用，直接选择ALT0 )
    //需要查 K60 Signal Multiplexing and Pin Assignments
    ALT0         = 0x00 << PORT_PCR_MUX_SHIFT,
    ALT1         = 0x01 << PORT_PCR_MUX_SHIFT,    //GPIO
    ALT2         = 0x02 << PORT_PCR_MUX_SHIFT,
    ALT3         = 0x03 << PORT_PCR_MUX_SHIFT,
    ALT4         = 0x04 << PORT_PCR_MUX_SHIFT,
    ALT5         = 0x05 << PORT_PCR_MUX_SHIFT,
    ALT6         = 0x06 << PORT_PCR_MUX_SHIFT,
    ALT7         = 0x07 << PORT_PCR_MUX_SHIFT,
} port_cfg;


extern PORT_MemMapPtr PORTX[PTX_MAX];


extern void  port_init         (PTXn_e , uint32 cfg);   //PORT初始化(配置 MUX 复用功能)
extern void  port_init_NoALT   (PTXn_e , uint32 cfg);   //PORT初始化(不改变 MUX 复用功能)


/*      中断复位函数模版    */
extern void porta_handler(void);                        //中断复位函数，仅供参考（需用户自行实现）

//如下函数可用在 PORT 中断复位函数里，func就是对应管脚的中断复位函数
//具体用法可参考 porta_handler
#define PORT_FUNC(X,num,func)       do{                                     \
                                        if(PORT##X##_ISFR & (1 << num))     \
                                        {                                   \
                                            PORT##X##_ISFR  = (1 << num);   \
                                            func();                         \
                                        }                                   \
                                    }while(0)


#endif      //__MK60_PORT_H__
