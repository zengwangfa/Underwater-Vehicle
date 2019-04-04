#ifndef __CONTROL_H_
#define __CONTROL_H_

#include "DataType.h"

typedef struct{
		uint16  THR;
		uint16  YAW;
		uint16  ROL;
		uint16  PIT;
	
}ReceiveComputerData;



void Angle_Control(void);









#endif


