#include "light.h"
#include <rtthread.h>
#include <elog.h>
#include "sys.h"
#include <stdlib.h>
#include "drv_pwm.h"

#include <drivers/pin.h>
int light_value = 0;

/*******************************************
* 函 数 名：Servo_Output_Limit
* 功    能：舵机输出限制
* 输入参数：输入值：舵机结构体地址 
* 返 回 值：None
* 注    意：
********************************************/
void Light_Output_Limit(int vlaue)
{
		vlaue = vlaue  > 100 ? 100 : vlaue ;//限幅
		vlaue = vlaue  < 0   ? 0   : vlaue ;//限幅
	
}


/*【云台】舵机 修改 【当前】 MSH方法 */
static int light(int argc, char **argv)
{
    int result = 0;
    if (argc > 1){
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

//		TIM10_PWM_Init(99,7);	//168M/168=1Mhz的计数频率,重装载值(即PWM精度)20000，所以PWM频率为 1M/20000=50Hz.  【现在为500Hz】
//		TIM11_PWM_Init(99,7);	//84M/84=1Mhz的计数频率,重装载值(即PWM精度)20000，所以PWM频率为 1M/20000=50Hz.  
//		TIM_Cmd(TIM10, ENABLE);  //使能TIM1
//		TIM_Cmd(TIM11, ENABLE);  //使能TIM4
		rt_pin_mode (18, PIN_MODE_OUTPUT);  //输出模式
		rt_pin_mode (19, PIN_MODE_OUTPUT);  //输出模式
		rt_pin_write(18, PIN_LOW);
		rt_pin_write(19, PIN_LOW);
		rt_thread_mdelay(1000);
		while(1)
		{
//				TIM_SetCompare1(TIM10,light_value);
//				TIM_SetCompare1(TIM11,light_value);
			
//				rt_pin_write(18, PIN_LOW);
//				rt_pin_write(19, PIN_LOW);
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

				log_i("light_init()");
			
				rt_thread_startup(servo_tid);
		}

		return 0;
}
INIT_APP_EXPORT(light_thread_init);
