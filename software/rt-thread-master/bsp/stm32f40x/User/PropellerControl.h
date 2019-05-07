#ifndef __PROPELLERCONTROL_H
#define __PROPELLERCONTROL_H

#include "DataType.h"
#include "propeller.h"

void Propeller_Control(void);
void Propeller_Output(void);
void robot_upDown(float depth); 

void robotForward(void);  //前进
void robotBackAway(void); //后退

void turnRight(void);  //右转
void turnLeft(void);  //左转

void moveLeft(void);  //左移
void moveRight(void);  //右移

extern PropellerPower_Type  PropellerPower; //推进器推理控制器
extern int16 Expect_Depth;
#endif



