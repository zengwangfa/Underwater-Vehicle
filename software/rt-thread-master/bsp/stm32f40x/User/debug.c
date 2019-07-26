/*
 * debug.c
 *
 *  Created on: 2019年2月5日
 *      Author: zengwangfa
 *      Notes:  发送数据至上位机
 */
#define LOG_TAG    "debug"

#include <rtthread.h>
#include <string.h>
#include <elog.h>
#include "PID.h"
#include "debug.h"
#include "drv_ano.h"
#include "filter.h"
#include "drv_cpu_temp.h"
#include "gyroscope.h"
#include "drv_MS5837.h"
#include "PID.h"
#include "oled.h"
/*---------------------- Constant / Macro Definitions -----------------------*/		



/*----------------------- Variable Declarations. -----------------------------*/

extern rt_device_t debug_uart_device;	
extern uint8 uart_startup_flag;
extern float  Yaw;



enum 
{
		DEBUG_NULL = 0,
		PC_VCAN,
		PC_ANO,
		//不发送
		
}PC_TOOL;//上位机工具

char *debug_tool_name[3]={"NULL","VCAN","ANO"};

uint8 debug_tool = PC_ANO; //山外 / 匿名上位机 调试标志位


/*-----------------------Debug Thread Begin-----------------------------*/
void debug_send_thread_entry(void* parameter)
{
		rt_thread_mdelay(3000);//等待 串口设备初始化完成
	
		while(uart_startup_flag)//当debug_uart初始化完毕后 才进行上位机通信
		{		
				rt_thread_mdelay(1);
				switch(debug_tool)//选择上位机
				{

						case PC_VCAN: Vcan_Send_Data();break;
						case PC_ANO :	ANO_SEND_StateMachine();break;
						default :break;
				}
				
		}
}



int Debug_thread_init(void)
{
	  rt_thread_t debug_send_tid;
		/*创建动态线程*/
    debug_send_tid = rt_thread_create("debug",			 //线程名称
                    debug_send_thread_entry, //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    2048,										 //线程栈大小，单位是字节【byte】
                    10,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (debug_send_tid != RT_NULL){
				log_i("Debug_Init()");
				rt_thread_startup(debug_send_tid);
		}
		return 0;
}
INIT_APP_EXPORT(Debug_thread_init);

/*-----------------------Debug Thread End-----------------------------*/



#define CMD_WARE    3
/* VCAN山外上位机调试 BEGIN */
void Vcan_Send_Cmd(void *wareaddr, unsigned int waresize)
{

    static uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE};    //串口调试 使用的前命令
    static uint8 cmdr[2] = {~CMD_WARE, CMD_WARE};    //串口调试 使用的后命令

    rt_device_write(debug_uart_device, 0,cmdf, 2);    //先发送前命令
    rt_device_write(debug_uart_device, 0,(uint8 *)wareaddr, waresize);    //发送数据
    rt_device_write(debug_uart_device, 0,cmdr, 2);    //发送后命令
}


void Vcan_Send_Data(void)
{   

		static short list[8]= {0};

	
		list[0] = (short)Total_Controller.Yaw_Angle_Control.Err; 	//横滚角 Roll 
		list[1] = (short)Total_Controller.Yaw_Angle_Control.Control_OutPut;  //俯仰角 Pitch
		list[2] = (short)Sensor.JY901.Euler.Yaw; 	  //偏航角 Yaw
		list[3] = (short)Yaw;    //CPU温度 temp
		list[4] = (short)0;//
		list[5] = (short)Total_Controller.High_Position_Control.Err;//MS_TEMP;//get_vol();
		list[6] = (short)Total_Controller.High_Position_Control.Control_OutPut;//MS5837_Pressure;	
		list[7] = (short)Total_Controller.High_Position_Control.Expect;	//camera_center;
		
		Vcan_Send_Cmd(list,sizeof(list));
}
/* VCAN山外上位机调试  END */




/* debug 开启或者关闭 */
static int debug(int argc, char **argv)
{
    int result = 0;

    if (argc != 2){
				log_e("Proper Usage: debug <on/off>");//用法:设置上位机工具
				result = -RT_ERROR; 
				goto _exit;  
    }
		if( !strcmp(argv[1],"on") ){ //设置为 山外上位机 strcmp 检验两边相等 返回0
				uart_startup_flag = 1;
				log_v("debug open\r\n");
		}
		else if( !strcmp(argv[1],"off") ){ //设置为 山外上位机 strcmp 检验两边相等 返回0
				uart_startup_flag = 0;
				log_v("debug off\r\n");
		}
		else {
				log_e("Proper Usage: debug <on/off>");//用法:设置上位机工具
		}

_exit:
    return result;
}
MSH_CMD_EXPORT(debug,ag: debug on);



/* debug 设置上位机工具 */
static int set_debug_tool(int argc,char **argv)
{
		int result = 0;
    if (argc != 2){
				log_e("set_dubug_tool <vcan/ano/null>");//用法:设置上位机工具
				result = -RT_ERROR;
        goto _exit;
    }

		if( !strcmp(argv[1],"vcan") ){ //设置为 山外上位机 strcmp 检验两边相等 返回0
				debug_tool = PC_VCAN;
				log_v("Debug Tool:VCAN\r\n");
		}

		else if( !strcmp(argv[1],"ano") ){ //设置为 匿名上位机
				debug_tool = PC_ANO;
				log_v("Debug Tool:ANO\r\n");
		}
		else if( !strcmp(argv[1],"null") ){ //设置为 山外上位机
				debug_tool = DEBUG_NULL;
				log_v("Debug Tool:NULL\r\n");
		}
		else {
				log_e("set_dubug_tool <vcan/ano/null>");//用法:设置上位机工具
				goto _exit;
		}
		Flash_Update();
_exit:
    return result;
}
MSH_CMD_EXPORT(set_debug_tool,set_dubug_tool <vcan|ano|null>);



void get_memory_situation(void)
{
		 static rt_uint32_t total_mem, used_mem, max_used_mem;
	   rt_memory_info(&total_mem, &used_mem, &max_used_mem);
     rt_kprintf("Total_Mem:%ld  Used_Mem:%ld  Max_Used_Mem:%ld\n",total_mem,used_mem,max_used_mem);
}
MSH_CMD_EXPORT(get_memory_situation,get memory situation);


/*  设置机器工作模式 (几轴：几自由度) */
static int set_vehicle_axis(int argc,char **argv)
{
		int result = 0;
    if (argc != 2){
				log_e("Proper Usage: set_vehicle_mode <4/6>");//用法:设置工作模式
				result = -RT_ERROR;
        return result;
    }
		
		if( !strcmp(argv[1],"4") ){ //设置为 ROV
				VehicleMode = FOUR_AXIS;
				Flash_Update();
				log_i("Current Mode:%s\r\n",VehicleModeName[VehicleMode]);
		}
		else if( !strcmp(argv[1],"6") ){ //设置为工作模式 strcmp 检验两边相等 返回0
				VehicleMode = SIX_AXIS;
				Flash_Update();
				log_i("Current Mode:%s\r\n",VehicleModeName[VehicleMode]);
		}
		else {
				log_e("Proper Usage: set_vehicle_mode <4/6>");
		}

    return result;
}
MSH_CMD_EXPORT(set_vehicle_axis,set_vehicle_mode <4/6>);


