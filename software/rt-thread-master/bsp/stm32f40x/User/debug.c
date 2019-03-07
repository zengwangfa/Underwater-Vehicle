#define LOG_TAG    "debug"

#include "init.h"
#include <string.h>

/*---------------------- Constant / Macro Definitions -----------------------*/		

#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp) + 0) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

#define VCAN_UART "uart3"

/*----------------------- Variable Declarations. -----------------------------*/
enum 
{
		PC_VCAN,
		PC_ANO	
}PC_TOOL;//上位机工具

extern rt_device_t debug_uart_device;	
extern struct JY901_t JY901; //JY901真实值结构体
extern struct SAngle 	stcAngle;

short debug_count = 0;

u8 data_to_send[50];//ANO地面站发送数据缓冲
u8 debug_tool = PC_ANO;//山外 / 匿名上位机 调试标志位




void debug_thread_entry(void* parameter)
{
	
		
		while(1)
		{

				if(debug_count != 0)
				{
						switch(debug_tool)
						{
								case PC_VCAN:ware_test();break;
								case PC_ANO:	ANO_SEND_StateMachine();break;
						}
						
						if(debug_count % 100 == 0){rt_kprintf(".");}
						debug_count --;
				}
				rt_thread_mdelay(1);
				
		}


}







int Debug_thread_init(void)
{
	  rt_thread_t debug_tid;
		/*创建动态线程*/
    debug_tid = rt_thread_create("debug",			 //线程名称
                    debug_thread_entry,									 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    1024,										 //线程栈大小，单位是字节【byte】
                    30,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (debug_tid != RT_NULL){

				rt_thread_startup(debug_tid);
		}
		return 0;
}
INIT_APP_EXPORT(Debug_thread_init);

void ANO_Data_Send_Status(void)//发送基本信息（姿态、锁定状态）
{
		u8 _cnt=0;
		vs16 _temp;
		vs32 _temp2;
		u8 sum = 0;
		u8 i;
		data_to_send[_cnt++]=0xAA;
		data_to_send[_cnt++]=0xAA;
		data_to_send[_cnt++]=0x01;
		data_to_send[_cnt++]=0;
		
		_temp = (int)(JY901.Euler.Roll*100);
		data_to_send[_cnt++]=BYTE1(_temp);
		data_to_send[_cnt++]=BYTE0(_temp);
		_temp = (int)(JY901.Euler.Pitch*100);
		data_to_send[_cnt++]=BYTE1(_temp);
		data_to_send[_cnt++]=BYTE0(_temp);
		_temp = (int)(-JY901.Euler.Yaw*100);
		data_to_send[_cnt++]=BYTE1(_temp);
		data_to_send[_cnt++]=BYTE0(_temp);
		
		_temp2 = 0;//单位cm
		data_to_send[_cnt++]=BYTE3(_temp2);
		data_to_send[_cnt++]=BYTE2(_temp2);
		data_to_send[_cnt++]=BYTE1(_temp2);
		data_to_send[_cnt++]=BYTE0(_temp2);
		
		data_to_send[_cnt++]=0x01;//飞行模式
		data_to_send[_cnt++]=1;//上锁0、解锁1
		
		data_to_send[3] = _cnt-4;
		sum = 0;
		for(i=0;i<_cnt;i++)
			sum += data_to_send[i];
		data_to_send[_cnt++]=sum;
		rt_device_write(debug_uart_device, 0,data_to_send, _cnt);    //发送后命令
}

/* ANO匿名地面站调试 */
void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z)//发送传感器原始数字量
{
		u8 _cnt=0;
		vs16 _temp;
		u8 sum = 0;
		u8 i = 0;
		data_to_send[_cnt++]=0xAA;
		data_to_send[_cnt++]=0xAA;
		data_to_send[_cnt++]=0x02;
		data_to_send[_cnt++]=0;
		
		_temp = a_x;
		data_to_send[_cnt++]=BYTE1(_temp);
		data_to_send[_cnt++]=BYTE0(_temp);
		_temp = a_y;
		data_to_send[_cnt++]=BYTE1(_temp);
		data_to_send[_cnt++]=BYTE0(_temp);
		_temp = a_z;
		data_to_send[_cnt++]=BYTE1(_temp);
		data_to_send[_cnt++]=BYTE0(_temp);
		
		_temp = g_x;
		data_to_send[_cnt++]=BYTE1(_temp);
		data_to_send[_cnt++]=BYTE0(_temp);
		_temp = g_y;
		data_to_send[_cnt++]=BYTE1(_temp);
		data_to_send[_cnt++]=BYTE0(_temp);
		_temp = g_z;
		data_to_send[_cnt++]=BYTE1(_temp);
		data_to_send[_cnt++]=BYTE0(_temp);
		
		_temp = m_x;
		data_to_send[_cnt++]=BYTE1(_temp);
		data_to_send[_cnt++]=BYTE0(_temp);
		_temp = m_y;
		data_to_send[_cnt++]=BYTE1(_temp);
		data_to_send[_cnt++]=BYTE0(_temp);
		_temp = m_z;
		data_to_send[_cnt++]=BYTE1(_temp);
		data_to_send[_cnt++]=BYTE0(_temp);
		
		data_to_send[3] = _cnt-4;
		
		sum = 0;
		for(i=0;i<_cnt;i++){
				sum += data_to_send[i];
		}
		data_to_send[_cnt++] = sum;
		
		rt_device_write(debug_uart_device, 0,data_to_send, _cnt);    //发送后命令
}


