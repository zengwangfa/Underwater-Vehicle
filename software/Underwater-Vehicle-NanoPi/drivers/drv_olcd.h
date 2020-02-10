/*
 * @Description: olcd ��غ���
 * @Author: chenxi
 * @Date: 2020-02-10 12:15:34
 * @LastEditTime : 2020-02-10 16:56:42
 * @LastEditors  : chenxi
 */

#ifndef __DRV_OLCD_H
#define __DRV_OLCD_H

#include "..//user//DataType.h"
#include "..//applications//olcdfont.h"

#define I2C_OLCD_Device "/dev/i2c-2" // OLCD ʹ�õ� I2C
#define I2C_OLCD_Addr 0x3C           // Ĭ�� OLCD ��ַ

#define OLCD_CMD 0  //д����
#define OLCD_DATA 1 //д����
// OLCD�����ú���
void OLCD_WR_Byte(uint8 dat, uint8 cmd);
void OLCD_Display_On(void);
void OLCD_Display_Off(void);
void OLCD_Refresh_Gram(void);

int OLCD_Init(void);
void OLCD_Clear(void);

void OLCD_DrawPoint(uint8 x, uint8 y, uint8 t);
void OLCD_Fill(uint8 x1, uint8 y1, uint8 x2, uint8 y2, uint8 dot);
void OLCD_ShowChar(uint8 x, uint8 y, uint8 chr, uint8 size, uint8 mode);
void OLCD_ShowNum(uint8 x, uint8 y, uint32 num, uint8 len, uint8 size);
void OLCD_ShowString(uint8 x, uint8 y, const uint8 *p, uint8 size);
void OLCD_ShowPicture(uint8 x, uint8 y, const uint8 *p, uint8 p_w, uint8 p_h);

void OLCD_DrawBMP(uint8 x, uint8 y, uint8 chr, uint8 size, uint8 mode);

#endif
