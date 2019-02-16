#include "init.h"
#include <board.h>

/*----------------------- Variable Declarations -----------------------------*/
/* ALL_init 事件控制块 */
struct rt_event init_event;

rt_thread_t 	init_tid;

void init_thread_entry(void* parameter)
{
	  rt_uint32_t e;
		rt_err_t result;
		 /* 接收事件，判断是否所有外设初始化完成 ，接收完后清除事件标志 */
    if (rt_event_recv(&init_event, (LED_EVENT | KEY_EVENT | BUZZ_EVENT | OLED_EVENT | GYRO_EVENT | ADC_EVENT | PWM_EVENT),
                      RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                      RT_WAITING_FOREVER, &e) == RT_EOK)
    {
				LOG_W("System Self-Check Completed :0x%x Success!!!\n ", e);
				//rt_thread_delete(init_tid);
				result = rt_thread_suspend (init_tid);
			  if (result != RT_EOK){
						LOG_E("init_tid thread suspend failed.\n");
				}

		}
    
		rt_thread_mdelay(1000);

}


int All_thread_init(void)
{

		/*创建动态线程*/
    init_tid = rt_thread_create("init",			 //线程名称
                    init_thread_entry,			 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    512,										 //线程栈大小，单位是字节【byte】
                    10,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (init_tid != RT_NULL){
				LOG_I("Init_Init()");
				rt_thread_startup(init_tid);
		}
		return 0;
}
INIT_APP_EXPORT(All_thread_init);





int test(void)
{
    rt_uint8_t i;
    for(i = 1 ; i <= 3 ;i++){       
        rt_kprintf("Test, count : %d\r\n", i);
        rt_thread_mdelay(500);
    }
		rt_kprintf("Test-->successed!\n");
    return 0;
}
MSH_CMD_EXPORT(test, This is a test example.);


void rt_hw_us_delay(rt_uint32_t us)
{
    rt_uint32_t delta;
    /* 获得延时经过的 tick 数 */
    us = us * (SysTick->LOAD/(1000000/RT_TICK_PER_SECOND));
    /* 获得当前时间 */
    delta = SysTick->VAL;
    /* 循环获得当前时间，直到达到指定的时间后退出循环 */
    while (delta - SysTick->VAL< us);
}


void delay_us(u32 nTimer)
{
	u32 i=0;
	for(i=0;i<nTimer;i++){
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	}
}

void delay_ms(u32 nTimer)
{
		u32 i=1000*nTimer;
		delay_us(i);
}





