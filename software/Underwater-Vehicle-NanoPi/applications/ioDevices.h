/*
 * @Description: 蜂鸣器
 * @Author: chenxi
 * @Date: 2020-02-02 16:58:32
 * @LastEditTime : 2020-02-10 17:05:19
 * @LastEditors  : chenxi
 */

#ifndef __IO_DEVICES_H__
#define __IO_DEVICES_H__

#include "..//user//DataType.h"

#define KEY_PIN 24    //PA7   按键IO
#define Buzzer_PIN 12 //PC0   蜂鸣器IO

typedef struct
{
    uint8 count; // 响的次数
    uint32 time; // 响的时间长度(ms)
} Buzzer_Type;

// uint8 get_boma_value(void);
// uint8 is_wifi_connect(void);

void key_down(void);

void Buzzer_Init(void);

#define Buzzer_ON() digitalWrite(Buzzer_PIN, HIGH)
#define Buzzer_OFF() digitalWrite(Buzzer_PIN, LOW)

/* count为响的次数  length响的时间长度(ms)  */
void Buzzer_Set(Buzzer_Type *buzzer, uint8 count, uint32 time);
void Buzzer_Process(Buzzer_Type buzzer);

extern Buzzer_Type Beep;

#endif
