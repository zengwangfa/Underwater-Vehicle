/*
 * Change Logs:
 * Date           Author       Notes

*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "my_init.h"
#include <rtthread.h>

#include <rtdevice.h>
#include "board.h"

#define SAMPLE_UART_NAME       "uart2"


extern rt_err_t rt_pwm_enable(int channel);
extern rt_err_t rt_pwm_set(int channel, rt_uint32_t period, rt_uint32_t pulse);

extern rt_uint8_t FX_NUMBER;

extern rt_uint16_t Temperature[3];
extern rt_uint16_t voltage[3];

extern bool   HEAT_START_FLAG;          //开始控制加热丝加热标志位
extern bool   WASHING_START_FLAG;       //开始清洗蜂箱标志位
extern bool   FEED_START_FLAG;          //开始喂食蜂箱标志位

extern rt_uint8_t TEXT_Buffer[2];    //第一个字节为蜂箱的开口状态，0x04表示开口最大，0x02居中，0x01最小

extern rt_uint8_t DataBuffer[8];   //蜂箱数据暂存数组，包括2字节的重量+2字节的饲料重量+2字节的温度+1字节的(清洗、加热、喂食)标志位+1字节的蜂王状态
extern rt_uint8_t send_buffer[(MAX_DATA_LENS)]; //串口1发送数据缓冲变量,
extern rt_uint8_t recv_buffer[MAX_DATA_LENS]; //WIFI 串口2接收数据缓冲变量,
extern rt_uint8_t recv_data_p;              //WIFI   串口2接收数据指针
extern rt_uint8_t recv_data_ok;                //接收到正确的数据包
extern rt_uint8_t sum_check;
extern rt_uint16_t Washing_Time;                 //清洗时间，由上位机给定
extern rt_uint16_t Heat_Time;                    //加热时间，由上位机给定，单位秒
extern rt_uint16_t Feed_Time;                    //喂食时间，由上位机给定，单位秒



static struct rt_semaphore rx_sem;/* 用于接收消息的信号量 */
static char uart_name[RT_NAME_MAX];


static rt_err_t uart_input(rt_device_t dev, rt_size_t size)/* 接收数据回调函数 */
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送收接信号量 */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

static void serial_thread_entry(void* parameter)
{
    char ch;
		rt_uint8_t i=0,j=0;
    rt_device_t serial;

		//char str[] = "hello World!\r\n";
    /* 查找系统中的串口设备 */
    serial = rt_device_find(SAMPLE_UART_NAME);

	
    if (serial != RT_NULL)
    {
        rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
        /* 以读写及中断接收方式打开串口设备 */
        rt_device_open(serial, RT_DEVICE_OFLAG_RDWR|RT_DEVICE_FLAG_INT_RX);
        /* 设置接收回调函数 */
        rt_device_set_rx_indicate(serial, uart_input);
        /* 发送字符串 */
      //  rt_device_write(serial, 0, str, (sizeof(str) - 1)); //----发送----  
			
        while (1)//发送接收到的数据
        {
            while (rt_device_read(serial, 0,&ch, 1) != 1){    /* 从串口读取一个字节的数据，没有读取到则等待接收信号量 */
                rt_sem_take(&rx_sem, RT_WAITING_FOREVER); /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
            }
						recv_buffer[recv_data_p] = ch;
						recv_data_p++;
						if(recv_data_p<(MAX_DATA_LENS+4))    //定义数据长度未包括包头和包长3个字节,+4
						 {

							if(recv_buffer[0]==0xaa)  //数据包包头字节
								{

									if(recv_data_p>3)
										{

											if(recv_buffer[1]==0x55)
												{	
															
													if(recv_data_p>=(recv_buffer[2]+3))  //接收完数据包（第三个字节为数据长度，数据长度不包含开头和校验字）
														{
															
																for(i=0;i<=(recv_data_p-2);i++)  //接收到数据包的最后一个字节为校验字；校验方法对整包数据进行累加（不包含校验字）
																{
																	j +=recv_buffer[i];	
																}
																
																if(j==recv_buffer[recv_data_p-1])     //判断校验是否成功
																{	
																	recv_data_ok=1;  
																													//接收到正确完整数据包标志位置位
																	//USART_Sends(HT_USART0,recv_buffer,sizeof(recv_buffer));//返回接收正确的数据
																}													
																else
																{
																	recv_data_ok=0;
																	recv_data_p=0;
																}
																recv_data_p=0;
																j=0;			
														}	
												}					
												else
												recv_data_p=0;
										}
								}
								else
								recv_data_p=0;
							}
							else
							recv_data_p=0;
								

							

							
            //rt_device_write(serial, 0, recv_buffer,15);
        }
    }
    else{
        rt_kprintf("uart sample run failed! can't find %s device!\n",uart_name);
    }
}

