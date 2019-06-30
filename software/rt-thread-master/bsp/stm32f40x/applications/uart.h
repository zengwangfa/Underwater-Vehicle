#ifndef __UART_H_
#define __UART_H_

#include <rtthread.h>
#include "DataType.h"

static int list_serial_devices(void);


extern rt_device_t control_uart_device;	
extern rt_device_t debug_uart_device;	
extern rt_device_t gyro_uart_device;	
extern rt_device_t focus_uart_device;	


extern uint8 uart_startup_flag;






#endif

