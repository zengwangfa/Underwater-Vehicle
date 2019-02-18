#ifndef _DCMI_H
#define _DCMI_H
#include "sys.h"    									
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//DCMI 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/14
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

void My_DCMI_Init(void);
void DCMI_DMA_Init(u32 DMA_Memory0BaseAddr,u16 DMA_BufferSize,u32 DMA_MemoryDataSize,u32 DMA_MemoryInc);
void DCMI_Start(void);
void DCMI_Stop(void);

void DCMI_Set_Window(u16 sx,u16 sy,u16 width,u16 height);
void DCMI_CR_Set(u8 pclk,u8 hsync,u8 vsync);

#endif





















