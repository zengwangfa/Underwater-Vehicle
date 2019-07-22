/*
 * led.c
 *
 *  Created on: 2019年2月30日
 *      Author: zengwangfa
 *      Notes:  LED指示
 */
#define LOG_TAG    "led"

#include "led.h"
#include "flash.h"
#include "drv_ano.h"
#include "ioDevices.h"
#include <string.h>
#include <elog.h>
#include <drivers/pin.h>
#include <easyflash.h>
#include "sensor.h"

/*----------------------- Variable Declarations -----------------------------*/
/* ALL_init 事件控制块. */
extern struct rt_event init_event;
extern uint8 VehicleStatus;

Bling_Light Light_1,Light_2,Light_3;

uint8 Bling_Mode = 0;
/*----------------------- Function Implement --------------------------------*/
void led_thread_entry(void *parameter)
{	

	  uint8 i=0;/*颜色节拍表> 空   红   绿   蓝   青   粉   黄   白 */
		uint8 inputdata[8] = {0x00,0x04,0x02,0x01,0x03,0x05,0x06,0x07};
		
		while(i <= 7){
				system_led_blink(inputdata[i++]);}
		
		LED_OFF(LED_Red);			//初始化为高电平 【熄灭】
		LED_OFF(LED_Green);			
		LED_OFF(LED_Blue);
				
		rt_thread_mdelay(1000);		
    while (1)
    {			
				/* FLASH保存 或者 复位PID参数 */
				Save_Or_Reset_PID_Parameter();  
				Bling_Working(0);
				led_blink_task();
				rt_thread_mdelay(10); //10ms
			



    }
}

/* led闪烁任务【系统正常运行指示灯】 */
void led_blink_task(void)
{
		static rt_uint8_t status = 1;
	  static rt_uint8_t cnt = 0;
		cnt++;
		if(cnt >= 40 && Sensor.PowerSource.Voltage >= 9 ){
				cnt = 0;
				LED_Turn(LED_Green,status);	//初始化为高电平 【熄灭】
				LED_OFF(LED_Red);			
				LED_OFF(LED_Blue);	
		}
		else if(Sensor.PowerSource.Voltage < 9 && cnt >= 40) //当电压小于9V时，亮红灯
		{
				cnt = 0;
				LED_Turn(LED_Red,status);	//初始化为高电平 【熄灭】		
				LED_OFF(LED_Green);			
				LED_OFF(LED_Blue);			
		}
		
}
	

/* 系统初始化led闪烁状态【显示7种颜色】 -->[颜色节拍表> 空  红  绿  蓝  青  粉  黄  白] */
void system_led_blink(uint8 InputData)
{
    if(InputData & 0x04){	
						LED_ON(LED_Red); }
		else{ 	LED_OFF(LED_Red);}
		
	  if(InputData & 0x02){	
					LED_ON(LED_Green); }
		else{ LED_OFF(LED_Green);}
		
		if(InputData & 0x01){	
					 LED_ON(LED_Blue); }
		else{  LED_OFF(LED_Blue);}
		rt_thread_mdelay(300);//等待系统初始化  完毕，系统稳定后，在读取数据
}


