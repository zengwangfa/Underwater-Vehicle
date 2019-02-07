#include "init.h"














int MY_TEST(void)
{
    rt_uint8_t i;
    for(i = 1 ; i <= 3 ;i++){       
        rt_kprintf("Test, count : %d\r\n", i);
        rt_thread_mdelay(500);
    }
		rt_kprintf("Test-->successed!\n");
    return 0;
}
MSH_CMD_EXPORT(MY_TEST, This is a test example.);




