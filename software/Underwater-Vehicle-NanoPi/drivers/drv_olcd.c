/*
 * @Description: olcd ��غ���
 * @Author: chenxi
 * @Date: 2020-02-10 12:15:34
 * @LastEditTime : 2020-02-10 17:10:28
 * @LastEditors  : chenxi
 */

#define LOG_TAG "drv_olcd"

#include "../easylogger/inc/elog.h"
#include <stdlib.h>
#include <math.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "drv_olcd.h"

static int olcd_fd; //�����ļ�������

//OLCD���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
uint8 OLCD_GRAM[128][8];

//��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;

void OLCD_WR_Byte(uint8 dat, uint8 cmd)
{
	if (cmd)
	{
		wiringPiI2CWriteReg8(olcd_fd, 0x40, dat);
	}
	else
	{
		wiringPiI2CWriteReg8(olcd_fd, 0x0, dat);
	}
}

//�����Դ浽LCD
void OLCD_Refresh_Gram(void)
{
	uint8 i, n;
	for (i = 0; i < 8; i++)
	{
		OLCD_WR_Byte(0xb0 + i, OLCD_CMD); //����ҳ��ַ��0~7��
		OLCD_WR_Byte(0x00, OLCD_CMD);	 //������ʾλ�á��е͵�ַ
		OLCD_WR_Byte(0x10, OLCD_CMD);	 //������ʾλ�á��иߵ�ַ
		for (n = 0; n < 128; n++)
			OLCD_WR_Byte(OLCD_GRAM[n][i], OLCD_DATA);
	}
}

//����OLCD��ʾ
void OLCD_Display_On(void)
{
	OLCD_WR_Byte(0X8D, OLCD_CMD); //SET DCDC����
	OLCD_WR_Byte(0X14, OLCD_CMD); //DCDC ON
	OLCD_WR_Byte(0XAF, OLCD_CMD); //DISPLAY ON`
}
//�ر�OLCD��ʾ
void OLCD_Display_Off(void)
{
	OLCD_WR_Byte(0X8D, OLCD_CMD); //SET DCDC����
	OLCD_WR_Byte(0X10, OLCD_CMD); //DCDC OFF
	OLCD_WR_Byte(0XAE, OLCD_CMD); //DISPLAY OFF
}
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
void OLCD_Clear(void)
{
	uint8 i, n;
	for (i = 0; i < 8; i++)
		for (n = 0; n < 128; n++)
			OLCD_GRAM[n][i] = 0X00;
	OLCD_Refresh_Gram(); //������ʾ
}

//����
//x:0~127
//y:0~63
//t:1 ��� 0,���
void OLCD_DrawPoint(uint8 x, uint8 y, uint8 t)
{
	uint8 pos, bx, temp = 0;
	if (x > 127 || y > 63)
		return; //������Χ��.
	pos = 7 - y / 8;
	bx = y % 8;
	temp = 1 << (7 - bx);
	if (t)
		OLCD_GRAM[x][pos] |= temp;
	else
		OLCD_GRAM[x][pos] &= ~temp;
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63
//dot:0,���;1,���
void OLCD_Fill(uint8 x1, uint8 y1, uint8 x2, uint8 y2, uint8 dot)
{
	uint8 x, y;
	for (x = x1; x <= x2; x++)
	{
		for (y = y1; y <= y2; y++)
			OLCD_DrawPoint(x, y, dot);
	}
	OLCD_Refresh_Gram(); //������ʾ
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ
//size:ѡ������ 12/16/24
void OLCD_ShowChar(uint8 x, uint8 y, uint8 chr, uint8 size, uint8 mode)
{
	uint8 temp, t, t1;
	uint8 y0 = y;
	uint8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); //�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr = chr - ' ';											  //�õ�ƫ�ƺ��ֵ
	for (t = 0; t < csize; t++)
	{
		if (size == 12)
			temp = asc2_1206[chr][t]; //����1206����
		else if (size == 16)
			temp = asc2_1608[chr][t]; //����1608����
		else if (size == 24)
			temp = asc2_2412[chr][t]; //����2412����
		else
			return; //û�е��ֿ�
		for (t1 = 0; t1 < 8; t1++)
		{
			if (temp & 0x80)
				OLCD_DrawPoint(x, y, mode);
			else
				OLCD_DrawPoint(x, y, !mode);
			temp <<= 1;
			y++;
			if ((y - y0) == size)
			{
				y = y0;
				x++;
				break;
			}
		}
	}
}

//��ʾͼƬ
//x,y:�������
//p_w:ͼƬ����λ���أ�
//p_h:ͼƬ�ߣ���λ���أ�
//*p:ͼƬ��ʼ��ַ
void OLCD_ShowPicture(uint8 x, uint8 y, const uint8 *p, uint8 p_w, uint8 p_h)
{
	uint8 temp, i, col, row;
	uint8 y0 = y;
	uint8 width = p_w;
	if (x + p_w > 128)
		width = 128 - p_w; //ʵ����ʾ���
	uint8 high = p_h;
	if (y + p_h > 64)
		high = 64 - p_h;									 //ʵ����ʾ�߶�
	uint8 exp_col_bytes = (p_h / 8 + ((p_h % 8) ? 1 : 0));   //��ʾһ�е��ֽ���
	uint8 act_col_bytes = (high / 8 + ((high % 8) ? 1 : 0)); //ʵ����ʾһ�е��ֽ���

	for (row = 0; row < width; row++) //��++
	{
		for (col = 0; col < act_col_bytes; col++) //��ʾһ��
		{
			temp = p[col + row * exp_col_bytes];
			for (i = 0; i < 8; i++)
			{
				if (temp & 0x80)
					OLCD_DrawPoint(x, y, 1);
				else
					OLCD_DrawPoint(x, y, 0);
				temp <<= 1;
				y++;
				if ((y - y0) == high)
				{
					y = y0;
					x++;
					break;
				}
			}
		}
	}
	OLCD_Refresh_Gram();
}

//��ʾ2������
//x,y :�������
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);
void OLCD_ShowNum(uint8 x, uint8 y, uint32 num, uint8 len, uint8 size)
{
	uint8 t, temp;
	uint8 enshow = 0;
	for (t = 0; t < len; t++)
	{
		temp = (uint32)(num / pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				OLCD_ShowChar(x + (size / 2) * t, y, ' ', size, 1);
				continue;
			}
			else
				enshow = 1;
		}
		OLCD_ShowChar(x + (size / 2) * t, y, temp + '0', size, 1);
	}
	OLCD_Refresh_Gram();
}

