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

/* ------------------------【数据结构体定义】---------------------------------*/
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
		float UP_P1;		//左标志位前进系数
		float UP_P2;		//右标志位前进系数
		float DOWN_P1;		//左标志位后退系数
		float DOWN_P2;		//有标志位后退系数
		float LEFT_P;		//左标志系数
		float RIGHT_P;		//右标志系数
}Direction_Type;//方向系数

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
		uint8 Type;
		float Depth;  	   //深度
		float Temperature; //水温
		float PessureValue; 		 //压力值
		float Init_PessureValue; //初始化采集到得压力值
}Depth_Sensor_Type;

typedef struct 
{
		uint8 Percent; //电量百分比
		float Current; //电流
		float Voltage; //电压
		float Capacity;//电池容量

}PowerSource_Type;


/***********************传感器数据 句柄******************************/
typedef  struct{
	
		CPU_Type CPU;    //CPU【温度】【使用率】
		JY901_Type JY901;//【欧拉角】【速度】
		Depth_Sensor_Type DepthSensor;//水【温度】【深度】
 		PowerSource_Type PowerSource; //电源
}Sensor_Type;


typedef enum {
		System_NORMAL_STATUS = 1,//正常模式
		System_DEBUG_STATUS = 2, //调试模式
		System_ERROR_STATUS,
}VehicleStatus_Enum;  //枚举系统状态

typedef enum
{
		DirectionUp = 1,
		DirectionDown ,
		DirectionLeft ,
		DirectionRight ,
	
		DirectionMode_MAX,
		
}DirectionMode_Eunm;//枚举方向系数模式



/* ------------------------【重要定义】---------------------------------*/

#define LED_EVENT 			(1 << 0)  //LED事件标志位
#define KEY_EVENT 			(1 << 1)  //KEY事件标志位
#define BUZZ_EVENT 			(1 << 2)  //BUZZER事件标志位
#define OLED_EVENT 			(1 << 3)  //OLED事件标志位
#define GYRO_EVENT 			(1 << 4)  //Gyroscope事件标志位
#define ADC_EVENT 			(1 << 5)  //ADC事件标志位
#define PWM_EVENT 			(1 << 6)  //PWM事件标志位
#define CAM_EVENT 			(1 << 7)  //Camera事件标志位
#define MS5837_EVENT 	  (1 << 8)  //Sensor事件标志位

#define PI 3.141592f  //大写标明其为常量
#define Rad2Deg(Rad) (Rad * 180.0f / PI) //弧度制转角度值
#define Deg2Rad(Deg) (Deg * PI / 180.0f) //角度值转弧度制

/* --------------【电池 参数】-----------------*/

#define STANDARD_VOLTAGE 3.7f			 //锂电池标准电压
#define FULL_VOLTAGE     4.2f		   //锂电池满电压

/* ----------【航行器 总推进器数量】-----------*/

#define FOUR_AXIS  0// ROV标志
#define SIX_AXIS   1// AUV标志

/* -----------【解锁、锁定 标志】--------------*/

#define UNLOCK    1   //全局解锁【启动】  宏定义
#define LOCK      2   //全局锁  【停止】

/* -----------【深度传感器类型 标志】-----------*/

#define MS5837    0   //深度传感器：MS5837
#define SPL1301   1   //深度传感器：SPL1301

/* ---------------【推进器 参数】--------------*/

#define PropellerPower_Med  1500
#define PropellerPower_Min  1000
#define PropellerPower_Max  2000


/* 最重要定义 直接从设定好的模式(Flash中读取)*/
extern volatile uint8 VehicleMode;   //【FOUR_AXIS】 or 【SIX_AXIS】

extern Sensor_Type Sensor;



#endif