void ANO_SEND_StateMachine(void)//各组数据循环发送
{
	
		ANO_Data_Send_Status();
	
    ANO_DT_Send_Senser((int16_t)JY901.Acc.x,(int16_t)JY901.Acc.y,(int16_t)JY901.Acc.z,
                       (int16_t)JY901.Gyro.x,(int16_t)JY901.Gyro.y,(int16_t)JY901.Gyro.z,
                       (int16_t)JY901.Mag.x,(int16_t)JY901.Mag.y,(int16_t)JY901.Mag.z);
	
}

/* VCAN山外上位机调试 */
void vcan_sendware(void *wareaddr, unsigned int waresize)
{
		#define CMD_WARE     3
    u8 cmdf[2] = {CMD_WARE, ~CMD_WARE};    //串口调试 使用的前命令
    u8 cmdr[2] = {~CMD_WARE, CMD_WARE};    //串口调试 使用的后命令

    rt_device_write(debug_uart_device, 0,cmdf, 2);    //先发送前命令
    rt_device_write(debug_uart_device, 0,(u8 *)wareaddr, waresize);    //发送数据
    rt_device_write(debug_uart_device, 0,cmdr, 2);    //发送后命令
}



void ware_test(void)
{   

	static int list[8]= {0};
	list[0] = (int)JY901.Euler.Roll; 	//横滚角 Roll 
	list[1] = (int)JY901.Euler.Pitch; //俯仰角 Pitch
	list[2] = (int)JY901.Euler.Yaw; 	//偏航角 Yaw
	list[3] = 180;//-(Servo_Duty-Servo_Duty_Md);
	list[4] = 120;//corner_meet_rn;//edge_start[1];//
	list[5] = 60;
	list[6] = 30;	//camera_forecast_center;//
	list[7] = 0;	//camera_center;
	
	vcan_sendware(list,sizeof(list));
}





/* debug 山外上位机调试 MSH方法 */
static int debug(int argc, char **argv)
{
    int result = 0;

    if (argc != 2){
				rt_kprintf("Usage: debug 10  [notes:debug 10s]\n");
				result = -RT_ERROR; 
				goto _exit;  
    }

		debug_count = (atoi(argv[1]))*1000;  //1ms发送一次
		rt_kprintf("Debug %d sending. ",atoi(argv[1]));
_exit:
    return result;
}
MSH_CMD_EXPORT(debug,ag: debug 10);



/* debug 设置上位机工具 */
static int set_debug_tool(int argc,char **argv)
{
		int result = 0;
    if (argc != 2){
				log_e("Proper Usage: debug_by vcan / ano");//用法:设置上位机工具
				result = -RT_ERROR;
        goto _exit;
    }

		if( !strcmp(argv[1],"vcan") ){ //设置为 山外上位机
				debug_tool = PC_VCAN;
		}
		else {
				log_e("Proper Usage: debug_by vcan / ano");//用法:设置上位机工具
		}
		
		if( !strcmp(argv[1],"ano") ){ //设置为 匿名上位机
				debug_tool = PC_ANO;
		}
		else {
				log_e("Proper Usage: debug_by vcan / ano");//用法:设置上位机工具
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(set_debug_tool,debug_by vcan / ano);

