/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_dac.h
 * @brief      DAC函数
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-28
 */

#ifndef     _MK60_DAC_H_
#define     _MK60_DAC_H_ 1u

typedef enum DACn       //DAC模块
{
    DAC0,
    DAC1
} DACn_e;

extern void dac_init(DACn_e);               //DAC一次转换初始化
extern void dac_out(DACn_e, uint16 val);    //DAC一次转换操作

#endif  //_MK60_DAC_H_

