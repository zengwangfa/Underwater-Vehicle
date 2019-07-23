#ifndef _FLASH_H_
#define _FLASH_H_

#define FLIGHT_PARAMETER_TABLE_NUM  60
#define PID_USE_NUM  	8

#include "DataType.h"

extern float Adjust1,Adjust2;

typedef struct
{
		uint8 No_0xFF[PID_USE_NUM];
}PID_Parameter_Flag;

typedef enum  // _e 表示枚举
{
		VEHICLE_MODE_e,              //工作模式  rov/auv
		DEBUG_TOOL_e,                //Debug工具 ano/vcan
		
		ROBOTIC_ARM_MAX_VALUE_e,    //机械臂开启最大值  
		ROBOTIC_ARM_MIN_VALUE_e,   	//机械臂关闭值
		ROBOTIC_ARM_MED_VALUE_e, 		//机械臂当前值
		ROBOTIC_ARM_SPEED_e,   			//机械臂转动速度
	
		YUNTAI_MAX_VALUE_e,    	 //云台向上最大当前值
		YUNTAI_MIN_VALUE_e,	 		 //云台向下当前值	
		YUNTAI_MED_VALUE_e,   	 //云台中值
		YUNTAI_SPEED_e,  				 //云台转动速度

		COMPASS_OFFSET_ANGLE_e,    //指南针补偿角度
	
	/*
				TIM_SetCompare1(TIM1,power_test.rightUp);     //右上	 E9	
				TIM_SetCompare2(TIM1,power_test.leftDown);    //左下	 E11
				TIM_SetCompare3(TIM1,power_test.leftUp); 	    //左上   E13
				TIM_SetCompare4(TIM1,power_test.rightDown);   //右下   E14
			
				TIM_SetCompare1(TIM4,power_test.leftMiddle);  //左中   D12
				TIM_SetCompare2(TIM4,power_test.rightMiddle); //右中   D13
	*/                        
																//【推进器方向】
		PROPELLER_RIGHT_UP_DIR_e,   // 右上
		PROPELLER_LEFT_DOWN_DIR_e,  //左下
		PROPELLER_LEFT_UP_DIR_e,    //左上
		PROPELLER_RIGHT_DOWN_DIR_e, //右下
		PROPELLER_LEFT_MED_DIR_e,   //左中
		PROPELLER_RIGHT_MED_DIR_e,  //右中
	
		PROPELLER_POWER_e,  //推进器动力系数
	
		DEPTH_SENSOR_TYPE_e,  //深度传感器 类型
	
		BATTERY_CAPACITY_e, //电池电量参数 3s/4s/6s
		
		DIRECTION_UP_P1_e,			//
		DIRECTION_UP_P2_e,
		DIRECTION_DOWN_P1_e,
		DIRECTION_DOWN_P2_e,
		DIRECTION_LEFT_P1_e,
		DIRECTION_RIGHT_P1_e,
		
	/******************************************/
		PARAMEMER_MAX_NUMBER_e,    //最大参数值
	
}NORMAL_PARAMETER_TABLE;


typedef enum //FLASH 地址address枚举
{
		PID1_PARAMETER_KP,
		PID1_PARAMETER_KI,
		PID1_PARAMETER_KD,
		PID2_PARAMETER_KP,
		PID2_PARAMETER_KI,
		PID2_PARAMETER_KD,
		PID3_PARAMETER_KP,
		PID3_PARAMETER_KI,
		PID3_PARAMETER_KD,
		PID4_PARAMETER_KP,
		PID4_PARAMETER_KI,
		PID4_PARAMETER_KD,
		PID5_PARAMETER_KP,
		PID5_PARAMETER_KI,
		PID5_PARAMETER_KD,
		PID6_PARAMETER_KP,
		PID6_PARAMETER_KI,
		PID6_PARAMETER_KD,
		PID7_PARAMETER_KP,
		PID7_PARAMETER_KI,
		PID7_PARAMETER_KD,
		PID8_PARAMETER_KP,
		PID8_PARAMETER_KI,
		PID8_PARAMETER_KD,   //地址枚举
		
		
		PID9_PARAMETER_KP,
		PID9_PARAMETER_KI,
		PID9_PARAMETER_KD,   //地址枚举
		
				
		PID10_PARAMETER_KP,
		PID10_PARAMETER_KI,
		PID10_PARAMETER_KD,   //地址枚举
	

}IMPORTANT_PARAMETER_TABLE;

void Parameter_SelfCheck(uint32 *RealParameter,uint32 *TempParameter);
void Normal_Parameter_SelfCheck_With_Flash(void); //Flash参数自检 若为-1 或 0 则为 非正常数据 
/* FLASH 更新 普通值 */
void Flash_Update(void);

int Normal_Parameter_Init_With_Flash(void);
															 
void Save_PID_Parameter(void);			
															 
#endif



