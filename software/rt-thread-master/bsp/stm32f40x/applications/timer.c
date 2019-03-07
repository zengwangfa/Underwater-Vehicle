#define LOG_TAG    "timer"

#include "init.h"
#include <string.h>
#include "board.h"
#include <rthw.h>
#include "timer.h"

/*---------------------- Constant / Macro Definitions -----------------------*/



/*----------------------- Variable Declarations -----------------------------*/

extern struct JY901_t JY901; //JY901真实值结构体

u8 ov_frame = 0;
u8 ov_frame_flag = 0;

/*----------------------- Function Implement --------------------------------*/

static void time_out(void* parameter)// 定时器1超时函数  进行JY901模块数据转换
{
		static int count = 0;
	  /* 调度器上锁，上锁后，将不再切换到其他线程，仅响应中断 */
    rt_enter_critical();
	
		JY901_Convert(&JY901);

		/* 调度器解锁 */
    rt_exit_critical();
	
		count ++;
		if(count == 20){	
				if(ov_frame_flag == 1)
				{
						rt_kprintf("OV Frame:%d\n",ov_frame);
						ov_frame_flag = 0; //清零
				}
				count = 0;
				ov_frame = 0;
		}
}



int timer1_init(void)
{
		/* 定时器的控制块 */
		static rt_timer_t timer1;
    /* 创建定时器1 */
    timer1 = rt_timer_create("timer1",  /* 定时器名字是 timer1 */
                        time_out, 		  /* 超时时回调的处理函数 */
                        RT_NULL, 			  /* 超时函数的入口参数 */
                        5,      			  /* 定时长度，以OS Tick为单位，即5个OS Tick   --> 50MS*/  
                        RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_HARD_TIMER); /* 周期性定时器 */
    /* 启动定时器 */
    if (timer1 != RT_NULL){ 
				rt_timer_start(timer1);
		}

    return 0;
}
INIT_APP_EXPORT(timer1_init);


/* Get Camera 帧率 */
void get_ov_frame(void)
{
		ov_frame_flag = 1;//开启 打印帧率
}
MSH_CMD_EXPORT(get_ov_frame, get ov frame [fps]);






