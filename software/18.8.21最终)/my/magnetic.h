#ifndef MAGNETIC_H_H
#define MAGNETIC_H_H

#include "include.h"



/*************【【【【车号】】】*************/
#define work_mode    1
/*************【【【【车号】】】*************/
/************************************/


//ADC0_SE9







//>>>>>>>>>>>>1号车<<<<<<<<<<<<<<//
#if work_mode == 1

#define M_I 6
#define M_P 40
#define add_bias 1
#define circle_line_in 45


#define critical_val   180   //直道会车判定临界值
#define AD_right ADC0_SE13
#define AD_middle ADC0_SE9
#define AD_left ADC1_SE11

#define Servo_Duty_Md 1100// for泽凯  1
#define Servo_Duty_Lf 1260// for泽凯
#define Servo_Duty_Rt 930//for泽凯


//>>>>>>>>>>>>2号车<<<<<<<<<<<<<<//
#else

#define M_I 6
#define M_P 42
#define add_bias -2             //加偏差
#define circle_line_in 44       //圆环触发行数


#define critical_val   200     //直道会车判定临界值
/**************[左] [中] [右] []**************************/


#define AD_right ADC0_SE13  
#define AD_middle ADC0_SE12
#define AD_left ADC1_SE11

#define Servo_Duty_Md 1480//1480for嘉鑫  2
#define Servo_Duty_Lf 1670//1670for嘉鑫
#define Servo_Duty_Rt 1290//1290for嘉鑫


#endif


void magnetic_information();
uint8 Bubble(uint8 *v);
void ad_collect();
void magnetic_circle();
void Insurance_Mag();

#endif