#ifndef __INIT_H
#define __INIT_H


#include "sys.h" 
#include "led.h"
#include "key.h"

enum VehicleStatus{
	System_NULL = 1,
	System_Er,
	System_OK

};  //枚举系统状态



enum VehicleMode{
	ROV_Mode = 1,
	AUV_Mode,
};  //枚举系统模式



#endif



