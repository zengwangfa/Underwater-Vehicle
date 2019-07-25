#ifndef __EASY_THREAD_H_
#define __EASY_THREAD_H_

#include "DataType.h"

void flash_thread_entry(void* parameter);
extern void ioDevices_thread_entry(void* parameter);
extern void oled_thread_entry(void* parameter);
extern void led_thread_entry(void *parameter);

#endif


