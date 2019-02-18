#include "init.h"
#include <string.h>
#include "board.h"
#include <rthw.h>

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

struct JY901_t JY901; //JY901真实值结构体

/*----------------------- Function Implement --------------------------------*/

//CopeSerialData为串口2中断调用函数，串口每收到一个数据，调用一次这个函数。
void CopeSerial2Data(unsigned char Data)
{
		static unsigned char RxBuffer[250];
		static unsigned char RxCnt = 0;	

		RxBuffer[RxCnt++]=Data;	//将收到的数据存入缓冲区中
		if (RxBuffer[0]!=0x55){ //数据头不对，则重新开始寻找0x55数据头
				RxCnt=0;
				return;
		}
		if (RxCnt<11) {return;}//数据不满11个，则返回
		else{
				switch(RxBuffer[1]){//判断数据是哪种数据，然后将其拷贝到对应的结构体中，有些数据包需要通过上位机打开对应的输出后，才能接收到这个数据包的数据
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
static void JY901_Convert(void* parameter)// 定时器1超时函数  进行JY901模块数据转换
{
		static u8 i = 0;


		for(i = 0;i < 3;i++){	
				JY901.Acc[i] = (float)stcAcc.a[i]/32768*16;
				JY901.Gyro[i] = (float)stcGyro.w[i]/32768*2000;
				JY901.Angle[i] = (float)stcAngle.angle[i]/32768*180;
				JY901.Mag[i] 		= stcMag.h[i];
		}
		JY901.Temperature = (float)stcAcc.T/100;

}


int timer_init(void)
{
		/* 定时器的控制块 */
		static rt_timer_t timer1;
    /* 创建定时器1 */
    timer1 = rt_timer_create("timer1",  /* 定时器名字是 timer1 */
                        JY901_Convert, 	/* 超时时回调的处理函数 */
                        RT_NULL, 			  /* 超时函数的入口参数 */
                        5,      			  /* 定时长度，以OS Tick为单位，即5个OS Tick   --> 50MS*/  
                        RT_TIMER_FLAG_PERIODIC); /* 周期性定时器 */
    /* 启动定时器 */
    if (timer1 != RT_NULL) rt_timer_start(timer1);

    return 0;
}
INIT_APP_EXPORT(timer_init);





/* Get时间  time */
void get_time(void)
{
		//数据打包成string型       因为RT-Thread rt_kprintf()函数无法输出浮点型，因此现将数据打包成String型发出
		char str[100];
		sprintf(str,"Time:20%d-%d-%d %d:%d:%.3f\r\n",stcTime.ucYear,stcTime.ucMonth,stcTime.ucDay,stcTime.ucHour,stcTime.ucMinute,(float)stcTime.ucSecond+(float)stcTime.usMiliSecond/1000);
		rt_kprintf(str);
}
MSH_CMD_EXPORT(get_time,get acceleration[a]);

/* Get加速度  acceleration */
void *get_acc(void)
{		
		char str[100];
		sprintf(str,"Acc:%.3f %.3f %.3f\r\n",JY901.Acc[0],JY901.Acc[1],JY901.Acc[2]);
		rt_kprintf(str);
		return JY901.Acc;
}
MSH_CMD_EXPORT(get_acc,get acceleration[a]);

/* Get 输出角速度  gyroscope*/
void *get_gyro(void)
{
		char str[100];
		sprintf(str,"Gyro:%.3f %.3f %.3f\r\n",JY901.Gyro[0],JY901.Gyro[1],JY901.Gyro[2]);
		rt_kprintf(str);	
		return JY901.Gyro;
}
MSH_CMD_EXPORT(get_gyro, get gyroscope[w]);

/* Get 角度  angle */
void *get_angle(void)
{ 
		char str[100];
		sprintf(str,"Angle:%.3f %.3f %.3f\r\n",JY901.Angle[0],JY901.Angle[1],JY901.Angle[2]);
		rt_kprintf(str);		
		return JY901.Angle;
}
MSH_CMD_EXPORT(get_angle, get angle[o]);


/* Get 磁场  magnetic */
void *get_mag(void)
{
		char str[100];
		sprintf(str,"Mag:%d %d %d\r\n",JY901.Mag[0],JY901.Mag[1],JY901.Mag[2]);
		rt_kprintf(str);	
		return JY901.Mag;
}
MSH_CMD_EXPORT(get_mag, get magnetic[B]);

/* Get 温度  Temperature */
float get_temperature(void)
{
		char str[100];
		sprintf(str,"Temperature:%.2f C\r\n",JY901.Temperature);
		rt_kprintf(str);	
		return JY901.Temperature;
}
MSH_CMD_EXPORT(get_temperature, get Temperature[T]);








