/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_mcg.c
 * @brief      MCG PLL驱动
 * @author     山外科技
 * @version    v5.0
 * @date       2013-06-29
 */


#include "common.h"
#include "MK60_mcg.h"
#include "MK60_lptmr.h"

#if defined(MK60DZ10)
mcg_cfg_t mcg_cfg[PLL_MAX] =
{
    {EXTAL_IN_MHz*(VDIV + 24) / (PRDIV + 1), PRDIV, VDIV}, //PLLUSR
    {48,    24,     0       },                          //PLL48
    {50,    24,     1       },                          //PLL50
    {96,    24,     24      },                          //PLL96
    {100,   24,     26      },                          //PLL100
    {110,   24,     31      },                          //PLL110
    {120,   19,     24      },                          //PLL120
    {125,   19,     26      },                          //PLL125
    {130,   19,     28      },                          //PLL130
    {140,   14,     18      },                          //PLL140
    {150,   14,     21      },                          //PLL150
    {160,   14,     24      },                          //PLL160
    {170,   14,     27      },                          //PLL170
    {180,   14,     30      },                          //PLL180
    {200,   12,     28      },                          //PLL200
    {225,   11,     30      },                          //PLL225
    {250,   10,     31      },                          //PLL250
};

#elif defined(MK60F15)

mcg_cfg_t mcg_cfg[PLL_MAX] =
{
    {EXTAL_IN_MHz*(VDIV + 16) / (PRDIV + 1) /2, PRDIV, VDIV}, //PLLUSR
    {50,    7 ,     1       },                          //PLL50
    {100,   7,      16      },                          //PLL100
    {110,   4 ,     6       },                          //PLL110
    {120,   4 ,     8       },                          //PLL120
    {130,   4 ,     10      },                          //PLL130
    {140,   4 ,     12      },                          //PLL140
    {150,   4 ,     14      },                          //PLL150
    {160,   4 ,     16      },                          //PLL160
    {170,   4 ,     18      },                          //PLL170
    {180,   4 ,     20      },                          //PLL180
    {190,   4 ,     22      },                          //PLL190
    {200,   4 ,     24      },                          //PLL200
    {210,   4 ,     26      },                          //PLL210
    {220,   4 ,     28      },                          //PLL220
    {225,   4 ,     29      },                          //PLL225
    {230,   4 ,     30      },                          //PLL230
    {235,   4 ,     31      },                          //PLL235
    {244,   3 ,     23      },                          //PLL244
    {250,   3 ,     24      },                          //PLL250
    {275,   1 ,     6       },                          //PLL275
    {300,   2 ,     20      },                          //PLL300
};
#endif
mcg_div_t mcg_div = {CORE_DIV, BUS_DIV, FLEX_DIV, FLASH_DIV};


/*!
 *  @brief      MCG 分频配置
 *  @param      PLL_e       频率设置参数
 *  @since      v5.0
 */
