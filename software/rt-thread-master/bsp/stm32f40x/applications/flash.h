#ifndef _FLASH_H_
#define _FLASH_H_

#define FLIGHT_PARAMETER_TABLE_NUM  60

#include "sys.h"

typedef struct
{
  float Parameter_Table[FLIGHT_PARAMETER_TABLE_NUM];
}FLIGHT_PARAMETER;


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
		
		DEBUG_TOOL_A,                //Debug工具
		
		ROBOTIC_ARM_OPEN_VALUE_A,    //机械臂开启最大值  
		ROBOTIC_ARM_CLOSE_VALUE_A,   //机械臂关闭值
		ROBOTIC_ARM_CURRENT_VALUE_A, //机械臂当前值
		
		YUNTAI_OPEN_VALUE_A,     //云台向上最大当前值
		YUNTAI_CLOSE_VALUE_A,	 //云台向下当前值	
		YUNTAI_CURRENT_VALUE_A,    //云台当前值
	
		PropellerParamter_MED_A,   //推进器中值
		PropellerParamter_MAX_A,   //推进器正转最大值
		PropellerParamter_MIN_A,   //推进器反转最大值
		
		

}FLIGHT_PARAMETER_TABLE;



/* FLASH 更新 普通值 */
void Flash_Update(void);

int Nor_Paramter_Init_With_Flash(void);
															 
void Save_PID_Parameter(void);			
															 
#endif



