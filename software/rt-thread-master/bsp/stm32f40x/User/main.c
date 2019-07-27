/******************************************************************************
* Copyright 2019-2020 JMU Electronic Technology Association / zengwangfa@outlook.com
*
* ProjectName: Underwater Vehicle Control
* Desc: 			 Peripherals control
*
* Author:  Dwfish
* Date: 	 2019-02-06
*
* -----------------------------------------------------------------
 * Change Logs:
 * Date           Author       Notes
 * 2019-02-06     Dwfish       Creat LED / KEY / Buzzer /PWM /ADC Thread.
 * 2019-02-07     Dwfish       Creat OLED Display Thread.
 * 2019-02-09     Dwfish       Creat Get_Gyroscope Thread. 
 * 2019-02-11     Dwfish       Creat some MSHs (get methods) & Improved OLED display methods. 
 * 2019-02-13     Dwfish       Creat Can change Servo Motor Mid-Value MSH. 
 * 2019-02-16     Dwfish       Creat System self-check methods[events]. 
 * 2019-02-16     Dwfish       Add W25Q128 FLASH To Save Important Parameters.
 * 2019-02-16     Dwfish       Creat OLED Electronic compass.
 * 2019-02-19     Dwfish       Add VACN & ANO host computer debug.
 * 2019-02-20     Dwfish       Add Easylog & Easyflash & <update> msh.
 * 2019-03-24     Dwfish       Add Cpuusage.
 * 2019-04-19     Dwfish       Add Focus Camera ControlCmd.
 * 2019-07-01     Dwfish       Add Control Method.
	
	Bugs:
 * 2019-04-29     BUG:IAP Download dispaly success,but Download fail.

*
******************************************************************************/
#define LOG_TAG    "main"


#include <rtthread.h>
#include "DataType.h" //自定义数据类型 头文件【重要】

/* 最重要定义 直接从设定好的模式(Flash中读取)*/
volatile uint8 VehicleMode = FOUR_AXIS;   //【FOUR_AXIS】 or 【SIX_AXIS】
volatile uint8 WorkMode    = WORK;        //【WORK】 or 【DEBUG】
//volatile 的作用 是作为指令关键字，确保本条指令不会因编译器的优化而省略，且要求每次直接在其内存中读值。
/**
 * @addtogroup STM32
 */

/* 打印相关日志信息 */
int main(void)
{
    /* user app entry. */

//		rt_kprintf("* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");	
//		rt_kprintf("* ProjectName: Underwater Vehicle Control	\n");
//		rt_kprintf("* Desc:    Peripherals control	\n");
//		rt_kprintf("*	\n");
//		rt_kprintf("* Author:  Dwfish	\n");
//		rt_kprintf("* Date:	   2019-02-06	\n");
//		rt_kprintf("*	\n");
//		rt_kprintf("* -----------------------------------------------------------------\n");
//		rt_kprintf("* Change Logs:\n");
//		rt_kprintf("* Date           Author       Notes\n");
//	
//		rt_kprintf("* 2019-02-06     Dwfish       Creat LED / KEY / Buzzer /PWM /ADC Thread.\n");
//		rt_kprintf("* 2019-02-07     Dwfish       Creat OLED Thread\n"); 
//		rt_kprintf("* 2019-02-09     Dwfish       Creat Get_Gyroscope Thread.\n");
//	
//		rt_kprintf("* 2019-02-11     Dwfish       Creat MSHs (some get methods) && Improved OLED Display. \n");
//		rt_kprintf("* 2019-02-13     Dwfish       Creat Can change Servo Motor Mid-Value MSH-methods.  \n");
//		rt_kprintf("* 2019-02-16     Dwfish       Creat System self-check methods [ Events ].\n");
//		rt_kprintf("* 2019-02-16     Dwfish       Add W25Q128 FLASH To Save Important Parameters.\n");
//		rt_kprintf("* 2019-02-16     Dwfish       Creat OLED Electronic compass.\n");
//		rt_kprintf("* 2019-02-19     Dwfish       Add VACN host computer debug.\n");
//		rt_kprintf("* 2019-02-20     Dwfish       Add Easylog & Easyflash & <update> msh.\n");

//		rt_kprintf("* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");	 
		//print_time();  //打印参考时间戳
		
		return 0;
}

/*
0.[A]：断言(Assert)
1.[E]：错误(Error)
2.[W]：警告(Warn)
3.[I]：信息(Info)
4.[D]：调试(Debug)
5.[V]：详细(Verbose)
*/











