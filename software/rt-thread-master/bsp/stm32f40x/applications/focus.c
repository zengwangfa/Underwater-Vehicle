#include "focus.h"
#include <rtthread.h>
#include <elog.h>
#include "uart.h"



/*---------------------- Constant / Macro Definitions -----------------------*/


/*----------------------- Variable Declarations -----------------------------*/

uint8 addFocus_Data[6] = {0xAA,0x55,0x02,0x01,0x00,0x00}; //放大
uint8 minFocus_Data[6] = {0xAA,0x55,0x02,0x02,0x00,0x01}; //缩小

uint8 addZoom_Data[6]  = {0xAA,0x55,0x02,0x00,0x01,0x00}; //聚焦
uint8 minZoom_Data[6]  = {0xAA,0x55,0x02,0x00,0x02,0x01}; //放焦

/*----------------------- Function Implement --------------------------------*/

/**
  * @brief  Focus_Camera(摄像头聚焦)
  * @param  控制字符数据 [0x01放大、0x02缩小]
  * @retval None
  * @notice 
  */
void Focus_Camera(uint8 focus)
{
		if(0x01 == focus){ //放大
				rt_device_write(focus_uart_device, 0,addFocus_Data, 6);    //发送后命令
		}
		else if(0x02 == focus ){ //缩小
				rt_device_write(focus_uart_device, 0,minFocus_Data, 6);    //发送后命令
		}
}

/**
  * @brief  Zoom_Camera(摄像头变焦)
  * @param  控制字符数据 [0x01聚焦、0x02放焦]
  * @retval None
  * @notice 
  */
void Zoom_Camera(uint8 zoom)
{
	
		if(0x01 == zoom ){ //聚焦
				rt_device_write(focus_uart_device, 0,addZoom_Data, 6);    //发送后命令
				
		}
		else if(0x02 == zoom){ //放焦
				rt_device_write(focus_uart_device, 0,minZoom_Data, 6);    //发送后命令
		}
	
}


/* 设置 放大 */
void focus_camera_add(void)
{
		Focus_Camera(1);
		log_i("focus_camera_add ");
}
MSH_CMD_EXPORT(focus_camera_add,focus camera add);












