/*
 * flash.c
 *
 *  Created on: 2019年2月30日
 *      Author: zengwangfa
 *      Notes:  Flash读写
 */
#define LOG_TAG       "flash"


#include <math.h>
#include <easyflash.h>
#include <rtthread.h>
#include <elog.h>

#include "PID.h"
#include "debug.h"
#include "flash.h"
#include "drv_ano.h"
#include "servo.h"
#include "propeller.h"
#include "PropellerControl.h"

//FLASH起始地址   W25Q128 16M 的容量分为 256 个块（Block）
#define Nor_FLASH_ADDRESS    (0x0000) 	//W25Q128 FLASH的 普通起始地址   【第一个扇区】

#define IMP_FLASH_ADDRESS    (0x1000) 	//W25Q128 FLASH的 重要参数起始地址 【第二个扇区】

extern char *VehicleModeName[2];


PID_Parameter_Flag  PID_Parameter_Read_Flag;

uint32 Normal_Parameter[PARAMEMER_MAX_NUMBER_A]={0};

/*******************************************
* 函 数 名：Normal_Parameter_Init_With_Flash
* 功    能：读取Flash普通参数
* 输入参数：none
* 返 回 值：none
* 注    意：NORMAL_PARAMETER_TABLE 枚举表 中添加自身需要的的参数的 枚举值
						NORMAL_PARAMETER_TABLE 枚举表 中添加自身需要的的参数

********************************************/
int Normal_Parameter_Init_With_Flash(void)
{
		char i = 0;
		for(i = 0;i < PARAMEMER_MAX_NUMBER_A;i++ ){
				ef_port_read(Nor_FLASH_ADDRESS+4*i,(uint32 *)(&Normal_Parameter[i]),4);		
		}
		Normal_Parameter_SelfCheck_With_Flash();//Flash参数自检 若为 0 则为 非正常数据，则不传递给真实数据 

		log_i("Flash_Read()");
		return 0;
}
//INIT_APP_EXPORT(Normal_Parameter_Init_With_Flash);

void Normal_Parameter_SelfCheck_With_Flash(void) //Flash参数自检 若为 0 则为 非正常数据 
{
		Parameter_SelfCheck( (uint32 *)&VehicleMode,(uint32 *)&Normal_Parameter[VEHICLE_MODE_A] );
		Parameter_SelfCheck( (uint32 *)&debug_tool,(uint32 *)&Normal_Parameter[DEBUG_TOOL_A] );
		
		Parameter_SelfCheck( (uint32 *)&RoboticArm.MaxValue,(uint32 *)&Normal_Parameter[ROBOTIC_ARM_MAX_VALUE_A] );
		Parameter_SelfCheck( (uint32 *)&RoboticArm.MinValue,(uint32 *)&Normal_Parameter[ROBOTIC_ARM_MIN_VALUE_A] );
//    Parameter_SelfCheck( (uint32 *)&RoboticArm.CurrentValue,(uint32 *)&Normal_Parameter[ROBOTIC_ARM_CURRENT_VALUE_A] );
	
	
		Parameter_SelfCheck( (uint32 *)&YunTai.MaxValue,(uint32 *)&Normal_Parameter[YUNTAI_MAX_VALUE_A] );
		Parameter_SelfCheck( (uint32 *)&YunTai.MinValue,(uint32 *)&Normal_Parameter[YUNTAI_MIN_VALUE_A] );	
    Parameter_SelfCheck( (uint32 *)&YunTai.MedValue,(uint32 *)&Normal_Parameter[YUNTAI_MED_VALUE_A] );  //云台中值
	
		Parameter_SelfCheck( (uint32 *)&PropellerParamter.PowerMed,(uint32 *)&Normal_Parameter[PropellerParamter_MED_A] );
		Parameter_SelfCheck( (uint32 *)&PropellerParamter.PowerMax,(uint32 *)&Normal_Parameter[PropellerParamter_MAX_A] );
		Parameter_SelfCheck( (uint32 *)&PropellerParamter.PowerMin,(uint32 *)&Normal_Parameter[PropellerParamter_MIN_A] );
	
		Parameter_SelfCheck( (uint32 *)&RoboticArm.Speed,(uint32 *)&Normal_Parameter[ROBOTIC_ARM_SPEED_A] );
		Parameter_SelfCheck( (uint32 *)&YunTai.Speed,(uint32 *)&Normal_Parameter[YUNTAI_SPEED_A] );	

}
/* FLASH 更新 普通参数 */
void Flash_Update(void)
{
		ef_port_erase(Nor_FLASH_ADDRESS,4);	//【普通参数FLASH】先擦后写  擦除的为一个扇区4096 Byte 
//------------------------------------------------------------------------------------------//
		ef_port_write(Nor_FLASH_ADDRESS+4*VEHICLE_MODE_A,(uint32 *)(&VehicleMode),4);		
		ef_port_write(Nor_FLASH_ADDRESS+4*DEBUG_TOOL_A,(uint32 *)(&debug_tool),4);		  

		ef_port_write(Nor_FLASH_ADDRESS+4*ROBOTIC_ARM_MAX_VALUE_A,(uint32 *)&RoboticArm.MaxValue,4);		// 地址
		ef_port_write(Nor_FLASH_ADDRESS+4*ROBOTIC_ARM_MIN_VALUE_A,(uint32 *)&RoboticArm.MinValue,4); // 地址
		//ef_port_write(Nor_FLASH_ADDRESS+4*ROBOTIC_ARM_CURRENT_VALUE_A,(uint32 *)&RoboticArm.CurrentValue,4); // 地址
	
		ef_port_write(Nor_FLASH_ADDRESS+4*YUNTAI_MAX_VALUE_A,(uint32 *)&YunTai.MaxValue,4);		// 地址
		ef_port_write(Nor_FLASH_ADDRESS+4*YUNTAI_MIN_VALUE_A,(uint32 *)&YunTai.MinValue,4); // 地址
		ef_port_write(Nor_FLASH_ADDRESS+4*YUNTAI_MED_VALUE_A,(uint32 *)&YunTai.MedValue,4); 		// 地址  云台中值
	
		ef_port_write(Nor_FLASH_ADDRESS+4*PropellerParamter_MED_A,(uint32 *)&PropellerParamter.PowerMed,4);		// 地址
		ef_port_write(Nor_FLASH_ADDRESS+4*PropellerParamter_MAX_A,(uint32 *)&PropellerParamter.PowerMax,4); // 地址
		ef_port_write(Nor_FLASH_ADDRESS+4*PropellerParamter_MIN_A,(uint32 *)&PropellerParamter.PowerMin,4); // 地址
	
		ef_port_write(Nor_FLASH_ADDRESS+4*ROBOTIC_ARM_SPEED_A,(uint32 *)&RoboticArm.Speed,4); // 地址
		ef_port_write(Nor_FLASH_ADDRESS+4*YUNTAI_SPEED_A,(uint32 *)&YunTai.Speed,4); // 地址
	
}	
MSH_CMD_EXPORT(Flash_Update,Flash Update);

