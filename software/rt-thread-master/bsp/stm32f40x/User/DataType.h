#ifndef __DATA_TYPE_H
#define __DATA_TYPE_H

#define int8 	 char
#define uint8  unsigned char
	
#define int16  short
#define uint16 unsigned short
	
#define int32  int
#define uint32 unsigned int

#define int64  long long
#define uint64 unsigned long long
	
#define my_abs(x)  (((x)>0)?(x):-(x))

typedef struct
{
		float p;
		float i;
		float d;
}Vector3f_pid;


typedef struct
{
		float x;
		float y;
		float z;
}Vector3f; //3轴向量 float型

typedef struct
{
		short x;  
		short y;
		short z;
}Vector3s;//3轴向量 short型 16为短整型



typedef struct
{
		float Roll;		 //x 
		float Pitch;   //y
		float Yaw;     //z
}Euler3f;//欧拉角3轴向量 short型 16为短整型

typedef struct 
{
		Vector3f Acc; 		//加速度
		Vector3f Gyro; 		//角速度
		Vector3f Speed;   //速度
		Vector3s Mag;     //磁场
		Euler3f Euler;		//欧拉角
		float Temperature;	//JY901温度
}JY901_Type;

typedef struct 
{
		float Temperature; //CPU 温度
		float Usage; 		   //CPU 使用率
}CPU_Type;

typedef struct 
{
		float Temperature; //水温
		uint32 PessureValue; 		 //压力值
		uint32 Init_PessureValue; //初始化采集到得压力值
}MS5837_Type;

typedef struct 
{
		float Current; //水温
		float Voltage;
}PowerSource_Type;

typedef  struct{
	
		int32 Depth;  	 //深度
		CPU_Type CPU;           //CPU【温度】【使用率】
		JY901_Type JY901;       //【欧拉角】【速度】
		MS5837_Type MS5837;     //水【温度】【深度】
 		PowerSource_Type PowerSource; //电源
}Sensor_Type;


typedef enum {
		System_NORMAL_STATUS = 1,//正常模式
		System_DEBUG_STATUS = 2, //调试模式
		System_ERROR_STATUS,
}VehicleStatus_Enum;  //枚举系统状态

extern Sensor_Type Sensor;
extern  uint8 VehicleMode;   //ROV_Mode or AUV_Mode

/************重要定义***************/


#define LED_EVENT 			(1 << 0)  //LED事件标志位
#define KEY_EVENT 			(1 << 1)  //KEY事件标志位
#define BUZZ_EVENT 			(1 << 2)  //BUZZER事件标志位
#define OLED_EVENT 			(1 << 3)  //OLED事件标志位
#define GYRO_EVENT 			(1 << 4)  //Gyroscope事件标志位
#define ADC_EVENT 			(1 << 5)  //ADC事件标志位
#define PWM_EVENT 			(1 << 6)  //PWM事件标志位
#define CAM_EVENT 			(1 << 7)  //Camera事件标志位
#define MS5837_EVENT 	  (1 << 8)  //Sensor事件标志位

#define PI 3.141592f
#define Rad2Deg(Rad) (Rad * 180.0f / PI) //弧度制转角度值
#define Deg2Rad(Deg) (Deg * PI / 180.0f) //角度值转弧度制


#define AUV_Mode  0//0
#define ROV_Mode  1//1

#define LOCK   1 //全局锁   宏定义
#define UNLOCK 2 //全军解锁 

#define PropellerPower_Med  1500
#define PropellerPower_Min  1000
#define PropellerPower_Max  2000


#endif



