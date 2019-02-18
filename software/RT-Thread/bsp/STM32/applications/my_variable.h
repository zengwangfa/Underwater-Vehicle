#ifndef __MY_VARIABLE_H
#define __MY_VARIABLE_H



#define MAX_DATA_LENS 15

#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"


typedef enum {FALSE = 0, TRUE = !FALSE} bool;


typedef enum {  //枚举蜂王状态
	ZhongWang,  //=0
	WangTai,
	ShiWang,
	ChuNuWang

}FW_status;//定义蜂王状态 变量为枚举类型

typedef struct A //数据 结构体全局变量
{
  rt_uint8_t fx_number;  //蜂箱编号
  rt_uint8_t fw_status;  //蜂王状态
  rt_uint8_t pishu;      //脾数
  rt_uint16_t temp;      //温度
	rt_uint16_t weight;    //重量
  rt_uint32_t vol;       //电压
 
}fx_date;






#endif

