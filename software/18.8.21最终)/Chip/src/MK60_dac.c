/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_dac.c
 * @brief      DAC函数
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-28
 */

#include "common.h"
#include "MK60_dac.h"

DAC_MemMapPtr DACN[2] = {DAC0_BASE_PTR, DAC1_BASE_PTR}; //定义两个指针数组保存 DACN 的地址


/*!
 *  @brief      DAC初始化
 *  @param      DACn_e      DAC模块号
 *  @since      v5.0
 *  Sample usage:       dac_init (DAC1 );    //初始化 DAC1
 */
void dac_init(DACn_e dacn)
{
    /* 使能时钟 */
    SIM_SCGC2 |= (SIM_SCGC2_DAC0_MASK << dacn) ;    //使能DAC模块

    /*  配置DAC寄存器  */

    //配置DAC_C0寄存器
    DAC_C0_REG(DACN[dacn])  = ( 0
                                |  DAC_C0_DACTRGSEL_MASK                //选择软件触发
                                |  DAC_C0_DACRFS_MASK                   //选择参考VDD电压(3.3V)
                                |  DAC_C0_DACEN_MASK                    //使能DAC模块
                              );

    //配置DAC_C1寄存器
    DAC_C1_REG(DACN[dacn]) = ( 0
                             ) ;
    //配置DAC_C2寄存器
    DAC_C2_REG(DACN[dacn]) = ( 0
                               | DAC_C2_DACBFRP(0)             //设置缓冲区读指针指向0
                             );

    DAC_DATH_REG(DACN[dacn], 0) = 0;   //默认输出最低电压
    DAC_DATL_REG(DACN[dacn], 0) = 0;
}

/*!
 *  @brief      DAC输出
 *  @param      DACn_e      DAC模块号
 *  @param      val         输出模拟量所对应的数字量（12bit）
 *  @since      v5.0
 *  Sample usage:       dac_out (DAC1 ,0x100);    //初始化 DAC1 输出 0x100 数字量对应的模拟量
 */
void dac_out(DACn_e dacn, uint16 val)
{
    ASSERT(val < 0x1000);               //val 为 12bit

    DAC_DATH_REG(DACN[dacn], 0) = (val >> 8);   //输出电压
    DAC_DATL_REG(DACN[dacn], 0) = (val & 0xFF);
}
