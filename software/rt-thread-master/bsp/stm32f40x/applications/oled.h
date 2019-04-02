#ifndef __OLED_H
#define __OLED_H

#include <drivers/pin.h>
#include "init.h"




void Boot_Animation(void);//开机动画
void menu_define(void); //菜单定义
void OLED_StatusPage(void);
void OLED_PicturePage(void);
void OLED_GyroscopePage(void);
void draw_circle(u8 x,u8 y,u8 r); //圆心(x,y),半径r
void draw_line(u8 x0,u8 y0,float k,u8 dot); //过固定点(x0,y0),斜率k  dot:0,清空;1,填充	  

void draw_fill_circle(u8 x0,u8 y0,u8 r,u8 dot);//写画实心圆心(x0,y0),半径r



typedef struct {
		u8 pagenum;  				//页码
		u8 pagechange;
	  u8 pagechange_flag;
		char *pagename[20]; //页名
}OledType;


typedef enum
{
		StatusPage = 1, //页码从第一页开始
		GyroscopePage,
		FlashPage,   
		PicturePage,
	
		OLED_Page_MAX   //页码最大值
	
}
MENU_LIST_e; //菜单枚举

extern char *VehicleModeName[2];


#endif


