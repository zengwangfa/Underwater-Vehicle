/*!
电磁融合摄像头
圆环：电磁上升沿，摄像头判断过中心圈，摄像头方向，摄像头打脚，摄像头收回打脚

//7.27  晚上
*/

#include "common.h"
#include "include.h"

extern MENU_LIST_e MENU;
extern uint8 var_max[3];
extern uint8 var_min[3];
extern uint8 imgbuff[CAMERA_SIZE];
extern uint8 img[CAMERA_H][CAMERA_W];
extern uint8 send_picture[CAMERA_SIZE];
extern uint8 sending_flag;
extern uint8 fitting_over_flag;
/*!
*  @brief      main函数
*  @since      v5.0
*  @note       测试 LED 功能是否正常
看到的效果是LED0和LED1同时亮灭闪烁
*/
void main()
{    
	
	
	
	init();
	Boma_OLED();
	while(MENU==normaliz)
	{  
		magnetic_information();
		Boma_OLED();
		if(MENU!=normaliz)
		{
			flash_erase_sector(magnetic_sector);
			flash_write(magnetic_sector, 0, var_max[0]);
			flash_write(magnetic_sector , 4, var_max[1]);
			flash_write(magnetic_sector , 8, var_max[2]);
			
			flash_write(magnetic_sector, 12, var_min[0]);
			flash_write(magnetic_sector , 16, var_min[1]);
			flash_write(magnetic_sector , 20, var_min[2]);
		}
	}
	
	var_max[0]=flash_read(magnetic_sector , 0, uint8);
	var_max[1]=flash_read(magnetic_sector , 4, uint8);
	var_max[2]=flash_read(magnetic_sector , 8, uint8);
	
	var_min[0]=flash_read(magnetic_sector , 12, uint8);
	var_min[1]=flash_read(magnetic_sector , 16, uint8);
	var_min[2]=flash_read(magnetic_sector , 20, uint8);
	
	
	//待到归一化再开正常AD采集和舵机控制
	DisableInterrupts;
	FTM_PWM_init(FTM1, FTM_CH0, 100,Servo_Duty_Md);
	enable_irq(PIT1_IRQn );
	enable_irq(PORTA_IRQn);
	enable_irq(PIT2_IRQn );
	EnableInterrupts;
	
	buzzer_bibi(1,1);
	while(1)
	{
		
		
		Boma_OLED();
		key_control();
		Check_Communication();
//		if(sending_flag==0)
//		{
//			sending_flag=1;
//			vcan_sendimg(send_picture,CAMERA_SIZE);
//		}
//		ware_test();
		
		
	}
}

