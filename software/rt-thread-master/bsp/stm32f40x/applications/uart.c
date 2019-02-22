#include "init.h"
#include <rthw.h>

/*---------------------- Constant / Macro Definitions -----------------------*/

#define JY901_UART_NAME       "uart2"
#define DEBUG_UART_NAME       "uart3"

#define Query_JY901_data 0     /* "1"为调试查询  "0"为正常读取 */

#if Query_JY901_data
char recv_buffer[128]; 				//串口2接收数据缓冲变量,
unsigned char recv_data_p=0x00;  //  串口2接收数据指针
#endif

/*----------------------- Variable Declarations -----------------------------*/
/* ALL_init 事件控制块. */
extern struct rt_event init_event;

rt_device_t debug_uart_device;	

rt_device_t uart2_device;	
struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; /* 配置参数 */
static struct rt_semaphore rx_sem;/* 用于接收消息的信号量 */

u8 gyroscope_save[5] 		={0xFF,0xAA,0x00,0x00,0x00};	 //0x00-设置保存  0x01-恢复出厂设置并保存
u8 gyroscope_package[5] ={0xFF,0xAA,0x02,0xFF,0x07};	 //设置回传的数据包【0xFF 0x07 为所有数据包】
u8 gyroscope_rate[5] 		={0xFF,0xAA,0x03,0x06,0x00};	 //传输速率 0x05-5Hz  0x06-10Hz(默认)  0x07-20Hz
u8 gyroscope_led[5] 		={0xFF,0xAA,0x1B,0x00,0x00}; 	 //0x00-开启LED  0x01-关闭LED   

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

/* 开启 九轴模块 数据包 */
void on_gyroscope_package(void)
{
		rt_device_write(uart2_device, 0, gyroscope_package, 5);   //ON package 开启回传数据包
		rt_device_write(uart2_device, 0, gyroscope_save, 5);  //SAVE
}
MSH_CMD_EXPORT(on_gyroscope_package,turn on gyroscope_led);

/* 开启 九轴模块 LED */
void on_gyroscope_led(void)
{
		gyroscope_led[3] = 0x00;
		rt_device_write(uart2_device, 0, gyroscope_led, 5);   //ON LED
		rt_device_write(uart2_device, 0, gyroscope_save, 5);  //SAVE
}
MSH_CMD_EXPORT(on_gyroscope_led,turn on gyroscope_led);

/* 关闭 九轴模块 LED */
void off_gyroscope_led(void)
{
		gyroscope_led[3] = 0x01;
		rt_device_write(uart2_device, 0, gyroscope_led, 5);   //OFF LED
		rt_device_write(uart2_device, 0, gyroscope_save, 5);  //SAVE
}
MSH_CMD_EXPORT(off_gyroscope_led,turn off gyroscope_led);


int uart_gyroscope(void)
{
	  rt_thread_t gyroscope_tid;
	  /* 查找系统中的串口设备 */
		uart2_device = rt_device_find(JY901_UART_NAME);
		debug_uart_device = rt_device_find(DEBUG_UART_NAME);
	
		LOG_I("gyroscope serial:  %s", uart2_device);
		LOG_I("debug serial:  %s", debug_uart_device);
		rt_device_open(debug_uart_device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_DMA_RX);
	
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
		gyroscope_tid = rt_thread_create("gyroscope",
																			gyroscope_thread_entry,
																			RT_NULL, 
																			1024, 
																			25,
																			10);
    /* 创建成功则启动线程 */
    if (gyroscope_tid != RT_NULL)
    {
        rt_thread_startup(gyroscope_tid);
				rt_event_send(&init_event, GYRO_EVENT); //发送事件  表示初始化完成
    }
		return 0;
}

INIT_APP_EXPORT(uart_gyroscope);





