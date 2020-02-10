/*
 * @Description: 获取 CPU 状况
 * @Author: chenxi
 * @Date: 2020-02-09 11:45:20
 * @LastEditTime : 2020-02-10 17:10:08
 * @LastEditors  : chenxi
 */

#define LOG_TAG "drv_cpu_status"

#include "../easylogger/inc/elog.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "drv_cpu_status.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TEMP_PATH "/sys/class/thermal/thermal_zone0/temp"
#define MAX_SIZE 32

/**
 * 获取 CPU 温度函数
 *
 * @return CPU温度值
 */
float get_cpu_temp(void)
{
    int fd;
    double temp = 0;
    char buf[MAX_SIZE];
    // 打开/sys/class/thermal/thermal_zone0/temp
    fd = open(TEMP_PATH, O_RDONLY);
    if (fd < 0)
    {
        fprintf(stderr, "failed to open thermal_zone0/temp\n");
        return -1;
    }
    // 读取内容
    if (read(fd, buf, MAX_SIZE) < 0)
    {
        fprintf(stderr, "failed to read temp\n");
        return -1;
    }
    // 转换为浮点数打印
    temp = atoi(buf) / 1000.0;
    // 关闭文件
    close(fd);

    return temp;
}

// 获取 CPU 内存情况
void get_memoccupy(MEM_OCCUPY *mem)
{
    FILE *fd;
    char buff[256];
    MEM_OCCUPY *m;
    m = mem;

    fd = fopen("/proc/meminfo", "r");

    fgets(buff, sizeof(buff), fd);
    fgets(buff, sizeof(buff), fd);
    fgets(buff, sizeof(buff), fd);
    fgets(buff, sizeof(buff), fd);
    sscanf(buff, "%s %lu %s", m->name, &m->total, m->name2);

    fgets(buff, sizeof(buff), fd); //从fd文件中读取长度为buff的字符串再存到起始地址为buff这个空间里
    sscanf(buff, "%s %lu %s", m->name, &m->free, m->name2);

    fclose(fd); //关闭文件fd
}

// 获取 cpu 使用情况
void get_cpuoccupy(CPU_OCCUPY *cpust)
{
    FILE *fd;
    char buff[256];
    CPU_OCCUPY *cpu_occupy;
    cpu_occupy = cpust;

    fd = fopen("/proc/stat", "r");
    fgets(buff, sizeof(buff), fd);

    sscanf(buff, "%s %u %u %u %u", cpu_occupy->name, &cpu_occupy->user, &cpu_occupy->nice, &cpu_occupy->system, &cpu_occupy->idle);

    fclose(fd);
}

// 获取 5s 内的 cpu 使用率
float get_cpu_usage(void)
{
    CPU_OCCUPY cpu_stat1;
    CPU_OCCUPY cpu_stat2;

    //第一次获取cpu使用情况
    get_cpuoccupy(&cpu_stat1);
    sleep(5);
    //第二次获取cpu使用情况
    get_cpuoccupy(&cpu_stat2);

    unsigned long od, nd;
    unsigned long id, sd;
    float cpu_use = 0;

    od = (unsigned long)(cpu_stat1.user + cpu_stat1.nice + cpu_stat1.system + cpu_stat1.idle); //第一次(用户+优先级+系统+空闲)的时间再赋给od
    nd = (unsigned long)(cpu_stat2.user + cpu_stat2.nice + cpu_stat2.system + cpu_stat2.idle); //第二次(用户+优先级+系统+空闲)的时间再赋给od

    id = (unsigned long)(cpu_stat2.user - cpu_stat1.user);     //用户第一次和第二次的时间之差再赋给id
    sd = (unsigned long)(cpu_stat2.system - cpu_stat1.system); //系统第一次和第二次的时间之差再赋给sd
    if ((nd - od) != 0)
        cpu_use = (float)((sd + id) * 100) / (nd - od); //((用户+系统)乖100)除(第一次和第二次的时间差)再赋给g_cpu_used
    else
        cpu_use = 0;

    return cpu_use;
}
