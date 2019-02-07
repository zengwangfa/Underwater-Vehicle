#include "init.h"





//void CP2102_SEND_BUFF(void* parameter)  //向串口1  定时发送板子状态信息
//{
//	rt_uint8_t count = 0;
//	buzzer_bibi(3,2);
//	
//	while(1)
//	{	
//		fx_data1.fx_number = FX_NUMBER;
//		if(BOMA3)  //BOMA3为1时
//		{
//			rt_kprintf("\r\n\r\n1.FX_NUMBER:%d\n", FX_NUMBER);
//			//
//			
//			rt_kprintf("2.left_weight : %d\t right_weight : %d\r\n", Weight[0],Weight[1]);
//			rt_kprintf("3.Temperature:%d.%dC\r\n",Temperature[1],Temperature[2]);
//			rt_kprintf("4.Voltage_Vaule:%d.%dv\n", voltage[1],voltage[2]);
//		  rt_kprintf("5.Now--FW_Status:%d-->%s\r\n",FW_status_flag,FW_status_string[FW_status_flag]);
// 			rt_kprintf("6.Now--Pi Shu:%d\r\n",PiShu);
//			count++	;	if(count>=100)count=0;
//			rt_kprintf("\r\nCount:%d",count);
//		}

//		rt_thread_delay(1000);  //10s
//	}
//	
//}





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





//void delay_us(unsigned int us)
//{
//	unsigned int i = 0;
//	for(i=0;i<20*us;i++)
//	{
//			__NOP();
//	}
//}
//	
//void delay_ms(unsigned int ms)
//{
//	unsigned int i = 0;
//	for(i=0;i<20*ms;i++)
//	{
//			delay_us(1000);
//	}
//}
	

void delay_us(u32 nTimer)
{
	u32 i=0;
	for(i=0;i<nTimer;i++){
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	}
}

void delay_ms(u32 nTimer)
{
	u32 i=1000*nTimer;
	delay_us(i);
}
//----------------整形转换字符型--------------------//
char *myitoa(int value, char *string, int radix)
{
    rt_uint32_t     i, d;
    int     flag = 0;
    char    *ptr = string;
 
    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }
    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }
    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';
        /* Make the value positive. */
        value *= -1;
    }
    for (i = 10000000; i > 0; i /= 10)
    {
        d = value / i;
 
        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }
 
    /* Null terminate the string. */
    *ptr = 0;
 
    return string;
 
} /* NCL_Itoa */





