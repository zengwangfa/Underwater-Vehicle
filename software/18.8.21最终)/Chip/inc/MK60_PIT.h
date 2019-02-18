/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_pit.h
 * @brief      pit定时器函数库
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-23
 */


#ifndef _MK60_PIT_H_
#define _MK60_PIT_H_

//定义PIT模块号
typedef enum
{
    PIT0,
    PIT1,
    PIT2,
    PIT3,

    PIT_MAX,
} PITn;

#define     PIT_Flag_Clear(PITn)          PIT_TFLG(PITn)|=PIT_TFLG_TIF_MASK        //清中断标志（写1 清空标志位）

//ms是毫秒=0.001秒 us是微秒=0.000001秒 ns是纳秒=0.000000001秒

/*****************************以下代码用于PIT中断*************************************/

void        pit_init(PITn, uint32 cnt);                                            //初始化PITn，并设置定时时间(单位为bus时钟周期)
#define     pit_init_ms(PITn,ms)          pit_init(PITn,ms * bus_clk_khz);         //初始化PITn，并设置定时时间(单位为 ms)
#define     pit_init_us(PITn,us)          pit_init(PITn,us * bus_clk_khz/1000);    //初始化PITn，并设置定时时间(单位为 us)
#define     pit_init_ns(PITn,ns)          pit_init(PITn,ns * bus_clk_khz/1000000); //初始化PITn，并设置定时时间(单位为 ns)

/*****************************以上代码用于PIT中断*************************************/
/*****************************以下代码用于PIT延时*************************************/

//注意了，延时函数不需要初始化的，直接调用

void        pit_delay(PITn pitn, uint32 cnt);                                       //PIT延时（不需要初始化的）
#define     pit_delay_ms(PITn,ms)          pit_delay(PITn,ms * bus_clk_khz);        //PIT延时 ms
#define     pit_delay_us(PITn,us)          pit_delay(PITn,us * bus_clk_khz/1000);   //PIT延时 us
#define     pit_delay_ns(PITn,ns)          pit_delay(PITn,ns * bus_clk_khz/1000000);//PIT延时 ns

/*****************************以上代码用于PIT延时*************************************/
/*****************************以下代码用于PIT计时*************************************/

void    pit_time_start  (PITn pitn);                                                //PIT开始计时
uint32  pit_time_get    (PITn pitn);                                                //获取 PITn计时时间(超时时会关闭 定时器)（单位为 bus时钟）(若值为 0xFFFFFFFF，则表示溢出)
void    pit_time_close  (PITn pitn);                                                //关闭 PIT 计时

#define pit_time_get_ms(pitn)   (pit_time_get(pitn)/bus_clk_khz)                    //获取计时时间（单位为 ms）
#define pit_time_get_us(pitn)   (pit_time_get(pitn)/(bus_clk_khz/1000))             //获取计时时间（单位为 us）

/*****************************以上代码用于PIT计时*************************************/


#endif  //_MK60_PIT_H_
