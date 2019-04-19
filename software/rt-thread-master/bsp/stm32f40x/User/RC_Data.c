/*
 * RC_Data.c
 *
 *  Created on: 2019年4月5日
 *      Author: zengwangfa
 *      Notes:  遥控器数据解析 Remote Control receiving Data
 */
#include "RC_Data.h"

ReceiveDataType ReceiveData = {
		.THR = 1500,
		.YAW = 1500,
		.ROL = 1500,
	  .PIT = 1500
};

uint8 Control_Data[20] = {0};
uint8 Receive_Data_OK = 0;
ControlDataType Control;

/**
  * @brief  Remote_Control_Data_Analysis(控制数据解析)
  * @param  控制字符数据 uint8 Data
  * @retval None
  * @notice 从第四个字节开始为控制字符
  */
void Remote_Control_Data_Analysis(uint8 Data) //控制数据解析
{
		static uint8 RxCheck = 0;	  //尾校验字
		static uint8 RxCount = 0;	  //接收计数
		static uint8 i = 0;	   		  //
	
		Control_Data[RxCount++] = Data;	//将收到的数据存入缓冲区中
	
		if(Control_Data[0] == 0xAA){ //接收到包头0xAA
				if(Control_Data[1] == 0x55){ //接收到包头0x55
						if(RxCount >= Control_Data[2]){ //判断数据包长度
								for(i = 0;i <= (RxCount-2);i++){ //累加和校验
										RxCheck += Control_Data[i];
								}
								if(RxCheck == Control_Data[RxCount-1]){
										Receive_Data_OK = 1; //接收数据包成功
								}
								else {Receive_Data_OK = 0;}
								
								RxCheck = 0; //接收完成清零
								RxCount = 0;	
						}
						else {Receive_Data_OK = 0;RxCount = 0;return;} //接收不成功清零
				}
				else {Receive_Data_OK = 0;RxCount = 0;return;} //接收不成功清零
		}
		else {Receive_Data_OK = 0;RxCount = 0;return;} //接收不成功清零
		
		if(1 == Receive_Data_OK){
			
				Control.Depth_Lock     = Control_Data[3]; //姿态控制
				Control.Direction_Lock = Control_Data[4];
				Control.Move					 = Control_Data[5];
				Control.Translation		 = Control_Data[6];
				Control.Vertical 			 = Control_Data[7];
				Control.Rotate 				 = Control_Data[8];
				
				Control.Power 				 = Control_Data[9];  //动力控制
				Control.Light 				 = Control_Data[10]; //灯光控制
				
				Control.Focus 				 = Control_Data[11]; //变焦摄像头控制
				Control.Zoom 					 = Control_Data[12];
				
				Control.Yuntai 				 = Control_Data[13]; //云台控制
				Control.Arm						 = Control_Data[14]; //机械臂控制
		}
}





			



	
	
	
	
