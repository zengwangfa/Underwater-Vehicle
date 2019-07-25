/*
 * focus.c
 *
 *  Created on: 2019年2月30日
 *      Author: zengwangfa
 *      Notes:  变焦摄像头设备(控制通信)
 */

#include "focus.h"
#include <rtthread.h>
#include <elog.h>
#include "uart.h"
#include "ret_data.h"
#include "Control.h"
/*---------------------- Constant / Macro Definitions -----------------------*/


/*----------------------- Variable Declarations -----------------------------*/
uint8 focus_data_ok = 0;

uint8 addFocus_Data[6] = {0xAA,0x55,0x02,0x01,0x00,0x02}; //放大
uint8 minFocus_Data[6] = {0xAA,0x55,0x02,0x02,0x00,0x03}; //缩小

uint8 addZoom_Data[6]  = {0xAA,0x55,0x02,0x00,0x01,0x02}; //聚焦
uint8 minZoom_Data[6]  = {0xAA,0x55,0x02,0x00,0x02,0x03}; //放焦

uint8 Camera_Clear_Data[6] = {0xAA,0x55,0x02,0x88,0x88,0x11}; //恢复初始值
uint8 Camera_Stop_Data[6]  = {0xAA,0x55,0x02,0x00,0x00,0x01}; //停止
/*----------------------- Function Implement --------------------------------*/
float Adjust1= 1,Adjust2 = 1;
uint8 focus_data[10] = {0};
/**
  * @brief  Focus_Zoom_Camera(摄像头变焦、放大)
  * @param  控制字符数据action [0x01聚焦、0x02放焦、0x11放大、0x12缩小]
  * @retval None
  * @notice 
  */
void Focus_Zoom_Camera(uint8 *action)
{		
		switch(*action){
				case 0x01:Adjust1 += 0.01f;break;         //rt_device_write(focus_uart_device, 0,addFocus_Data,   6); break;
				case 0x02:Adjust1 -= 0.01f;break;         //rt_device_write(focus_uart_device, 0,minFocus_Data,   6); break;
				case 0x11:Adjust2 += 0.01f;break;         //rt_device_write(focus_uart_device, 0,addZoom_Data,    6); break;
				case 0x12:Adjust2 -= 0.01f;break;    	 //rt_device_write(focus_uart_device, 0,minZoom_Data,    6); break;
				case 0x88:Adjust1 = 1,Adjust2 = 1;break; //rt_device_write(focus_uart_device, 0,Camera_Clear_Data,    6); break; //恢复初始值
				default  :break; //可能为错误命令 停止控制
		}
		*action = 0x00;
}


/**
  * @brief  Camera_Focus_Data_Analysis(变焦摄像头返回数据解析)
  * @param  控制字符数据 uint8 Data
  * @retval None
  * @notice 从第四个字节开始为控制字符
  */
void Camera_Focus_Data_Analysis(uint8 Data) //控制数据解析
{
		static uint8 RxCheck = 0;	  //尾校验字
		static uint8 RxCount = 0;	  //接收计数
		static uint8 i = 0;	   		  //

		focus_data[RxCount++] = Data;	//将收到的数据存入缓冲区中
	
		if(focus_data[0] == 0xAA){ //接收到包头0xAA
				if(RxCount > 3){
						if(focus_data[1] == 0x55){ //接收到包头0x55
								if(RxCount >= focus_data[2]+4){ //接收到数据包长度位，开始判断什么时候开始计算校验
										for(i = 0;i <= (RxCount-2);i++){ //累加和校验
												RxCheck += focus_data[i];
										}
										if(RxCheck == focus_data[RxCount-1]){
												focus_data_ok = 1; //接收数据包成功
										}
										else {focus_data_ok = 0;}
										
										RxCheck = 0; //接收完成清零
										RxCount = 0;	
								}
						}
						else {focus_data_ok = 0;RxCount = 0;return;} //接收不成功清零
				}
		}
		else {focus_data_ok = 0;RxCount = 0;return;} //接收不成功清零
		
		if(focus_data_ok){ // 提示字符 标志计算
			
				if(focus_data[3] == 0xFF ){
						device_hint_flag |= 0x04;
				}
				else {
						device_hint_flag &= 0xFB;
				}
				if(focus_data[4] == 0xFF ){
						device_hint_flag |= 0x08;
				}
				else {
						device_hint_flag &= 0xF7;
				}
		}

}

/* 设置 恢复初始值 */
void focus_camera_clear(void)
{
		rt_device_write(focus_uart_device, 0,Camera_Clear_Data,    6);
		log_i("focus_camera_clear... ");
}
MSH_CMD_EXPORT(focus_camera_clear,focus camera clear);


/* 设置 放大 */
void focus_camera_add(void)
{
		static int *p;
		*p = 0x0l;
		Focus_Zoom_Camera((uint8 *)p);
		log_i("focus_camera_add ");
}
MSH_CMD_EXPORT(focus_camera_add,focus camera add);

