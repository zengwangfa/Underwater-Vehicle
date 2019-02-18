#ifndef __DS18B20_H
#define __DS18B20_H 

#include <rtthread.h>
#include <board.h>
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK MiniSTM32开发板
//DS18B20驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/12
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

#define DS18B20_PIN 54
//IO方向设置
#define DS18B20_IO_IN()  {rt_pin_mode(DS18B20_PIN, PIN_MODE_INPUT);} //PD2
#define DS18B20_IO_OUT() {rt_pin_mode(DS18B20_PIN, PIN_MODE_OUTPUT);}
////IO操作函数											   
#define	DS18B20_DQ_OUT_HIGH rt_pin_write(DS18B20_PIN, PIN_HIGH)//数据端口	PD2
#define	DS18B20_DQ_OUT_LOW  rt_pin_write(DS18B20_PIN, PIN_LOW)	//数据端口	PD2
#define	DS18B20_DQ_IN  rt_pin_read(DS18B20_PIN)  //数据端口	PD2 
   	
rt_uint8_t DS18B20_Init(void);			//初始化DS18B20
short DS18B20_Get_Temp(void);	//获取温度
void DS18B20_Start(void);		//开始温度转换
void DS18B20_Write_Byte(rt_uint8_t dat);//写入一个字节
rt_uint8_t DS18B20_Read_Byte(void);		//读出一个字节
rt_uint8_t DS18B20_Read_Bit(void);		//读出一个位
rt_uint8_t DS18B20_Check(void);			//检测是否存在DS18B20
void DS18B20_Rst(void);			//复位DS18B20    
#endif















