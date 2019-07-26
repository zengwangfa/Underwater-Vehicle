#include "EasyThread.h"
#include <rtthread.h>
#include <math.h>
#include "elog.h"

/**** 设备调用 ****/
#include "ioDevices.h"
#include "oled.h"
#include "led.h"
#include "flash.h"
#include "drv_ano.h"

/*******************************************
* 函 数 名：flash_thread_entry
* 功    能：IO设备线程任务
* 输入参数：none
* 返 回 值：none
* 注    意：none
********************************************/
void flash_thread_entry(void* parameter)// --- PID Flash写入 ---
{

    while (1)
    {
				/* FLASH保存 或者 复位PID参数 */
				Save_Or_Reset_PID_Parameter();  
				rt_thread_mdelay(10);
    }
}

/*******************************************
* 函 数 名：ioDevices_thread_entry
* 功    能：IO设备线程任务
* 输入参数：none
* 返 回 值：none
* 注    意：none
********************************************/
void ioDevices_thread_entry(void* parameter)// --- 按键、拨码 ---
{
		uint8 boma_value = get_boma_value();	//初始化得到当前拨码状态;	//暂存拨码状态 判断拨码状态是否改变
	
		if(VehicleMode == FOUR_AXIS || VehicleMode == SIX_AXIS){
				Buzzer_Set(&Beep,1,1);
		}
		
    while (1)

    {
				Buzzer_Process(&Beep); //蜂鸣器控制任务
				if(boma_value != get_boma_value()){ //若拨码开关 波动，蜂鸣器响一声
						Buzzer_Set(&Beep,1,1);	
						boma_value = get_boma_value();	
						log_i("\nCurrent Change: BOMA_Value = %d", boma_value);
				}
				rt_thread_mdelay(10);
    }
}


/*******************************************
* 函 数 名：oled_thread_entry
* 功    能：OLED屏幕线程任务
* 输入参数：none
* 返 回 值：none
* 注    意：菜单号越大 刷新速率越大
********************************************/
void oled_thread_entry(void* parameter)
{
		Boot_Animation();	//开机动画
		OLED_Clear();
		while(1){	
				menu_define();//菜单定义选择
				rt_thread_mdelay(1000/pow(MENU+2,3)); //菜单号越大 刷新速率越大
		}
}



/*******************************************
* 函 数 名：led_thread_entry
* 功    能：LED线程任务
* 输入参数：none
* 返 回 值：none
* 注    意：
********************************************/
void led_thread_entry(void *parameter)
{	

		system_init_led_blink();			
		rt_thread_mdelay(1000);		
    while (1)
    {			

				Bling_Working(0);
				led_voltage_task();
				rt_thread_mdelay(10); //10ms
    }
}
