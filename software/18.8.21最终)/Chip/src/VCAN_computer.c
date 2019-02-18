/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2015,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_computer.c
 * @brief      山外多功能调试助手上位机驱动代码
 * @author     山外科技
 * @version    v5.2.2
 * @date       2015-03-24
 */
  

/*
 * 包含头文件
 */
#include "common.h"
#include "MK60_uart.h"
#include "VCAN_computer.h"
#include "include.h"

extern uint8 sending_flag;

//extern uint8 DMA_Over_Flg;
//extern uint8 img_mix_data[CAMERA_H][CAMERA_W];
/*!
 *  @brief      山外多功能调试助手上位机，摄像头显示函数
 *  @param      imgaddr    图像起始地址
 *  @param      imgsize    图像占用空间的大小
 *  @since      v5.0
*  Sample usage:
             具体用法参考这帖子:
            【山外摄像头】鹰眼上位机例程和微焦效果 - 智能车资料区
             http://vcan123.com/forum.php?mod=viewthread&tid=6242&ctid=27
 */
#define VCAN_POR UART0
void vcan_sendimg(void *imgaddr, uint32_t imgsize)
{
#define CMD_IMG     1
    uint8_t cmdf[2] = {CMD_IMG, ~CMD_IMG};    //山外上位机 使用的命令
    uint8_t cmdr[2] = {~CMD_IMG, CMD_IMG};    //山外上位机 使用的命令
    uart_putbuff(VCAN_POR, cmdf, sizeof(cmdf));    //先发送命令
    uart_putbuff(VCAN_POR, (uint8_t *)imgaddr, imgsize); //再发送图
    uart_putbuff(VCAN_POR, cmdr, sizeof(cmdr));    //先发送命令
	sending_flag=0;
}
void mat_sendimg(void *imgaddr, uint32_t imgsize)
{
    uint8_t ff[1]={255};
    uart_putbuff(VCAN_POR, ff, sizeof(ff));
    uart_putbuff(VCAN_POR, (uint8_t *)imgaddr, imgsize); //再发送图像
}
extern void vcan_hsw(void *imgaddr, uint32_t imgsize){
     uart_putchar(VCAN_POR, 0xff);
     uart_putbuff(VCAN_POR, (uint8_t *)imgaddr, imgsize); //再发送图像
     
}
void vcan_sendware(void *wareaddr, uint32_t waresize)
{
#define CMD_WARE     3
    uint8_t   cmdf[2] = {CMD_WARE, ~CMD_WARE};    //串口调试 使用的前命令
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    //串口调试 使用的后命令

    uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));    //先发送前命令
    uart_putbuff(VCAN_PORT, (uint8_t *)wareaddr, waresize);    //发送数据
    uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));    //发送后命令

}


//void sendimage()
//{
//    if(DMA_Over_Flg==1)     //发送图像到上位机
//    {
//     DisableInterrupts;         //关闭中断
////      uart_putchar(UART4,0xff);
//      uint8 h,l;
//      for(h=0;h<CAMERA_H;h++)
//      {
//        for(l=0;l<CAMERA_W;l++)
//        {
//          if(img_mix_data[h][l] == 0xff)
//            img_mix_data[h][l] = 0xfe;
//          uart_putchar(UART4,img_mix_data[h][l]);
//        }
//      }
//      uart_putchar(UART4,0xff);
//      DMA_Over_Flg=0;
//      EnableInterrupts  ;  //开总中断
//    }
//}