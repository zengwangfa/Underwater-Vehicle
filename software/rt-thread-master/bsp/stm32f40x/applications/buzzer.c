#include "buzzer.h"

/*---------------------- Constant / Macro Definitions -----------------------*/

#define Buzzer_PIN 	59   //PE8

/*----------------------- Variable Declarations -----------------------------*/

struct 
{
	rt_uint8_t time;
	rt_uint8_t count;
	rt_uint8_t number;
	
}buzzer;

/*----------------------- Function Implement --------------------------------*/

void buzzer_thread_entry(void *parameter)
{

		buzzer_bibi(3,1);
    while (1)
    {
				buzzer_ring_task();
				rt_thread_mdelay(30);
    }
}


int buzzer_thread_init(void)
{
    rt_thread_t buzzer_tid;
		/*创建动态线程*/
    buzzer_tid = rt_thread_create("buzzer",	 //线程名称
                    buzzer_thread_entry,		 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    512,										 //线程栈大小，单位是字节【byte】
                    10,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (buzzer_tid != RT_NULL){
				rt_pin_mode (Buzzer_PIN, PIN_MODE_OUTPUT);  //输出模式
				rt_pin_write(Buzzer_PIN, PIN_LOW);
				LOG_I("Buzzer_Init()");
				rt_thread_startup(buzzer_tid);
		}
		return 0;
}
INIT_APP_EXPORT(buzzer_thread_init);


void buzzer_once()   
{
	buzzer.count=1;
	buzzer.time=2;
	buzzer.number=2;
}  

/* count为响的次数  length响的时间长度  */
void buzzer_bibi(rt_uint8_t count,rt_uint8_t length)
{
	buzzer.count  = count *2;	
	buzzer.time   = length*5;
	buzzer.number = length*5;

}  

/* 蜂鸣器鸣响任务【可指示系统各个状态】 */
void buzzer_ring_task(void)
{
		
	if(buzzer.count >= 1)
	{  
			if(buzzer.number >= 1){
						if(buzzer.count%2 == 1)   {rt_pin_write(Buzzer_PIN, PIN_HIGH); }
						else                    	{rt_pin_write(Buzzer_PIN, PIN_LOW);} 
						buzzer.number--;	
			}
			else{
						buzzer.number = buzzer.time;
						buzzer.count--;
			}
	}
	else
	{
			rt_pin_write(Buzzer_PIN, PIN_LOW); //【响一声】
			buzzer.time  = 0;
			buzzer.count = 0;
	}
}
