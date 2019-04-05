/*
 * key.c
 *
 *  Created on: 2019年2月30日
 *      Author: zengwangfa
 *      Notes:  按键、拨码任务
 */
#define LOG_TAG    "key"

#include "key.h"
#include "buzzer.h"
#include "oled.h"
#include <drivers/pin.h>
#include <elog.h>
/*---------------------- Constant / Macro Definitions -----------------------*/

#define KEY_PIN  		79 	 //PD10

#define BOMA1_PIN	  14	 //PF4
#define BOMA2_PIN 	15	 //PF5

#define boma1_read 					rt_pin_read(BOMA1_PIN)
#define boma2_read 					rt_pin_read(BOMA2_PIN)

/*----------------------- Variable Declarations -----------------------------*/
/* ALL_init 事件控制块. */
extern struct rt_event init_event;
extern OledType oled;

uint8 boma_value = 0;	//暂存拨码状态 判断拨码状态是否改变

/*----------------------- Function Implement --------------------------------*/

void key_thread_entry(void* parameter)// --- KEY   BOMA ---
{
		if(System_NORMAL_STATUS == boma_value_get())
				buzzer_bibi(3,1);
    while (1)
    {
				if(boma_value != boma_value_get()){
						buzzer_bibi(1,1);	
						boma_value = boma_value_get();	
						log_i("\nCurrent Change: BOMA_Value = %d", boma_value);
				}
				rt_thread_mdelay(10);
    }
}


/* get 2位拨码值 */
uint8 boma_value_get(void)
{
    uint8 val; //reserve(存储)
    
		val = boma1_read *2 + boma2_read *1 + 1; //得到四种状态
    return val;
}

/* 按键按下产生的任务 */
void key_down(void *args)  
{
				oled.pagenum ++;				
}


int key_thread_init(void)
{
   rt_thread_t key_tid;

    key_tid = rt_thread_create("key",
                    key_thread_entry,
                    RT_NULL,
                    512,
                    25,
                    10);

    if (key_tid != RT_NULL){			
				rt_pin_mode(KEY_PIN, PIN_MODE_INPUT_PULLUP);    //功能按键  上拉输入
				rt_pin_mode(BOMA1_PIN, PIN_MODE_INPUT_PULLUP);  //拨码开关  上拉输入
				rt_pin_mode(BOMA2_PIN, PIN_MODE_INPUT_PULLUP);  
				
				rt_pin_mode(KEY_PIN, PIN_MODE_INPUT_PULLUP);	/* 按键引脚为输入模式 */
				rt_pin_attach_irq(KEY_PIN, PIN_IRQ_MODE_FALLING, key_down, RT_NULL);/* 绑定中断，上升沿模式，回调函数名为beep_on */
				rt_pin_irq_enable(KEY_PIN, PIN_IRQ_ENABLE);/* 使能中断 */
			
				log_i("KEY_Init()");
				boma_value = boma_value_get();	//初始化得到当前拨码状态 --> VehicleStatus
				//log_i("Current: BOMA_Value = %d", boma_value);
	
				rt_thread_startup(key_tid);
				rt_event_send(&init_event, KEY_EVENT);
		}
		return 0;
}
INIT_APP_EXPORT(key_thread_init);




