#include "init.h"


/*----------------------- Variable Declarations -----------------------------*/
extern rt_uint8_t VehicleStatus;

/*---------------------- Constant / Macro Definitions -----------------------*/
//RGB灯引脚号
#define LED_Red 		68 
#define LED_Green 	69
#define LED_Blue 		70

//RGB灯共阳极接到+3.3V，电平 0亮 1灭
#define LED_ON(led_pin) 						rt_pin_write(led_pin ,PIN_LOW )
#define LED_OFF(led_pin) 						rt_pin_write(led_pin ,PIN_HIGH)
#define LED_Turn(led_pin,status) 		rt_pin_write(led_pin ,status = ! status) //取反


void led_thread_entry(void *parameter)
{
    rt_pin_mode( LED_Red, 	PIN_MODE_OUTPUT);//设置输出模式	
    rt_pin_mode( LED_Green, PIN_MODE_OUTPUT);	
    rt_pin_mode( LED_Blue, 	PIN_MODE_OUTPUT);	
	
		rt_pin_write(LED_Red, 	PIN_HIGH);			//初始化为高电平 【熄灭】
		rt_pin_write(LED_Green, PIN_HIGH);			
		rt_pin_write(LED_Blue,  PIN_HIGH);
		
		LED_ON(LED_Red);	//等待系统初始化 1s 完毕，使系统更稳定
		rt_thread_mdelay(1000);
		LED_OFF(LED_Red);

	
    while (1)
    {
				led_blink_task();
    }
}

/* led闪烁任务【系统正常运行指示灯】 */
void led_blink_task(void)
{
		static rt_uint8_t status = 1;
		if(boma_value_get() == 1)
		{
				LED_Turn(LED_Green,status);	//初始化为高电平 【熄灭】
				rt_thread_mdelay(500);
		}
}
	




int led_thread_init(void)
{
    rt_thread_t led_tid;
		/*创建动态线程*/
    led_tid = rt_thread_create("led",//线程名称
                    led_thread_entry,				 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    1024,										 //线程栈大小，单位是字节【byte】
                    5,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (led_tid != RT_NULL)
     rt_thread_startup(led_tid);
		return 0;
}
INIT_APP_EXPORT(led_thread_init);





