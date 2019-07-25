#ifndef __DRV_OLED_H
#define __DRV_OLED_H			  	 

#include "DataType.h"
#include "oledfont.h" 	 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//OLED 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	  
 

//OLED模式设置
//0: 4线串行模式  （模块的BS1，BS2均接GND）
//1: 并行8080模式 （模块的BS1，BS2均接VCC）
#define OLED_MODE 	0 
		    						  

#define OLED_CMD  	0		//写命令
#define OLED_DATA 	1		//写数据
//OLED控制用函数
void OLED_WR_Byte(uint8 dat,uint8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowMyChar(uint8 x,uint8 y,uint8 chr,uint8 size,uint8 mode);

void Half_OLED_Clear(void) ;
void OLED_DrawPoint(uint8 x,uint8 y,uint8 t);
void OLED_Fill(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 dot);
void OLED_ShowChar(uint8 x,uint8 y,uint8 chr,uint8 size,uint8 mode);
void OLED_ShowNum(uint8 x,uint8 y,uint32 num,uint8 len,uint8 size);
void OLED_ShowString(uint8 x,uint8 y,const uint8 *p,uint8 size);	
void OLED_ShowPicture(uint8 x,uint8 y,const uint8 *p,uint8 p_w,uint8 p_h);

void OLED_DrawBMP(uint8 x,uint8 y,uint8 chr,uint8 size,uint8 mode);


#endif  
	 



