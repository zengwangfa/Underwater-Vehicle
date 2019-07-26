#ifndef __PROPELLERCONTROL_H
#define __PROPELLERCONTROL_H

#include "DataType.h"
#include "rc_data.h"

void AUV_Depth_Control(Rocker_Type *rc);

void ROV_Depth_Control(Rocker_Type *rc);

void ROV_Rotate_Control(Rocker_Type *rc);

void robot_upDown(float depth); 
void Horizontal_Propeller_Power_Clear(void);//水平方向推力清零
void Propller_stop(void);  //推进器停转

void robotForward(void);  //前进
void robotBackAway(void); //后退

void turnRight(uint16 power); //右转
void turnLeft(uint16 power);  //左转

void Propeller_Output(void);/* 推进器的真实输出 */

extern float Expect_Depth;
#endif



