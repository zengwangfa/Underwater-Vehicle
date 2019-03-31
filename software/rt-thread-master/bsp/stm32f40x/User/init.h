#ifndef __INIT_H_
#define __INIT_H_



#include "sys.h" 
#include "led.h"
#include "key.h"
#include "buzzer.h"
#include "drv_oled.h"
#include "oled.h"
#include "gyroscope.h"
#include "adc.h"
#include "pwm.h"



/*---------------------- Constant / Macro Definitions -----------------------*/		

/************重要定义****************/


typedef enum
{
	AUV_Mode,
	ROV_Mode
	
}VehicleMode_E;


/************重要定义***************/


#define LED_EVENT 			(1 << 0)  //LED事件标志位
#define KEY_EVENT 			(1 << 1)  //KEY事件标志位
#define BUZZ_EVENT 			(1 << 2)  //BUZZER事件标志位
#define OLED_EVENT 			(1 << 3)  //OLED事件标志位
#define GYRO_EVENT 			(1 << 4)  //Gyroscope事件标志位
#define ADC_EVENT 			(1 << 5)  //ADC事件标志位
#define PWM_EVENT 			(1 << 6)  //PWM事件标志位
#define CAM_EVENT 			(1 << 7)  //Camera事件标志位


#define PI 3.1415926f //float型
#define Rad2Deg(Rad) (Rad * 180.0f / PI)
#define Deg2Rad(Deg) (Deg * PI / 180.0f)


enum VehicleStatus{
	System_NORMAL_STATUS = 1,//正常模式
  System_DEBUG_STATUS = 2, //调试模式
	System_ERROR_STATUS,
};  //枚举系统状态




//void delay_us(unsigned int us);
//void delay_ms(unsigned int ms);

//void delay_us(u32 nTimer);
//void delay_ms(u32 nTimer);
void rt_hw_us_delay(u32 us);
void rt_hw_ms_delay(u32 ms);
void delay_us(u32 nTimer);

int sprintf(char *buf, const char *format, ...);

#endif



