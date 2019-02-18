/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_ftm.c
 * @brief      FTM定时器函数库
 * @author     山外科技
 * @version    v5.1
 * @date       2014-04-25
 */


#ifndef _MK60_FTM_H_
#define _MK60_FTM_H_

//定义FTM模块号
typedef enum
{
    FTM0,
    FTM1,
    FTM2,

    FTM_MAX,
} FTMn_e;

//定义FTM 通道号
typedef enum
{

    FTM_CH0,
    FTM_CH1,
    FTM_CH2,
    FTM_CH3,
    FTM_CH4,
    FTM_CH5,
    FTM_CH6,
    FTM_CH7,

} FTM_CHn_e;

//分频系数
typedef enum
{
    FTM_PS_1,
    FTM_PS_2,
    FTM_PS_4,
    FTM_PS_8,
    FTM_PS_16,
    FTM_PS_32,
    FTM_PS_64,
    FTM_PS_128,

    FTM_PS_MAX,
}FTM_PS_e;      //分频值 =  (1<< FTM_PS_e) ,例如  FTM_PS_2  对应的 分频值 = (1<<FTM_PS_2) = (1<<1) = 2


/*********************** PWM **************************/

#define FTM0_PRECISON 10000u     //定义占空比精度，100即精度为1%，1000u则精度为0.1%，用于占空比 duty 形参传入，即占空比为 duty/FTM_PRECISON
#define FTM1_PRECISON 10000u     //定义占空比精度，100即精度为1%，1000u则精度为0.1%，用于占空比 duty 形参传入，即占空比为 duty/FTM_PRECISON
#define FTM2_PRECISON 100u     //定义占空比精度，100即精度为1%，1000u则精度为0.1%，用于占空比 duty 形参传入，即占空比为 duty/FTM_PRECISON

extern void  FTM_PWM_init(FTMn_e, FTM_CHn_e, uint32 freq, uint32 duty);  //初始化FTM的PWM功能并设置频率、占空比。设置通道输出占空比。同一个FTM，各通道的PWM频率是一样的，共3个FTM

extern void  FTM_PWM_Duty(FTMn_e, FTM_CHn_e,uint32 duty);  //设置通道占空比,占空比为 （duty * 精度） % ，如果 FTM_PRECISON 定义为 1000 ，duty = 100 ，则占空比 100*0.1%=10%
extern void  FTM_PWM_freq(FTMn_e,            uint32 freq);               //设置FTM的频率（改频率后，需要重新配置占空比）


/*********************** 输入捕捉 **************************/
//FTM 输入捕捉配置
typedef enum
{
    FTM_Rising,               //上升沿捕捉
    FTM_Falling,              //下降沿捕捉
    FTM_Rising_or_Falling     //跳变沿捕捉
} FTM_Input_cfg;


extern void FTM_Input_init(FTMn_e, FTM_CHn_e, FTM_Input_cfg,FTM_PS_e ps);   //输入捕捉初始化函数
extern void FTM1_Input_test_handler(void);                      //可供参考的 FTM1 输入捕捉中断服务函数

#define FTM_IRQ_EN(FTMn,CHn)        FTM_CnSC_REG(FTMN[FTMn],CHn) |= FTM_CnSC_CHIE_MASK       //开启 FTMn_CHn 中断
#define FTM_IRQ_DIS(FTMn,CHn)       FTM_CnSC_REG(FTMN[FTMn],CHn) &= ~FTM_CnSC_CHIE_MASK      //关闭 FTMn_CHn 中断

/*********************** 正交解码功能 **************************/
extern void     FTM_QUAD_Init(FTMn_e ftmn);         //初始化FTM 的正交解码 功能
extern int16    FTM_QUAD_get(FTMn_e ftmn);          //获取FTM 正交解码 的脉冲数(负数表示反方向)
extern void     FTM_QUAD_clean(FTMn_e ftmn);        //清 FTM 正交解码 的脉冲数

#endif  //_MK60_FTM_H_



