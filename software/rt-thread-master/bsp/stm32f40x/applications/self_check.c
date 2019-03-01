#define LOG_TAG    "self_check"

#include "init.h"


/*----------------------- Variable Declarations -----------------------------*/

struct rt_event init_event;/* ALL_init 事件控制块. */

rt_thread_t init_tid;

/*----------------------- Function Implement --------------------------------*/


void check(void* parameter)
{
	  rt_uint32_t e;
		rt_err_t result;
	  RCC_ClocksTypeDef Get_RCC_Clocks;
		RCC_GetClocksFreq(&Get_RCC_Clocks); //获取系统时钟

		 /* 接收事件，判断是否所有外设初始化完成 ，接收完后清除事件标志 */
    if (rt_event_recv(&init_event, (LED_EVENT | KEY_EVENT | BUZZ_EVENT | OLED_EVENT | GYRO_EVENT | ADC_EVENT | PWM_EVENT ),
                      RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                      RT_WAITING_FOREVER, &e) == RT_EOK)
    {
				log_w("System Self-Check:0x%x --> Success!", e);
				log_w("Clock: %d Hz.",Get_RCC_Clocks.SYSCLK_Frequency); //打印系统时钟
				result = rt_thread_suspend (init_tid);  //线程挂起
			  if (result != RT_EOK){
						log_e("init_tid thread suspend failed.");
				}
		}

}


int All_thread_init(void)
{

		/*创建动态线程*/
    init_tid = rt_thread_create("self_check",			 //线程名称
                    check,									 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    1024,										 //线程栈大小，单位是字节【byte】
                    8,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (init_tid != RT_NULL){
				log_i("Init_Init()");
				rt_thread_startup(init_tid);
		}
		return 0;
}
INIT_APP_EXPORT(All_thread_init);









