/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       system_MK60DZ10.h
 * @brief      系统启动相关函数
 * @author     山外科技
 * @version    v5.0
 * @date       2013-07-02
 */

#ifndef __SYSTEM_MK60_H__
#define __SYSTEM_MK60_H__

extern int core_clk_khz;
extern int core_clk_mhz;
extern int bus_clk_khz;


void start(void);           //kinetis 启动函数
void sysinit (void);        //系统初始化，设置系统频率，初始化printf端口
void trace_clk_init(void);  //trace 时钟输出 初始化
void fb_clk_init(void);     //fb 时钟输出 初始化

void common_startup(void);  //复制中断向量表 和 必要的数据到 RAM里

#endif  //__SYSTEM_MK60_H__
