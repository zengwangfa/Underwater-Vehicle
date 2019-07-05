#ifndef __DRV_ANO_
#define __DRV_ANO_

#include "DataType.h"
#include "flash.h"

extern Vector3f_pid PID_Parameter[PID_USE_NUM];

void ANO_SEND_StateMachine(void);//各组数据循环发送

void ANO_Data_Send_Version(int hardwareType,int hardwareVER,int softwareVER,int protocolVER,int bootloaderVER);//发送版本信息
void ANO_Data_Send_Status(int roll,int pitch,int yaw,int depth);//发送基本信息（姿态、锁定状态）

void ANO_Data_Send_Voltage_Current(float volatge,float current);//发送电压电流

void ANO_DT_Data_Receive_Prepare(uint8 data);//ANO地面站数据解析
void ANO_DT_Data_Receive_Anl(uint8 *data_buf,uint8 num);//ANO数据解析


void Save_Or_Reset_PID_Parameter(void);











#endif