/* list 相关重要参数 */
void list_value(void)
{

		Normal_Parameter_Init_With_Flash();
		log_i	("variable  name          value");
    log_i("----------------------   ---------");
		log_i("VehicleMode               %s",VehicleModeName[VehicleMode]);
		log_i("debug_tool                %s",debug_tool_name[debug_tool]);
	
	  log_i("RoboticArm_MaxValue       %d",RoboticArm.MaxValue);
	  log_i("RoboticArm_MinValue       %d",RoboticArm.MinValue);
		log_i("RoboticArm_CurrentValue   %d",RoboticArm.CurrentValue);
		log_i("RoboticArm_Speed          %d",RoboticArm.Speed);
	
	  log_i("YunTai_MaxValue           %d",YunTai.MaxValue);
	  log_i("YunTai_MinValue           %d",YunTai.MinValue);
		log_i("YunTai_MedValue           %d",YunTai.MedValue);
		log_i("YunTai_Speed              %d",YunTai.Speed);
	
	  log_i("Propeller_max             %d",PropellerParamter.PowerMax);
	  log_i("Propeller_min             %d",PropellerParamter.PowerMin);
		log_i("Propeller_med             %d",PropellerParamter.PowerMed);

    rt_kprintf("                         \n");
}
MSH_CMD_EXPORT(list_value,list some important values);



void Save_PID_Parameter(void)
{
		uint8 i = 0;
		
		ef_port_erase(IMP_FLASH_ADDRESS,4);	//【重要参数FLASH】先擦后写  擦除的为一个扇区4096 Byte
		for(i = 0;i < PID_USE_NUM;i++){
				ef_port_write(IMP_FLASH_ADDRESS+12*(PID1_PARAMETER_KP+i*3)+0,(uint32 *)&PID_Parameter[i].p,4);//第i+1组PID
				ef_port_write(IMP_FLASH_ADDRESS+12*(PID1_PARAMETER_KP+i*3)+4,(uint32 *)&PID_Parameter[i].i,4);
				ef_port_write(IMP_FLASH_ADDRESS+12*(PID1_PARAMETER_KP+i*3)+8,(uint32 *)&PID_Parameter[i].d,4);
			
		}
}

