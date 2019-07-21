/*
 * gyroscope.c
 *
 *  Created on: 2019年2月30日
 *      Author: zengwangfa
 *      Notes:  九轴模块读取并转换数据 以及 内置方法
 */
#define LOG_TAG    "gyro"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <rtthread.h>
#include <elog.h>
#include <math.h>

#include "gyroscope.h"
#include "flash.h"

/*---------------------- Constant / Macro Definitions -----------------------*/

#define JY901_PACKET_LENGTH      11    //数据包长度

/*----------------------- Variable Declarations -----------------------------*/
struct STime		stcTime;
struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SAngle 	stcAngle;
struct SMag 		stcMag;
struct SDStatus stcDStatus;
struct SPress 	stcPress;
struct SLonLat 	stcLonLat;
struct SGPSV 		stcGPSV;
struct SQ       stcQ;

uint8 gyroscope_save_array[5] 	 = {0xFF,0xAA,0x00,0x00,0x00};	 //0x00-设置保存  0x01-恢复出厂设置并保存
uint8 gyroscope_package_array[5] = {0xFF,0xAA,0x02,0x1F,0x00};	 //设置回传的数据包【0x1F 0x00 为 <时间> <加速度> <角速度> <角度> <磁场>】
uint8 gyroscope_rate_array[5] 	 = {0xFF,0xAA,0x03,0x06,0x00};	 //传输速率 0x05-5Hz  0x06-10Hz(默认)  0x07-20Hz
uint8 gyroscope_led_array[5] 	   = {0xFF,0xAA,0x1B,0x00,0x00}; 	 //倒数第二位 0x00-开启LED  0x01-关闭LED   
uint8 gyroscope_baud_array[5] 	 = {0xFF,0xAA,0x04,0x02,0x00}; 	 //0x06 - 115200
short Compass_Offset_Angle = 0;  //指南针补偿角度   由于受到板子磁场干扰，需要加一个补偿角度  -360 ~ +360

extern rt_device_t gyro_uart_device;	
/*----------------------- Function Implement --------------------------------*/

//CopeSerialData为串口2中断调用函数，串口每收到一个数据，调用一次这个函数。
void CopeSerial2Data(uint8 Data)
{
		static uint8 RxBuffer[20] = {0};  //数据包
		static uint8 RxCheck = 0;	  //尾校验字
		static uint8 RxCount = 0;	    //接收计数
		static uint8 i = 0;	   		  //接收计数
	
		RxBuffer[RxCount++] = Data;	//将收到的数据存入缓冲区中
	
		if (RxBuffer[0]!=0x55){ //数据头不对，则重新开始寻找0x55数据头
				RxCount=0;					  //清空缓存区
				return;
		}
		if (RxCount < JY901_PACKET_LENGTH) {return;}//数据不满11个，则返回
		
		/*********** 只有接收满11个字节数据 才会进入以下程序 ************/
		for(i = 0;i < 10;i++){
				RxCheck += RxBuffer[i]; //校验位累加
		}
		
		if(	RxCheck == RxBuffer[JY901_PACKET_LENGTH-1]){//判断数据包校验 是否正确
	
				switch(RxBuffer[1]){		//判断数据是哪种数据，然后将其拷贝到对应的结构体中，有些数据包需要通过上位机打开对应的输出后，才能接收到这个数据包的数据
						case 0x50:	memcpy(&stcTime,&RxBuffer[2],8);	 break;//memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据结构体里面，从而实现数据的解析。
						case 0x51:	memcpy(&stcAcc,&RxBuffer[2],8);		 break;
						case 0x52:	memcpy(&stcGyro,&RxBuffer[2],8);	 break;
						case 0x53:	memcpy(&stcAngle,&RxBuffer[2],8);	 break;
						case 0x54:	memcpy(&stcMag,&RxBuffer[2],8);		 break;
						case 0x55:	memcpy(&stcDStatus,&RxBuffer[2],8);break;
						case 0x56:	memcpy(&stcPress,&RxBuffer[2],8);	 break;
						case 0x57:	memcpy(&stcLonLat,&RxBuffer[2],8); break;
						case 0x58:	memcpy(&stcGPSV,&RxBuffer[2],8);	 break;
						case 0x59:	memcpy(&stcQ,&RxBuffer[2],8);			 break;
				}
				RxCount = 0;//清空缓存区
				RxCheck = 0;//校验位清零
		}
		else{  //错误清零
				RxCount = 0;//清空缓存区
				RxCheck = 0;//校验位清零
				return;
		}
		/*********** ------------------------------------- ************/

}

