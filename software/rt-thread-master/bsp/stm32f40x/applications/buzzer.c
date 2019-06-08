/*
 * buzzer.c
 *
 *  Created on: 2019年2月30日
 *      Author: zengwangfa
 *      Notes:  蜂鸣器指示
 */
#define LOG_TAG    "buzzer"


#include <rtthread.h>
#include <elog.h>
#include <drivers/pin.h>
/*---------------------- Constant / Macro Definitions -----------------------*/



/*----------------------- Variable Declarations -----------------------------*/

/* ALL_init 事件控制块 */
extern struct rt_event init_event;



/*----------------------- Function Implement --------------------------------*/

//void buzzer_thread_entry(void *parameter)
//{
//    while (1)
//    {
//	
//				rt_thread_mdelay(10);
//    }
//}


//int buzzer_thread_init(void)
//{
//    rt_thread_t buzzer_tid;
//		/*创建动态线程*/
//    buzzer_tid = rt_thread_create("buzzer",	 //线程名称
//                    buzzer_thread_entry,		 //线程入口函数【entry】
//                    RT_NULL,							   //线程入口函数参数【parameter】
//                    1024,										 //线程栈大小，单位是字节【byte】
//                    25,										 	 //线程优先级【priority】
//                    10);										 //线程的时间片大小【tick】= 100ms

//    if (buzzer_tid != RT_NULL){

//				rt_thread_startup(buzzer_tid);
//				//rt_event_send(&init_event, BUZZ_EVENT);
//		}
//		return 0;
//}
//INIT_APP_EXPORT(buzzer_thread_init);



