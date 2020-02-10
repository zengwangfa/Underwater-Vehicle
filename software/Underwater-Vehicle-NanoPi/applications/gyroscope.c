/*
 * @Description: ����ģ��
 * @Author: chenxi
 * @Date: 2020-02-10 12:15:34
 * @LastEditTime : 2020-02-10 17:03:57
 * @LastEditors  : chenxi
 */

#define LOG_TAG "gyro"

#include "../easylogger/inc/elog.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "gyroscope.h"

#include <wiringPi.h>
#include <wiringSerial.h>

extern Sensor_Type Sensor;
static int JY901_fd = 0;

/*---------------------- Constant / Macro Definitions -----------------------*/

#define JY901_PACKET_LENGTH 11 //���ݰ�����

/*----------------------- Variable Declarations -----------------------------*/
struct STime stcTime;
struct SAcc stcAcc;
struct SGyro stcGyro;
struct SAngle stcAngle;
struct SMag stcMag;
struct SDStatus stcDStatus;
struct SPress stcPress;
struct SLonLat stcLonLat;
struct SGPSV stcGPSV;
struct SQ stcQ;

uint8 gyroscope_save_array[5] = {0xFF, 0xAA, 0x00, 0x00, 0x00};	//0x00-���ñ���  0x01-�ָ��������ò�����
uint8 gyroscope_package_array[5] = {0xFF, 0xAA, 0x02, 0x1F, 0x00}; //���ûش������ݰ���0x1F 0x00 Ϊ <ʱ��> <���ٶ�> <���ٶ�> <�Ƕ�> <�ų�>��
uint8 gyroscope_rate_array[5] = {0xFF, 0xAA, 0x03, 0x06, 0x00};	//�������� 0x05-5Hz  0x06-10Hz(Ĭ��)  0x07-20Hz
uint8 gyroscope_led_array[5] = {0xFF, 0xAA, 0x1B, 0x00, 0x00};	 //�����ڶ�λ 0x00-����LED  0x01-�ر�LED
uint8 gyroscope_baud_array[5] = {0xFF, 0xAA, 0x04, 0x02, 0x00};	//0x06 - 115200
short Compass_Offset_Angle = 0;									   //ָ���벹���Ƕ�   �����ܵ����Ӵų����ţ���Ҫ��һ�������Ƕ�  -360 ~ +360

/*----------------------- Function Implement --------------------------------*/

int JY901_Init(void)
{
	if ((JY901_fd = serialOpen("/dev/ttyS2", 115200)) < 0)
	{
		log_e("Unable to open serial device: %s", strerror(errno));
		return -1;
	}
	log_i("[%s %s] [%s: %s: %d]", __DATE__, __TIME__, __FILE__, __func__, __LINE__);
	log_d("JY901_fd:%d", JY901_fd);

	if (JY901_fd < 0)
	{
		log_e("JY901 init failed");
	}
	return JY901_fd;
}

// CopeSerialDataΪ����2�жϵ��ú���������ÿ�յ�һ�����ݣ�����һ�����������
void CopeSerial2Data(uint8 Data)
{
	static uint8 RxBuffer[20] = {0}; // ���ݰ�
	static uint8 RxCheck = 0;		 // βУ����
	static uint8 RxCount = 0;		 // ���ռ���
	static uint8 i = 0;				 // ���ռ���

	RxBuffer[RxCount++] = Data; // ���յ������ݴ��뻺������

	if (RxBuffer[0] != 0x55)
	{				 // ����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
		RxCount = 0; // ��ջ�����
		return;
	}
	if (RxCount < JY901_PACKET_LENGTH)
	{
		return;
	} // ���ݲ���11�����򷵻�

	/*********** ֻ�н�����11���ֽ����� �Ż�������³��� ************/
	for (i = 0; i < 10; i++)
	{
		RxCheck += RxBuffer[i]; //У��λ�ۼ�
	}

	if (RxCheck == RxBuffer[JY901_PACKET_LENGTH - 1]) // �ж����ݰ�У���Ƿ���ȷ
	{
		// �ж��������������ݣ�Ȼ���俽������Ӧ�Ľṹ���У���Щ���ݰ���Ҫͨ����λ���򿪶�Ӧ������󣬲��ܽ��յ�������ݰ�������
		switch (RxBuffer[1])
		{
		case 0x50:
			memcpy(&stcTime, &RxBuffer[2], 8);
			break;
		case 0x51:
			memcpy(&stcAcc, &RxBuffer[2], 8);
			break;
		case 0x52:
			memcpy(&stcGyro, &RxBuffer[2], 8);
			break;
		case 0x53:
			memcpy(&stcAngle, &RxBuffer[2], 8);
			break;
		case 0x54:
			memcpy(&stcMag, &RxBuffer[2], 8);
			break;
		case 0x55:
			memcpy(&stcDStatus, &RxBuffer[2], 8);
			break;
		case 0x56:
			memcpy(&stcPress, &RxBuffer[2], 8);
			break;
		case 0x57:
			memcpy(&stcLonLat, &RxBuffer[2], 8);
			break;
		case 0x58:
			memcpy(&stcGPSV, &RxBuffer[2], 8);
			break;
		case 0x59:
			memcpy(&stcQ, &RxBuffer[2], 8);
			break;
		}
		RxCount = 0; // ��ջ�����
		RxCheck = 0; // У��λ����

		JY901_Convert(&Sensor.JY901); // JY901����ת��
	}
	else // ��������
	{
		RxCount = 0; // ��ջ�����
		RxCheck = 0; // У��λ����
		return;
	}
}

