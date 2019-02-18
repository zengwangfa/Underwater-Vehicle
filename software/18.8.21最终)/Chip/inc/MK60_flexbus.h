/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_flexbus.h
 * @brief      flexbus函数库
 * @author     山外科技
 * @version    v5.0
 * @date       2013-09-02
 */

//下面两个参数都与 flexbus_8080_init 函数相关，修改后可能需要修改 flexbus_8080_init 函数的内容
#define FB_BA       0x6000      //基地址     = FB_BA * 0x10000 ，用于区分 不同的片选信号
                                //参考 System memory map 的说明， External Memory 的范围为 0x6000_0000 ~ 0x9FFF_FFFF
                                //即 FB_BA 的取值范围为 0x6000 ~ 0x9FFF

#define FB_BAM      0x0800      //基地址掩膜 = (FB_BA + FB_BAM)* 0x10000 ，用于区分 同一个CS控制的两个不连续的块的地址

#define FB_8080_CMD     (*(volatile uint16 *)(FB_BA * 0x10000))
#define FB_8080_DATA    (*(volatile uint16 *)((FB_BA + FB_BAM )* 0x10000))



extern void flexbus_8080_init();


