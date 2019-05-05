/*
 * RC_Data.c
 *
 *  Created on: 2019年4月5日
 *      Author: zengwangfa
 *      Notes:  遥控器数据解析 Remote Control receiving Data
 */
#include "RC_Data.h"
#include "led.h"
ReceiveDataType ReceiveData = {
		.THR = 1500,
		.YAW = 1500,
		.ROL = 1500,
	  .PIT = 1500
};
#define MAX_DATA_LENS 16  //有效数据包长度【不包含 包头、长度位、校验位】
uint8 Control_Data[30] = {0};
uint8 Receive_Data_OK = 0;
ControlDataType Control;
uint8 Control_RxCheck = 0;	  //尾校验字
uint8 Control_RxCount = 0;	  //接收计数
/**
  * @brief  Remote_Control_Data_Analysis(控制数据解析)
  * @param  控制字符数据 uint8 Data
  * @retval None
  * @notice 从第四个字节开始为控制字符
  */
void Remote_Control_Data_Analysis(uint8 Data) //控制数据解析
{

		static uint8 i = 0;	
	
		Control_Data[Control_RxCount++] = Data;	//将收到的数据存入缓冲区中
		if(Control_RxCount <= (MAX_DATA_LENS+8)){ //定义数据长度未包括包头和包长3个字节,+4)          
				if(Control_Data[0] == 0xAA){ //接收到包头0xAA
						if(Control_RxCount > 3){
								if(Control_Data[1] == 0x55){ //接收到包头0x55
										if(Control_Data[2] > MAX_DATA_LENS+4){Receive_Data_OK = 0;Control_RxCount = 0;return;} //接收超过协议字符数
											
										if(Control_RxCount >= Control_Data[2]+4){ //接收到数据包长度位，开始判断什么时候开始计算校验
												for(i = 0;i <= (Control_RxCount-2);i++){ //累加和校验
														Control_RxCheck += Control_Data[i];
												}
												if(Control_RxCheck == Control_Data[Control_RxCount-1]){
														Receive_Data_OK = 1; //接收数据包成功
												}
												else {Receive_Data_OK = 0;}
												 
												Control_RxCheck = 0; //接收完成清零
												Control_RxCount = 0;	
										}
								}
								else {Receive_Data_OK = 0;Control_RxCount = 0;Control_Clear();return;} //接收不成功清零
						}//
				}
				else {Receive_Data_OK = 0;Control_RxCount = 0;Control_Clear();return;} //接收不成功清零
		}
		else {Receive_Data_OK = 0;Control_RxCount = 0;Control_Clear();return;} //接收不成功清零
		
		if(1 == Receive_Data_OK){
			
				Control.Depth_Lock     = Control_Data[3]; //姿态控制
				Control.Direction_Lock = Control_Data[4];
				Control.Move					 = Control_Data[5];
				Control.Translation		 = Control_Data[6];
				Control.Vertical 			 = Control_Data[7];
				Control.Rotate 				 = Control_Data[8];
				
				Control.Power 				 = Control_Data[9];  //动力控制 两倍[0~500]
				Control.Light 				 = Control_Data[10]; //灯光控制
				
				Control.Focus 				 = Control_Data[11]; //变焦摄像头控制
				
				Control.Yuntai 				 = Control_Data[12]; //云台控制
				Control.Arm						 = Control_Data[13]; //机械臂控制
		}
		else{
				Control_Clear();
		}
}

void Control_Clear(void)
{
		Control.Depth_Lock     = 0x00; //姿态控制
		Control.Direction_Lock = 0x00;
		Control.Move					 = 0x00;
		Control.Translation		 = 0x00;
		Control.Vertical 			 = 0x00;
		Control.Rotate 				 = 0x00;
		
		//Control.Power 				 = 0x00;  //动力控制
		Control.Light 				 = 0x00; //灯光控制
		
		Control.Focus 				 = 0x00; //变焦摄像头控制
		
		Control.Yuntai 				 = 0x00; //云台控制
		Control.Arm						 = 0x00; //机械臂控制
}

			



	
	
	
	
