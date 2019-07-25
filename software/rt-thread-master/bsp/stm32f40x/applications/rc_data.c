/*
 * rc_data.c
 *
 *  Created on: 2019年4月5日
 *      Author: zengwangfa
 *      Notes:  遥控器数据解析 Remote Control Receiving Data
 */
#define LOG_TAG    "rc_data"

#include "rc_data.h"
#include "led.h"
#include "PropellerControl.h"
#include <string.h>

/*---------------------- Constant / Macro Definitions -----------------------*/

#define MAX_DATA_LENS 16  //有效数据包长度【不包含 包头、长度位、校验位】

/*----------------------- Variable Declarations -----------------------------*/

ReceiveData_Type ReceiveData = {
		.THR = 1500,
		.YAW = 1500,
		.ROL = 1500,
	  .PIT = 1500
};

ControlCmd_Type ControlCmd = { //上位机控制数据
										.Power = 0,
										.All_Lock = LOCK //初始化定义为锁定状态
};

Rocker_Type Rocker; // 摇杆数据结构体

uint32 Frame_Conut = 0;//数据包 帧计数
uint8 Frame_EndFlag = 0; //接收数据包结束标志 
uint8 RC_Control_Data[30] = {0};
uint8 Receive_Data_OK = 0;
uint8 Control_RxCheck = 0;	  //尾校验字
uint8 Control_RxCount = 0;	  //接收计数

/*----------------------- Function Implement --------------------------------*/
/**
  * @brief  Remote_Control_Data_Analysis(控制数据解析)
  * @param  控制字符数据 uint8 Data
  * @retval None
  * @notice 从第四个字节开始为控制字符
  */
void Remote_Control_Data_Analysis(uint8 Data) //控制数据解析
{

		static uint8 i = 0;	
	
		RC_Control_Data[Control_RxCount++] = Data;	//将收到的数据存入缓冲区中
		if(Control_RxCount <= (MAX_DATA_LENS+8)){ //定义数据长度未包括包头和包长3个字节,+4)          
				if(RC_Control_Data[0] == 0xAA){ //接收到包头0xAA
						if(Control_RxCount > 3){
								if(RC_Control_Data[1] == 0x55){ //接收到包头0x55
										if(RC_Control_Data[2] > MAX_DATA_LENS+4){Receive_Data_OK = 0;Control_RxCount = 0;return;} //接收超过协议字符数
											
										if(Control_RxCount >= RC_Control_Data[2]+4){ //接收到数据包长度位，开始判断什么时候开始计算校验
												for(i = 0;i <= (Control_RxCount-2);i++){ //累加和校验
														Control_RxCheck += RC_Control_Data[i];
												}
												if(Control_RxCheck == RC_Control_Data[Control_RxCount-1]){
														Receive_Data_OK = 1; //接收数据包成功
														Frame_EndFlag = 1;
														Frame_Conut ++;
												}
												else {Receive_Data_OK = 0;}
												 
												Control_RxCheck = 0; //接收完成清零
												Control_RxCount = 0;	
										}
								}
								else {Receive_Data_OK = 0;Control_RxCount = 0;Receive_Data_OK = 0;Control_Cmd_Clear(&ControlCmd);return;} //接收不成功清零
						}
				}
				else {Receive_Data_OK = 0;Control_RxCount = 0;Receive_Data_OK = 0;Control_Cmd_Clear(&ControlCmd);;return;} //接收不成功清零
		}
		else {Receive_Data_OK = 0;Control_RxCount = 0;Receive_Data_OK = 0;Control_Cmd_Clear(&ControlCmd);return;} //接收不成功清零
}

		

//控制字获取
void Control_Cmd_Get(ControlCmd_Type *cmd) //控制命令获取
{
		if(1 == Receive_Data_OK){
				cmd->Depth_Lock     = RC_Control_Data[3]; //深度锁定
				cmd->Direction_Lock = RC_Control_Data[4]; //方向锁定
			
				cmd->Move					  = RC_Control_Data[5]; //前后
				cmd->Translation	  = RC_Control_Data[6]; //左右平移
				cmd->Vertical 			= RC_Control_Data[7]; //垂直
				cmd->Rotate 				= RC_Control_Data[8]; //旋转
				
			  cmd->Power 				  = RC_Control_Data[9];  //动力控制  推进器动力系数
				cmd->Light 				  = RC_Control_Data[10]; //灯光控制
				cmd->Focus 				  = RC_Control_Data[11]; //变焦摄像头控制
				cmd->Yuntai 				= RC_Control_Data[12]; //云台控制
				cmd->Arm						= RC_Control_Data[13]; //机械臂控制
				cmd->Raspi          = RC_Control_Data[14]; //树莓派启动位
				cmd->All_Lock       = RC_Control_Data[18];
			
				Receive_Data_OK = 0x00;//清零标志位

		}
}

/* 
	 【注意】这里仅清空控制数据指令，不能清除 控制状态指令
		因此，不在采用meset直接填充结构体为0
*/
void Control_Cmd_Clear(ControlCmd_Type *cmd) //memset(&addr,0,sizeof(addr));
{
		//cmd->All_Lock       = 0; 
		//cmd->Depth_Lock     = 0; //深度锁定
		//cmd->Direction_Lock = 0; //方向锁定
		//cmd->Raspi          = 0; //树莓派启动位
	
		cmd->Move					  = 0; //前后
		cmd->Translation	  = 0; //左右平移
		cmd->Vertical 			= 0; //垂直
		cmd->Rotate 				= 0; //旋转
		
		cmd->Power 				  = 0;  //动力控制  推进器动力系数
		cmd->Light 				  = 0; //灯光控制
		cmd->Focus 				  = 0; //变焦摄像头控制
		cmd->Yuntai 				= 0; //云台控制
		cmd->Arm						= 0; //机械臂控制

		//memset(cmd,0,sizeof(*cmd));//内存块内填充0 【不适用】
}



/**
	* @brief  is_raspi_start(树莓派是否启动)
  * @param  None
  * @retval 1 连接上   0无连接
  * @notice 
  */
uint8 is_raspi_start(void)
{
		return ControlCmd.Raspi; 
}
			



	
	
	
	
