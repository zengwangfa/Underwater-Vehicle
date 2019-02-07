#include "init.h"  

/*---------------------- Constant / Macro Definitions -----------------------*/

#define Buzzer_PIN 59  //PE8
#define KEY_PIN  79 	 //PD10

#define BOMA1_PIN 14	 //PF4
#define BOMA2_PIN 15	 //PF5

#define boma1_read 					rt_pin_read(BOMA1_PIN)
#define boma2_read 					rt_pin_read(BOMA2_PIN)

/*----------------------- Variable Declarations -----------------------------*/

static rt_uint8_t boma_value = 0;	//暂存拨码状态 判断拨码状态是否改变


/*----------------------- Function Implement --------------------------------*/

void key_thread_entry(void* parameter)// --- Buzzer   KEY   BOMA ---
{
		rt_pin_mode(Buzzer_PIN, PIN_MODE_OUTPUT);  //输出模式
		rt_pin_write(Buzzer_PIN, PIN_LOW);

		rt_pin_mode(KEY_PIN, PIN_MODE_INPUT_PULLUP);    //功能按键  上拉输入
	
		rt_pin_mode(BOMA1_PIN, PIN_MODE_INPUT_PULLUP);  //拨码开关  上拉输入
		rt_pin_mode(BOMA2_PIN, PIN_MODE_INPUT_PULLUP);  

		boma_value = boma_value_get();	//初始化得到当前拨码状态 --> VehicleStatus
		rt_kprintf("Current: BOMA_Value = %d\n", boma_value);
    while (1)
    {
				key_down_task();

				if(boma_value != boma_value_get()){
						//buzzer_bibi(1,1);	
						boma_value = boma_value_get();	
						rt_kprintf("\nCurrent Change: BOMA_Value = %d", boma_value);
		
				}


				rt_thread_mdelay(50);
    }
}


/* get 2位拨码值 */
rt_uint8_t boma_value_get(void)
{
    rt_uint8_t val; //reserve(存储)
    
		val = boma1_read *2 + boma2_read *1 + 1; //得到四种状态
    return val;
}

/* 按键按下产生的任务 */
void key_down_task(void)  
{
		if (rt_pin_read(KEY_PIN) == PIN_LOW){
				//buzzer_bibi(1,1);	
				rt_kprintf("\nKEY DOWN!\n");
					
    }while(rt_pin_read(KEY_PIN) == PIN_LOW);

}











int key_thread_init(void)
{
   rt_thread_t key_tid;

    key_tid = rt_thread_create("key",
                    key_thread_entry,
                    RT_NULL,
                    1024,
                    10,
                    10);

    if (key_tid != RT_NULL)
     rt_thread_startup(key_tid);
		return 0;
}
INIT_APP_EXPORT(key_thread_init);




