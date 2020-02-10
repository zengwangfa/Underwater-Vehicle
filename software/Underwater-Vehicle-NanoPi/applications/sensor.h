/*
 * @Description: 开启各个传感器线程函数、打印传感器信息
 * @Author: chenxi
 * @Date: 2020-02-09 12:30:19
 * @LastEditTime : 2020-02-10 16:50:22
 * @LastEditors  : chenxi
 */

#ifndef __SENSOR_H_
#define __SENSOR_H_

#include "../user/DataType.h"

int sensor_thread_init(void);
void print_sensor_info(void); // 打印传感器信息

extern char *Depth_Sensor_Name[3];

#endif
