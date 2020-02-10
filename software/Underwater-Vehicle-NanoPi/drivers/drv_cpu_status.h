/*
 * @Description: 获取 CPU 状况
 * @Author: chenxi
 * @Date: 2020-02-09 11:45:20
 * @LastEditTime : 2020-02-10 16:55:18
 * @LastEditors  : chenxi
 */

#ifndef _DRV_CPU_TEMP_H_
#define _DRV_CPU_TEMP_H_

typedef struct // 定义一个cpu occupy的结构体
{
    char name[20];       // 定义一个char类型的数组名name有20个元素
    unsigned int user;   // 定义一个无符号的int类型的user
    unsigned int nice;   // 定义一个无符号的int类型的nice
    unsigned int system; // 定义一个无符号的int类型的system
    unsigned int idle;   // 定义一个无符号的int类型的idle
} CPU_OCCUPY;

typedef struct // 定义一个mem occupy的结构体
{
    char name[20]; // 定义一个char类型的数组名name有20个元素
    unsigned long total;
    char name2[20];
    unsigned long free;
} MEM_OCCUPY;

float get_cpu_temp(void);            // 获取 CPU 温度
void get_memoccupy(MEM_OCCUPY *mem); // 获取 CPU 内存情况
float get_cpu_usage(void);           // 获取 5s 内的 cpu 使用率

#endif /* _DRV_CPU_TEMP_H_ */
