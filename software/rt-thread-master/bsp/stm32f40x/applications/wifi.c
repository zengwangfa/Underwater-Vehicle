#include "init.h"
#include "wifi.h"
#include <rtdevice.h>
#include <elog.h>
/*---------------------- Constant / Macro Definitions -----------------------*/
#define WIFI_UART_NAME        "uart3"   // uart3 WIFI 

/*----------------------- Variable Declarations -----------------------------*/
//extern rt_device_t wifi_uart_device;	
//extern struct rt_semaphore wifi_rx_sem;/* 用于接收消息的信号量 */

//u8 wifi_recv_buffer[128] = {0}; //wifi数据包缓冲区


/*----------------------- Function Implement --------------------------------*/

//void Query_WiFi_Connected(void)
//{
//		rt_device_write(wifi_uart_device, 0, "AT+WSLK\r\n", sizeof("AT+WSLK\r\n")); //查询STA的无线Link状态
//	
//}

///* 设置 九轴模块 加速度校准 */
//void query_wifi_connect(void)
//{
//		  Query_WiFi_Connected();
//			log_i("Query_WiFi_Connected\r\n");
//}
//MSH_CMD_EXPORT(query_wifi_connect,query_wifi_connect);


///* 接收数据回调函数 */
//rt_err_t wifi_uart_input(rt_device_t dev, rt_size_t size)
//{
//    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
//    rt_sem_release(&wifi_rx_sem);

//    return RT_EOK;
//}


//static void wifi_thread_entry(void *parameter)
//{
//    u8 i = 0,ch;
//		
//		while (1)
//		{
//				/* 从串口读取一个字节的数据，没有读取到则等待接收信号量 */
//				while (rt_device_read(wifi_uart_device, 0, &ch, 1) != 1)
//				{
//						/* 阻塞等待接收信号量，等到信号量后再次读取数据 */
//						rt_sem_take(&wifi_rx_sem, RT_WAITING_FOREVER);
//				}
//				wifi_recv_buffer[i++] = ch;
//		}
//}





//int wifi_uart_init(void)
//{
//		rt_thread_t wifi_tid;
//		/* 创建 serial 线程 */
//		wifi_tid = 	rt_thread_create("wifi_thread",
//																	wifi_thread_entry,
//																	RT_NULL, 
//																	1024, 
//																	15,
//																	10);
//    /* 创建成功则启动线程 */
//    if (wifi_tid != RT_NULL){
//				log_i("WiFi_Init()");
//				rt_thread_startup(wifi_tid);

//    }
//		return 0;
//}
//INIT_APP_EXPORT(wifi_uart_init);






















