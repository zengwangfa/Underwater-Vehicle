#ifndef __CONTROL_H_
#define __CONTROL_H_

#include "DataType.h"

extern Direction_Type Direction;
extern char ACC1,ACC2,ACC3,ACC4 ;

void Angle_Control(void);
void Depth_PID_Control(float expect_depth,float sensor_depth);










#endif


