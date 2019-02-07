#include "my_init.h"  //IO Control
#include "sfud.h"
#include "string.h"
#include "spi_flash.h"
#include "spi_flash_sfud.h"

#define Flow_Count_PIN PBin(1)  //脉冲计数引脚输入

extern rt_uint8_t FX_NUMBER;
extern rt_uint8_t FW_status_flag;
extern rt_uint8_t PiShu;
extern char *FW_status_string[4]; //蜂王状态名 字符串
extern rt_uint8_t set1kg_flag;
extern rt_uint32_t Flow_Count;

extern sfud_flash *flash;

void key_thread_entry(void* parameter)// !!! KEY   BOMA  BEEP  !!!
{
		static rt_uint8_t boma1 = 0;
		static rt_uint8_t boma2 = 0;
		static rt_uint8_t boma3 = 0;
		static rt_uint8_t boma4 = 0;
	
		rt_pin_mode(BEEP_PIN, PIN_MODE_OUTPUT);  //输出模式
		rt_pin_write(BEEP_PIN, PIN_HIGH);
		rt_pin_mode(LED2, PIN_MODE_OUTPUT);  //输出模式
		rt_pin_mode(LED3, PIN_MODE_OUTPUT);
		rt_pin_mode(Feeding_PIN, PIN_MODE_OUTPUT);  //输出模式
		rt_pin_mode(Watering_PIN, PIN_MODE_OUTPUT); 
	
		rt_pin_mode(Heat1_PIN, PIN_MODE_OUTPUT);  //输出模式
		rt_pin_mode(Heat2_PIN, PIN_MODE_OUTPUT); 
	
		rt_pin_mode(BOMA1_PIN, PIN_MODE_INPUT_PULLUP);  //拨码开关  上拉输入
		rt_pin_mode(BOMA2_PIN, PIN_MODE_INPUT_PULLUP);  
		rt_pin_mode(BOMA3_PIN, PIN_MODE_INPUT_PULLUP);  
		rt_pin_mode(BOMA4_PIN, PIN_MODE_INPUT_PULLUP); 
	
		rt_pin_mode(KEY1, PIN_MODE_INPUT_PULLUP);  //上拉输入
		rt_pin_mode(KEY2, PIN_MODE_INPUT_PULLUP);  
		rt_pin_mode(KEY3, PIN_MODE_INPUT_PULLUP);  


    while (1)
    {
			
			  FX_NUMBER = BOMA1*8+BOMA2*4+BOMA3*2+BOMA4*1+1;  ///上电后固定 蜂箱号数
				if(boma1 != BOMA1 || boma2 != BOMA2 || boma3 != BOMA3 ||boma4 != BOMA4){
					buzzer_bibi(1,1);
					boma1 = BOMA1;boma2 = BOMA2;boma3 = BOMA3;boma4 = BOMA4;
				}
		

        if (rt_pin_read(KEY1) == PIN_LOW){
						FW_status_flag++;
						if(FW_status_flag>3)FW_status_flag=0;
					  rt_kprintf("Now--FW_Status:%d\t-->%s\r\n",FW_status_flag,FW_status_string[FW_status_flag][20]);

						sfud_erase(flash, 0, 1);//
						sfud_write(flash, 0, 1, &FW_status_flag);//W25Q128 FLASH写入【蜂王状态】
						sfud_write(flash, 1, 1, &PiShu);				 //W25Q128 FLASH写入【脾数】
						buzzer_bibi(1,1);


					
				}while(rt_pin_read(KEY1) == PIN_LOW);
				
				
        if (rt_pin_read(KEY2) == PIN_LOW){
						PiShu++;
						if(PiShu>7)PiShu=0;
						rt_kprintf("Now--Pi Shu:%d\r\n",PiShu);
						buzzer_bibi(1,1);
				  	sfud_erase(flash, 0, 1);
						sfud_write(flash, 0, 1, &FW_status_flag);//W25Q128 FLASH写入【蜂王状态】
						sfud_write(flash, 1, 1, &PiShu);				 //W25Q128 FLASH写入【脾数】

        }
				while(rt_pin_read(KEY2) == PIN_LOW);

				
        if (rt_pin_read(KEY3) == PIN_LOW){
					buzzer_bibi(1,1);	rt_kprintf("KEY3_DOWN!\t");
					set1kg_flag = 1;
        }while(rt_pin_read(KEY3) == PIN_LOW);

        rt_thread_delay(rt_tick_from_millisecond(10));
    }
}

int key_thread_init(void)
{
   rt_thread_t key_tid;

    key_tid = rt_thread_create("key",
                    key_thread_entry,
                    RT_NULL,
                    1024,
                    2,
                    10);

    if (key_tid != RT_NULL)
     rt_thread_startup(key_tid);
		return 0;
}
INIT_APP_EXPORT(key_thread_init);



void flow_callback(void *args)// 回调函数
{
    char *a = args;// 获取参数
    //rt_kprintf("Flow_Count! %s\n",a);
		Flow_Count++;  //脉冲计数值累加
}

void flow_count_thread(void* parameter)
{
    // 输入模式
		rt_pin_mode(27, PIN_MODE_INPUT);  //脉冲计数引脚  PB1   NO.27
    // 绑定中断，上降沿模式，回调函数名为 flow_callback
    rt_pin_attach_irq(27, PIN_IRQ_MODE_RISING, flow_callback, (void*)"callbackargs");
    // 使能中断
    rt_pin_irq_enable(27, PIN_IRQ_ENABLE);

}

int flow_count_init(void)
{
   rt_thread_t key_tid;

    key_tid = rt_thread_create("key",
                    flow_count_thread,
                    RT_NULL,
                    1024,
                    5,
                    10);

    if (key_tid != RT_NULL)
     rt_thread_startup(key_tid);
		return 0;
}
INIT_APP_EXPORT(flow_count_init);