int PID_Paramter_Init_With_Flash()//初始化读取PID参数
{
		uint8 i = 0;
		Total_PID_Init();//初始化PID参数表
		for(i = 0;i < PID_USE_NUM;i++){
				ef_port_read(IMP_FLASH_ADDRESS+12*(PID1_PARAMETER_KP+i*3)+0,(uint32 *)&PID_Parameter[i].p,4);//第i+1组PID
				ef_port_read(IMP_FLASH_ADDRESS+12*(PID1_PARAMETER_KP+i*3)+4,(uint32 *)&PID_Parameter[i].i,4);
				ef_port_read(IMP_FLASH_ADDRESS+12*(PID1_PARAMETER_KP+i*3)+8,(uint32 *)&PID_Parameter[i].d,4);
			
				if(isnan(PID_Parameter[i].p)==0
						&&isnan(PID_Parameter[i].i)==0
							&&isnan(PID_Parameter[i].d)==0){ //判断数据是否isnan(not a number无效数字)
						PID_Parameter_Read_Flag.No_0xFF[i]=0x01;
				 }
			
		}
		
		if(PID_Parameter_Read_Flag.No_0xFF[0]!=0x00
			 &&PID_Parameter_Read_Flag.No_0xFF[1]!=0x00
				 &&PID_Parameter_Read_Flag.No_0xFF[2]!=0x00
					 &&PID_Parameter_Read_Flag.No_0xFF[3]!=0x00
						 &&PID_Parameter_Read_Flag.No_0xFF[4]!=0x00
							 &&PID_Parameter_Read_Flag.No_0xFF[5]!=0x00
								 &&PID_Parameter_Read_Flag.No_0xFF[6]!=0x00
									 &&PID_Parameter_Read_Flag.No_0xFF[7]!=0x00)//Flash内数据正常，更新PID参数值
		{
				Total_Controller.Roll_Gyro_Control.Kp=PID_Parameter[0].p;
				Total_Controller.Roll_Gyro_Control.Ki=PID_Parameter[0].i;
				Total_Controller.Roll_Gyro_Control.Kd=PID_Parameter[0].d;
		
				Total_Controller.Pitch_Gyro_Control.Kp=PID_Parameter[1].p;
				Total_Controller.Pitch_Gyro_Control.Ki=PID_Parameter[1].i;
				Total_Controller.Pitch_Gyro_Control.Kd=PID_Parameter[1].d;
					
				Total_Controller.Yaw_Gyro_Control.Kp=PID_Parameter[2].p;
				Total_Controller.Yaw_Gyro_Control.Ki=PID_Parameter[2].i;
				Total_Controller.Yaw_Gyro_Control.Kd=PID_Parameter[2].d;
						
				Total_Controller.Roll_Angle_Control.Kp=PID_Parameter[3].p;
				Total_Controller.Roll_Angle_Control.Ki=PID_Parameter[3].i;
				Total_Controller.Roll_Angle_Control.Kd=PID_Parameter[3].d;
				
				Total_Controller.Pitch_Angle_Control.Kp=PID_Parameter[4].p;
				Total_Controller.Pitch_Angle_Control.Ki=PID_Parameter[4].i;
				Total_Controller.Pitch_Angle_Control.Kd=PID_Parameter[4].d;

				Total_Controller.Yaw_Angle_Control.Kp=PID_Parameter[5].p;
				Total_Controller.Yaw_Angle_Control.Ki=PID_Parameter[5].i;
				Total_Controller.Yaw_Angle_Control.Kd=PID_Parameter[5].d;
				
				Total_Controller.High_Speed_Control.Kp=PID_Parameter[6].p;
				Total_Controller.High_Speed_Control.Ki=PID_Parameter[6].i;
				Total_Controller.High_Speed_Control.Kd=PID_Parameter[6].d;
				
				Total_Controller.High_Position_Control.Kp=PID_Parameter[7].p;
				Total_Controller.High_Position_Control.Ki=PID_Parameter[7].i;
				Total_Controller.High_Position_Control.Kd=PID_Parameter[7].d;
				
		}
		return 0;
}
INIT_APP_EXPORT(PID_Paramter_Init_With_Flash);


void Parameter_SelfCheck(uint32 *RealParameter,uint32 *TempParameter)
{
		if(*TempParameter != 0xFFFFFFFF)	//Flash内数据正常 （不等于0xFFFFFFFF 即不为无效数据）
		{
				*RealParameter = *TempParameter; //Flash 数据正确则替换为真实变量
		}
}