// Sensor.JY901 ����ת��
void JY901_Convert(JY901_Type *pArr)
{
	pArr->Acc.x = (float)stcAcc.a[0] / 2048; // 32768*16
	pArr->Acc.y = (float)stcAcc.a[1] / 2048;
	pArr->Acc.z = (float)stcAcc.a[2] / 2048;

	pArr->Gyro.x = (float)stcGyro.w[0] / 2048 * 125; // 32768*2000
	pArr->Gyro.y = (float)stcGyro.w[1] / 2048 * 125;
	pArr->Gyro.z = (float)stcGyro.w[2] / 2048 * 125;

	pArr->Euler.Roll = (float)stcAngle.angle[0] / 8192 * 45; // 32768*180;
	pArr->Euler.Pitch = (float)stcAngle.angle[1] / 8192 * 45;
	pArr->Euler.Yaw = (float)stcAngle.angle[2] / 8192 * 45; // Ϊ�˴ų�Ư�ƿ��Բ���

	// ƫ�ƽǶ� ���ڵ�ָ�� ����ʱ�ĽǶ�(-360 ~ +360 )
	if (Compass_Offset_Angle != 0) // ���δ���ò����Ƕȣ��򲻽��нǶȲ���������Ϊ ���Ƕȡ�
	{
		pArr->Euler.Yaw -= Compass_Offset_Angle; // ��ȥ�����Ƕ�
		if (pArr->Euler.Yaw < -180)
		{
			pArr->Euler.Yaw += 360; // �Ƕȷ��򲹳�
		}
		if (pArr->Euler.Yaw > 180)
		{
			pArr->Euler.Yaw -= 360; // �Ƕȷ��򲹳�
		}
	}

	pArr->Mag.x = stcMag.h[0];
	pArr->Mag.y = stcMag.h[1];
	pArr->Mag.z = stcMag.h[2];

	pArr->Temperature = (float)stcAcc.T / 100;
}

