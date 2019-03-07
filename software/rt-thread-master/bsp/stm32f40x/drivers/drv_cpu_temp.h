/*
 * File      : board.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2017, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-12-19     armink       first version
 */

#ifndef _DRV_CPU_TEMP_H_
#define _DRV_CPU_TEMP_H_

int stm32_cpu_temperature_init(void);
uint32_t cpu_temp_get(void);

#endif /* _DRV_CPU_TEMP_H_ */
