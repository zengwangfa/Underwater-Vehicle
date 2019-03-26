#ifndef __LED_H
#define __LED_H

#include "init.h"

#define LEDR PEout(15)
#define LEDG PBout(10)
#define LEDB PBout(11)

//RGB灯共阳极接到+3.3V，电平 0亮 1灭
#define LED_ON(led_pin) 						rt_pin_write(led_pin ,PIN_LOW )
#define LED_OFF(led_pin) 						rt_pin_write(led_pin ,PIN_HIGH)
#define LED_Turn(led_pin,status) 		rt_pin_write(led_pin ,status =! status) //取反



void led_blink_task(void);
void system_led_blink(u8 InputData);


typedef struct
{
  u32 Bling_Contiune_Time;//闪烁持续时间
  u32 Bling_Period;//闪烁周期
  float  Bling_Percent;//闪烁占空比
  u32  Bling_Cnt;//闪烁计数器
  u32 Port; //端口
  u32 Pin;//引脚
  u8 Endless_Flag;//无尽模式
}Bling_Light;


void Bling_Set(Bling_Light *Light,
               u32 Continue_time,//持续时间
               u32 Period,//周期100ms~1000ms
               float Percent,//0~100%
               u32  Cnt,
               u32 Port,
               u32 Pin,
							 u8 Flag);
void Bling_Process(Bling_Light *Light);
void Bling_Working(u8 bling_mode);
							 
							 
void Bling_Init(void);
void Quad_Start_Bling(void);
extern Bling_Light Light_1,Light_2,Light_3,Light_4;
extern u8 Bling_Mode;
							 
							 
#endif


