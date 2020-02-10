/*
 * @Description: 主函数
 * @Author: chenxi
 * @Date: 2020-01-01 13:06:46
 * @LastEditTime : 2020-02-10 17:11:37
 * @LastEditors  : chenxi
 */

#define LOG_TAG "main"

#include "elog.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#include "DataType.h"

#include "../applications/I2C_PWM.h"
#include "../applications/sensor.h"

#include <wiringPi.h>

void easyloggerInit(void)
{
  setbuf(stdout, NULL);
  /* initialize EasyLogger */
  elog_init();
  /* set EasyLogger log format */
  elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
  elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
  elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
  elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
  elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
  elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
#ifdef ELOG_COLOR_ENABLE
  elog_set_text_color_enabled(true);
#endif
  /* start EasyLogger */
  elog_start();
}

void *I2C_PWM_callback_fun(void *arg)
{
  I2C_PWM_Init();
  // I2C_PWM_SetPWMFreq(50.0);

  int i = 0;
  while (1)
  {
    I2C_PWM_SetPWM(0, i, 4096 - i);
    if (i <= 4096)
      i += 32;
    else
      i = 0;
    delay(1000);
  }
  return NULL;
}

int main()
{
  easyloggerInit();
  if (wiringPiSetup() == -1)
  {
    log_e("Unable to start wiringPi: %s", strerror(errno));
    return 1;
  }

  // pthread_t I2C_PWM_tid;
  // if (pthread_create(&I2C_PWM_tid, NULL, I2C_PWM_callback_fun, NULL) == -1)
  // {
  //   log_e("I2C_PWM_thread create error!");
  //   return 1;
  // }
  // if (pthread_detach(I2C_PWM_tid))
  // {
  //   log_w("I2C_PWM_thread detach failed...");
  //   return -2;
  // }

  sensor_thread_init();
  while (1)
  {
    sleep(2);
    print_sensor_info();
  }

  return 0;
}