/* Sensor.JY901 数据转换 */
void JY901_Convert(JY901_Type * pArr) 
{

		pArr->Acc.x  = (float)stcAcc.a[0]/2048;   //32768*16
		pArr->Acc.y  = (float)stcAcc.a[1]/2048;
		pArr->Acc.z  = (float)stcAcc.a[2]/2048;
	
		pArr->Gyro.x = (float)stcGyro.w[0]/2048*125;  //32768*2000
		pArr->Gyro.y = (float)stcGyro.w[1]/2048*125;
		pArr->Gyro.z = (float)stcGyro.w[2]/2048*125;
	
		pArr->Euler.Roll = (float)stcAngle.angle[0]/8192*45;   //32768*180; 
		pArr->Euler.Pitch = (float)stcAngle.angle[1]/8192*45;
		pArr->Euler.Yaw = (float)stcAngle.angle[2]/8192*45 - 90;//为了磁场漂移可以补偿 -90
		if(pArr->Euler.Yaw < -180){pArr->Euler.Yaw += Compass_Offset_Angle;}//磁场漂移 补偿-20 deg
		
		pArr->Mag.x  = stcMag.h[0];
		pArr->Mag.y	 = stcMag.h[1];
		pArr->Mag.z  = stcMag.h[2];
	
		pArr->Temperature = (float)stcAcc.T/100;
}

/* 粗略 获取轴速度 */
void get_speed(float *acc,float *speed)
{
		static uint8 time_count = 0;
		static float res = 0;
		time_count ++;
		res += fabs(*acc);
		if(time_count >= 5){
				*speed = res;
				time_count = 0;
				res = 0;
		}
}

/* 粗略 获取x轴速度 */
void get_zspeed(void)
{
		static uint8 time_count = 0;
		static float res = 0;
		time_count ++;
		res += (Sensor.JY901.Acc.z-1);
		if(time_count >= 5){
				Sensor.JY901.Speed.z = res;
				time_count = 0;
				res = 0;
		}
}

