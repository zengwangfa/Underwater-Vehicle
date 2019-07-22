/*
 * Return_Data.c
 *
 *  Created on: 2019年4月20日
 *      Author: zengwangfa
 *      Notes:  返回数据包 Return Data
 */

#include "Return_Data.h"
#include "sensor.h"
#include <rtthread.h>
#include "uart.h"
#include "gyroscope.h"

uint8 Return_Data[22] = {0};
uint8 device_hint_flag;		//设备提示字符


extern struct SAngle 	stcAngle;

void return_computer_thread_entry(void* parameter)
{
		static uint8 begin_buff[3] = {0xAA,0x55,0x00};
		
		rt_thread_mdelay(5000);//等待5s 设备初始化完成后开始发送
		
		while(uart_startup_flag){ //当debug_uart初始化完毕后 才进行上位机通信
			
				Convert_Return_Computer_Data(&Sensor); //转换返回上位机的数据
				Send_Buffer_Agreement(begin_buff,Return_Data,22); //发送数据包协议
				rt_thread_mdelay(100);
		}
}

int return_computer_thread_init(void)
{
    rt_thread_t return_computer_tid;
		/*创建动态线程*/
    return_computer_tid = rt_thread_create("return_data",//线程名称
													return_computer_thread_entry,				 //线程入口函数【entry】
													RT_NULL,							   //线程入口函数参数【parameter】
													2048,										 //线程栈大小，单位是字节【byte】
													15,										 	 //线程优先级【priority】
													10);										 //线程的时间片大小【tick】= 100ms

    if (return_computer_tid != RT_NULL){

				rt_thread_startup(return_computer_tid);
		}
		return 0;
}
INIT_APP_EXPORT(return_computer_thread_init);




/**
  * @brief  get_decimal(得到浮点型数据头两位小数的100倍)
  * @param  浮点型数据 data
  * @retval 头两位小数的100倍
  * @notice 
  */
uint8 get_decimal(float data){ //得到浮点型 的1位小数位

		return (uint8)((float)(data - (int)data)*100);
}

/**
  * @brief  Convert_Return_Computer_Data(转换返回上位机的数据包)
  * @param  None
  * @retval None
  * @notice 
  */
void Convert_Return_Computer_Data(Sensor_Type *sensor) //返回上位机数据 转换
{
		static short temp_Roll = 0; //暂存数据
		static short temp_Pitch = 0;
		static short temp_Yaw = 0;
	
		temp_Roll  = (short)((sensor->JY901.Euler.Roll+180) *100);  //数据转换:将角度数据转为正值并放大100倍
		temp_Pitch = (short)((sensor->JY901.Euler.Pitch+180)*100);
		temp_Yaw   = (short)((sensor->JY901.Euler.Yaw+180)*100);
	
		Return_Data[0] = sensor->PowerSource.Voltage; //整数倍
		Return_Data[1] = get_decimal(sensor->PowerSource.Voltage);//小数的100倍
	
		Return_Data[2] = (int)sensor->CPU.Temperature; //整数倍
		Return_Data[3] = get_decimal(sensor->CPU.Temperature) ;//小数的100倍
	
		Return_Data[4] = (int)sensor->DepthSensor.Temperature; //整数倍
		Return_Data[5] = get_decimal(sensor->DepthSensor.Temperature);//小数的100倍	
	
		Return_Data[6] = (int)(sensor->DepthSensor.Depth) >> 16; //高8位
		Return_Data[7] = (int)(sensor->DepthSensor.Depth) >> 8 ;//中8位
		Return_Data[8] = (int)(sensor->DepthSensor.Depth) ; //低8位
	
	
		Return_Data[9]  = temp_Yaw>> 8 ; // Yaw 高8位
		Return_Data[10] = (uint8)temp_Yaw; //低8位
	
		Return_Data[11] = temp_Pitch >> 8;// Pitch 高8位
		Return_Data[12] = (uint8)temp_Pitch;//低8位
	
		Return_Data[13] = temp_Roll >> 8; // Roll 高8位
		Return_Data[14] = (uint8)temp_Roll; //低8位

		Return_Data[15] = (uint8)sensor->JY901.Speed.x;//x轴航速
		Return_Data[16] = 0x02;//device_hint_flag;  //设备提示字符
}

/**
  * @brief  Calculate_Check_Byte(计算得到最后校验位)
  * @param  起始包头*begin_buff、数据包*buff、数据包长度len
  * @retval None
  * @notice 
  */
uint8 Calculate_Check_Byte(uint8 *begin_buff,uint8 *buff,uint8 len)
{
		uint8 Check_Byte = 0;
		uint8 i = 0;
		for(i = 0;i < 3;i++){
				Check_Byte += begin_buff[i];
		}
		for(i = 0;i < len;i++){
			 	Check_Byte += buff[i];
		}
		return Check_Byte;
}

/**
	* @brief  Send_Buffer_Agreement(发送数据包的协议)
  * @param  起始包头*begin_buff、数据包*buff、数据包长度len
  * @retval None
  * @notice 
  */
void Send_Buffer_Agreement(uint8 *begin_buff,uint8 *buff,uint8 len)
{
		uint8 Check_Byte = Calculate_Check_Byte(begin_buff ,buff ,len); //计算校验位
		
		begin_buff[2] = len; //长度位
		rt_device_write(control_uart_device,0,begin_buff, 3);    //发送包头
		rt_device_write(control_uart_device,0,buff, len);    		 //发送数据包
		rt_device_write(control_uart_device,0,&Check_Byte, 1);   //发送校验位
}





















