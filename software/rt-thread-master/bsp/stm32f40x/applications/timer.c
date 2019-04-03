#define LOG_TAG    "timer"


#include <string.h>

#include "timer.h"
#include "flash.h"
#include <rtthread.h>
#include <elog.h>
#include "drv_MS5837.h"
#include  "Control.h"
/*---------------------- Constant / Macro Definitions -----------------------*/



/*----------------------- Variable Declarations -----------------------------*/



uint8 ov_frame = 0;
uint8 ov_frame_flag = 0;

/*----------------------- Function Implement --------------------------------*/

static void timer1_out(void* parameter)// 定时器1超时函数  进行JY901模块数据转换
{
		static int count = 0;

		count ++;

	
	
		Angle_Control();
		if(20 == count){	//1s
				
				if(1 == ov_frame_flag)
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
                        timer1_out, 		  /* 超时时回调的处理函数 */
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