//��ʾ�ַ���
//x,y:�������
//size:�����С
//*p:�ַ�����ʼ��ַ
void OLCD_ShowString(uint8 x, uint8 y, const uint8 *p, uint8 size)
{
	while ((*p <= '~') && (*p >= ' ')) //�ж��ǲ��ǷǷ��ַ�!
	{
		if (x > (128 - (size / 2)))
		{
			x = 0;
			y += size;
		}
		if (y > (64 - size))
		{
			y = x = 0;
			OLCD_Clear();
		}
		OLCD_ShowChar(x, y, *p, size, 1);
		x += size / 2;
		p++;
	}
	OLCD_Refresh_Gram();
}

//��ʼ��SSD1306
int OLCD_Init(void)
{
	olcd_fd = wiringPiI2CSetupInterface(I2C_OLCD_Device, I2C_OLCD_Addr);
	log_i("[%s %s] [%s: %s: %d]", __DATE__, __TIME__, __FILE__, __func__, __LINE__);
	log_d("olcd_fd:%d", olcd_fd);

	if (olcd_fd < 0)
	{
		log_w("OLCD init failed");
	}

	OLCD_WR_Byte(0xAE, OLCD_CMD); //�ر���ʾ
	OLCD_WR_Byte(0xD5, OLCD_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
	OLCD_WR_Byte(80, OLCD_CMD);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	OLCD_WR_Byte(0xA8, OLCD_CMD); //��������·��
	OLCD_WR_Byte(0X3F, OLCD_CMD); //Ĭ��0X3F(1/64)
	OLCD_WR_Byte(0xD3, OLCD_CMD); //������ʾƫ��
	OLCD_WR_Byte(0X00, OLCD_CMD); //Ĭ��Ϊ0

	OLCD_WR_Byte(0x40, OLCD_CMD); //������ʾ��ʼ�� [5:0],����.

	OLCD_WR_Byte(0x8D, OLCD_CMD); //��ɱ�����
	OLCD_WR_Byte(0x14, OLCD_CMD); //bit2������/�ر�
	OLCD_WR_Byte(0x20, OLCD_CMD); //�����ڴ��ַģʽ
	OLCD_WR_Byte(0x02, OLCD_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
	OLCD_WR_Byte(0xA1, OLCD_CMD); //���ض�������,bit0:0,0->0;1,0->127;
	OLCD_WR_Byte(0xC0, OLCD_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	OLCD_WR_Byte(0xDA, OLCD_CMD); //����COMӲ����������
	OLCD_WR_Byte(0x12, OLCD_CMD); //[5:4]����

	OLCD_WR_Byte(0x81, OLCD_CMD); //�Աȶ�����
	OLCD_WR_Byte(0xEF, OLCD_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLCD_WR_Byte(0xD9, OLCD_CMD); //����Ԥ�������
	OLCD_WR_Byte(0xf1, OLCD_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLCD_WR_Byte(0xDB, OLCD_CMD); //����VCOMH ��ѹ����
	OLCD_WR_Byte(0x30, OLCD_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLCD_WR_Byte(0xA4, OLCD_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLCD_WR_Byte(0xA6, OLCD_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ
	OLCD_WR_Byte(0xAF, OLCD_CMD); //������ʾ
	OLCD_Clear();

	return olcd_fd;
}
