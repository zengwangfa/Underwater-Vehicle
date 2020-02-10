/*
 * @Description: olcd 相关函数
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

static int olcd_fd; //驱动文件描述符

//OLCD的显存
//存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
uint8 OLCD_GRAM[128][8];

//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;

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

//更新显存到LCD
void OLCD_Refresh_Gram(void)
{
	uint8 i, n;
	for (i = 0; i < 8; i++)
	{
		OLCD_WR_Byte(0xb0 + i, OLCD_CMD); //设置页地址（0~7）
		OLCD_WR_Byte(0x00, OLCD_CMD);	 //设置显示位置—列低地址
		OLCD_WR_Byte(0x10, OLCD_CMD);	 //设置显示位置—列高地址
		for (n = 0; n < 128; n++)
			OLCD_WR_Byte(OLCD_GRAM[n][i], OLCD_DATA);
	}
}

//开启OLCD显示
void OLCD_Display_On(void)
{
	OLCD_WR_Byte(0X8D, OLCD_CMD); //SET DCDC命令
	OLCD_WR_Byte(0X14, OLCD_CMD); //DCDC ON
	OLCD_WR_Byte(0XAF, OLCD_CMD); //DISPLAY ON`
}
//关闭OLCD显示
void OLCD_Display_Off(void)
{
	OLCD_WR_Byte(0X8D, OLCD_CMD); //SET DCDC命令
	OLCD_WR_Byte(0X10, OLCD_CMD); //DCDC OFF
	OLCD_WR_Byte(0XAE, OLCD_CMD); //DISPLAY OFF
}
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLCD_Clear(void)
{
	uint8 i, n;
	for (i = 0; i < 8; i++)
		for (n = 0; n < 128; n++)
			OLCD_GRAM[n][i] = 0X00;
	OLCD_Refresh_Gram(); //更新显示
}

//画点
//x:0~127
//y:0~63
//t:1 填充 0,清空
void OLCD_DrawPoint(uint8 x, uint8 y, uint8 t)
{
	uint8 pos, bx, temp = 0;
	if (x > 127 || y > 63)
		return; //超出范围了.
	pos = 7 - y / 8;
	bx = y % 8;
	temp = 1 << (7 - bx);
	if (t)
		OLCD_GRAM[x][pos] |= temp;
	else
		OLCD_GRAM[x][pos] &= ~temp;
}
//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63
//dot:0,清空;1,填充
void OLCD_Fill(uint8 x1, uint8 y1, uint8 x2, uint8 y2, uint8 dot)
{
	uint8 x, y;
	for (x = x1; x <= x2; x++)
	{
		for (y = y1; y <= y2; y++)
			OLCD_DrawPoint(x, y, dot);
	}
	OLCD_Refresh_Gram(); //更新显示
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 12/16/24
void OLCD_ShowChar(uint8 x, uint8 y, uint8 chr, uint8 size, uint8 mode)
{
	uint8 temp, t, t1;
	uint8 y0 = y;
	uint8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); //得到字体一个字符对应点阵集所占的字节数
	chr = chr - ' ';											  //得到偏移后的值
	for (t = 0; t < csize; t++)
	{
		if (size == 12)
			temp = asc2_1206[chr][t]; //调用1206字体
		else if (size == 16)
			temp = asc2_1608[chr][t]; //调用1608字体
		else if (size == 24)
			temp = asc2_2412[chr][t]; //调用2412字体
		else
			return; //没有的字库
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

//显示图片
//x,y:起点坐标
//p_w:图片宽（单位像素）
//p_h:图片高（单位像素）
//*p:图片起始地址
void OLCD_ShowPicture(uint8 x, uint8 y, const uint8 *p, uint8 p_w, uint8 p_h)
{
	uint8 temp, i, col, row;
	uint8 y0 = y;
	uint8 width = p_w;
	if (x + p_w > 128)
		width = 128 - p_w; //实际显示宽度
	uint8 high = p_h;
	if (y + p_h > 64)
		high = 64 - p_h;									 //实际显示高度
	uint8 exp_col_bytes = (p_h / 8 + ((p_h % 8) ? 1 : 0));   //显示一列的字节数
	uint8 act_col_bytes = (high / 8 + ((high % 8) ? 1 : 0)); //实际显示一列的字节数

	for (row = 0; row < width; row++) //列++
	{
		for (col = 0; col < act_col_bytes; col++) //显示一列
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

//显示2个数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);
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

//显示字符串
//x,y:起点坐标
//size:字体大小
//*p:字符串起始地址
void OLCD_ShowString(uint8 x, uint8 y, const uint8 *p, uint8 size)
{
	while ((*p <= '~') && (*p >= ' ')) //判断是不是非法字符!
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

//初始化SSD1306
int OLCD_Init(void)
{
	olcd_fd = wiringPiI2CSetupInterface(I2C_OLCD_Device, I2C_OLCD_Addr);
	log_i("[%s %s] [%s: %s: %d]", __DATE__, __TIME__, __FILE__, __func__, __LINE__);
	log_d("olcd_fd:%d", olcd_fd);

	if (olcd_fd < 0)
	{
		log_w("OLCD init failed");
	}

	OLCD_WR_Byte(0xAE, OLCD_CMD); //关闭显示
	OLCD_WR_Byte(0xD5, OLCD_CMD); //设置时钟分频因子,震荡频率
	OLCD_WR_Byte(80, OLCD_CMD);   //[3:0],分频因子;[7:4],震荡频率
	OLCD_WR_Byte(0xA8, OLCD_CMD); //设置驱动路数
	OLCD_WR_Byte(0X3F, OLCD_CMD); //默认0X3F(1/64)
	OLCD_WR_Byte(0xD3, OLCD_CMD); //设置显示偏移
	OLCD_WR_Byte(0X00, OLCD_CMD); //默认为0

	OLCD_WR_Byte(0x40, OLCD_CMD); //设置显示开始行 [5:0],行数.

	OLCD_WR_Byte(0x8D, OLCD_CMD); //电荷泵设置
	OLCD_WR_Byte(0x14, OLCD_CMD); //bit2，开启/关闭
	OLCD_WR_Byte(0x20, OLCD_CMD); //设置内存地址模式
	OLCD_WR_Byte(0x02, OLCD_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLCD_WR_Byte(0xA1, OLCD_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
	OLCD_WR_Byte(0xC0, OLCD_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLCD_WR_Byte(0xDA, OLCD_CMD); //设置COM硬件引脚配置
	OLCD_WR_Byte(0x12, OLCD_CMD); //[5:4]配置

	OLCD_WR_Byte(0x81, OLCD_CMD); //对比度设置
	OLCD_WR_Byte(0xEF, OLCD_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	OLCD_WR_Byte(0xD9, OLCD_CMD); //设置预充电周期
	OLCD_WR_Byte(0xf1, OLCD_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLCD_WR_Byte(0xDB, OLCD_CMD); //设置VCOMH 电压倍率
	OLCD_WR_Byte(0x30, OLCD_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLCD_WR_Byte(0xA4, OLCD_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLCD_WR_Byte(0xA6, OLCD_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示
	OLCD_WR_Byte(0xAF, OLCD_CMD); //开启显示
	OLCD_Clear();

	return olcd_fd;
}
