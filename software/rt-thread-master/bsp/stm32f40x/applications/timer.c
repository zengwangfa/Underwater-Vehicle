/*
 * timer.c
 *
 *  Created on: 2019年2月30日
 *      Author: zengwangfa
 *      Notes:  定时器
 */
#define LOG_TAG    "timer"

#include <string.h>
#include "timer.h"
#include "flash.h"
#include <rtthread.h>
#include <elog.h>
#include "drv_MS5837.h"
#include "Control.h"
#include "gyroscope.h"
#include "debug.h"
#include "ret_data.h"
#include "PropellerControl.h"
/*---------------------- Constant / Macro Definitions -----------------------*/



/*----------------------- Variable Declarations -----------------------------*/




/*----------------------- Function Implement --------------------------------*/


/**
  * @brief  timer1_out(定时器中断函数)
  * @param  void* parameter
  * @retval None
  * @notice Attention! -> 定时器中不能存在延时或者释放线程的操作
  */
static void timer1_out(void* parameter)// 定时器1超时函数  进行JY901模块数据转换
{


		/* 调度器上锁，上锁后，将不再切换到其他线程，仅响应中断 */
		rt_enter_critical();
				

	
		get_speed(&Sensor.JY901.Acc.x,&Sensor.JY901.Speed.x);//得到x速度,数据有问题，累积效应过大
		get_zspeed(); 
		//Angle_Control(); //角度控制


		/* 调度器解锁 */
		rt_exit_critical();

	
}



int timer1_init(void)
{
		/* 定时器的控制块 */
		static rt_timer_t timer1;
    /* 创建定时器1 */
    timer1 = rt_timer_create("timer1",  /* 定时器名字是 timer1 */
                        timer1_out, 		  /* 超时时回调的处理函数 */
                        RT_NULL, 			  /* 超时函数的入口参数 */
                        10,      			  /* 定时长度，以OS Tick为单位，即5个OS Tick   --> 50MS*/  
                        RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_HARD_TIMER); /* 周期性定时器 */
    /* 启动定时器 */
    if (timer1 != RT_NULL){ 
				
				rt_timer_start(timer1);
				
		}

    return 0;
}
//INIT_APP_EXPORT(timer1_init);









