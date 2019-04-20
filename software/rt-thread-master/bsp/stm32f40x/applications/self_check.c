/*
 * self_check.c
 *
 *  Created on: 2019年2月30日
 *      Author: zengwangfa
 *      Notes:  系统自检
 */
#define LOG_TAG    "self_check"

#include "init.h"
#include "self_check.h"
#include <rtthread.h>
#include <elog.h>
/*----------------------- Variable Declarations -----------------------------*/

struct rt_event init_event;/* ALL_init 事件控制块. */

rt_thread_t self_check_tid;

/*----------------------- Function Implement --------------------------------*/

/**
  * @brief  self_check_entry(系统自检函数)
  * @param  void* parameter
  * @retval None
  * @notice 当所有设备发送初始化完成事件后，判断为系统自检完成
  */
void self_check_entry(void* parameter)
{
	  rt_uint32_t e;
	
	  RCC_ClocksTypeDef Get_RCC_Clocks;
		RCC_GetClocksFreq(&Get_RCC_Clocks); //获取系统时钟

		 /* 接收事件，判断是否所有外设初始化完成 ，接收完后清除事件标志 */
    if (rt_event_recv(&init_event, (LED_EVENT | KEY_EVENT | BUZZ_EVENT | OLED_EVENT | GYRO_EVENT 
																	  | ADC_EVENT | PWM_EVENT | /*CAM_EVENT |*/ MS5837_EVENT  ),
                      RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                      RT_WAITING_FOREVER, &e) == RT_EOK)
    {
				log_w("System Self-Check:0x%x --> Success!", e);
				log_v("Clock: %d Hz.",Get_RCC_Clocks.SYSCLK_Frequency); //打印系统时钟
		}
		else {
				log_e("some devices initialization failed.");
		}
}


int Self_Check_thread_init(void)
{
	
		/*创建动态线程*/
    self_check_tid = rt_thread_create("self_check",			 //线程名称
                    self_check_entry,				 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    512,										 //线程栈大小，单位是字节【byte】
                    8,										 	 //线程优先级【priority】
                    1);										 //线程的时间片大小【tick】= 100ms

    if (self_check_tid != RT_NULL){
				log_i("SelfCheck_Init()");
				//rt_thread_startup(self_check_tid);
		}
		return 0;
}
INIT_APP_EXPORT(Self_Check_thread_init);









