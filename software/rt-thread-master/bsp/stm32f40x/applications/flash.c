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
#include "gyroscope.h"
//FLASH起始地址   W25Q128 16M 的容量分为 256 个块（Block）
#define Nor_FLASH_ADDRESS    (0x0000) 	//W25Q128 FLASH的 普通起始地址   【第个扇区】

#define IMP_FLASH_ADDRESS    (0x1000) 	//W25Q128 FLASH的 重要参数起始地址 【第个扇区】

extern char *VehicleModeName[2];


PID_Parameter_Flag  PID_Parameter_Read_Flag;

uint32 Normal_Parameter[PARAMEMER_MAX_NUMBER_e]={0};

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
		uint8 i = 0;
		for(i = 0;i < PARAMEMER_MAX_NUMBER_e;i++ ){
				ef_port_read(Nor_FLASH_ADDRESS+4*i,(Normal_Parameter+i),4);		 //ef_set_env
		}
		Normal_Parameter_SelfCheck_With_Flash();//Flash参数自检 若为 0 则为 非正常数据，则不传递给真实数据 

		log_i("Flash_Read()");
		return 0;
}
INIT_APP_EXPORT(Normal_Parameter_Init_With_Flash); //先将此句注释，msh />输入“Flash_Update”，再打开此句 再下载


void Normal_Parameter_SelfCheck_With_Flash(void) //Flash参数自检 若为 0 则为 非正常数据 
{
		Parameter_SelfCheck( (uint32 *)&VehicleMode,&Normal_Parameter[VEHICLE_MODE_e] );//航行器模式  rov/auv
		Parameter_SelfCheck( (uint32 *)&debug_tool, &Normal_Parameter[DEBUG_TOOL_e] );   //debug工具   vcan/ano
		
		Parameter_SelfCheck( (uint32 *)&RoboticArm.MaxValue,&Normal_Parameter[ROBOTIC_ARM_MAX_VALUE_e] );//机械臂参数
		Parameter_SelfCheck( (uint32 *)&RoboticArm.MinValue,&Normal_Parameter[ROBOTIC_ARM_MIN_VALUE_e] );
    Parameter_SelfCheck( (uint32 *)&RoboticArm.MedValue,&Normal_Parameter[ROBOTIC_ARM_MED_VALUE_e] );
		Parameter_SelfCheck( (uint32 *)&RoboticArm.Speed   ,&Normal_Parameter[ROBOTIC_ARM_SPEED_e] );	
	
		Parameter_SelfCheck( (uint32 *)&YunTai.MaxValue,&Normal_Parameter[YUNTAI_MAX_VALUE_e] );//云台参数
		Parameter_SelfCheck( (uint32 *)&YunTai.MinValue,&Normal_Parameter[YUNTAI_MIN_VALUE_e] );	
    Parameter_SelfCheck( (uint32 *)&YunTai.MedValue,&Normal_Parameter[YUNTAI_MED_VALUE_e] );  
		Parameter_SelfCheck( (uint32 *)&YunTai.Speed   ,&Normal_Parameter[YUNTAI_SPEED_e] );	
	
		Parameter_SelfCheck( (uint32 *)&Compass_Offset_Angle,&Normal_Parameter[COMPASS_OFFSET_ANGLE_e] );//指南针补偿角度

}
/*
void test_env(void) {
    uint32_t i_boot_times = NULL;
    char *c_old_boot_times, c_new_boot_times[11] = {0};

    // get the boot count number from Env 
    c_old_boot_times = ef_get_env("boot_times");
    RT_ASSERT(c_old_boot_times);
    i_boot_times = atol(c_old_boot_times);//atoi
    // boot count +1 
    i_boot_times ++;
    rt_kprintf("The system now boot %d times\n", i_boot_times);
    // interger to string 
    sprintf(c_new_boot_times,"%ld", i_boot_times);
    // set and store the boot count number to Env 
    ef_set_env("boot_times", c_new_boot_times);
    ef_save_env();
}
*/
/* FLASH 更新 普通参数 */
void Flash_Update(void)
{
		ef_port_erase(Nor_FLASH_ADDRESS,4);	//【普通参数FLASH】先擦后写  擦除的为一个扇区4096 Byte 
//------------------------------------------------------------------------------------------//
		ef_port_write(Nor_FLASH_ADDRESS+4*VEHICLE_MODE_e,(uint32 *)(&VehicleMode),4);		
		ef_port_write(Nor_FLASH_ADDRESS+4*DEBUG_TOOL_e,(uint32 *)(&debug_tool),4);		  

		ef_port_write(Nor_FLASH_ADDRESS+4*ROBOTIC_ARM_MAX_VALUE_e,(uint32 *)&RoboticArm.MaxValue,4);		// 地址
		ef_port_write(Nor_FLASH_ADDRESS+4*ROBOTIC_ARM_MIN_VALUE_e,(uint32 *)&RoboticArm.MinValue,4); // 地址
		ef_port_write(Nor_FLASH_ADDRESS+4*ROBOTIC_ARM_MED_VALUE_e,(uint32 *)&RoboticArm.MedValue,4); // 地址
		ef_port_write(Nor_FLASH_ADDRESS+4*ROBOTIC_ARM_SPEED_e,(uint32 *)&RoboticArm.Speed,4); // 地址
	
	
		ef_port_write(Nor_FLASH_ADDRESS+4*YUNTAI_MAX_VALUE_e,(uint32 *)&YunTai.MaxValue,4);		// 地址
		ef_port_write(Nor_FLASH_ADDRESS+4*YUNTAI_MIN_VALUE_e,(uint32 *)&YunTai.MinValue,4); // 地址
		ef_port_write(Nor_FLASH_ADDRESS+4*YUNTAI_MED_VALUE_e,(uint32 *)&YunTai.MedValue,4); 		// 地址  云台中值
		ef_port_write(Nor_FLASH_ADDRESS+4*YUNTAI_SPEED_e,(uint32 *)&YunTai.Speed,4); // 地址

		ef_port_write(Nor_FLASH_ADDRESS+4*COMPASS_OFFSET_ANGLE_e,(uint32 *)&Compass_Offset_Angle,4); // 地址

}	
MSH_CMD_EXPORT(Flash_Update,Flash Update);

