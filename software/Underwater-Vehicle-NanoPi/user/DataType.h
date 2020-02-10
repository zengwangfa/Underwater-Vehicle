/*
 * @Description: �������ݽṹ�����ļ�
 * @Author: chenxi
 * @Date: 2020-02-08 21:46:49
 * @LastEditTime : 2020-02-10 16:58:46
 * @LastEditors  : chenxi
 */

#ifndef __DATA_TYPE_H
#define __DATA_TYPE_H

#define int8 char
#define uint8 unsigned char

#define int16 short
#define uint16 unsigned short

#define int32 int
#define uint32 unsigned int

#define int64 long long
#define uint64 unsigned long long

#define my_abs(x) (((x) > 0) ? (x) : -(x))

/* ------------------------�����ݽṹ�嶨�塿---------------------------------*/
typedef struct
{
	float p;
	float i;
	float d;
} Vector3f_pid;

typedef struct
{
	float x;
	float y;
	float z;
} Vector3f; //3������ float��

typedef struct
{
	short x;
	short y;
	short z;
} Vector3s; //3������ short�� 16Ϊ������

typedef struct
{
	float Roll;  //x
	float Pitch; //y
	float Yaw;   //z
} Euler3f;		 //ŷ����3������ short�� 16Ϊ������

typedef struct
{
	float UP_P1;   //���־λǰ��ϵ��
	float UP_P2;   //�ұ�־λǰ��ϵ��
	float DOWN_P1; //���־λ����ϵ��
	float DOWN_P2; //�б�־λ����ϵ��
	float LEFT_P;  //���־ϵ��
	float RIGHT_P; //�ұ�־ϵ��
} Direction_Type;  //����ϵ��

typedef struct
{
	Vector3f Acc;	  //���ٶ�
	Vector3f Gyro;	 //���ٶ�
	Vector3f Speed;	//�ٶ�
	Vector3s Mag;	  //�ų�
	Euler3f Euler;	 //ŷ����
	float Temperature; //JY901�¶�
} JY901_Type;

typedef struct
{
	float Temperature; //CPU �¶�
	float Usage;	   //CPU ʹ����
} CPU_Type;

typedef struct
{
	uint8 Type;
	float Depth;			 //���
	float Temperature;		 //ˮ��
	float PessureValue;		 //ѹ��ֵ
	float Init_PessureValue; //��ʼ���ɼ�����ѹ��ֵ
} Depth_Sensor_Type;

typedef struct
{
	uint8 Percent;  //�����ٷֱ�
	float Current;  //����
	float Voltage;  //��ѹ
	float Capacity; //�������
} PowerSource_Type;

/***********************���������� ���******************************/
typedef struct
{

	CPU_Type CPU;				   //CPU���¶ȡ���ʹ���ʡ�
	JY901_Type JY901;			   //��ŷ���ǡ����ٶȡ�
	Depth_Sensor_Type DepthSensor; //ˮ���¶ȡ�����ȡ�
	PowerSource_Type PowerSource;  //��Դ
} Sensor_Type;

typedef enum
{
	System_NORMAL_STATUS = 1, //����ģʽ
	System_DEBUG_STATUS = 2,  //����ģʽ
	System_ERROR_STATUS,
} VehicleStatus_Enum; //ö��ϵͳ״̬

typedef enum
{
	DirectionUp = 1,
	DirectionDown,
	DirectionLeft,
	DirectionRight,

	DirectionMode_MAX,
} DirectionMode_Eunm; //ö�ٷ���ϵ��ģʽ

/* ------------------------����Ҫ���塿---------------------------------*/

#define LED_EVENT (1 << 0)	//LED�¼���־λ
#define KEY_EVENT (1 << 1)	//KEY�¼���־λ
#define BUZZ_EVENT (1 << 2)   //BUZZER�¼���־λ
#define OLED_EVENT (1 << 3)   //OLED�¼���־λ
#define GYRO_EVENT (1 << 4)   //Gyroscope�¼���־λ
#define ADC_EVENT (1 << 5)	//ADC�¼���־λ
#define PWM_EVENT (1 << 6)	//PWM�¼���־λ
#define CAM_EVENT (1 << 7)	//Camera�¼���־λ
#define MS5837_EVENT (1 << 8) //Sensor�¼���־λ

#define PI 3.141592f					 //��д������Ϊ����
#define Rad2Deg(Rad) (Rad * 180.0f / PI) //������ת�Ƕ�ֵ
#define Deg2Rad(Deg) (Deg * PI / 180.0f) //�Ƕ�ֵת������

/* --------------����� ������-----------------*/

#define STANDARD_VOLTAGE 3.7f //﮵�ر�׼��ѹ
#define FULL_VOLTAGE 4.2f	 //﮵������ѹ

/* ----------�������� ���ƽ���������-----------*/

#define FOUR_AXIS 0 // ROV��־
#define SIX_AXIS 1  // AUV��־

/* ---------������ģʽ ���������ԡ�------------*/

#define WORK 0  // ����ģʽ
#define DEBUG 1 // ����ģʽ

/* -----------������������ ��־��--------------*/

#define UNLOCK 1 //ȫ�ֽ�����������  �궨��
#define LOCK 2   //ȫ����  ��ֹͣ��

/* -----------����ȴ��������� ��־��-----------*/

#define MS5837 0  //��ȴ�������MS5837
#define SPL1301 1 //��ȴ�������SPL1301
#define DS_NULL 2 //����ȴ����� Depth_Sensor:null

/* ---------------���ƽ��� ������--------------*/

#define PropellerPower_Med 1500
#define PropellerPower_Min 1000
#define PropellerPower_Max 2000

/* ����Ҫ���� ֱ�Ӵ��趨�õ�ģʽ(Flash�ж�ȡ)*/
extern volatile uint8 VehicleMode; //��FOUR_AXIS�� or ��SIX_AXIS��
extern volatile uint8 WorkMode;	//��WORK��or ��DEBUG��

Sensor_Type Sensor;

#endif
