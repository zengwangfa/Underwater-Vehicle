#include "init.h"
#include <string.h>

/*----------------------- Variable Declarations -----------------------------*/
extern  int sprintf(char *buf, const char *format, ...);

char str[100];

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

/*----------------------- Function Implement --------------------------------*/

//CopeSerialData为串口2中断调用函数，串口每收到一个数据，调用一次这个函数。
void CopeSerial2Data(unsigned char Data)
{
		static unsigned char RxBuffer[250];
		static unsigned char RxCnt = 0;	

	
		RxBuffer[RxCnt++]=Data;	//将收到的数据存入缓冲区中
		if (RxBuffer[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
		{
				RxCnt=0;
				return;
		}
		if (RxCnt<11) {return;}//数据不满11个，则返回
		else
		{
				switch(RxBuffer[1])//判断数据是哪种数据，然后将其拷贝到对应的结构体中，有些数据包需要通过上位机打开对应的输出后，才能接收到这个数据包的数据
				{
						case 0x50:	memcpy(&stcTime,&RxBuffer[2],8);break;//memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据结构体里面，从而实现数据的解析。
						case 0x51:	memcpy(&stcAcc,&RxBuffer[2],8);break;
						case 0x52:	memcpy(&stcGyro,&RxBuffer[2],8);break;
						case 0x53:	memcpy(&stcAngle,&RxBuffer[2],8);break;
						case 0x54:	memcpy(&stcMag,&RxBuffer[2],8);break;
						case 0x55:	memcpy(&stcDStatus,&RxBuffer[2],8);break;
						case 0x56:	memcpy(&stcPress,&RxBuffer[2],8);break;
						case 0x57:	memcpy(&stcLonLat,&RxBuffer[2],8);break;
						case 0x58:	memcpy(&stcGPSV,&RxBuffer[2],8);break;
						case 0x59:	memcpy(&stcQ,&RxBuffer[2],8);break;
				}
				RxCnt=0;//清空缓存区
		}

}

/* Get时间  time */
void get_time(void)
{
		//数据打包成string型
		sprintf(str,"Time:20%d-%d-%d %d:%d:%.3f\r\n",stcTime.ucYear,stcTime.ucMonth,stcTime.ucDay,stcTime.ucHour,stcTime.ucMinute,(float)stcTime.ucSecond+(float)stcTime.usMiliSecond/1000);
		rt_kprintf(str);
}
MSH_CMD_EXPORT(get_time,get acceleration[a]);

/* Get加速度  acceleration */
void *get_acc(void)
{
		sprintf(str,"Acc:%.3f %.3f %.3f\r\n",(float)stcAcc.a[0]/32768*16,(float)stcAcc.a[1]/32768*16,(float)stcAcc.a[2]/32768*16);
		rt_kprintf(str);
		return stcAcc.a;
}
MSH_CMD_EXPORT(get_acc,get acceleration[a]);



/* Get 输出角速度  gyroscope*/
void *get_gyro(void)
{
		sprintf(str,"Gyro:%.3f %.3f %.3f\r\n",(float)stcGyro.w[0]/32768*2000,(float)stcGyro.w[1]/32768*2000,(float)stcGyro.w[2]/32768*2000);
		rt_kprintf(str);	
		return stcGyro.w;
}
MSH_CMD_EXPORT(get_gyro, get gyroscope[w]);

/* Get 角度  angle */
void *get_angle(void)
{ 
		sprintf(str,"Angle:%.3f %.3f %.3f\r\n",(float)stcAngle.Angle[0]/32768*180,(float)stcAngle.Angle[1]/32768*180,(float)stcAngle.Angle[2]/32768*180);
		rt_kprintf(str);		
		return stcAngle.Angle;
}
MSH_CMD_EXPORT(get_angle, get angle[o]);


/* Get 磁场  magnetic */
void *get_mag(void)
{
		sprintf(str,"Mag:%d %d %d\r\n",stcMag.h[0],stcMag.h[1],stcMag.h[2]);
		rt_kprintf(str);	
		return stcMag.h;
}
MSH_CMD_EXPORT(get_mag, get magnetic[B]);

/* Get 温度  Temperature */
float get_temp(void)
{
		sprintf(str,"Temperature:%.2f C\r\n",(float)stcAcc.T/0x100);
		rt_kprintf(str);	
		return (float)stcAcc.T/0x100;
}
MSH_CMD_EXPORT(get_temp, get magnetic[T]);








