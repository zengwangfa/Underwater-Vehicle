#define LOG_TAG    "light"

#include "light.h"
#include <rtthread.h>
#include <elog.h>
#include "sys.h"
#include <stdlib.h>
#include "drv_pwm.h"

#include <drivers/pin.h>

short light_value = 0;

/*******************************************
* 函 数 名：Light_Output_Limit
* 功    能：灯光亮度输出限制
* 输入参数：灯光亮度值 0~100%
* 返 回 值：None
* 注    意：
********************************************/
int Light_Output_Limit(short *value)
{
		*value = *value >  100 ? 100 : *value ;//限幅
		*value = *value <= 0   ? 0   : *value ;//限幅
		return *value;
}


/*【云台】舵机 修改 【当前】 MSH方法 */
static int light(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: YunTai_medvalue_set 2000");
				result = -RT_ERROR;
        goto _exit;
    }

		if(atoi(argv[1]) <= 100 ){		
				light_value = atoi(argv[1]) ;
				log_i("Light Value:  %d",light_value);
		}
		else {
				log_e("Error! The value is out of range!");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(light,ag: light <0~100>);



/**
  * @brief  light_thread_entry(舵机初始化任务函数)
  * @param  void* parameter
  * @retval None
  * @notice 
  */
void explore_light_thread_entry(void *parameter)//高电平1.5ms 总周期20ms  占空比7.5% volatil
{
//		TIM_Cmd(TIM10, ENABLE); //使能TIM10
//		TIM_Cmd(TIM11, ENABLE); //使能TIM11
		while(1)
		{

				light_value = Light_Output_Limit(&light_value);
				TIM10_PWM_CH1_F6(light_value);
				TIM11_PWM_CH1_F7(light_value);
			
				rt_thread_mdelay(10);
		}
	
}


int light_thread_init(void)
{
    rt_thread_t servo_tid;
		/*创建动态线程*/
    servo_tid = rt_thread_create("light",		 //线程名称
                    explore_light_thread_entry,			 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    1024,										 //线程栈大小，单位是字节【byte】
                    15,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (servo_tid != RT_NULL){
			
				//TIM10_PWM_Init(100-1,168-1);	//168M/168=1Mhz的计数频率,重装载值(即PWM精度)20000，所以PWM频率为 1M/20000=50Hz.  【现在为500Hz】
				//TIM11_PWM_Init(100-1,168-1);	//168M/168=1Mhz的计数频率,重装载值(即PWM精度)20000，所以PWM频率为 1M/20000=50Hz.  

				//log_i("light_init()");
			
				rt_thread_startup(servo_tid);
		}

		return 0;
}
INIT_APP_EXPORT(light_thread_init);
