#define LOG_TAG       "flash"

#include "init.h"

#include <spi_flash.h>
#include <spi_flash_sfud.h>
#include <easyflash.h>


extern  servo_t servo_motor;



/* FLASH 开机时读取 所有值*/
int Flash_Read(void)
{
		ef_port_read(FLASH_INIT_ADDRESS,&servo_motor.open_value,4);		// 地址0
		ef_port_read(FLASH_INIT_ADDRESS+4,&servo_motor.close_value,4);		// 地址4

		log_i("Flash_Read()");
		return 0;
	
}INIT_APP_EXPORT(Flash_Read);

/* FLASH 更新 所有值*/
void Flash_Update(void)
{
		ef_port_erase(FLASH_INIT_ADDRESS,4);	//先擦后写  擦除的为一个扇区4096 Byte
		ef_port_write(FLASH_INIT_ADDRESS,&servo_motor.open_value,4);		//从0个地址处写入数据
		ef_port_write(FLASH_INIT_ADDRESS+4,&servo_motor.close_value,4);	//从4个地址处写入数据
	
}














