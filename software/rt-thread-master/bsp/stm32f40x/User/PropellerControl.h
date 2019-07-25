#ifndef __PROPELLERCONTROL_H
#define __PROPELLERCONTROL_H

#include "DataType.h"
#include "propeller.h"
#include "rc_data.h"

void Propeller_Control(void);
void Propeller_Output(void);
void robot_upDown(float depth); 
void Horizontal_Propeller_Power_Clear(void);//水平方向推力清零
void Propller_stop(void);  //推进器停转

void robotForward(void);  //前进
void robotBackAway(void); //后退

void turnRight(uint16 power); //右转
void turnLeft(uint16 power);  //左转

extern float Expect_Depth;
#endif



