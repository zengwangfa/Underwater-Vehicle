/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2015,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_computer.h
 * @brief      山外多功能调试助手上位机驱动代码
 * @author     山外科技
 * @version    v5.2.2
 * @date       2015-03-24
 */
#ifndef _VCAN_COMPUTER_H_
#define _VCAN_COMPUTER_H_

/*
 * 包含头文件
 */
#include "common.h"
#include "MK60_uart.h"


//上位机的下载地址：
//【山外多功能调试助手V1.1.1】串口、摄像头、GSM... - 智能车资料区 - http://vcan123.com/forum.php?mod=viewthread&tid=5988&ctid=27

extern void vcan_sendimg(void *imgaddr, uint32_t imgsize);
extern void vcan_hsw(void *imgaddr, uint32_t imgsize);
void mat_sendimg(void *imgaddr, uint32_t imgsize);
void vcan_sendware(void *wareaddr, uint32_t waresize);//发送中值

#endif  //_VCAN_COMPUTER_H_