int uart2_wifi_init(void)
{
		/* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("serial",serial_thread_entry, RT_NULL, 1024, 3, 10);
    /* 创建成功则启动线程 */
    if (thread!= RT_NULL){
        rt_thread_startup(thread);
    }

		return 0;
}
INIT_APP_EXPORT(uart2_wifi_init);




void Uart2DataAnalyzing(void)       //WIFI 串口2数据解析
{
	//unsigned char datatemp[1],i;
	rt_uint8_t i;
	rt_device_t serial;

	serial = rt_device_find(SAMPLE_UART_NAME);
	if (serial != RT_NULL)
  {
		if(recv_data_ok == 1)      //串口1接收一帧数据完成
		{
			if(((recv_buffer[4]==FX_NUMBER%255)&&(recv_buffer[3]==FX_NUMBER/255))||((recv_buffer[4] == 0xff)&&(recv_buffer[3]==0xff))) 
			//接收到的蜂箱编号与本地编号相同,或者为广播信号(FFFF)  才进行解码
			{
				switch(recv_buffer[5])                                  
				{	
					case SET_WASHING :                                    
					{
							rt_device_write(serial, 0, recv_buffer,15);
							if(recv_buffer[8]==0x01)                            
							{
									WASHING_START_FLAG=TRUE;                       //开始清洗时间计数开启
									Washing_Time=recv_buffer[6];                   //清洗时间低字节
									Washing_Time=(Washing_Time<<8)+recv_buffer[7]; //清洗时间
									rt_kprintf("Washing_Tim:%d",Washing_Time);
									rt_pin_write(Watering_PIN, PIN_HIGH);rt_kprintf("Water_ON!\t");

							}
							else
							{
									WASHING_START_FLAG=FALSE;               //开始清洗时间计数关闭
									rt_pin_write(Watering_PIN, PIN_LOW);rt_kprintf("Water_OFF!\t");
							}
					}		
					break;			
					case SET_FEED :                                 //进行喂食
					{
							rt_device_write(serial, 0, recv_buffer,15);
							if(recv_buffer[8]==0x01)                          	
							{
									FEED_START_FLAG=TRUE;                    //开始喂食时间计数开启
								  Feed_Time=recv_buffer[6];                //喂食时间低字节
									Feed_Time=(Feed_Time<<8)+recv_buffer[7]; //喂食时间
         					rt_kprintf("Feed_Time:%d",Feed_Time);
									rt_pin_write(Feeding_PIN, PIN_HIGH);rt_kprintf("Feed_ON!\t");
						
							}
							else
							{
									FEED_START_FLAG=FALSE;                   //开始喂食时间计数关闭
									rt_pin_write(Feeding_PIN, PIN_LOW);rt_kprintf("Feed_OFF!\t");
										
							}
					}
					break;
					
					case SET_TEMPTURE :  //执行加热
					{
	            rt_device_write(serial, 0, recv_buffer,15);
							if(recv_buffer[8]==0x01)                     //上位机发送加热开
							{
									HEAT_START_FLAG=TRUE;                    //开始加热时间计数开启
									Heat_Time=recv_buffer[6];                //加热时间低字节
									Heat_Time=(Heat_Time<<8)+recv_buffer[7]; //加热时间
						      rt_kprintf("Heat_Time:%d\r\n",Heat_Time);		
              		rt_pin_write(Heat1_PIN, PIN_HIGH);rt_kprintf("Heat1_ON!\t");
							  	rt_pin_write(Heat2_PIN, PIN_HIGH);rt_kprintf("Heat2_ON!\t");
									rt_pin_write(LED3, PIN_HIGH);		
							}
							else
							{
								  HEAT_START_FLAG=FALSE;                                //开始加热时间计数关闭
								  rt_pin_write(Heat1_PIN, PIN_LOW);rt_kprintf("Heat1_OFF!\t");
							  	rt_pin_write(Heat2_PIN, PIN_LOW);rt_kprintf("Heat2_OFF!\t");
	
							}                                       
					}	
						break;
					
					case RETRUN_MESSAGE :
					{
						DataBuffer[7]=TEXT_Buffer[0];                             //存入数组  
						send_buffer[0]=0xAA;                                   //数据包 开头
						send_buffer[1]=0x55;
						send_buffer[2]=0x0C;                                   //数据包数据长度
						send_buffer[3]=FX_NUMBER/255;                          //蜂箱编号
						send_buffer[4]=FX_NUMBER%255;   
						send_buffer[5]=0x04;                                   //命令字节返回
						for(i=0;i<8;i++)                                       //将重量、温度、开口状态存入发送数组
						send_buffer[i+6]=DataBuffer[i];         
			
						for(i=0;i<14;i++)                                    //计算校验字
						sum_check+=send_buffer[i];
						send_buffer[14]=sum_check;                                   //数据包数据长度
						rt_device_write(serial, 0, send_buffer, (sizeof(send_buffer)));   
						sum_check=0;
					}
					break;
					
					case CONTR_BEE_STATE1 :                                  //开口打开最大全部蜜蜂都可通过
					{
	            rt_device_write(serial, 0, recv_buffer,15);
						if(TEXT_Buffer[0]==0x02)                                  //蜂箱开口居中，顺时针转动120度，回到开口最大
						{		
							rt_pwm_set(3, 512, 250);
						}
						else if(TEXT_Buffer[0]==0x01)                             //蜂箱开口最小，顺时针转动240度，回到开口最大
						{	
							rt_pwm_set(3, 512, 250);
						}
					 
					}
					break;
					
					case CONTR_BEE_STATE2 :                                //蜂王禁止通行
					{
						 rt_device_write(serial, 0, recv_buffer,15);
						if(TEXT_Buffer[0]==0x04)                                  //蜂箱开口居中，顺时针转动120度，回到开口最大
						{		
							rt_pwm_set(3, 512, 250);
						}
						else if(TEXT_Buffer[0]==0x01)                             //蜂箱开口最小，顺时针转动240度，回到开口最大
						{	

						}

					}
					break;
					
					case CONTR_BEE_STATE3 :                                //蜂王禁止通行且开口最小
					{
					  rt_device_write(serial, 0, recv_buffer,15);
						
						if(TEXT_Buffer[0]==0x04)                                  //蜂箱开口居中，顺时针转动120度，回到开口最大
						{		
							rt_pwm_set(3, 512, 250);
						}
						else if(TEXT_Buffer[0]==0x02)                             //蜂箱开口最小，顺时针转动240度，回到开口最大
						{	
							rt_pwm_set(3, 512, 250);
						}
					 
					}
						break;
					default:
						break;	
				}
			
			}
			 recv_data_ok=0;    //接收标志位清0，等待下一次接收
		}

	}
}
MSH_CMD_EXPORT(Uart2DataAnalyzing, Uart2DataAnalyzing );