int led_thread_init(void)
{
    rt_thread_t led_tid;
		/*创建动态线程*/
    led_tid = rt_thread_create("led",//线程名称
                    led_thread_entry,				 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    1024,										 //线程栈大小，单位是字节【byte】
                    15,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (led_tid != RT_NULL){
				rt_pin_mode(LED_Red, 	 PIN_MODE_OUTPUT);//设置输出模式	
				rt_pin_mode(LED_Green, PIN_MODE_OUTPUT);	
				rt_pin_mode(LED_Blue,  PIN_MODE_OUTPUT);
				rt_pin_mode(LED_Camera,PIN_MODE_OUTPUT); //OV2640 LED
			
				rt_pin_mode(Light_PIN, 	PIN_MODE_OUTPUT);	  //探照灯
				rt_pin_write(Light_PIN ,PIN_LOW);
				log_i("LED_Init()");
				//rt_event_send(&init_event, LED_EVENT);
				rt_thread_startup(led_tid);
		}
		return 0;
}
INIT_APP_EXPORT(led_thread_init);


void Light_Control(uint8 *action)//探照灯
{
		static uint8 Light_Mode_Count = 0;//探照灯模式计数
		// 探照灯连续开关会产生三种模式 【高亮】【普通】【快闪】
	
		switch(*action)
		{
				case 0x01:
						Light_Mode_Count ++;
						if(Light_Mode_Count <= 3){// 探照灯前三次开启
								rt_pin_write(Light_PIN ,PIN_LOW);//关闭继电器
								rt_thread_mdelay(500);//0.5s
								rt_pin_write(Light_PIN ,PIN_HIGH);//打开继电器
						}
						else {
								Light_Mode_Count = 0;//探照灯模式计数 清零
								rt_pin_write(Light_PIN ,PIN_LOW);// 探照灯第四次关闭
						}
						break;

				default:break;
		}
		*action = 0x00;//清除控制字
}




/***************************************************
函数名: void Bling_Set(Bling_Light *Light,
uint32_t Continue_time,//持续时间
uint16_t Period,//周期100ms~1000ms
float Percent,//0~100%
uint16_t  Cnt,
GPIO_TypeDef* Port,
uint16_t Pin
,uint8_t Flag)
说明:	状态指示灯设置函数
入口:	时间、周期、占空比、端口等
出口:	无
备注:	程序初始化后、始终运行
****************************************************/
void Bling_Set(Bling_Light *Light,
               uint32 Continue_time,//持续时间
               uint32 Period,//周期100ms~1000ms
               float Percent,//0~100%
               uint32  Cnt,
               uint32 Pin,
               uint8 Flag)
{
		Light->Bling_Contiune_Time=(Continue_time/5);//持续时间
		Light->Bling_Period=Period;//周期
		Light->Bling_Percent=Percent;//占空比
		Light->Pin=Pin;//引脚
		Light->Endless_Flag=Flag;//无尽模式
}

/***************************************************
函数名: void Bling_Process(Bling_Light *Light)//闪烁运行线程
说明:	状态指示灯实现
入口:	状态灯结构体     
出口:	无
备注:	程序初始化后、始终运行
****************************************************/
void Bling_Process(Bling_Light *Light)//闪烁运行线程 Cnt 
{
		if(Light->Bling_Contiune_Time>=1) { 
				Light->Bling_Contiune_Time--;
		}
		else {LED_ON(Light->Pin);}//亮
		if(Light->Bling_Contiune_Time != 0//总时间未清0
				||Light->Endless_Flag==1)//判断无尽模式是否开启
		{
				Light->Bling_Cnt++;
				if(5*Light->Bling_Cnt>=Light->Bling_Period){
						Light->Bling_Cnt=0;//计满清零
				}
				if(5*Light->Bling_Cnt <= Light->Bling_Period * Light->Bling_Percent){	
						LED_ON(Light->Pin);   //亮
				}
				else {LED_OFF(Light->Pin);}//灭
		}
		else {	
				LED_OFF(Light->Pin);		//高电平 【熄灭】		
		}
}



/***************************************************
函数名: Bling_Working(uint16 bling_mode)
说明:	状态指示灯状态机
入口:	当前模式
出口:	无
备注:	程序初始化后、始终运行
****************************************************/
void Bling_Working(uint8 bling_mode)
{
		if(0 == bling_mode)
		{
				Bling_Process(&Light_1);
				Bling_Process(&Light_2);
				Bling_Process(&Light_3);
		}
		else if(1 == bling_mode)//
		{
				Bling_Process(&Light_1);

		}
		else if(2 == bling_mode)//
		{
				Bling_Process(&Light_2);
		}
		else if(3 == bling_mode)//全灭
		{
				Bling_Process(&Light_3);
		}
		 
}



/* led on MSH方法 */
int led_on(int argc, char **argv)
{
    int result = 0;

    if (argc != 2){
        log_e("Error! Proper Usage: led_on r\n Species:r/g/b/c/l");
				result = -RT_ERROR;
				return result;
    }
		
		switch(*argv[1]){
				case 'r':LED_ON(LED_Red);break;
				case 'g':LED_ON(LED_Green);break;
				case 'b':LED_ON(LED_Blue);break;
				case 'c':LED_OFF(LED_Camera);break;
				case 'l':LED_OFF(Light_PIN);break;
				default:log_e("Error! Proper Usage: led_on R\n Species:r/g/b/c/l");break;
		}

		return result;
}
MSH_CMD_EXPORT(led_on,ag: led_on r  );


/* led off MSH方法 */
int led_off(int argc, char **argv)
{
    int result = 0;

    if (argc != 2){
        log_e("Error! Proper Usage: led_off r\n Species:r/g/b/c/l");
				result = -RT_ERROR;
        goto _exit;
    }
		
		switch(*argv[1]){
			
				case 'r':LED_OFF(LED_Red);break;
				case 'g':LED_OFF(LED_Green);break;
				case 'b':LED_OFF(LED_Blue);break;
				case 'c':LED_ON(LED_Camera);break;
				case 'l':LED_ON(Light_PIN);break;
				default:log_e("Error! Proper Usage: led_off r\n Species:r/g/b/c/l");break;
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(led_off,ag:led_off r);





void ErrorStatus_LED(void)
{
		rt_pin_write(LED_Red  ,PIN_LOW  );//红灯亮
		rt_pin_write(LED_Green,PIN_HIGH );//其余灯灭
		rt_pin_write(LED_Blue ,PIN_HIGH );
}

