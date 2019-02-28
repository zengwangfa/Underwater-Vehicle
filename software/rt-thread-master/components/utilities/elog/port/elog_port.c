/*
 * This file is part of the EasyLogger Library.
 *
 * Copyright (c) 2015, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2015-04-28
 */

#include <elog.h>
#include <rthw.h>
#include <rtthread.h>
#include <time.h>
#include <elog_flash.h>

static struct rt_mutex output_lock;

#ifdef ELOG_ASYNC_OUTPUT_ENABLE
static struct rt_semaphore output_notice;

static void async_output(void *arg);
#endif

/**
 * EasyLogger port initialize
 *
 * @return result
 */
ElogErrCode elog_port_init(void) {
    ElogErrCode result = ELOG_NO_ERR;

    rt_mutex_init(&output_lock, "elog lock", RT_IPC_FLAG_PRIO);

#ifdef ELOG_ASYNC_OUTPUT_ENABLE
    rt_sem_init(&output_notice, "elog async", 0, RT_IPC_FLAG_PRIO);
    rt_thread_t async_thread = rt_thread_create("elog_async", async_output, NULL, 1024, RT_THREAD_PRIORITY_MAX - 1, 10);
    if (async_thread) {
        rt_thread_startup(async_thread);
    }
#endif

    return result;
}

/**
 * 输出日志到控制台或者 flash 中
 *
 * @param console 是否输出到控制台
 * @param flash 是否输出到 flash 中
 * @param log 日志缓冲区
 * @param size 日志大小
 */
void output_log_to_console_or_flash(bool console, bool flash, const char *log, size_t size) {
    if(console) {
        rt_kprintf("%.*s", size, log);
    }
    if(flash) {
        elog_flash_write(log, size);
    }
}

/**
 * output log port interface
 *
 * @param log output of log
 * @param size log size
 */
void elog_port_output(const char *log, size_t size) {
    int8_t lvl = elog_find_lvl(log);
    /* output to terminal */
    output_log_to_console_or_flash(true, false, log, size);
    /* 只将断言、错误及警告日志保存至 Flash */
    if (lvl == ELOG_LVL_ASSERT || lvl == ELOG_LVL_ERROR || lvl == ELOG_LVL_WARN) {
        /* output to flash */
        output_log_to_console_or_flash(false, true, log, size);
        elog_flash_flush();
    }
}

#ifdef ELOG_ASYNC_OUTPUT_ENABLE
void elog_async_output_notice(void) {
    rt_sem_release(&output_notice);
}

static void async_output(void *arg) {
    size_t get_log_size = 0;
    static char poll_get_buf[ELOG_LINE_BUF_SIZE - 4];

    while(true) {
        /* waiting log */
        rt_sem_take(&output_notice, RT_WAITING_FOREVER);
        /* polling gets and outputs the log */
        while(true) {
            get_log_size = elog_async_get_line_log(poll_get_buf, sizeof(poll_get_buf));
            if (get_log_size) {
                elog_port_output(poll_get_buf, get_log_size);
            } else {
                break;
            }
        }
    }
}
#endif

/**
 * output lock
 */
void elog_port_output_lock(void) {
    rt_mutex_take(&output_lock, RT_WAITING_FOREVER);
}

/**
 * output unlock
 */
void elog_port_output_unlock(void) {
    rt_mutex_release(&output_lock);
}

/**
 * get current time interface
 *
 * @return current time
 */
const char *elog_port_get_time(void) {
#ifdef RT_USING_RTC
    extern uint32_t RTC_GetSubSecond(void);
    static char cur_system_time[32] = { 0 };
    static time_t now;
    static struct tm* p_tm;
    /* 输出当前时间 */
    now = time(RT_NULL);
    p_tm=localtime(&now);
    rt_snprintf(cur_system_time, 32, "%02d-%02d-%02d %02d:%02d:%02d.%03d", p_tm->tm_year - 100,
            p_tm->tm_mon + 1, p_tm->tm_mday, p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec,
            999 - RTC_GetSubSecond()/10);
    return cur_system_time;
#else
    static char cur_system_time[16] = { 0 };
    rt_snprintf(cur_system_time, 16, "tick:%010d", rt_tick_get());
    return cur_system_time;
#endif
}

/**
 * get current process name interface
 *
 * @return current process name
 */
const char *elog_port_get_p_info(void) {
    return "";
}

/**
 * get current thread name interface
 *
 * @return current thread name
 */
const char *elog_port_get_t_info(void) {
    return rt_thread_self()->name;
}

#if defined(RT_USING_FINSH) && defined(FINSH_USING_MSH)
#include <finsh.h>
static void elog(uint8_t argc, char **argv) {
    if (argc > 1) {
        if (!strcmp(argv[1], "on") || !strcmp(argv[1], "ON")) {
            elog_set_output_enabled(true);
        } else if (!strcmp(argv[1], "off") || !strcmp(argv[1], "OFF")) {
            elog_set_output_enabled(false);
        } else {
            rt_kprintf("Please input elog on or elog off.\n");
        }
    } else {
        rt_kprintf("Please input elog on or elog off.\n");
    }
}
MSH_CMD_EXPORT(elog, EasyLogger output enabled [on/off]);

static void elog_lvl(uint8_t argc, char **argv) {
    if (argc > 1) {
        if ((atoi(argv[1]) <= ELOG_LVL_VERBOSE) && (atoi(argv[1]) >= 0)) {
            elog_set_filter_lvl(atoi(argv[1]));
        } else {
            rt_kprintf("Please input correct level(0-5).\n");
        }
    } else {
        rt_kprintf("Please input level.\n");
    }
}
MSH_CMD_EXPORT(elog_lvl, Set EasyLogger filter level);

static void elog_tag(uint8_t argc, char **argv) {
    if (argc > 1) {
        if (rt_strlen(argv[1]) <= ELOG_FILTER_TAG_MAX_LEN) {
            elog_set_filter_tag(argv[1]);
        } else {
            rt_kprintf("The tag length is too long. Max is %d.\n", ELOG_FILTER_TAG_MAX_LEN);
        }
    } else {
        elog_set_filter_tag("");
    }
}
MSH_CMD_EXPORT(elog_tag, Set EasyLogger filter tag);

static void elog_kw(uint8_t argc, char **argv) {
    if (argc > 1) {
        if (rt_strlen(argv[1]) <= ELOG_FILTER_KW_MAX_LEN) {
            elog_set_filter_kw(argv[1]);
        } else {
            rt_kprintf("The keyword length is too long. Max is %d.\n", ELOG_FILTER_KW_MAX_LEN);
        }
    } else {
        elog_set_filter_kw("");
    }
}
MSH_CMD_EXPORT(elog_kw, Set EasyLogger filter keyword);
#endif /* defined(RT_USING_FINSH) && defined(FINSH_USING_MSH) */