void mcg_div_count(PLL_e pll)
{
    uint16 clk = mcg_cfg[pll].clk;
    if( (pll > PLLUSR) && (pll < PLL_MAX) )
    {
        //设置分频
        mcg_div.core_div    = 0;                                        // core = MCG

        if     (clk <= 1 * MAX_BUS_CLK)   mcg_div.bus_div = 0;          // bus  = MCG
        else if(clk <= 2 * MAX_BUS_CLK)   mcg_div.bus_div = 1;          // bus  = MCG/2
        else if(clk <= 3 * MAX_BUS_CLK)   mcg_div.bus_div = 2;          // bus  = MCG/3
        else if(clk <= 4 * MAX_BUS_CLK)   mcg_div.bus_div = 3;          // bus  = MCG/4
        else if(clk <= 5 * MAX_BUS_CLK)   mcg_div.bus_div = 4;          // bus  = MCG/5
        else if(clk <= 6 * MAX_BUS_CLK)   mcg_div.bus_div = 5;          // bus  = MCG/6
        else if(clk <= 7 * MAX_BUS_CLK)   mcg_div.bus_div = 6;          // bus  = MCG/7
        else if(clk <= 8 * MAX_BUS_CLK)   mcg_div.bus_div = 7;          // bus  = MCG/8
        else if(clk <= 9 * MAX_BUS_CLK)   mcg_div.bus_div = 8;          // bus  = MCG/9
        else if(clk <= 10 * MAX_BUS_CLK)   mcg_div.bus_div = 9;         // bus  = MCG/10
        else if(clk <= 11 * MAX_BUS_CLK)   mcg_div.bus_div = 10;        // bus  = MCG/11
        else if(clk <= 12 * MAX_BUS_CLK)   mcg_div.bus_div = 11;        // bus  = MCG/12
        else if(clk <= 13 * MAX_BUS_CLK)   mcg_div.bus_div = 12;        // bus  = MCG/13
        else if(clk <= 14 * MAX_BUS_CLK)   mcg_div.bus_div = 13;        // bus  = MCG/14
        else if(clk <= 15 * MAX_BUS_CLK)   mcg_div.bus_div = 14;        // bus  = MCG/15
        else                              mcg_div.bus_div = 15;         // bus  = MCG/16

        if     (clk <= 1 * MAX_FLEXBUS_CLK)   mcg_div.flex_div = 0;     // flex bus  = MCG
        else if(clk <= 2 * MAX_FLEXBUS_CLK)   mcg_div.flex_div = 1;     // flex bus  = MCG/2
        else if(clk <= 3 * MAX_FLEXBUS_CLK)   mcg_div.flex_div = 2;     // flex bus  = MCG/3
        else if(clk <= 4 * MAX_FLEXBUS_CLK)   mcg_div.flex_div = 3;     // flex bus  = MCG/4
        else if(clk <= 5 * MAX_FLEXBUS_CLK)   mcg_div.flex_div = 4;     // flex bus  = MCG/5
        else if(clk <= 6 * MAX_FLEXBUS_CLK)   mcg_div.flex_div = 5;     // flex bus  = MCG/6
        else if(clk <= 7 * MAX_FLEXBUS_CLK)   mcg_div.flex_div = 6;     // flex bus  = MCG/7
        else if(clk <= 8 * MAX_FLEXBUS_CLK)   mcg_div.flex_div = 7;     // flex bus  = MCG/8
        else if(clk <= 9 * MAX_FLEXBUS_CLK)   mcg_div.flex_div = 8;     // flex bus  = MCG/9
        else if(clk <= 10 * MAX_FLEXBUS_CLK)   mcg_div.flex_div = 9;    // flex bus  = MCG/10
        else if(clk <= 11 * MAX_FLEXBUS_CLK)   mcg_div.flex_div = 10;   // flex bus  = MCG/11
        else if(clk <= 12 * MAX_FLEXBUS_CLK)   mcg_div.flex_div = 11;   // flex bus  = MCG/12
        else if(clk <= 13 * MAX_FLEXBUS_CLK)   mcg_div.flex_div = 12;   // flex bus  = MCG/13
        else if(clk <= 14 * MAX_FLEXBUS_CLK)   mcg_div.flex_div = 13;   // flex bus  = MCG/14
        else if(clk <= 15 * MAX_FLEXBUS_CLK)   mcg_div.flex_div = 14;   // flex bus  = MCG/15
        else                                  mcg_div.flex_div = 15;    // flex bus  = MCG/16

        if     (clk <= 1 * MAX_FLASH_CLK)   mcg_div.flash_div = 0;      // flash  = MCG
        else if(clk <= 2 * MAX_FLASH_CLK)   mcg_div.flash_div = 1;      // flash  = MCG/2
        else if(clk <= 3 * MAX_FLASH_CLK)   mcg_div.flash_div = 2;      // flash  = MCG/3
        else if(clk <= 4 * MAX_FLASH_CLK)   mcg_div.flash_div = 3;      // flash  = MCG/4
        else if(clk <= 5 * MAX_FLASH_CLK)   mcg_div.flash_div = 4;      // flash  = MCG/5
        else if(clk <= 6 * MAX_FLASH_CLK)   mcg_div.flash_div = 5;      // flash  = MCG/6
        else if(clk <= 7 * MAX_FLASH_CLK)   mcg_div.flash_div = 6;      // flash  = MCG/7
        else if(clk <= 8 * MAX_FLASH_CLK)   mcg_div.flash_div = 7;      // flash  = MCG/8
        else if(clk <= 9 * MAX_FLASH_CLK)   mcg_div.flash_div = 8;      // flash  = MCG/9
        else if(clk <= 10 * MAX_FLASH_CLK)   mcg_div.flash_div = 9;     // flash  = MCG/10
        else if(clk <= 11 * MAX_FLASH_CLK)   mcg_div.flash_div = 10;    // flash  = MCG/11
        else if(clk <= 12 * MAX_FLASH_CLK)   mcg_div.flash_div = 11;    // flash  = MCG/12
        else if(clk <= 13 * MAX_FLASH_CLK)   mcg_div.flash_div = 12;    // flash  = MCG/13
        else if(clk <= 14 * MAX_FLASH_CLK)   mcg_div.flash_div = 13;    // flash  = MCG/14
        else if(clk <= 15 * MAX_FLASH_CLK)   mcg_div.flash_div = 14;    // flash  = MCG/15
        else                                mcg_div.flash_div = 15;     // flash  = MCG/16
    }
}

