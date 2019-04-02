#define LOG_TAG    "debug"

#include "init.h"
#include <string.h>
#include "PID.h"
#include "debug.h"
#include "flash.h"
#include "drv_ano.h"
#include "filter.h"
#include "drv_cpu_temp.h"
#include <rtdevice.h>
#include <elog.h>
#include "drv_MS5837.h"
/*---------------------- Constant / Macro Definitions -----------------------*/		



/*----------------------- Variable Declarations. -----------------------------*/

extern rt_device_t debug_uart_device;	
extern u8 debug_startup_flag;
extern float  volatge;
enum 
{
		DEBUG_NULL,
		PC_VCAN,
		PC_ANO,
		//不发送
		
}PC_TOOL;//上位机工具

char *debug_tool_name[3]={"NULL","VCAN","ANO"};

u8 debug_tool = PC_ANO; //山外 / 匿名上位机 调试标志位
volatile u32 debug_count = 0;

/*-----------------------Debug Thread Begin-----------------------------*/

void debug_send_thread_entry(void* parameter)
{
		while(1)
		{
				rt_thread_mdelay(1);
				if(1 == debug_startup_flag)//当debug_uart初始化完毕后 才进行上位机通信
				{
						switch(debug_tool)//选择上位机
						{
								case PC_VCAN: Vcan_Send_Data();break;
								case PC_ANO :	ANO_SEND_StateMachine();break;
								default :break;
						}
				}		
		}
}



int Debug_thread_init(void)
{
	  rt_thread_t debug_send_tid;
		/*创建动态线程*/
    debug_send_tid = rt_thread_create("debug",			 //线程名称
                    debug_send_thread_entry,									 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    1024,										 //线程栈大小，单位是字节【byte】
                    30,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (debug_send_tid != RT_NULL){
				rt_thread_startup(debug_send_tid);
		}
		return 0;
}
INIT_APP_EXPORT(Debug_thread_init);

/*-----------------------Debug Thread End-----------------------------*/




/* VCAN山外上位机调试 BEGIN */
void Vcan_Send_Cmd(void *wareaddr, unsigned int waresize)
{
		#define CMD_WARE     3
    u8 cmdf[2] = {CMD_WARE, ~CMD_WARE};    //串口调试 使用的前命令
    u8 cmdr[2] = {~CMD_WARE, CMD_WARE};    //串口调试 使用的后命令

    rt_device_write(debug_uart_device, 0,cmdf, 2);    //先发送前命令
    rt_device_write(debug_uart_device, 0,(u8 *)wareaddr, waresize);    //发送数据
    rt_device_write(debug_uart_device, 0,cmdr, 2);    //发送后命令
}


void Vcan_Send_Data(void)
{   
		float temp = 0.0f;
	
		static float list[8]= {0};
		temp =get_cpu_temp();
	
		list[0] = JY901.Euler.Roll; 	//横滚角 Roll 
		list[1] = JY901.Euler.Pitch;  //俯仰角 Pitch
		list[2] = JY901.Euler.Yaw; 	  //偏航角 Yaw
		list[3] = temp;    //CPU温度
		list[4] = KalmanFilter(&temp);//卡尔曼滤波后的温度
		list[5] = MS_TEMP;//get_vol();
		list[6] = MS5837_Pressure;	//KalmanFilter(&vol)
		list[7] = 0;	//camera_center;
		
		Vcan_Send_Cmd(list,sizeof(list));
}
/* VCAN山外上位机调试  END */




/* debug 开启或者关闭 */
static int debug(int argc, char **argv)
{
    int result = 0;

    if (argc != 2){
				log_e("Proper Usage: debug on /off");//用法:设置上位机工具
				result = -RT_ERROR; 
				goto _exit;  
    }
		if( !strcmp(argv[1],"on") ){ //设置为 山外上位机 strcmp 检验两边相等 返回0
				debug_startup_flag = 1;
				log_v("debug open\r\n");
		}
		else if( !strcmp(argv[1],"off") ){ //设置为 山外上位机 strcmp 检验两边相等 返回0
				debug_startup_flag = 0;
				log_v("debug off\r\n");
		}
		else {
				log_e("Proper Usage: debug on /off");//用法:设置上位机工具
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
				log_e("Proper Usage: debug_by vcan / ano / null");//用法:设置上位机工具
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
				log_v("Debug Tool:DEBUG_NULL\r\n");
		}
		else {
				log_e("Proper Usage: debug_by vcan / ano / null");//用法:设置上位机工具
				goto _exit;
		}
		Flash_Update();
_exit:
    return result;
}
MSH_CMD_EXPORT(set_debug_tool,debug_by vcan / ano / null);

