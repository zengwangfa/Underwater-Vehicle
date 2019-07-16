/*
 * Control.c
 *
 *  Created on: 2019年7月15日
 *      Author: zengwangfa
 *      Notes:  深度控制
 */
 
 
 
 #include "Depth.h"
 #include <math.h>
#include "propeller.h"
#include "PropellerControl.h"
#include "RC_Data.h"
#include "drv_pwm.h"
#include "PID.h"
#include "Control.h"
 
 /*******************************************
* 函 数 名：Depth_Control
* 功    能：深度垂直运动控制
* 输入参数：摇杆结构体
* 返 回 值：none
* 注    意：none
********************************************/

void Depth_Control(Rocker_Type *rc)
{
		
		if(UNLOCK == ControlCmd.All_Lock){ //解锁
		
				if(AUV_Mode == VehicleMode){	 //AUV深度控制位数字量
						switch(ControlCmd.Vertical){//有控制数据不定深度
								case RiseUp: 
										 Expect_Depth-=3 ; 
										 if(Expect_Depth < 0) {Expect_Depth= 0;}//超过空气中的深度值，期望值不再上升
										 break;  //上升
							
								case Dive:   
											if(Total_Controller.High_Position_Control.Control_OutPut < 450){ //超过输出范围 停止累积
													Expect_Depth+=3 ;
											}
											
										 break;  //下潜
								default:break/*定深度PID*/;
						}
						ControlCmd.Vertical = 0x00;
				}
		}

			/* 垂直控制 */
		if(ROV_Mode == VehicleMode)	 //ROV由 摇杆模拟量控制
		{
				if(rc->Z > 5){
					 Expect_Depth -=( (float)rc->Z /80); 
					 if(Expect_Depth < 0) {//超过空气中的深度值，期望值不再上升
							Expect_Depth= 0;
						}
				}
				else if(rc->Z < -5){
						if(Total_Controller.High_Position_Control.Control_OutPut < 450){ //超过输出范围 停止累积
								Expect_Depth += (fabs((float)rc->Z)/80);
						}
				}
		}
	
		Depth_PID_Control(Expect_Depth,Sensor.DepthSensor.Depth);//深度控制 不受【解锁】字节控制
}




