#ifndef __DEBUG_
#define __DEBUG_


void ANO_SEND_StateMachine(void);//各组数据循环发送

void ANO_Data_Send_Status(void);//发送基本信息（姿态、锁定状态）
void ware_test(void);
void vcan_sendware(void *wareaddr, unsigned int waresize);














#endif


