#include "pwm.h"

/*---------------------- Constant / Macro Definitions -----------------------*/

#define TIM1_CH1_PIN 		60 //	E9
#define TIM1_CH2_PIN 		64 //	E11
#define TIM1_CH3_PIN 		66 //	E13
#define TIM1_CH4_PIN 		67 //	E14

#define PWM_DEV_NAME        "pwm"  /* PWM设备名称 */
#define PWM_DEV_CHANNEL     1       /* PWM通道 */


/*----------------------- Variable Declarations -----------------------------*/

struct rt_device_pwm *pwm_dev;      /* PWM设备句柄 */
/*----------------------- Function Implement --------------------------------*/

void pwm_thread_entry(void *parameter)
{
		rt_uint32_t period = 50000, pulse = 0;
	
    rt_pin_mode(TIM1_CH1_PIN, PIN_MODE_OUTPUT);
	  rt_pin_mode(TIM1_CH2_PIN, PIN_MODE_OUTPUT);
	  rt_pin_mode(TIM1_CH3_PIN, PIN_MODE_OUTPUT);
	  rt_pin_mode(TIM1_CH4_PIN, PIN_MODE_OUTPUT);
	
		/* 查找设备 */
    pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
	  if (pwm_dev == RT_NULL)
    {
        rt_kprintf("pwm sample run failed! can't find %s device!\n", PWM_DEV_NAME);
    }

		/* 设置PWM周期和脉冲宽度默认值 */
    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, 	 period, pulse);
		rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL+1, period, pulse);
		rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL+2, period, pulse);
		rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL+3, period, pulse);
	
    /* 使能设备 */
    rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL	);
		rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL+1);
		rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL+2);
		rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL+3);
    while (1)
    {	
				rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, 20000);
				rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, 40000);
				rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, 60000);
				rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, 80000);
			
				rt_thread_mdelay(500);
    }
}



int pwm_thread_init(void)
{
    rt_thread_t pwm_tid;
		/*创建动态线程*/
    pwm_tid = rt_thread_create("pwm",//线程名称
                    pwm_thread_entry,				 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    512,										 //线程栈大小，单位是字节【byte】
                    10,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (pwm_tid != RT_NULL)
     rt_thread_startup(pwm_tid);
		return 0;
}
INIT_APP_EXPORT(pwm_thread_init);





