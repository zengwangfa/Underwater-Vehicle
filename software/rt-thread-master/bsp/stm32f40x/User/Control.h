#ifndef __CONTROL_H_
#define __CONTROL_H_

#include "DataType.h"
#include "rc_data.h"


void Angle_Control(void);
void Depth_PID_Control(float expect_depth,float sensor_depth);


void FourtAxis_Control(Rocker_Type *rc);

void SixAxis_Control(Rocker_Type *rc);


void Convert_RockerValue(Rocker_Type *rc); //ªÒ»°“°∏À÷µ

void Speed_Buffer(short *now_value,short *last_value,short BufferRange);



#endif


