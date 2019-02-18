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

 
/*
 * 包含头文件
 */
#include "common.h"
#include  "MK60_FTM.h"

/*
 * 定义数组
 */
FTM_MemMapPtr FTMN[3] = {FTM0_BASE_PTR, FTM1_BASE_PTR, FTM2_BASE_PTR}; //定义三个指针数组保存 FTMn_e 的地址

/*!
 *  @brief      初始化FTM 的端口
 *  @param      FTMn_e    模块号（FTM0、  FTM1、  FTM2）
 *  @param      FTM_CHn_e     通道号（CH0~CH7）
 *  @since      v5.0
 */
static void FTM_port_mux(FTMn_e ftmn, FTM_CHn_e ch)
{
    /******************* 开启时钟 和 复用IO口*******************/
    switch(ftmn)
    {
    case FTM0:
        SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;       //使能FTM0时钟
        switch(ch)
        {
        case FTM_CH0:
            if(FTM0_CH0 == PTC1)
            {
                port_init(FTM0_CH0, ALT4);
            }
            else if(FTM0_CH0 == PTA3)
            {
                port_init(FTM0_CH0, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH1:
            if(FTM0_CH1 == PTC2)
            {
                port_init(FTM0_CH1, ALT4);
            }
            else if(FTM0_CH1 == PTA4)
            {
                port_init(FTM0_CH1, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH2:
            if(FTM0_CH2 == PTC3)
            {
                port_init(FTM0_CH2, ALT4);
            }
            else if(FTM0_CH2 == PTA5)
            {
                port_init(FTM0_CH2, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH3:
            if(FTM0_CH3 == PTC4)
            {
                port_init(FTM0_CH3, ALT4);
            }
            else if(FTM0_CH3 == PTA6)
            {
                port_init(FTM0_CH3, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH4:
            if(FTM0_CH4 == PTD4)
            {
                port_init(FTM0_CH4, ALT4);
            }
            else if(FTM0_CH4 == PTA7)
            {
                port_init(FTM0_CH4, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH5:
            if(FTM0_CH5 == PTD5)
            {
                port_init(FTM0_CH5, ALT4);
            }
            else if(FTM0_CH5 == PTA0)
            {
                port_init(FTM0_CH5, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH6:
            if(FTM0_CH6 == PTD6)
            {
                port_init(FTM0_CH6, ALT4);
            }
            else if(FTM0_CH6 == PTA1)
            {
                port_init(FTM0_CH6, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH7:
            if(FTM0_CH7 == PTD7)
            {
                port_init(FTM0_CH7, ALT4);
            }
            else if(FTM0_CH7 == PTA2)
            {
                port_init(FTM0_CH7, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;
        default:
            return;
        }
        break;

    case FTM1:
        SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;       //使能FTM1时钟
        switch(ch)
        {
        case FTM_CH0:
            if((FTM1_CH0 == PTA8) || (FTM1_CH0 == PTA12) || (FTM1_CH0 == PTB0) )
            {
                port_init(FTM1_CH0, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;


        case FTM_CH1:
            if((FTM1_CH1 == PTA9) || (FTM1_CH1 == PTA13) || (FTM1_CH1 == PTB1) )
            {
                port_init(FTM1_CH1, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        default:
            return;
        }
        break;

    case FTM2:
        SIM_SCGC3 |= SIM_SCGC3_FTM2_MASK;                           //使能FTM2时钟
        switch(ch)
        {
        case FTM_CH0:
            if((FTM2_CH0 == PTA10) || (FTM2_CH0 == PTB18) )
            {
                port_init(FTM2_CH0, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        case FTM_CH1:
            if((FTM2_CH1 == PTA11) || (FTM2_CH1 == PTB19))
            {
                port_init(FTM2_CH1, ALT3);
            }
            else
            {
                ASSERT(0);                      //设置管脚有误？
            }
            break;

        default:
            return;
        }
        break;
    default:
        break;
    }
}

/*!
 *  @brief      初始化FTM 的PWM 功能
 *  @param      FTMn_e    模块号（FTM0、  FTM1、  FTM2）
 *  @param      FTM_CHn_e     通道号（CH0~CH7）
 *  @param      freq    频率（单位为Hz）
 *  @param      duty    占空比分子，占空比 = duty / FTMn_PRECISON
 *  @since      v5.0
 *  @note       同一个FTM，PWM频率是必须一样的，但占空比可不一样。共3个FTM，即可以输出3个不同频率PWM
 *  Sample usage:       FTM_PWM_init(FTM0, FTM_CH6,200, 10);    //初始化 FTM0_CH6 为 频率 200Hz 的PWM，占空比为 10/FTM0_PRECISON
 */
void FTM_PWM_init(FTMn_e ftmn, FTM_CHn_e ch, uint32 freq, uint32 duty)
{
    uint32 clk_hz ;
    uint16 mod;
    uint8  ps;
    uint16 cv;

    ASSERT( (ftmn == FTM0) || ( (ftmn == FTM1 || ftmn == FTM2 ) && (ch <= FTM_CH1))   );  //检查传递进来的通道是否正确

    /******************* 开启时钟 和 复用IO口*******************/
    FTM_port_mux(ftmn,ch);

    /*       计算频率设置        */
    //  若 CPWMS = 1 ，即双边捕捉脉冲，则 PMW频率 =  bus频率 /2 /(2^预分频因子)/模数
    //  若 CPWMS = 0 ，即单边捕捉脉冲，则 PMW频率 =  bus频率    /(2^预分频因子)/模数
    //  EPWM的周期 ：MOD - CNTIN + 0x0001   (CNTIN 设为0)
    //  脉冲宽度：CnV - CNTIN

    //  模数 MOD < 0x10000
    //  预分频因子 PS  < 0x07
    //  预分频因子 PS 越小时，模数 mod 就越大，计数就越精准，PWM输出更为准确
    //  MOD  = clk_hz/(freq*(1 << PS)) < 0x10000  ==>  clk_hz/(freq*0x10000) < (1<< PS)  ==>  (clk_hz/(freq*0x10000) >> PS) < 1
    //  即 (((clk_hz/0x10000 )/ freq ) >> PS ) < 1

    // 以 CPWMS = 0 ，即单边捕捉脉冲为例
    clk_hz = (bus_clk_khz * 1000) ;     // bus频率

    mod = (clk_hz >> 16 ) / freq ;      // 临时用 mod 缓存一下
    ps = 0;
    while((mod >> ps) >= 1)             // 等 (mod >> ps) < 1 才退出 while 循环 ，即求 PS 的最小值
    {
        ps++;
    }

    ASSERT(ps <= 0x07);                 // 断言， PS 最大为 0x07 ，超过此值，则 PWM频率设置过低，或 Bus 频率过高

    mod = (clk_hz >> ps) / freq;        // 求 MOD 的值

    switch(ftmn)                        // 初值 CNTIN 设为0 ，脉冲宽度：CnV - CNTIN ，即 CnV 就是 脉冲宽度了。
    {
        // EPWM的周期 ： MOD - CNTIN + 0x0001 == MOD - 0 + 1
        // 则 CnV = (MOD - 0 + 1) * 占空比 = (MOD - 0 + 1) * duty/ FTM_PRECISON
    case FTM0:
        cv = (duty * (mod - 0 + 1)) / FTM0_PRECISON;
        break;

    case FTM1:
        cv = (duty * (mod - 0 + 1)) / FTM1_PRECISON;
        break;

    case FTM2:
        cv = (duty * (mod - 0 + 1)) / FTM2_PRECISON;
        break;

    default:
        break;
    }

    /******************** 选择输出模式为 边沿对齐PWM *******************/
    //通道状态控制，根据模式来选择 边沿或电平
    FTM_CnSC_REG(FTMN[ftmn], ch) &= ~FTM_CnSC_ELSA_MASK;
    FTM_CnSC_REG(FTMN[ftmn], ch)  = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
    // MSnB:MSnA = 1x       边沿对齐PWM
    // ELSnB:ELSnA = 10     先高后低
    // ELSnB:ELSnA = 11     先低后高

    /******************** 配置时钟和分频 ********************/
    FTM_SC_REG(FTMN[ftmn])    = ( 0
                                  //| FTM_SC_CPWMS_MASK         //0：上升沿计数模式 ，1： 跳变沿计数模式选择 （注释了表示 0）
                                  | FTM_SC_PS(ps)             //分频因子，分频系数 = 2^PS
                                  | FTM_SC_CLKS(1)            //时钟选择， 0：没选择时钟，禁用； 1：bus 时钟； 2：MCGFFCLK； 3：EXTCLK（ 由SIM_SOPT4 选择输入管脚 FTM_CLKINx）
                                  //| FTM_SC_TOIE_MASK        //溢出中断使能（注释了表示 禁止溢出中断）
                                );
    FTM_MOD_REG(FTMN[ftmn])   = mod;                        //模数, EPWM的周期为 ：MOD - CNTIN + 0x0001
    FTM_CNTIN_REG(FTMN[ftmn]) = 0;                          //计数器初始化值。设置脉冲宽度：(CnV - CNTIN).
    FTM_CnV_REG(FTMN[ftmn], ch) = cv;
    FTM_CNT_REG(FTMN[ftmn])   = 0;                          //计数器。只有低16位可用（写任何值到此寄存器，都会加载 CNTIN 的值）
}

/*!
 *  @brief      设置FTM 的PWM 通道占空比
 *  @param      FTMn_e    模块号（FTM0、  FTM1、  FTM2）
 *  @param      FTM_CHn_e     通道号（CH0~CH7）
 *  @param      duty    占空比分子，占空比 = duty / FTMn_PRECISON
 *  @since      v5.0
 *  @note       同一个FTM，PWM频率是必须一样的，但占空比可不一样。共3个FTM，即可以输出3个不同频率PWM
 *  Sample usage:       FTM_PWM_Duty(FTM0, FTM_CH6, 10);    //设置 FTM0_CH6占空比为 10/FTM0_PRECISON
 */
void FTM_PWM_Duty(FTMn_e ftmn, FTM_CHn_e ch, uint32 duty)
{
    uint32 cv;
    uint32 mod = 0;

    ASSERT( (ftmn == FTM0) || ( (ftmn == FTM1 || ftmn == FTM2 ) && (ch <= FTM_CH1)) ); //检查传递进来的通道是否正确

    switch(ftmn)
    {
    case FTM0:
        ASSERT(duty <= FTM0_PRECISON);     //用断言检测 占空比是否合理
        break;

    case FTM1:
        ASSERT(duty <= FTM1_PRECISON);     //用断言检测 占空比是否合理
        break;

    case FTM2:
        ASSERT(duty <= FTM2_PRECISON);     //用断言检测 占空比是否合理
        break;

    default:
        break;
    }


    //占空比 = (CnV-CNTIN)/(MOD-CNTIN+1)

    do
    {
        mod = FTM_MOD_REG(FTMN[ftmn]);        //读取 MOD 的值
    }
    while(mod == 0);      //读一次，可能是0 ，需要多读取几次。

    switch(ftmn)
    {
    case FTM0:
        cv = (duty * (mod - 0 + 1)) / FTM0_PRECISON;
        break;

    case FTM1:
        cv = (duty * (mod - 0 + 1)) / FTM1_PRECISON;
        break;

    case FTM2:
        cv = (duty * (mod - 0 + 1)) / FTM2_PRECISON;
        break;
    default:
        break;
    }

    // 配置FTM通道值
    FTM_CnV_REG(FTMN[ftmn], ch) = cv;

}

/*!
 *  @brief      设置FTM的频率
 *  @param      freq    频率（单位为Hz）
 *  @since      v5.0
 *  @note       修改PWM频率后，必须调用 FTM_PWM_Duty 重新配置占空比。同一个模块，PWM频率必须相同。
 *  Sample usage:       FTM_PWM_freq(FTM0,200);    //设置 FTM0 的 频率 为 200Hz
 */
void FTM_PWM_freq(FTMn_e ftmn, uint32 freq)             //设置FTM的频率
{
    uint32 clk_hz = (bus_clk_khz * 1000) >> 1;        //bus频率/2
    uint32 mod;
    uint8 ps;

    /*       计算频率设置        */
    // 以 CPWMS = 0 ，即单边捕捉脉冲为例
    clk_hz = (bus_clk_khz * 1000) ;     // bus频率

    mod = (clk_hz >> 16 ) / freq ;      // 临时用 mod 缓存一下
    ps = 0;
    while((mod >> ps) >= 1)             // 等 (mod >> ps) < 1 才退出 while 循环 ，即求 PS 的最小值
    {
        ps++;
    }

    ASSERT(ps <= 0x07);                 // 断言， PS 最大为 0x07 ，超过此值，则 PWM频率设置过低，或 Bus 频率过高

    mod = (clk_hz >> ps) / freq;        // 求 MOD 的值

    /******************** 配置时钟和分频 ********************/
    FTM_SC_REG(FTMN[ftmn])    = ( 0
                                  //| FTM_SC_CPWMS_MASK         //0：上升沿计数模式 ，1： 跳变沿计数模式选择 （注释了表示 0）
                                  | FTM_SC_PS(ps)             //分频因子，分频系数 = 2^PS
                                  | FTM_SC_CLKS(1)            //时钟选择， 0：没选择时钟，禁用； 1：bus 时钟； 2：MCGFFCLK； 3：EXTCLK（ 由SIM_SOPT4 选择输入管脚 FTM_CLKINx）
                                  //| FTM_SC_TOIE_MASK        //溢出中断使能（注释了表示 禁止溢出中断）
                                );
    FTM_CNTIN_REG(FTMN[ftmn]) = 0;      //计数器初始化值。设置脉冲宽度：(CnV - CNTIN).
    FTM_MOD_REG(FTMN[ftmn])   = mod;    //模数, EPWM的周期为 ：MOD - CNTIN + 0x0001
    FTM_CNT_REG(FTMN[ftmn])   = 0;      //计数器。只有低16位可用（写任何值到此寄存器，都会加载 CNTIN 的值）
}

//////////////////////////////// 以上为输出PWM  //////////////////////////////////////////

//////////////////////////////// 以下为输入捕捉 //////////////////////////////////////////

/*************************************************************************
*                             山外嵌入式开发工作室
*
*  函数名称：FTM_Input_init
*  功能说明：输入捕捉初始化函数
*  参数说明：FTMn        模块号（FTM0、  FTM1、  FTM2）
*            FTM_CHn_e         通道号（CH0~CH7）
*            Input_cfg   输入捕捉配置（Rising、Falling、Rising_or_Falling）上升沿捕捉、下降沿捕捉、跳变沿捕捉
*  函数返回：无
*  修改时间：2012-1-26
*  备    注：CH0~CH3可以使用过滤器，未添加这功能
*************************************************************************/
void FTM_Input_init(FTMn_e ftmn, FTM_CHn_e ch, FTM_Input_cfg cfg,FTM_PS_e ps)
{
    ASSERT( (ftmn == FTM0) || ( (ftmn == FTM1 || ftmn == FTM2 ) && (ch <= FTM_CH1)) ); //检查传递进来的通道是否正确

    /******************* 开启时钟 和 复用IO口*******************/
    FTM_port_mux(ftmn,ch);

    /******************* 设置为输入捕捉功能 *******************/
    switch(cfg)
    {
        //输入捕捉模式下：DECAPEN = 0 ， DECAPEN = 0 ，CPWMS = 0， MSnB:MSnA = 0

        // ELSnB:ELSnA         1          10          11
        // 配置             上升沿      下降沿      跳变沿

    case FTM_Rising:    //上升沿触发
        FTM_CnSC_REG(FTMN[ftmn], ch) |=  ( FTM_CnSC_ELSA_MASK  | FTM_CnSC_CHIE_MASK );                   //置1
        FTM_CnSC_REG(FTMN[ftmn], ch) &= ~( FTM_CnSC_ELSB_MASK  | FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK); //清0
        break;

    case FTM_Falling:   //下降沿触发
        FTM_CnSC_REG(FTMN[ftmn], ch) |= (FTM_CnSC_ELSB_MASK  | FTM_CnSC_CHIE_MASK );                    //置1
        FTM_CnSC_REG(FTMN[ftmn], ch) &= ~( FTM_CnSC_ELSA_MASK | FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK); //清0
        break;

    case FTM_Rising_or_Falling: //上升沿、下降沿都触发
        FTM_CnSC_REG(FTMN[ftmn], ch) |=  ( FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK  | FTM_CnSC_CHIE_MASK ); //置1
        FTM_CnSC_REG(FTMN[ftmn], ch) &= ~( FTM_CnSC_MSB_MASK  | FTM_CnSC_MSA_MASK); //清0
        break;
    }

    FTM_SC_REG(FTMN[ftmn]) = ( 0
                              | FTM_SC_CLKS(0x1)       //选择 bus 时钟
							  | FTM_SC_PS(ps)          //选择 分频系数
                             );
    FTM_MODE_REG(FTMN[ftmn])    |= FTM_MODE_WPDIS_MASK; //禁止写保护
    FTM_COMBINE_REG(FTMN[ftmn]) = 0;
    FTM_MODE_REG(FTMN[ftmn])    &= ~FTM_MODE_FTMEN_MASK;    //使能FTM
    FTM_CNTIN_REG(FTMN[ftmn])   = 0;

    FTM_STATUS_REG(FTMN[ftmn])  = 0x00;               //清中断标志位

    //开启输入捕捉中断
    //enable_irq(FTM0_IRQn + ftmn);
}

/*!
 *  @brief      FTM测试中断服务函数
 *  @since      v5.0
 *  @warning    此函数需要用户根据自己需求完成，这里仅仅是提供一个模版
 *  Sample usage:       set_vector_handler(FTM0_IRQn , FTM1_Input_test_handler);    //把 FTM1_Input_test_handler 函数添加到中断向量表，不需要我们手动调用
 */
void FTM1_Input_test_handler(void)
{
    uint8 s = FTM1_STATUS;             //读取捕捉和比较状态  All CHnF bits can be checked using only one read of STATUS.
    uint8 CHn;

    FTM1_STATUS = 0x00;             //清中断标志位

    CHn = 0;
    if( s & (1 << CHn) )
    {
        //FTM_IRQ_DIS(FTM1, CHn);     //禁止输入捕捉中断
        /*     用户任务       */

        /*********************/
        //FTM_IRQ_EN(FTM1, CHn); //开启输入捕捉中断

    }

    /* 这里添加 n=1 的模版，根据模版来添加 */
    CHn = 1;
    if( s & (1 << CHn) )
    {
        //FTM_IRQ_EN(FTM1, CHn); //开启输入捕捉中断
        /*     用户任务       */


        /*********************/
        //不建议在这里开启输入捕捉中断
        //FTM_IRQ_EN(FTM1, CHn); //开启输入捕捉中断
    }
}

//////////////////////////////// 以上为输入捕捉  //////////////////////////////////////////

//////////////////////////////// 以下为正交解码 //////////////////////////////////////////

/*!
 *  @brief      初始化FTM 的正交解码 功能
 *  @param      FTMn_e    模块号（ FTM1、  FTM2）
 *  @since      v5.0
 *  Sample usage:       FTM_QUAD_Init(FTM1);    //初始化 FTM1 为正交解码模式
 */
void FTM_QUAD_Init(FTMn_e ftmn)
{
    ASSERT( (ftmn == FTM1) || (ftmn == FTM2 ) ); //检查传递进来的通道是否正确

    /******************* 开启时钟 和 复用IO口*******************/
    switch(ftmn)
    {

    case FTM1:
        SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;       //使能FTM1时钟
        if(FTM1_QDPHA == PTA8)                  //管脚复用
        {
            port_init(FTM1_QDPHA, ALT6);
        }
        else if(FTM1_QDPHA == PTA12)
        {
            port_init(FTM1_QDPHA, ALT7);
        }
        else if(FTM1_QDPHA == PTB0)
        {
            port_init(FTM1_QDPHA, ALT6);
        }
        else
        {
            ASSERT(0);                          //断言，配置的管脚不符合要求
        }

        if(FTM1_QDPHB == PTA9)
        {
            port_init(FTM1_QDPHB, ALT6);
        }
        else if(FTM1_QDPHB == PTA13)
        {
            port_init(FTM1_QDPHB, ALT7);
        }
        else if(FTM1_QDPHB == PTB1)
        {
            port_init(FTM1_QDPHB, ALT6);
        }
        else
        {
            ASSERT(0);                          //断言，配置的管脚不符合要求
        }
        break;

    case FTM2:
        SIM_SCGC3 |= SIM_SCGC3_FTM2_MASK;                           //使能FTM2时钟
        if(FTM2_QDPHA == PTA10)                  //管脚复用
        {
            port_init(FTM2_QDPHA, ALT6);
        }
        else if(FTM2_QDPHA == PTB18)
        {
            port_init(FTM2_QDPHA, ALT6);
        }
        else
        {
            ASSERT(0);                          //断言，配置的管脚不符合要求
        }

        if(FTM2_QDPHB == PTA11)                  //管脚复用
        {
            port_init(FTM2_QDPHB, ALT6);
        }
        else if(FTM2_QDPHB == PTB19)
        {
            port_init(FTM2_QDPHB, ALT6);
        }
        else
        {
            ASSERT(0);                          //断言，配置的管脚不符合要求
        }
        break;
    default:
        ASSERT(0);                              //断言，配置的模块有误
        break;
    }

    FTM_MODE_REG(FTMN[ftmn])  |=    (0
                                     | FTM_MODE_WPDIS_MASK  //写保护禁止
                                     //| FTM_MODE_FTMEN_MASK   //使能 FTM
                                    );
    FTM_QDCTRL_REG(FTMN[ftmn]) |=   (0
                                    | FTM_QDCTRL_QUADMODE_MASK
                                     );
    FTM_CNTIN_REG(FTMN[ftmn])   = 0;
    FTM_MOD_REG(FTMN[ftmn])     = FTM_MOD_MOD_MASK;
    FTM_QDCTRL_REG(FTMN[ftmn]) |=   (0
                                    | FTM_QDCTRL_QUADEN_MASK
                                     );
    FTM_MODE_REG(FTMN[ftmn])  |= FTM_QDCTRL_QUADEN_MASK;
    FTM_CNT_REG(FTMN[ftmn])     = 0;                    //计数器。只有低16位可用（写任何值到此寄存器，都会加载 CNTIN 的值）
}

/*!
 *  @brief      获取FTM 正交解码 的脉冲数
 *  @param      FTMn_e    模块号（ FTM1、  FTM2）
 *  @since      v5.0
 *  Sample usage:       int16 count = FTM_QUAD_get(FTM1);    //获取  FTM1 交解码 的脉冲数
 */
int16 FTM_QUAD_get(FTMn_e ftmn)
{
    int16 val;
    ASSERT( (ftmn == FTM1) || (ftmn == FTM2 ) );        //检查传递进来的通道是否正确
    val = FTM_CNT_REG(FTMN[ftmn]);

    return val;
}

/*!
 *  @brief      清 FTM 正交解码 的脉冲数
 *  @param      FTMn_e    模块号（ FTM1、  FTM2）
 *  @since      v5.0
 *  Sample usage:       FTM_QUAD_clean(FTM1);    //复位 FTM1 正交解码 的脉冲数
 */
void FTM_QUAD_clean(FTMn_e ftmn)
{
    ASSERT( (ftmn == FTM1) || (ftmn == FTM2 ) ); //检查传递进来的通道是否正确

    FTM_CNT_REG(FTMN[ftmn])     = 0;             //计数器。只有低16位可用（写任何值到此寄存器，都会加载 CNTIN 的值）
}



