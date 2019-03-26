#define LOG_TAG       "flash"

#include "init.h"
#include <math.h>
#include <spi_flash.h>
#include <spi_flash_sfud.h>
#include <easyflash.h>
#include <rtdevice.h>
#include <elog.h>

#include "PID.h"
#include "debug.h"
#include "flash.h"
#include "drv_ano.h"
#include "propeller.h"
#include "PropellerControl.h"

//FLASH起始地址   W25Q128 16M 的容量分为 256 个块（Block）
#define Nor_FLASH_ADDRESS    (0x0000) 	//W25Q128 FLASH的 普通起始地址   【第一个扇区】

#define IMP_FLASH_ADDRESS    (0x1000) 	//W25Q128 FLASH的 重要参数起始地址 【第二个扇区】

extern PropellerParamter_Type PropellerParamter;
extern ServoType RoboticArm,YunTai;
extern u8 VehicleMode;
extern u8 debug_tool;
extern char *VehicleModeName[2];

typedef struct
{
		u8 No_0xFF[PID_USE_NUM];
}PID_Parameter_Flag;

PID_Parameter_Flag  PID_Parameter_Read_Flag;

u32 Normal_Parameter[PARAMEMER_MAX_NUMBER_A]={0};

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
				ef_port_read(Nor_FLASH_ADDRESS+4*i,(u32 *)(&Normal_Parameter[i]),4);		
		}
		Normal_Parameter_SelfCheck_With_Flash();//Flash参数自检 若为 0 则为 非正常数据，则不传递给真实数据 

		log_i("Flash_Read()");
		return 0;
}
INIT_APP_EXPORT(Normal_Parameter_Init_With_Flash);

void Normal_Parameter_SelfCheck_With_Flash(void) //Flash参数自检 若为 0 则为 非正常数据 
{
		Parameter_SelfCheck( (u32 *)&VehicleMode,Normal_Parameter[VEHICLE_MODE_A] );
		Parameter_SelfCheck( (u32 *)&debug_tool,Normal_Parameter[DEBUG_TOOL_A] );
		
		Parameter_SelfCheck( (u32 *)&RoboticArm.OpenValue,Normal_Parameter[ROBOTIC_ARM_OPEN_VALUE_A] );
		Parameter_SelfCheck( (u32 *)&RoboticArm.CloseValue,Normal_Parameter[ROBOTIC_ARM_CLOSE_VALUE_A] );
		Parameter_SelfCheck( (u32 *)&RoboticArm.CurrentValue,Normal_Parameter[ROBOTIC_ARM_CURRENT_VALUE_A] );
	
	
		Parameter_SelfCheck( (u32 *)&YunTai.OpenValue,Normal_Parameter[YUNTAI_OPEN_VALUE_A] );
		Parameter_SelfCheck( (u32 *)&YunTai.CloseValue,Normal_Parameter[YUNTAI_CLOSE_VALUE_A] );	
		Parameter_SelfCheck( (u32 *)&YunTai.CurrentValue,Normal_Parameter[YUNTAI_CURRENT_VALUE_A] );
	
		Parameter_SelfCheck( (u32 *)&PropellerParamter.PowerMed,Normal_Parameter[PropellerParamter_MED_A] );
		Parameter_SelfCheck( (u32 *)&PropellerParamter.PowerMax,Normal_Parameter[PropellerParamter_MAX_A] );
		Parameter_SelfCheck( (u32 *)&PropellerParamter.PowerMin,Normal_Parameter[PropellerParamter_MIN_A] );
}
/* FLASH 更新 所有值*/
void Flash_Update(void)
{
		ef_port_erase(Nor_FLASH_ADDRESS,4);	//【普通参数FLASH】先擦后写  擦除的为一个扇区4096 Byte RT_NULL
//------------------------------------------------------------------------------------------//
		ef_port_write(Nor_FLASH_ADDRESS+4*VEHICLE_MODE_A,(u32 *)(&VehicleMode),4);		
		ef_port_write(Nor_FLASH_ADDRESS+4*DEBUG_TOOL_A,(u32 *)(&debug_tool),4);		  
	
		ef_port_write(Nor_FLASH_ADDRESS+4*ROBOTIC_ARM_OPEN_VALUE_A,&RoboticArm.OpenValue,4);		// 地址
		ef_port_write(Nor_FLASH_ADDRESS+4*ROBOTIC_ARM_CLOSE_VALUE_A,&RoboticArm.CloseValue,4); // 地址
		ef_port_write(Nor_FLASH_ADDRESS+4*ROBOTIC_ARM_CURRENT_VALUE_A,&RoboticArm.CurrentValue,4); // 地址
	
		ef_port_write(Nor_FLASH_ADDRESS+4*YUNTAI_OPEN_VALUE_A,&YunTai.OpenValue,4);		// 地址
		ef_port_write(Nor_FLASH_ADDRESS+4*YUNTAI_CLOSE_VALUE_A,&YunTai.CloseValue,4); // 地址
		ef_port_write(Nor_FLASH_ADDRESS+4*YUNTAI_CURRENT_VALUE_A,&YunTai.CurrentValue,4); // 地址
	
}


