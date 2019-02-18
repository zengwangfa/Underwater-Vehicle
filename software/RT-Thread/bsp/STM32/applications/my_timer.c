#include "my_init.h"
#include "board.h" 

/* 定时器的控制块 */
static rt_timer_t timer1;
static rt_timer_t timer2;

extern rt_uint16_t WASHING_count;                             //定时器清洗，计数值
extern rt_uint16_t HEAT_count;                                //定时加热，计数值
extern rt_uint16_t FEED_count;                                //定时喂食，计数值
extern rt_uint8_t STEER_START_RUN_FLAG;              //舵机开始运转标志位,0表示停止运转，1表示开始运转
extern bool          HEAT_START_FLAG;                   //开始控制加热丝加热标志位
extern bool          FEED_START_FLAG;                   //开始喂食蜂箱标志位
extern bool          WASHING_START_FLAG;                //开始清洗蜂箱标志位
extern rt_uint16_t Washing_Time;                      //清洗时间，由上位机给定
extern rt_uint16_t Heat_Time;                         //加热时间，由上位机给定，单位秒
extern rt_uint16_t Feed_Time;                         //喂食时间，由上位机给定，单位秒




void Timing_Count(void);


static void timeout1(void* parameter)// 定时器1超时函数
{
   //rt_kprintf("periodic timer is timeout\n");

	turn_led();
	Timing_Count(); //设定设备（继电器、MOS管）超时保护 [若未接受到上午机的定时关闭指令，自己的定时系统到达相应的设定时间时也自动关闭]


}

static void timeout2(void* parameter)  
{

	buzzer_ring();//蜂鸣器

}





void Timing_Count(void) //单片机内部定时器  确保服务器掉线后，也能定时关闭 功能状态  超时保护
{
			if(WASHING_START_FLAG==TRUE)
		  {
				WASHING_count++;
			  if(WASHING_count>=(Washing_Time*10))               //清洗时间到停止清洗【超时】
			   {
						WASHING_count=0;
						Washing_Time=0;                                //清洗完成，清洗时间清0
						WASHING_START_FLAG=FALSE;
					  //rt_kprintf("Water_OFF!\n");
		      }
	   	}
			else {
					rt_pin_write(Watering_PIN, PIN_LOW);//关断抽水电机
			}
			
			if(HEAT_START_FLAG==TRUE)
			{
		    	HEAT_count++;
				  if(HEAT_count>=(Heat_Time*10))                 //加热丝加热时间到停止加热【超时】
					{
						HEAT_count=0;                                //加热时间计数值清0
						Heat_Time=0;                                 //加热时间清0
						HEAT_START_FLAG=FALSE;                       //开始加热标志位清0
						//rt_kprintf("Heat1_OFF!\n");rt_kprintf("Heat2_OFF!\n");
					}
			}
			else {
				  rt_pin_write(Heat1_PIN, PIN_LOW);//关断加热MOS
			   	rt_pin_write(Heat2_PIN, PIN_LOW);
					rt_pin_write(LED3, PIN_LOW);
			}
			if(FEED_START_FLAG==TRUE)
			{
		    	FEED_count++;
				  if(FEED_count>=(Feed_Time*10))                 //喂食开启时间到停止时间【超时】
					{
						Feed_Time=0;                                 //喂食时间清0
					  FEED_count=0;                                //喂食时间计数值清0
						FEED_START_FLAG=FALSE;                       //开始喂食标志位清0
					}
			}
			else {
					rt_pin_write(Feeding_PIN, PIN_LOW);//关断电磁阀  
			}
}


int my_timer_init(void)
{
    /* 创建定时器1 */
    timer1 = rt_timer_create("timer1",  /* 定时器名字是 timer1 */
                        timeout1, /* 超时时回调的处理函数 */
                        RT_NULL,  /* 超时函数的入口参数 */
                        10,       /* 定时长度，以OS Tick为单位，即10个OS Tick   --> 10MS*/  
                        RT_TIMER_FLAG_PERIODIC); /* 周期性定时器 */
    /* 启动定时器 */
    if (timer1 != RT_NULL) rt_timer_start(timer1);


	
	
	    /* 创建定时器1 */
    timer2 = rt_timer_create("timer2",  /* 定时器名字是 timer1 */
                        timeout2, /* 超时时回调的处理函数 */
                        RT_NULL,  /* 超时函数的入口参数 */
                        1,       /* 定时长度，以OS Tick为单位，即10个OS Tick   --> 10MS*/  
                        RT_TIMER_FLAG_PERIODIC); /* 周期性定时器 */
    /* 启动定时器 */
    if (timer2 != RT_NULL) rt_timer_start(timer2);

    return 0;
}
INIT_APP_EXPORT(my_timer_init);