/* list 相关重要参数 */
void list_value(void)
{

		log_i	("variable  name          value");
    log_i("----------------------   ---------");
		log_i("VehicleMode               %s",VehicleModeName[VehicleMode]);
		log_i("debug_tool                %s",debug_tool_name[debug_tool]);
	  log_i("----------------------   ---------");
	  log_i("RoboticArm_MaxValue       %d",RoboticArm.MaxValue);
	  log_i("RoboticArm_MinValue       %d",RoboticArm.MinValue);
		log_i("RoboticArm_CurrentValue   %d",RoboticArm.CurrentValue);
		log_i("RoboticArm_Speed          %d",RoboticArm.Speed);
	  log_i("----------------------   ---------");
	  log_i("YunTai_MaxValue           %d",YunTai.MaxValue);
	  log_i("YunTai_MinValue           %d",YunTai.MinValue);
		log_i("YunTai_MedValue           %d",YunTai.MedValue);
		log_i("YunTai_CurrentValue       %d",YunTai.CurrentValue);
		log_i("YunTai_Speed              %d",YunTai.Speed);
	  log_i("----------------------   ---------");
	  log_i("Propeller_Max             %d",PropellerParameter.PowerMax);
	  log_i("Propeller_Min             %d",PropellerParameter.PowerMin);
		log_i("Propeller_Med             %d",PropellerParameter.PowerMed);
		log_i("----------------------   ---------");
		log_i("Compass_Offset_Angle      %d",Compass_Offset_Angle);
    rt_kprintf("\n");
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