/* list 相关重要参数 */
void list_value(void)
{

		Normal_Paramter_Init_With_Flash();
		log_i	("variable  name          value");
    log_i("----------------------   ---------");
		log_i("VehicleMode               %s",VehicleModeName[VehicleMode]);
	  log_i("RoboticArm_OpenValue      %d",RoboticArm.OpenValue);
	  log_i("RoboticArm_CloseValue     %d",RoboticArm.CloseValue);
		log_i("RoboticArm_CurrentValue   %d",RoboticArm.CurrentValue);
	  log_i("YunTai_OpenValue          %d",YunTai.OpenValue);
	  log_i("YunTai_CloseValue         %d",YunTai.CloseValue);
		log_i("YunTai_CurrentValue       %d",YunTai.CurrentValue);
		log_i("debug_tool                %s",debug_tool_name[debug_tool]);
    rt_kprintf("                         \n");
}
MSH_CMD_EXPORT(list_value,list some important values);



void Save_PID_Parameter(void)
{
		u8 i = 0;
		
		ef_port_erase(IMP_FLASH_ADDRESS,4);	//【重要参数FLASH】先擦后写  擦除的为一个扇区4096 Byte
		for(i = 0;i < PID_USE_NUM;i++){
				ef_port_write(IMP_FLASH_ADDRESS+12*(PID1_PARAMETER_KP+i*3)+0,(u32 *)&PID_Parameter[i].p,4);//第i+1组PID
				ef_port_write(IMP_FLASH_ADDRESS+12*(PID1_PARAMETER_KP+i*3)+4,(u32 *)&PID_Parameter[i].i,4);
				ef_port_write(IMP_FLASH_ADDRESS+12*(PID1_PARAMETER_KP+i*3)+8,(u32 *)&PID_Parameter[i].d,4);
			
		}
}

int PID_Paramter_Init_With_Flash()//初始化读取PID参数
{
		u8 i = 0;
		Total_PID_Init();//初始化PID参数表
		for(i = 0;i < PID_USE_NUM;i++){
				ef_port_read(IMP_FLASH_ADDRESS+12*(PID1_PARAMETER_KP+i*3)+0,(u32 *)&PID_Parameter[i].p,4);//第i+1组PID
				ef_port_read(IMP_FLASH_ADDRESS+12*(PID1_PARAMETER_KP+i*3)+4,(u32 *)&PID_Parameter[i].i,4);
				ef_port_read(IMP_FLASH_ADDRESS+12*(PID1_PARAMETER_KP+i*3)+8,(u32 *)&PID_Parameter[i].d,4);
			
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


void Parameter_SelfCheck(u32 *RealParameter,u32 TempParameter)
{
		if( TempParameter != 0 || (int)TempParameter != -1)	//Flash内数据正常
		{
				*RealParameter = TempParameter; //Flash 数据正确则替换为真实变量
		}
}



