#ifndef _FILTER_H_
#define _FILTER_H_

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

short bubble(short *adc_value);

float KalmanFilter(float *Original_Data);






#endif



