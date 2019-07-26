#ifndef __CONTROL_H_
#define __CONTROL_H_

#include "DataType.h"
#include "rc_data.h"

extern Direction_Type Direction;
extern char ACC1,ACC2,ACC3,ACC4 ;

void Angle_Control(void);
void Depth_PID_Control(float expect_depth,float sensor_depth);


void FourtAxis_Control(Rocker_Type *rc);

void SixAxis_Control(Rocker_Type *rc);


void Convert_RockerValue(Rocker_Type *rc); //ªÒ»°“°∏À÷µ




#endif


