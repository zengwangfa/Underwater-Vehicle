#ifndef __RC_DATA_H_
#define __RC_DATA_H_

#include "DataType.h"


typedef struct{
		uint16  THR;
		uint16  YAW;
		uint16  ROL;
		uint16  PIT;
	
}ReceiveDataType;

extern ReceiveDataType ReceiveData;




#endif 




