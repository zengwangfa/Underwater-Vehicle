/*
 * gyroscope.c
 *
 *  Created on: 2019年2月30日
 *      Author: zengwangfa
 *      Notes:  九轴模块读取并转换数据
 */
#define LOG_TAG    "gyro"

#include <string.h>
#include <stdio.h>
#include <rtthread.h>
#include <elog.h>
#include "gyroscope.h"
#include "drv_MS5837.h"

/*---------------------- Constant / Macro Definitions -----------------------*/

#define PACKET_LENGTH      11    //数据包长度

/*----------------------- Variable Declarations -----------------------------*/
short res[10] = {0};

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

struct JY901Type JY901 = {0}; //JY901真实值结构体


/*----------------------- Function Implement --------------------------------*/

//CopeSerialData为串口2中断调用函数，串口每收到一个数据，调用一次这个函数。
void CopeSerial2Data(u8 Data)
{
		static u8 RxBuffer[50];  //数据包
		static u8 RxCheck = 0;	  //尾校验字
		static u8 RxCount = 0;	    //接收计数
		static u8 i = 0;	   		  //接收计数
	
		RxBuffer[RxCount++] = Data;	//将收到的数据存入缓冲区中
	
		if (RxBuffer[0]!=0x55){ //数据头不对，则重新开始寻找0x55数据头
				RxCount=0;					  //清空缓存区
				return;
		}
		if (RxCount < PACKET_LENGTH) {return;}//数据不满11个，则返回
		
		/*********** 只有接收满11个字节数据 才会进入以下程序 ************/
		for(i = 0;i < 10;i++){
				RxCheck += RxBuffer[i]; //校验位累加
		}
		
		if(	RxCheck == RxBuffer[PACKET_LENGTH-1]){//判断数据包校验 是否正确
	
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

/* JY901 数据转换 */
void JY901_Convert(struct JY901Type * pArr) 
{

		pArr->Acc.x  = (float)stcAcc.a[0]/2048;   //32768*16
		pArr->Acc.y  = (float)stcAcc.a[1]/2048;
		pArr->Acc.z  = (float)stcAcc.a[2]/2048;
	
		pArr->Gyro.x = (float)stcGyro.w[0]/2048*125;  //32768*2000
		pArr->Gyro.y = (float)stcGyro.w[1]/2048*125;
		pArr->Gyro.z = (float)stcGyro.w[2]/2048*125;
	
		pArr->Euler.Roll = (float)stcAngle.angle[0]/8192*45;   //32768*180; 
		pArr->Euler.Pitch = (float)stcAngle.angle[1]/8192*45;
		pArr->Euler.Yaw = (float)stcAngle.angle[2]/8192*45;
	
		pArr->Mag.x 	= stcMag.h[0];
		pArr->Mag.y		= stcMag.h[1];
		pArr->Mag.z 	= stcMag.h[2];
	
		pArr->Temperature = (float)stcAcc.T/100;
}






void show_logo(void)
{
		rt_kprintf("      *      \n");
		rt_kprintf(" *  *   *  * \n");
		rt_kprintf("  *   e   *   \n");
		rt_kprintf(" *  *   *  * \n");
		rt_kprintf("      *      \n");	
	
}
MSH_CMD_EXPORT(show_logo,show_logo);


/* Get时间  time */
void get_time(void)
{
		//数据打包成string型       因为RT-Thread rt_kprintf()函数无法输出浮点型，因此现将数据打包成String型发出.
		char str[50];
		sprintf(str,"Time:20%d-%d-%d %d:%d:%.3f",stcTime.ucYear,stcTime.ucMonth,stcTime.ucDay,stcTime.ucHour,stcTime.ucMinute,(float)stcTime.ucSecond+(float)stcTime.usMiliSecond/1000);
		log_i(str);
}
MSH_CMD_EXPORT(get_time,get acceleration[a]);


/* Get加速度  acceleration */
void get_gyroscope(void)
{		
		char str[50];
		sprintf(str,"Acc:%.3f %.3f %.3f",  JY901.Acc.x,  JY901.Acc.y,  JY901.Acc.z);
		log_i(str);
		sprintf(str,"Gyro:%.3f %.3f %.3f", JY901.Gyro.x, JY901.Gyro.y, JY901.Gyro.z);
		log_i(str);
		sprintf(str,"Angle:%.3f %.3f %.3f",JY901.Euler.Roll,JY901.Euler.Pitch,JY901.Euler.Yaw);
		log_i(str);
		sprintf(str,"Mag:%d %d %d",				 JY901.Mag.x,  JY901.Mag.y,   JY901.Mag.z);
		log_i(str);	
	
		return;
}
MSH_CMD_EXPORT(get_gyroscope,get JY901[a]);


/* Get 温度  Temperature */
float get_temperature(void)
{
		char str[50];
		sprintf(str,"Temperature:%.2f C\r\n",JY901.Temperature);
		log_i(str);	
		return JY901.Temperature;
}
MSH_CMD_EXPORT(get_temperature, get Temperature[T]);








