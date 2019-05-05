#ifndef __RETURN_DATA_
#define __RETURN_DATA_

#include "DataType.h"




void Convert_Return_Computer_Data(void); //返回上位机数据 转换
void Send_Buffer_Agreement(uint8 *begin_buff,int8 *buff,uint8 len);

extern int8 Return_Data[18];

extern uint8 device_hint_flag;		//设备提示字符




#endif






