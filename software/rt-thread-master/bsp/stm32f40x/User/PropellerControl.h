#ifndef __PROPELLERCONTROL_H
#define __PROPELLERCONTROL_H

#include "DataType.h"


void Propeller_Control(void);
void Propeller_Output(void);
void Propeller_upDown(int depth);

void robotForward(void);  //前进
void robotBackAway(void); //后退

void turnRight(void);  //右转
void turnLeft(void);  //左转

void moveLeft(void);  //左移
void moveRight(void);  //右移
#endif



