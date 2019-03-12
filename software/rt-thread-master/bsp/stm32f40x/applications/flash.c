#define LOG_TAG       "flash"

#include "init.h"
#include "PID.h"
#include "drv_ano.h"
#include <math.h>
#include <spi_flash.h>
#include <spi_flash_sfud.h>
#include <easyflash.h>
//FLASH起始地址 
#define WQ_FLASH_ADDRESS    (0x0000) 	//  W25Q128 FLASH的起始地址

extern  servo_t servo_motor;
extern u8 debug_tool;

typedef struct
{
  u8 No_0xFF[PID_USE_NUM];
}PID_Parameter_Flag;

PID_Parameter_Flag PID_Parameter_Read_Flag;
float Parameter_Table[FLIGHT_PARAMETER_TABLE_NUM];


/* 读取 FLASH 普通参数*/
int Nor_Paramter_Init_With_Flash(void)
{
		ef_port_read(WQ_FLASH_ADDRESS+4*SERVO_Open_Value_N,&servo_motor.open_value,4);		// 地址0
		ef_port_read(WQ_FLASH_ADDRESS+4*SERVO_Close_Value_N,&servo_motor.close_value,4); // 地址4
		ef_port_read(WQ_FLASH_ADDRESS+8*DEBUG_TOOL_N,(u32 *)(&debug_tool),4);		  // 地址8
	
		log_i("Flash_Read()");
		return 0;
	
}
INIT_APP_EXPORT(Nor_Paramter_Init_With_Flash);

/* FLASH 更新 所有值*/
void Flash_Update(void)
{
		ef_port_erase(WQ_FLASH_ADDRESS,4);	//先擦后写  擦除的为一个扇区4096 Byte
	
		ef_port_write(WQ_FLASH_ADDRESS+4*SERVO_Open_Value_N,&servo_motor.open_value,4);		// 地址0
		ef_port_write(WQ_FLASH_ADDRESS+4*SERVO_Close_Value_N,&servo_motor.close_value,4); // 地址4
		ef_port_write(WQ_FLASH_ADDRESS+8*DEBUG_TOOL_N,(u32 *)(&debug_tool),4);		  // 地址8
}


/* list 相关重要参数 */
void list_value(void)
{

		Nor_Paramter_Init_With_Flash();
		log_i	("variable name     value");
    log_i("---------------  ---------");
	  log_i("ser_OpenValue      %d",servo_motor.open_value);
	  log_i("ser_CloseValue     %d",servo_motor.close_value);
		log_i("debug_tool         %s",debug_tool_name[debug_tool]);
    rt_kprintf("                         \n");
}
MSH_CMD_EXPORT(list_value,list some important values);


void ReadFlashParameterThree(u16 Label,float *ReadData1,float *ReadData2,float *ReadData3)
{
		ef_port_read(WQ_FLASH_ADDRESS+4*Label+0,(u32 *)ReadData1,4);		// 地址0
		ef_port_read(WQ_FLASH_ADDRESS+4*Label+4,(u32 *)ReadData1,4);		// 地址0
		ef_port_read(WQ_FLASH_ADDRESS+4*Label+8,(u32 *)ReadData1,4);		// 地址0
}



void Save_PID_Parameter(void)
{
		u8 i = 0;
		
		Flash_Update(); //普通参数一同写入 以防被误擦除
		for(i = 0;i < PID_USE_NUM;i++){
				ef_port_write(WQ_FLASH_ADDRESS+12*(PID1_PARAMETER_KP+i*3)+0,(u32 *)&PID_Parameter[i].p,4);//第i+1组PID
				ef_port_write(WQ_FLASH_ADDRESS+12*(PID1_PARAMETER_KP+i*3)+4,(u32 *)&PID_Parameter[i].i,4);
				ef_port_write(WQ_FLASH_ADDRESS+12*(PID1_PARAMETER_KP+i*3)+8,(u32 *)&PID_Parameter[i].d,4);
			
		}
}

int PID_Paramter_Init_With_Flash()//初始化读取PID参数
{
		u8 i = 0;
		Total_PID_Init();//初始化PID参数表
		for(i = 0;i < PID_USE_NUM;i++){
				ef_port_read(WQ_FLASH_ADDRESS+12*(PID1_PARAMETER_KP+i*3)+0,(u32 *)&PID_Parameter[i].p,4);//第i+1组PID
				ef_port_read(WQ_FLASH_ADDRESS+12*(PID1_PARAMETER_KP+i*3)+4,(u32 *)&PID_Parameter[i].i,4);
				ef_port_read(WQ_FLASH_ADDRESS+12*(PID1_PARAMETER_KP+i*3)+8,(u32 *)&PID_Parameter[i].d,4);
			
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


