#include "my_init.h"


#define LED1_PIN 55
#define LED6 PBout(3)

int turn_led(void)
{
    static rt_uint8_t count;
    rt_pin_mode(LED1_PIN, PIN_MODE_OUTPUT);  
 
		if(BOMA4)
		{
			if(count==0){
				LED6 = 1;
				//rt_pin_write(LED1_PIN, PIN_HIGH);
				count++;
			}
			else{
				LED6 = 0;
				//rt_pin_write(LED1_PIN, PIN_LOW);
				count=0;
			}
		}
		return 0;
}



int MY_TEST(void)
{
    rt_uint8_t count;
    for(count = 1 ; count <= 3 ;count++){       
        rt_kprintf("test, count : %d\r\n", count);
        rt_thread_mdelay(500);
    }
		rt_kprintf("test-->success");
    return 0;
}
MSH_CMD_EXPORT(MY_TEST, This is my test example);


