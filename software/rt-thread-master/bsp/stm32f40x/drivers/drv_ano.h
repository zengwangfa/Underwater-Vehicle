#ifndef __DRV_ANO_
#define __DRV_ANO_

#include "sys.h"



void ANO_SEND_StateMachine(void);//各组数据循环发送


void ANO_Data_Send_Version(void);//发送基本信息（姿态、锁定状态）；
void ANO_Data_Send_Status(void);//发送基本信息（姿态、锁定状态）

void ANO_Data_Send_Voltage_Current(void);


void ANO_DT_Data_Receive_Prepare(u8 data);//ANO地面站数据解析
void ANO_DT_Data_Receive_Anl(u8 *data_buf,u8 num);//ANO数据解析

void Save_Or_Reset_PID_Parameter(void); //PID参数内部Flash读、写操作












#endif




