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


void Remote_Control_Data_Analysis(uint8 Data)
{
		static uint8 RxBuffer[50];  //数据包
		static uint8 RxCheck = 0;	  //尾校验字
		static uint8 RxCount = 0;	   //接收计数
		static uint8 state = 0;
		static uint8 i = 0;	   		  //接收计数

	
		RxBuffer[RxCount++] = Data;	//将收到的数据存入缓冲区中
	
		if(Control_Data[0] != 0xAA){
				RxCount = 0;
				return;
		}
}





			



	
	
	
	
