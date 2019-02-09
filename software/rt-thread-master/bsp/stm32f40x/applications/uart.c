#include "init.h"

/*---------------------- Constant / Macro Definitions -----------------------*/

#define SAMPLE_UART_NAME       "uart2"
#define Query_JY901_data 0     /* "1"为调试查询  "0"为正常发送 */

#if Query_JY901_data
char recv_buffer[128]; 				//串口2接收数据缓冲变量,
unsigned char recv_data_p=0x00;  //  串口2接收数据指针
#endif

/*----------------------- Variable Declarations -----------------------------*/
rt_device_t uart2_device;	
struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; /* 配置参数 */

static struct rt_semaphore rx_sem;/* 用于接收消息的信号量 */

/*----------------------- Function Implement --------------------------------*/
/* 接收数据回调函数 */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

static void gyroscope_thread_entry(void *parameter)
{
    unsigned char ch;
		//unsigned char save[] ={0xFF,0xAA,0x00,0x00,0x00};	 //设置保存
		//unsigned char package[] ={0xFF,0xAA,0x02,0xFF,0x07};	 //设置回传的数据包
		//char rate[] ={0xFF,0xAA,0x03,0x06,0x00};	 //传输速率 10Hz
		//char led[] ={0xFF,0xAA,0x1B,0x01,0x00}; 	 //0x01 关闭LED    0x00 开启LED

    
		/* 设置相关配置 */
		//rt_device_write(uart2_device, 0, package, (sizeof(package)));  
		//rt_device_write(uart2_device, 0, save, (sizeof(save)));  
		while (1)
		{
				/* 从串口读取一个字节的数据，没有读取到则等待接收信号量 */
				while (rt_device_read(uart2_device, 0, &ch, 1) != 1)
				{
						/* 阻塞等待接收信号量，等到信号量后再次读取数据 */
						rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
				}
#if Query_JY901_data //在线调试 查询模式
				
				recv_buffer[recv_data_p] = ch;
				recv_data_p++;
				if(recv_data_p>127)recv_data_p = 0;
		
#else 
				CopeSerial2Data(ch); //正常传输模式
#endif

		}
	
}

int uart_sample(void)
{
	  rt_thread_t gyroscope_tid;
	  /* 查找系统中的串口设备 */
		uart2_device = rt_device_find(SAMPLE_UART_NAME);
		rt_kprintf("gy_serial:%s",uart2_device);
    if (uart2_device != RT_NULL)
    {
					/* 以读写以及中断接打开串口设备 */
					rt_device_open(uart2_device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_DMA_RX);
					config.baud_rate = BAUD_RATE_9600;
					config.data_bits = DATA_BITS_8;
					config.stop_bits = STOP_BITS_1;
					config.parity = PARITY_NONE;
			
					/* 打开设备后才可修改串口配置参数 */
					rt_device_control(uart2_device, RT_DEVICE_CTRL_CONFIG, &config);
					rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
					/* 设置接收回调函数 */
					rt_device_set_rx_indicate(uart2_device, uart_input);
		}
    /* 创建 serial 线程 */
		gyroscope_tid = rt_thread_create("gyroscope", gyroscope_thread_entry, RT_NULL, 1024, 25, 10);
    /* 创建成功则启动线程 */
    if (gyroscope_tid != RT_NULL)
    {
        rt_thread_startup(gyroscope_tid);
    }
		return 0;
}

INIT_APP_EXPORT(uart_sample);





