#define LOG_TAG    "init"

#include "init.h"
#include <rthw.h>
#include <board.h>
#include <stdio.h>

#include <spi_flash.h>
#include <easyflash.h>
#include <elog_flash.h>
#include <spi_flash_sfud.h>

/*----------------------- Variable Declarations -----------------------------*/

volatile u8 VehicleMode = ROV_Mode;   //ROV_Mode or AUV_Mode

rt_spi_flash_device_t nor_flash;
struct rt_thread thread_sys_monitor;

/*----------------------- Function Implement --------------------------------*/

static rt_err_t exception_hook(void *context);
static void rtt_user_assert_hook(const char* ex, const char* func, rt_size_t line);

/**
 * 系统监控线程
 * @param parameter parameter
 */
void thread_entry_sys_monitor(void* parameter)
{
    while (1)
    {
        IWDG_Feed(); //喂狗
				rt_thread_mdelay(500);
    }
}


/**
 * 系统初始化线程
 * @param parameter parameter
 */
void sys_init_thread(void* parameter){
	
	  /* 调度器上锁，上锁后，将不再切换到其他线程，仅响应中断 */
    rt_enter_critical();
	
    /* 初始化 nor_flash Flash 设备 */
    if ((nor_flash = rt_sfud_flash_probe("nor_flash", "spi20")) == NULL) {
				rt_kprintf("Error! No find nor_flash!");
        return;
    }
    /* 初始化 EasyFlash 模块 */
    easyflash_init();

    /* 初始化日志系统 */
    elog_init();
		
		/* 设置日志格式 */
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL & ~ELOG_FMT_P_INFO);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_P_INFO));
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_DEBUG,ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);

    elog_set_filter_lvl(ELOG_LVL_TOTAL_NUM);
		
#ifdef 	ELOG_COLOR_ENABLE  
		/* 使能日志颜色 */
    elog_set_text_color_enabled(true);
#endif
    /* 初始化EasyLogger的Flash插件 */
    elog_flash_init();
		
    /* 启动EasyLogger */
    elog_start();
		
		/* 设置硬件异常钩子 */
    rt_hw_exception_install(exception_hook);
		
    /* 设置RTT断言钩子 */
    rt_assert_set_hook(rtt_user_assert_hook);
		
		/* 调度器解锁 */
    rt_exit_critical();
}


/* 设置硬件异常钩子 */
static rt_err_t exception_hook(void *context) {
    extern long list_thread(void);
    uint8_t _continue = 1;
	

    rt_enter_critical();

#ifdef RT_USING_FINSH
    list_thread();
#endif
    while (_continue == 1);
    return RT_EOK;
}


/* 设置RTT断言钩子 */
static void rtt_user_assert_hook(const char* ex, const char* func, rt_size_t line) {
	

    rt_enter_critical();

#ifdef ELOG_ASYNC_OUTPUT_ENABLE
    elog_async_enabled(false);
#endif
    elog_a("rtt", "(%s) has assert failed at %s:%ld.", ex, func, line);

    while(1);
}

int rt_system_init(void)
{
    rt_thread_t sys_thread;  //系统部分初始化线程
		rt_thread_t monitor_thread;  //监视线程
				 
	  monitor_thread = rt_thread_create("monitor",
																			 thread_entry_sys_monitor, 
																			 NULL,
																			 512,
																			 30,
																			 10);
							 
    sys_thread = rt_thread_create("sys_init",
																	 sys_init_thread, 
																	 NULL,
																	 512,
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

//void delay_ms(u32 nTimer)
//{
//		u32 i=1000*nTimer;
//		delay_us(i);
//}





