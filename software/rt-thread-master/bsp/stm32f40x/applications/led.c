#define LOG_TAG    "led"

#include "init.h"
#include "string.h"

/*---------------------- Constant / Macro Definitions -----------------------*/
//RGB灯引脚号
#define LED_Red 		68 
#define LED_Green 	69
#define LED_Blue 		70

//RGB灯共阳极接到+3.3V，电平 0亮 1灭
#define LED_ON(led_pin) 						rt_pin_write(led_pin ,PIN_LOW )
#define LED_OFF(led_pin) 						rt_pin_write(led_pin ,PIN_HIGH)
#define LED_Turn(led_pin,status) 		rt_pin_write(led_pin ,status =! status) //取反

/*----------------------- Variable Declarations -----------------------------*/
/* ALL_init 事件控制块. */
extern struct rt_event init_event;
extern rt_uint8_t VehicleStatus;

/*----------------------- Function Implement --------------------------------*/
void led_thread_entry(void *parameter)
{
	  rt_int8_t i=0;/*颜色节拍表> 空   红   绿   蓝   青   粉   黄   白 */
		rt_int8_t inputdata[8] = {0x00,0x04,0x02,0x01,0x03,0x05,0x06,0x07};
		
		while(i <= 7){
				system_init_led_blink(inputdata[i++]);}
		
		LED_OFF(LED_Red);			//初始化为高电平 【熄灭】
		LED_OFF(LED_Green);			
		LED_OFF(LED_Blue);
				
    while (1)
    {	

				led_blink_task();
				rt_thread_mdelay(500);
    }
}

/* led闪烁任务【系统正常运行指示灯】 */
void led_blink_task(void)
{
		static rt_uint8_t status = 1;
		if(boma_value_get() == 1){
				LED_Turn(LED_Green,status);	//初始化为高电平 【熄灭】
		}
}
	

/* 系统初始化led闪烁状态【显示7种颜色】 -->[颜色节拍表> 空  红  绿  蓝  青  粉  黄  白 ] */
void system_init_led_blink(rt_uint8_t InputData)
{
    if(InputData & 0x04){	
						LED_ON(LED_Red); }
		else{ 	LED_OFF(LED_Red);}
		
	  if(InputData & 0x02){	
					LED_ON(LED_Green); }
		else{ LED_OFF(LED_Green);}
		
		if(InputData & 0x01){	
					 LED_ON(LED_Blue); }
		else{  LED_OFF(LED_Blue);}
		rt_thread_mdelay(300);//等待系统初始化  完毕，系统稳定后，在读取数据
}


int led_thread_init(void)
{
    rt_thread_t led_tid;
		/*创建动态线程*/
    led_tid = rt_thread_create("led",//线程名称
                    led_thread_entry,				 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    512,										 //线程栈大小，单位是字节【byte】
                    15,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (led_tid != RT_NULL){
				rt_pin_mode( LED_Red, 	PIN_MODE_OUTPUT);//设置输出模式	
				rt_pin_mode( LED_Green, PIN_MODE_OUTPUT);	
				rt_pin_mode( LED_Blue, 	PIN_MODE_OUTPUT);	
				log_i("LED_Init()");
				rt_event_send(&init_event, LED_EVENT);
				rt_thread_startup(led_tid);
		}
		return 0;
}
INIT_APP_EXPORT(led_thread_init);


/* led on MSH方法 */
static int led_on(int argc, char **argv)
{
    int result = 0;

    if (argc != 2){
        log_d("Error! Proper Usage: led_on R\n Species:R /G /B \n");
				result = -RT_ERROR;
        goto _exit;
    }
		
		switch(*argv[1]){
				case 'R':LED_ON(LED_Red);break;
				case 'G':LED_ON(LED_Green);break;
				case 'B':LED_ON(LED_Blue);break;
				default:log_d("Error! Proper Usage: led_on R\n Species:R /G /B \n");break;
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(led_on,ag: led_on R  );


/* led off MSH方法 */
static int led_off(int argc, char **argv)
{
    int result = 0;

    if (argc != 2){
        log_d("Error! Proper Usage: led_off R\n Species:R /G /B \n");
				result = -RT_ERROR;
        goto _exit;
    }
		
		switch(*argv[1]){
				case 'R':LED_OFF(LED_Red);break;
				case 'G':LED_OFF(LED_Green);break;
				case 'B':LED_OFF(LED_Blue);break;
				default:log_d("Error! Proper Usage: led_off R\n Species:R /G /B \n");break;
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(led_off,ag:led_off R  );
