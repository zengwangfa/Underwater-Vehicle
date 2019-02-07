#include "my_init.h"

extern rt_int32_t side_weight[2];
extern rt_int32_t Weight[3];
extern rt_uint16_t Temperature[3];
extern rt_uint16_t voltage[3];
extern rt_uint8_t FX_NUMBER;

extern rt_uint8_t FW_status_flag;
extern rt_uint8_t PiShu;
extern rt_int8_t *FW_status_string[4]; //蜂王状态名 字符串

extern rt_uint8_t buzzer_time;//蜂鸣器
extern rt_uint8_t buzzer_count;//蜂鸣器
extern rt_uint8_t buzzer_number;//蜂鸣器
extern fx_date fx_data1;

void CP2102_SEND_BUFF(void* parameter)  //向串口1  定时发送板子状态信息
{
	rt_uint8_t count = 0;
	buzzer_bibi(3,2);
	
	while(1)
	{	
		fx_data1.fx_number = FX_NUMBER;
		if(BOMA3)  //BOMA3为1时
		{
			rt_kprintf("\r\n\r\n1.FX_NUMBER:%d\n", FX_NUMBER);
			//
			
			rt_kprintf("2.left_weight : %d\t right_weight : %d\r\n", Weight[0],Weight[1]);
			rt_kprintf("3.Temperature:%d.%dC\r\n",Temperature[1],Temperature[2]);
			rt_kprintf("4.Voltage_Vaule:%d.%dv\n", voltage[1],voltage[2]);
		  rt_kprintf("5.Now--FW_Status:%d-->%s\r\n",FW_status_flag,FW_status_string[FW_status_flag]);
 			rt_kprintf("6.Now--Pi Shu:%d\r\n",PiShu);
			count++	;	if(count>=100)count=0;
			rt_kprintf("\r\nCount:%d",count);
		}

		rt_thread_delay(1000);  //10s
	}
	
}

int regularly_send_init()
{
	rt_thread_t tid;
        tid = rt_thread_create("CP2102_SEND_BUFF",CP2102_SEND_BUFF, RT_NULL,1024,15, 20);
        if (tid != RT_NULL)
                rt_thread_startup(tid);
        
        return 0;
}
INIT_APP_EXPORT(regularly_send_init);




void buzzer_once()   
{
	buzzer_time=2;
	buzzer_number=2;
	buzzer_count=1;

}  
void buzzer_bibi(rt_uint8_t t,rt_uint8_t l)
{
	buzzer_time=l*5;
	buzzer_number=l*5;
	buzzer_count=t*2;

}  
void buzzer_ring()
{
		
	if(buzzer_count>=1)
	{  
		if(buzzer_number>=1)
		{	
						if(buzzer_count%2==1)   {rt_pin_write(BEEP_PIN, PIN_LOW);}
						else                    {rt_pin_write(BEEP_PIN, PIN_HIGH);}
						buzzer_number--;	
		}
		else
		{
						buzzer_number=buzzer_time;
						buzzer_count--;
		}
	}
	else
	{
		rt_pin_write(BEEP_PIN, PIN_HIGH);
		buzzer_time=0;
		buzzer_count=0;
	}

}

void my_delay_us(unsigned int us)
{
	unsigned int i = 0;
	for(i=0;i<20*us;i++)
	{
		__NOP();
	}
}
	
void my_delay_ms(unsigned int ms)
{
	unsigned int i = 0;
	for(i=0;i<20*ms;i++)
	{
		my_delay_us(1000);
	}
}
	
rt_int32_t Bubble(rt_int32_t *v)
{
	rt_uint8_t i,j;
	rt_int32_t temp,k;
	for(j=0;j<10;j++)
	{
		for(i=0;i<9-j;i++)
		{
			if(v[i] > v[i+1])
			{
				temp = v[i];
				v[i] = v[i+1];
				v[i+1] = temp;
			}
		}
	}
	k = (v[4]+v[5]+v[6])/3; //取中值
	return k;
}


rt_uint16_t Bubble_filter(rt_uint16_t *v,rt_uint8_t NO) //NO为 1是为中值滤波   NO为其他时为 取最小
{
	rt_uint32_t i,j;
	rt_uint32_t res,val; //reserve保存中间变量
	
	for (j=0;j<10;j++){
		for(i=0;i<9-j;i++){
			if(v[i]>v[i+1]){
				res = v[i];
				v[i] = v[i+1];
				v[i+1] = res;
			}
		}
	}
	if(NO == 1){
		
		val = (v[4]+v[5]+v[6])/3;
		return val;
	}
	else {return v[0];}
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

