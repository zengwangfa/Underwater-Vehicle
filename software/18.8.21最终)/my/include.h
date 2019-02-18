#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

/*
 * Include 用户自定义的头文件
 */
#include  "MK60_wdog.h"
#include  "MK60_gpio.h"     //IO口操作
#include  "MK60_uart.h"     //串口
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"    //低功耗定时器(延时)
#include  "MK60_i2c.h"      //I2C
#include  "MK60_ftm.h"      //FTM
#include  "MK60_pit.h"      //PIT
#include  "MK60_adc.h"      //ADC
#include  "MK60_dac.h"      //DAC
#include  "MK60_dma.h"      //DMA
#include  "MK60_FLASH.h"    //FLASH
#include  "MK60DZ10.h"

	 
#include  "VCAN_computer.h"
#include  "VCAN_OV7725_Eagle.h"
#include  "VCAN_OV7725_REG.h"
#include  "VCAN_camera.h"       //摄像头总头文件
#include  "LQ_OLED.h"

#include  "debug.h"          
#include  "general.h"          
#include  "init.h"  
#include  "isr.h"  
#include  "key.h"  
#include  "magnetic.h"  
#include  "camera.h"
#include  "control.h"
#include  "message.h"
#include  "meetcar.h"
#include  "fuzzy.h"


#endif  //__INCLUDE_H__
