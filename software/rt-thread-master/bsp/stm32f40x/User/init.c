/**
 *                             _ooOoo_
 *                            o8888888o
 *                            88" . "88
 *                            (| -_- |)
 *                            O\  =  /O
 *                         ____/`---'\____
 *                       .'  \\|     |//  `.
 *                      /  \\|||  :  |||//  \
 *                     /  _||||| -:- |||||-  \
 *                     |   | \\\  -  /// |   |
 *                     | \_|  ''\---/''  |   |
 *                     \  .-\__  `-`  ___/-. /
 *                   ___`. .'  /--.--\  `. . __
 *                ."" '<  `.___\_<|>_/___.'  >'"".
 *               | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *               \  \ `-.   \_ __\ /__ _/   .-` /  /
 *          ======`-.____`-.___\_____/___.-`____.-'======
 *                             `=---='
 *          ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *                     佛祖保佑        永无BUG
*/

#define LOG_TAG    "init"

#include "init.h"
#include <rthw.h>
#include <board.h>
#include <stdio.h>

#include <spi_flash.h>
#include <easyflash.h>
#include <elog_flash.h>
#include <spi_flash_sfud.h>

#include "flash.h"
/*----------------------- Variable Declarations -----------------------------*/


rt_spi_flash_device_t nor_flash;

extern struct rt_event init_event;/* ALL_init 事件控制块 */
/*----------------------- Function Implement --------------------------------*/

static rt_err_t exception_hook(void *context);
static void rtt_user_assert_hook(const char* ex, const char* func, rt_size_t line);

/**
 * 系统监控线程
 * @param  parameter
 */
void thread_entry_sys_monitor(void* parameter)
{
    while (1)
    {
        IWDG_Feed(); //喂狗,放置系统异常卡死
				rt_thread_mdelay(500);
    }
}


/**
 * 系统初easylogger 与 easyflash 组件始化线程
 * @param parameter parameter
 */
void sys_init_thread(void* parameter){
	
		rt_err_t result;

    if ((nor_flash = rt_sfud_flash_probe("W25Q128", "spi20")) == NULL) { /* 初始化 nor_flash W25Q128 Flash 设备 */ 
				rt_kprintf("Error! No find W25Q128!");  //16MB Flash
        return;
    }

    easyflash_init();		/* 初始化 EasyFlash 模块 */
		elog_init_start();  /* 初始化日志系统 */
		
    rt_hw_exception_install(exception_hook);	/* 设置硬件异常钩子 */
    rt_assert_set_hook(rtt_user_assert_hook); /* 设置RTT断言钩子 */
		
    result = rt_event_init(&init_event, "event", RT_IPC_FLAG_FIFO);  /* 初始化事件对象 */
    if (result != RT_EOK){
        log_e("init event failed.\n");
		}
		Normal_Parameter_Init_With_Flash(); //Flash参数初始化读取
		rt_kprintf("file:%s,function:%s,line:%d\n",__FILE__,__FUNCTION__,__LINE__); //打印所在 文件、函数名、行号
}


/* 设置硬件异常钩子 */
static rt_err_t exception_hook(void *context) {
    extern long list_thread(void);
    uint8_t _continue = 1;
	
		ErrorStatus_LED(); //异常状态指示灯 红色
    rt_enter_critical();//禁止调度

    list_thread();      //打印线程
    while (_continue == 1);//异常卡死
    return RT_EOK;
}


/* 设置RTT断言钩子 */
static void rtt_user_assert_hook(const char* ex, const char* func, rt_size_t line) {
	
		ErrorStatus_LED();  //异常状态指示灯 红色
    rt_enter_critical();//禁止调度
    elog_async_enabled(false); //禁用异步输出模式
    elog_a("rtt", "(%s) has assert failed at %s:%ld.", ex, func, line);//打印造成断言C语言行数

    while(1);//异常卡死
}

int rt_system_init(void)
{
    rt_thread_t sys_thread;  //系统部分初始化线程
		rt_thread_t monitor_thread;  //监视线程
				 
	  monitor_thread = rt_thread_create("monitor",
																			 thread_entry_sys_monitor, 
																			 NULL,
																			 1024,
																			 30,
																			 10);
							 
    sys_thread = rt_thread_create("sys_init",
																	 sys_init_thread, 
																	 NULL,
																	 1024,
																	 5,
																	 10);
	
    if (monitor_thread != NULL) {
        rt_thread_startup(monitor_thread);
    }
		else {
		  	rt_kprintf("monitoring error!");
		}
    if (sys_thread != NULL) {
        rt_thread_startup(sys_thread);
    }
		else {
				rt_kprintf("sys init error!");
		}
    return 0;
}
INIT_DEVICE_EXPORT(rt_system_init);



void rt_hw_us_delay(u32 us)
{
    rt_uint32_t delta;
    /* 获得延时经过的 tick 数 */
    us = us * (SysTick->LOAD/(1000000/RT_TICK_PER_SECOND));
    /* 获得当前时间 */
    delta = SysTick->VAL;
    /* 循环获得当前时间，直到达到指定的时间后退出循环 */
    while (delta - SysTick->VAL< us);
}

void rt_hw_ms_delay(u32 ms)
{
		rt_hw_us_delay(1000 * ms);
}

void delay_us(u32 nTimer)
{
	u32 i=0;
	for(i=0;i<nTimer;i++){
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	}
}





