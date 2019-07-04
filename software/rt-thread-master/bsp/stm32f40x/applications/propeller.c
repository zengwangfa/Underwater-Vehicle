/*
 * servo.c
 *
 *  Created on: 2019年2月30日
 *      Author: zengwangfa
 *      Notes:  推进器设备
 */
 #define LOG_TAG    "propeller"
 
 
#include <stdlib.h>
#include <string.h>
#include "sys.h"
#include "propeller.h"
#include <elog.h>
#include <rtthread.h>

#include "flash.h"
#include "RC_Data.h"

#define Propeller_MedValue 1500
uint8 Propeller_Init_Flag = 0;
PropellerParameter_Type PropellerParameter = {//初始化推进器参数值
		 .PowerMax = 1700,//正向最大值
		 .PowerMed = 1500,//中值
		 .PowerMin = 1300,//反向最小值【反向推力最大】
	
	   .PowerDeadband = 10	//死区值
}; 

ActionType_Enum      Posture_Flag; //机器人姿态标志位

PropellerPower_Type  PropellerPower = {0,0,0,0,0,0,0}; //推进器推力控制器
PropellerError_Type  PropellerError = {0,0,0,0,0,0}; //推进器偏差值


PropellerPower_Type power_test; //调试用的变量

void PWM_Update(PropellerPower_Type* power)
{	
		power_test.rightUp = Propeller_MedValue + power->rightUp;
		power_test.leftDown = Propeller_MedValue + power->leftDown;
		power_test.leftUp = Propeller_MedValue + power->leftUp;
		power_test.rightDown = Propeller_MedValue + power->rightDown;
		
		power_test.leftMiddle = Propeller_MedValue + power->leftMiddle;
		power_test.rightMiddle = Propeller_MedValue + power->rightMiddle;
		if(1 == Propeller_Init_Flag && 1 == ControlCmd.All_Lock){
				
				TIM_SetCompare1(TIM1,power_test.rightUp);     //右上	 E9	
				TIM_SetCompare2(TIM1,power_test.leftDown);    //左下	 E11
				TIM_SetCompare3(TIM1,power_test.leftUp); 	    //左上   E13
				TIM_SetCompare4(TIM1,power_test.rightDown);   //右下   E14
			
				TIM_SetCompare1(TIM4,power_test.leftMiddle);  //左中   D12
				TIM_SetCompare2(TIM4,power_test.rightMiddle); //右中   D13
		}

}



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
void Propeller_Init(void)//这边都需要经过限幅在给定  原先为2000->1500
{

		TIM_SetCompare1(TIM1, 2000);  		//最高转速信号   	水平推进器1号
		TIM_SetCompare2(TIM1, 2000);  		//最高转速信号    水平推进器2号
		TIM_SetCompare3(TIM1, 2000); 		  //最高转速信号    水平推进器3号
		TIM_SetCompare4(TIM1, 2000);  		//最高转速信号    水平推进器4号
	
		TIM_SetCompare1(TIM4, 2000); 	 	//最高转速信号  	垂直推进器1号
		TIM_SetCompare2(TIM4, 2000);	  //最高转速信号  	垂直推进器2号

		TIM_SetCompare3(TIM4, 1900);		//中值
		//TIM_SetCompare4(TIM4, 1700);		//向上	

		rt_thread_mdelay(2000);  //2s


		TIM_SetCompare1(TIM1, 1500);			//停转信号
		TIM_SetCompare2(TIM1, 1500);			//停转信号
		TIM_SetCompare3(TIM1, 1500);			//停转信号
		TIM_SetCompare4(TIM1, 1500);			//停转信号
	
		TIM_SetCompare1(TIM4, 1500);		  //停转信号
		TIM_SetCompare2(TIM4, 1500);		  //停转信号

		TIM_SetCompare3(TIM4, 1900);		//中值
		//TIM_SetCompare4(TIM4, 1000);		//向下
		rt_thread_mdelay(1000);  //1s
		
	  log_i("Propeller_Init()");
		Propeller_Init_Flag = 1;
}


/*【推进器】 【Stop】MSH方法 */
void Propeller_Stop(void)
{
		memset(&PropellerPower,0,sizeof(PropellerPower));

}
MSH_CMD_EXPORT(Propeller_Stop,ag: Propeller Stop);



/*【推进器】 修改 【正向最大值】MSH方法 */
static int propeller_maxvalue_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: propeller_maxvalue_set 1600");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 2000){
				PropellerParameter.PowerMax = atoi(argv[1]);
				Flash_Update();
				log_d("Current propeller max_value_set:  %d",PropellerParameter.PowerMax);
		}
		
		else {
				log_e("Error! The value is out of range!");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(propeller_maxvalue_set,ag: propeller set 1600);


/*【推进器】 修改 【正向最大值】MSH方法 */
static int propeller_medvalue_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: propeller_medvalue_set 1500");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 1500){
				PropellerParameter.PowerMed = atoi(argv[1]);
				Flash_Update();
				log_d("Current propeller med_value_set:  %d",PropellerParameter.PowerMed);
		}
		
		else {
				log_e("Error! The value is out of range!");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(propeller_medvalue_set,ag: propeller set 1500);

/*【推进器】 修改 【正向最大值】MSH方法 */
static int propeller_minvalue_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: propeller_minvalue_set 1600");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 1500){
				PropellerParameter.PowerMin = atoi(argv[1]);
				Flash_Update();
				log_d("Current propeller min_value_set:  %d",PropellerParameter.PowerMin);
		}
		
		else {
				log_e("Error! The value is out of range!");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(propeller_minvalue_set,ag: propeller set 1200);

