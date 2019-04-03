#ifndef __LED_H
#define __LED_H

#include "DataType.h"

#define LEDR PEout(15)
#define LEDG PBout(10)
#define LEDB PBout(11)

//RGB灯共阳极接到+3.3V，电平 0亮 1灭
#define LED_ON(led_pin) 						rt_pin_write(led_pin ,PIN_LOW )
#define LED_OFF(led_pin) 						rt_pin_write(led_pin ,PIN_HIGH)
#define LED_Turn(led_pin,status) 		rt_pin_write(led_pin ,status =! status) //取反





typedef struct
{
  uint32 Bling_Contiune_Time;//闪烁持续时间
  uint32 Bling_Period;//闪烁周期
  float  Bling_Percent;//闪烁占空比
  uint32  Bling_Cnt;//闪烁计数器
  uint32 Port; //端口
  uint32 Pin;//引脚
  uint8 Endless_Flag;//无尽模式
}Bling_Light;

extern Bling_Light Light_1,Light_2,Light_3;



void led_blink_task(void);
void system_led_blink(uint8 InputData);

void Bling_Set(Bling_Light *Light,
               uint32 Continue_time,//持续时间
               uint32 Period,//周期100ms~1000ms
               float Percent,//0~100%
               uint32  Cnt,
               uint32 Port,
               uint32 Pin,
							 uint8 Flag);
							 
							 
							 
void Bling_Process(Bling_Light *Light);
void Bling_Working(uint8 bling_mode);
							 
							 
void Bling_Init(void);
void Quad_Start_Bling(void);
extern Bling_Light Light_1,Light_2,Light_3,Light_4;
extern uint8 Bling_Mode;
							 
							 
#endif