/*!
 *  @brief      PLL超频
 *  @param      PLL_e       频率设置参数
 *  @return     超频频率（MHz）
 *  @since      v5.0
 *  @warning    此函数只能在 复位后没进行任何频率设置情况下调用，即MCG在FEI模式下才可调用
 *  Sample usage:       uint8 clk = pll_init(PLL100);        //超频
 */
uint8 pll_init(PLL_e pll)
{

    mcg_div_count( pll);

    //上电复位后，单片机会自动进入 FEI 模式，使用 内部参考时钟

    //FEI -> FBE
    MCG_C2 &= ~MCG_C2_LP_MASK;
    MCG_C2 |= MCG_C2_RANGE(1);

    MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(7);

    while (MCG_S & MCG_S_IREFST_MASK) {};                       //等待FLL参考时钟 为 外部参考时钟（S[IREFST]=0，表示使用外部参考时钟，）

    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2) {}; //等待选择外部参考时钟

    //现在已经进入了 FBE模式

    //FBE -> PBE
    set_sys_dividers(mcg_div.core_div, mcg_div.bus_div, mcg_div.flex_div, mcg_div.flash_div); //设置系统分频因子选项

    MCG_C5 = MCG_C5_PRDIV(mcg_cfg[pll].prdiv);                      //分频， EXTAL_IN_MHz/( PRDIV+1)
    MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(mcg_cfg[pll].vdiv) ;    //倍频， EXTAL_IN_MHz/( PRDIV+1)  * (VDIV+24)

    while (!(MCG_S & MCG_S_PLLST_MASK)) {};                         //等待时钟源选择PLL

    while (!(MCG_S & MCG_S_LOCK_MASK)) {};                          //等待 PLL锁了（锁相环）

    // 现在已经进入了 PBE 模式

    // PBE -> PEE
    MCG_C1 &= ~MCG_C1_CLKS_MASK;

    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3) {};//等待选择输出PLL

    // 现在已经进入了 PEE 模式

    return mcg_cfg[pll].clk;
} //pll_init


/*!
 *  @brief      时钟分频设置函数
 *  @param      outdiv1    内核分频系数，       core    clk = MCG / (outdiv1 +1)
 *  @param      outdiv2    bus分频系数，        bus     clk = MCG / (outdiv2 +1)
 *  @param      outdiv3    flexbus分频系数，    flexbus clk = MCG / (outdiv3 +1)
 *  @param      outdiv4    flash分频系数，      flash   clk = MCG / (outdiv4 +1)
 *  @since      v1.0
 *  @author     飞思卡尔公司
 *  Sample usage:       set_sys_dividers(0,1, 9,3);     // core clk = MCG ; bus clk = MCG / 2 ; flexbus clk = MCG /10 ; flash clk = MCG / 4;
 */
__RAMFUNC  void set_sys_dividers(uint32 outdiv1, uint32 outdiv2, uint32 outdiv3, uint32 outdiv4)
{
    /*
    * This routine must be placed in RAM. It is a workaround for errata e2448.
    * Flash prefetch must be disabled when the flash clock divider is changed.
    * This cannot be performed while executing out of flash.
    * There must be a short delay after the clock dividers are changed before prefetch
    * can be re-enabled.
    */
    uint32 temp_reg;
    uint8 i;

    temp_reg = FMC_PFAPR; // store present value of FMC_PFAPR

    // set M0PFD through M7PFD to 1 to disable prefetch
    FMC_PFAPR |= FMC_PFAPR_M7PFD_MASK | FMC_PFAPR_M6PFD_MASK | FMC_PFAPR_M5PFD_MASK
                 | FMC_PFAPR_M4PFD_MASK | FMC_PFAPR_M3PFD_MASK | FMC_PFAPR_M2PFD_MASK
                 | FMC_PFAPR_M1PFD_MASK | FMC_PFAPR_M0PFD_MASK;

    // set clock dividers to desired value
    SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(outdiv1) | SIM_CLKDIV1_OUTDIV2(outdiv2)
                  | SIM_CLKDIV1_OUTDIV3(outdiv3) | SIM_CLKDIV1_OUTDIV4(outdiv4);

    // wait for dividers to change
    for (i = 0 ; i < outdiv4 ; i++)
        {}

    FMC_PFAPR = temp_reg; // re-store original value of FMC_PFAPR

    return;
} // set_sys_dividers
