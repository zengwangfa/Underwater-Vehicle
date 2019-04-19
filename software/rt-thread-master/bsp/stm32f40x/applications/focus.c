#include "focus.h"
#include <rtthread.h>
#include <elog.h>

extern rt_device_t focus_uart_device;	

uint8 addFocus_Data[5] = {0xAA,0x55,0x01,0x00,0x00}; //放大
uint8 minFocus_Data[5] = {0xAA,0x55,0x02,0x00,0x01}; //缩小

uint8 addZoom_Data[5]  = {0xAA,0x55,0x00,0x01,0x00}; //聚焦
uint8 minZoom_Data[5]  = {0xAA,0x55,0x00,0x02,0x01}; //放焦

void Focus_Camera(uint8 focus)
{
		if(0x01 == focus){ //聚焦
				rt_device_write(focus_uart_device, 0,addFocus_Data, 5);    //发送后命令
				
		}
		else if(0x02 == focus ){ //放焦
				rt_device_write(focus_uart_device, 0,minFocus_Data, 5);    //发送后命令
		}
}


void Zoom_Camera(uint8 zoom)
{
	
		if(0x01 == zoom ){ //聚焦
				rt_device_write(focus_uart_device, 0,addZoom_Data, 5);    //发送后命令
				
		}
		else if(0x02 == zoom){ //放焦
				rt_device_write(focus_uart_device, 0,minZoom_Data, 5);    //发送后命令
		}
	
}


/* 设置 九轴模块 波特率为9600 */
void focus_camera_add(void)
{
		Focus_Camera(1);
		log_i("focus_camera_add ");
}
MSH_CMD_EXPORT(focus_camera_add,focus camera add);