// ���Ի�ȡ���ٶ�
/*
void get_speed(float *acc, float *speed)
{
	static uint8 time_count = 0;
	static float res = 0;
	time_count++;
	res += fabs(*acc);
	if (time_count >= 5)
	{
		*speed = res;
		time_count = 0;
		res = 0;
	}
}

// ���Ի�ȡx���ٶ�
void get_zspeed(void)
{
	static uint8 time_count = 0;
	static float res = 0;
	time_count++;
	res += (Sensor.JY901.Acc.z - 1);
	if (time_count >= 5)
	{
		Sensor.JY901.Speed.z = res;
		time_count = 0;
		res = 0;
	}
}

//����е�ۡ���� �޸� �ٶ�ֵ
static int set_compass_offset_angle(int argc, char **argv)
{
	int result = 0;
	if (argc != 2)
	{
		log_e("Error! Proper Usage: RoboticArm_Speed <0~100>");
		result = -RT_ERROR;
		goto _exit;
	}
	if (atoi(argv[1]) <= 360 && atoi(argv[1]) >= -360)
	{
		Compass_Offset_Angle = atoi(argv[1]);
		Flash_Update();
		log_i("Write_Successed! Compass_Offset_Angle:  %d", Compass_Offset_Angle);
	}
	else
	{
		log_e("Error! The value is out of range!");
	}
_exit:
	return result;
}

// Getʱ��  time
void print_time(void)
{
	//���ݴ����string�ͣ���ΪRT-Thread rt_kprintf()�����޷���������ͣ�����ֽ����ݴ����String�ͷ���.
	char str[50];
	sprintf(str, "Time:20%d-%d-%d %d:%d:%.3f", stcTime.ucYear, stcTime.ucMonth, stcTime.ucDay, stcTime.ucHour, stcTime.ucMinute, (float)stcTime.ucSecond + (float)stcTime.usMiliSecond / 1000);
	log_i(str);
}
MSH_CMD_EXPORT(print_time, print time[a]);

// ���� ����ģ�� ��������
void gyroscope_save(void)
{
	rt_device_write(gyro_uart_device, 0, gyroscope_save_array, 5); // ������ٶ�У׼
	log_i("Sensor.JY901 Save successed!");
}
MSH_CMD_EXPORT(gyroscope_save, gyroscope_save);

// ����ģ��  ��λ
void gyroscope_reset(void)
{
	gyroscope_save_array[3] = 0x01;
	rt_device_write(gyro_uart_device, 0, gyroscope_save_array, 5); // ����
	log_i("Sensor.JY901 Reset!");
}
MSH_CMD_EXPORT(gyroscope_reset, gyroscope reset);

// ���� ����ģ�� ���ݰ�
void gyroscope_package_open(void)
{
	gyroscope_save_array[3] = 0x00;
	rt_device_write(gyro_uart_device, 0, gyroscope_package_array, 5); // ON package �����ش����ݰ�
	rt_device_write(gyro_uart_device, 0, gyroscope_save_array, 5);	// SAVE
	log_i("Open successed! Sensor.JY901: 1.Time  2.Acc  3.Gyro  4.Angle  5.Mag OPEN!");
}
MSH_CMD_EXPORT(gyroscope_package_open, gyroscope package open);
* /

	// ���� ����ģ�� LED
	static int gyroscope_led(int argc, char **argv)
{
	int result = 0;
	if (argc != 2)
	{
		log_e("Proper Usage: gyroscope_led on/off\n");
		result = -RT_ERROR;
		goto _exit;
	}

	if (!strcmp(argv[1], "on"))
	{
		gyroscope_led_array[3] = 0x00;
		log_i("gyroscope_led on\n");
	}
	else if (!strncmp(argv[1], "off", 3))
	{
		gyroscope_led_array[3] = 0x01;
		log_i("gyroscope_led off\n");
	}
	else
	{
		log_e("Error! Proper Usage: gyroscope_led on/off\n");
		goto _exit;
	}
	rt_device_write(gyro_uart_device, 0, gyroscope_led_array, 5); // ON LED
	rt_thread_mdelay(100);
	rt_device_write(gyro_uart_device, 0, gyroscope_save_array, 5); // ����

_exit:
	return result;
}
MSH_CMD_EXPORT(gyroscope_led, gyroscope_led on / off);

// ���� ����ģ�� ������Ϊ9600
void gyroscope_baud_9600(void)
{
	gyroscope_baud_array[3] = 0x02;
	rt_device_write(gyro_uart_device, 0, gyroscope_baud_array, 5); // ON LED
	rt_device_write(gyro_uart_device, 0, gyroscope_save_array, 5); // ����
	log_i("Sensor.JY901 baud:9600 ");
}
MSH_CMD_EXPORT(gyroscope_baud_9600, Modify Sensor.JY901 baud rate);

// ���� ����ģ�� ������Ϊ9600

void gyroscope_baud_115200(void)
{
	gyroscope_baud_array[3] = 0x06;
	rt_device_write(gyro_uart_device, 0, gyroscope_baud_array, 5); // 115200
	rt_device_write(gyro_uart_device, 0, gyroscope_save_array, 5); // ����
	log_i("Sensor.JY901 baud:115200 ");
}
MSH_CMD_EXPORT(gyroscope_baud_115200, Modify Sensor.JY901 baud rate);

// ���� ����ģ�� ���ٶ�У׼
void gyroscope_Acc_calibration_enter(void)
{
	uint8 Acc_calibration_enter[5] = {0xFF, 0xAA, 0x01, 0x01, 0x00};
	rt_device_write(gyro_uart_device, 0, Acc_calibration_enter, 5); // ON LED
	log_i("Acc_calibrationing... ");
	rt_thread_mdelay(500);
	log_i("calibration OK, Next -> [gyroscope_save]");
}
MSH_CMD_EXPORT(gyroscope_Acc_calibration_enter, gyroscope_Acc_calibration_enter);

// ���� ����ģ�� �ų� У׼
void gyroscope_Mag_calibration_enter(void)
{
	uint8 Mag_calibration_enter[5] = {0xFF, 0xAA, 0x01, 0x02, 0x00};
	rt_device_write(gyro_uart_device, 0, Mag_calibration_enter, 5); // ����ų�У׼
	log_i("Mag_calibrationing... ");
	rt_thread_mdelay(2000);
	log_i("After completing the rotation of the three axes... ");
	log_i("Nest -> [gyroscope_Mag_calibration_exit] ");
}
MSH_CMD_EXPORT(gyroscope_Mag_calibration_enter, gyroscope_Mag_calibration_enter);

// �˳� ����ģ�� �ų�У׼
void gyroscope_Mag_calibration_exit(void)
{
	uint8 Mag_calibration_exit[5] = {0xFF, 0xAA, 0x01, 0x00, 0x00};
	rt_device_write(gyro_uart_device, 0, Mag_calibration_exit, 5); // �˳��ų�У׼
	rt_thread_mdelay(100);
	gyroscope_save(); // ��������
	log_i("Mag_calibration OK & Saved! ");
}
MSH_CMD_EXPORT(gyroscope_Mag_calibration_exit, gyroscope_Mag_calibration_exit);
*/