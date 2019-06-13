#ifndef __FILTER_H_
#define __FILTER_H_

#include "DataType.h"

typedef struct
{
		float Input_Butter[3];
		float Output_Butter[3];
}Butter_BufferData;

typedef struct
{
		float a[3];
		float b[3];
}Butter_Parameter;

uint32 Bubble_Filter(uint32 *value);

float KalmanFilter(float *Original_Data);






#endif