/*【机械臂】舵机 修改 速度值 */
static int set_compass_offset_angle(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: RoboticArm_Speed <0~100>");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 360 && atoi(argv[1]) >= -360){
				Compass_Offset_Angle = atoi(argv[1]);
				Flash_Update();
				log_i("Write_Successed! Compass_Offset_Angle:  %d",Compass_Offset_Angle);
		}
		else {
				log_e("Error! The value is out of range!");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(set_compass_offset_angle,ag: set_compass_offset_angle 360);












///* Get时间  time */
//void print_time(void)
//{
//		//数据打包成string型       因为RT-Thread rt_kprintf()函数无法输出浮点型，因此现将数据打包成String型发出.
//		char str[50];
//		sprintf(str,"Time:20%d-%d-%d %d:%d:%.3f",stcTime.ucYear,stcTime.ucMonth,stcTime.ucDay,stcTime.ucHour,stcTime.ucMinute,(float)stcTime.ucSecond+(float)stcTime.usMiliSecond/1000);
//		log_i(str);
//}
//MSH_CMD_EXPORT(print_time,print time[a]);


/* 设置 九轴模块 保存配置 */
void gyroscope_save(void)
{
			rt_device_write(gyro_uart_device, 0, gyroscope_save_array, 5);  //进入加速度校准
			log_i("Sensor.JY901 Save successed!");
}
MSH_CMD_EXPORT(gyroscope_save,gyroscope_save);



///*  九轴模块  复位 */
//void gyroscope_reset(void)
//{
//		gyroscope_save_array[3] = 0x01;
//		rt_device_write(gyro_uart_device, 0, gyroscope_save_array, 5);  //保存
//		log_i("Sensor.JY901 Reset!");
//}
//MSH_CMD_EXPORT(gyroscope_reset,gyroscope reset);


/* 开启 九轴模块 数据包 */
void gyroscope_package_open(void)
{
		gyroscope_save_array[3] = 0x00;
		rt_device_write(gyro_uart_device, 0, gyroscope_package_array, 5);   //ON package 开启回传数据包
		rt_device_write(gyro_uart_device, 0, gyroscope_save_array, 5);  //SAVE
		log_i("Open successed! Sensor.JY901: 1.Time  2.Acc  3.Gyro  4.Angle  5.Mag OPEN!");
}
MSH_CMD_EXPORT(gyroscope_package_open,gyroscope package open);


/* 开启 九轴模块 LED */
static int gyroscope_led(int argc, char **argv)
{
	  int result = 0;
    if (argc != 2){
        log_e("Proper Usage: gyroscope_led on/off\n");
				result = -RT_ERROR;
        goto _exit;
    }

		if( !strcmp(argv[1],"on") ){
				gyroscope_led_array[3] = 0x00;
				log_i("gyroscope_led on\n");
		}
		else if( !strncmp(argv[1],"off",3) ){
				gyroscope_led_array[3] = 0x01;
				log_i("gyroscope_led off\n");
		}
		else {
				log_e("Error! Proper Usage: gyroscope_led on/off\n");goto _exit;
		}
		rt_device_write(gyro_uart_device, 0, gyroscope_led_array, 5);   //ON LED
		rt_thread_mdelay(100);
		rt_device_write(gyro_uart_device, 0, gyroscope_save_array, 5);  //保存
		
_exit:
    return result;
}
MSH_CMD_EXPORT(gyroscope_led, gyroscope_led on/off);

///* 设置 九轴模块 波特率为9600 */
//void gyroscope_baud_9600(void)
//{
//		gyroscope_baud_array[3] = 0x02;
//		rt_device_write(gyro_uart_device, 0, gyroscope_baud_array, 5);   //ON LED
//		rt_device_write(gyro_uart_device, 0, gyroscope_save_array, 5);  //保存
//		log_i("Sensor.JY901 baud:9600 ");
//}
//MSH_CMD_EXPORT(gyroscope_baud_9600,Modify Sensor.JY901 baud rate);

/* 设置 九轴模块 波特率为9600 */
void gyroscope_baud_115200(void)
{
		gyroscope_baud_array[3] = 0x06;
		rt_device_write(gyro_uart_device, 0, gyroscope_baud_array, 5);  //115200
		rt_device_write(gyro_uart_device, 0, gyroscope_save_array, 5);  //保存
		log_i("Sensor.JY901 baud:115200 ");
}
MSH_CMD_EXPORT(gyroscope_baud_115200,Modify Sensor.JY901 baud rate);

///* 设置 九轴模块 加速度校准 */
//void gyroscope_Acc_calibration_enter(void)
//{
//			uint8 Acc_calibration_enter[5]={0xFF,0xAA,0x01,0x01,0x00};
//			rt_device_write(gyro_uart_device, 0, Acc_calibration_enter, 5);   //ON LED
//			log_i("Acc_calibrationing... ");
//			rt_thread_mdelay(500);
//			log_i("calibration OK, Next -> [gyroscope_save]");
//}
//MSH_CMD_EXPORT(gyroscope_Acc_calibration_enter,gyroscope_Acc_calibration_enter);

///* 设置 九轴模块 磁场 校准 */
//void gyroscope_Mag_calibration_enter(void)
//{
//			uint8 Mag_calibration_enter[5]={0xFF,0xAA,0x01,0x02,0x00};
//			rt_device_write(gyro_uart_device, 0, Mag_calibration_enter, 5);   //进入磁场校准
//			log_i("Mag_calibrationing... ");
//			rt_thread_mdelay(2000);
//			log_i("After completing the rotation of the three axes... ");
//			log_i("Nest -> [gyroscope_Mag_calibration_exit] ");

//}
//MSH_CMD_EXPORT(gyroscope_Mag_calibration_enter,gyroscope_Mag_calibration_enter);


///* 退出 九轴模块 磁场校准 */
//void gyroscope_Mag_calibration_exit(void)
//{
//			uint8 Mag_calibration_exit[5]={0xFF,0xAA,0x01,0x00,0x00};       
//			rt_device_write(gyro_uart_device, 0, Mag_calibration_exit, 5);   //退出磁场校准
//			rt_thread_mdelay(100);
//			gyroscope_save();                                           //保配置
//			log_i("Mag_calibration OK & Saved! ");
//}
//MSH_CMD_EXPORT(gyroscope_Mag_calibration_exit,gyroscope_Mag_calibration_exit);





