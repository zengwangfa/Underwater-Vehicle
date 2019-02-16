#ifndef __OLED_H
#define __OLED_H

#include <drivers/pin.h>
#include "init.h"

void Boot_Animation(void);//开机动画
void menu_define(void); //菜单定义
void OLED_StatusPage(void);

void draw_circle(u8 x,u8 y,u8 r); //圆心(x,y),半径r
void draw_line(u8 x0,u8 y0,float k); //过固定点(x0,y0),斜率k








#endif