void rt_pwm_breathe_entry(void *parameter)
{
		rt_pwm_enable(3);

		while(1)
		{
				 rt_pwm_set(3, 512, rand());
				 rt_thread_delay(10);
		}
}
int rt_breathe_init()
{
		rt_device_t pwm_dev;
		rt_thread_t tid;
		pwm_dev = rt_device_find("pwm3");
		if(NULL != pwm_dev)
		{
				tid = rt_thread_create("pwm_led",
																rt_pwm_breathe_entry, 
																RT_NULL,
																512, 
																15, 
																20);
				if (tid != RT_NULL)
				rt_thread_startup(tid);
		} 
		return 0;
}


INIT_APP_EXPORT(rt_breathe_init);





//---------------------WIFI TEST-----------------------------//


static void WIFI_SEND_BUFF(void)
{
		rt_device_t serial;
	//--------------------注释说明-------------------------//

		char str1[] = "1.Weight:";
		char str2[] = "2.Temp:";
		char str3[] = "3.Voltage:";

		char str7[7];  //重量1
		char str8[7];  //重量1
	
		char str9[3];  //温度
		char str10[4]; //电压
	
	//--------------------数据转换-------------------------//

		//myitoa(Weight[0], str7, 10);     //整形转换字符型
		//myitoa(Weight[1], str8, 10);
		myitoa(Temperature[0], str9, 10);
		myitoa(voltage[0], str10, 10);

	
	
		serial = rt_device_find(SAMPLE_UART_NAME);
    if (serial != RT_NULL){
	//---------------------数据发送------------------------//
			//重量
			rt_thread_delay(10); 
			rt_device_write(serial, 0, str1, (sizeof(str1) - 1)); 
			rt_device_write(serial, 0, str7, (sizeof(str7))); 
			rt_device_write(serial, 0, "-", 1); 
			rt_device_write(serial, 0, str8, (sizeof(str8))); 

			//温度
		 	rt_thread_delay(10); 
			rt_device_write(serial, 0, str2, (sizeof(str2) - 1)); 
			rt_device_write(serial, 0, str9, (sizeof(str9))); 

			//电压
			rt_thread_delay(10); 
			rt_device_write(serial, 0, str3, (sizeof(str3) - 1)); 
			rt_device_write(serial, 0, str10, (sizeof(str10))); 
 
			

		}    
		else{
        rt_kprintf("WIFI_TEST run failed! can't find device!\n");
    }
		
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(WIFI_SEND_BUFF, wifi test);
