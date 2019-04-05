/*
 * servo.c
 *
 *  Created on: 2019年2月30日
 *      Author: zengwangfa
 *      Notes:  推进器设备
 */
#include "propeller.h"
#include <elog.h>
#include <rtthread.h>
#include "sys.h"

#define PropellerPower_Med  1500
#define PropellerPower_Min  1000
#define PropellerPower_Max  1800

PropellerParamter_Type PropellerParamter = {//初始化推进器参数值
		 .PowerMed = 1500,//中值
		 .PowerMax = 1800,//正向最大值
		 .PowerMin = 1200,//反向最小值【反向推力最大】
	   .PowerDeadband = 100	//死区值
}; 

ActionTypeEnum       Posture_Flag; //机器人姿态标志位
PropellerPower_Type  PropellerPower; //推进器推理控制器
PropellerError_Type  PropellerError; //推进器偏差值




/*******************************************
* 函 数 名：Propeller_Init
* 功    能：推进器的初始化
* 输入参数：none
* 返 回 值：none
* 注    意：初始化流程：
*           1,接线,上电，哔-哔-哔三声,表示开机正常
*           2,给电调2ms或1ms最高转速信号,哔一声
*           3,给电调1.5ms停转信号,哔一声
*           4,初始化完成，可以开始控制
********************************************/
void Propeller_Init(void)
{

	
		TIM_SetCompare1(TIM1, PropellerPower_Min);  		//最高转速信号   	水平推进器1号
		TIM_SetCompare2(TIM1, PropellerPower_Min);  		//最高转速信号    水平推进器2号
		TIM_SetCompare3(TIM1, PropellerPower_Min); 		//最高转速信号    水平推进器3号
		TIM_SetCompare4(TIM1, PropellerPower_Min);  		//最高转速信号    水平推进器4号
	
		TIM_SetCompare1(TIM4, PropellerPower_Min); 	 	//最高转速信号  	垂直推进器1号
		TIM_SetCompare2(TIM4, PropellerPower_Min);	  //最高转速信号  	垂直推进器2号
		TIM_SetCompare3(TIM4, PropellerPower_Min); 		//最高转速信号    水平推进器3号
		TIM_SetCompare4(TIM4, PropellerPower_Min);  		//最高转速信号    水平推进器4号
		rt_thread_mdelay(1000);   						 	//1s

		TIM_SetCompare1(TIM1, PropellerPower_Med);			//停转信号
		TIM_SetCompare2(TIM1, PropellerPower_Med);			//停转信号
		TIM_SetCompare3(TIM1, PropellerPower_Med);			//停转信号
		TIM_SetCompare4(TIM1, PropellerPower_Med);			//停转信号
	
		TIM_SetCompare1(TIM4, PropellerPower_Med);		//停转信号
		TIM_SetCompare2(TIM4, PropellerPower_Med);		//停转信号
		TIM_SetCompare3(TIM4, PropellerPower_Med); 		//最高转速信号    水平推进器3号
		TIM_SetCompare4(TIM4, PropellerPower_Med);  		//最高转速信号    水平推进器4号
		rt_thread_mdelay(1000);  
		
		log_v("Propeller_Init()\r\n");
}

