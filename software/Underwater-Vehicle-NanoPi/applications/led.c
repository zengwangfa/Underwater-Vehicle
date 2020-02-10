/*
 * @Description: RGB 灯
 * @Author: chenxi
 * @Date: 2020-02-02 15:32:14
 * @LastEditTime : 2020-02-10 17:05:29
 * @LastEditors  : chenxi
 */

#define LOG_TAG "led"

#include <stdio.h>
#include <stdlib.h>

#include "led.h"
#include "../easylogger/inc/elog.h"

#include <wiringPi.h>

void LED_Init(void)
{
    log_i("[%s %s] [%s: %s: %d]", __DATE__, __TIME__, __FILE__, __func__, __LINE__);
    pinMode(LED_Red, OUTPUT);
    digitalWrite(LED_Red, HIGH);
    pinMode(LED_Green, OUTPUT);
    digitalWrite(LED_Green, HIGH);
    pinMode(LED_Blue, OUTPUT);
    digitalWrite(LED_Blue, HIGH);
}

void ErrorStatus_LED(void)
{
    LED_ON(LED_Red);
    LED_OFF(LED_Green);
    LED_OFF(LED_Blue);
}

void ALL_LED_OFF(void)
{
    log_i("ALL_LED_OFF");
    digitalWrite(LED_Red, HIGH);
    digitalWrite(LED_Green, HIGH);
    digitalWrite(LED_Blue, HIGH);
}