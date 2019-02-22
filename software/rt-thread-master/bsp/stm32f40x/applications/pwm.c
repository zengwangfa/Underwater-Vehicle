#include "init.h"
/*---------------------- Constant / Macro Definitions -----------------------*/		

u32 	FLASH_SIZE=16*1024*1024;	//FLASH 大小为16字节

/*----------------------- Variable Declarations -----------------------------*/

extern struct rt_event init_event;/* ALL_init 事件控制块. */

struct  /* 定义u8 为方便 FLASH读写，使用时需要乘以 10 得到 真实PWM值 */
{
		u8 open_value;		//机械臂 打开的PWM值 
		u8 close_value;	 //机械臂 关闭的PWM值
}servo_motor;

/*----------------------- Function Implement --------------------------------*/

void pwm_thread_entry(void *parameter)
{
	
		while(1)
		{
				TIM_SetCompare1(TIM1,2000);  		//最高转速信号   	水平推进器1号
				TIM_SetCompare2(TIM1,4000);  		//最高转速信号    水平推进器2号
				TIM_SetCompare3(TIM1,6000); 		//最高转速信号    水平推进器3号
				TIM_SetCompare4(TIM1,10000);  	//最高转速信号    水平推进器4号
				rt_thread_mdelay(100);

		}
	
}


int pwm_thread_init(void)
{
    rt_thread_t pwm_tid;
		/*创建动态线程*/
    pwm_tid = rt_thread_create("pwm",//线程名称
                    pwm_thread_entry,				 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    512,										 //线程栈大小，单位是字节【byte】
                    10,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (pwm_tid != RT_NULL){
				TIM1_PWM_Init(20000-1,168-1);	//168M/168=1Mhz的计数频率,重装载值20000，所以PWM频率为 1M/20000=50Hz.  

				LOG_I("pwm_init()");

				rt_thread_startup(pwm_tid);
				rt_event_send(&init_event, PWM_EVENT); //发送事件  表示初始化完成
		}

		return 0;
}
INIT_APP_EXPORT(pwm_thread_init);



/*【机械臂】舵机 修改 【开启值】MSH方法 */
static int servo_motor_openvalue_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        rt_kprintf("Usage: servo_motor_OpenValue_set 160\n");
				result = -RT_ERROR;
        goto _exit;
    }
		servo_motor.open_value = atoi(argv[1]);
		
		W25QXX_Write(&servo_motor.open_value,0,1);		//从0个地址处写入数据
		rt_kprintf("Write_Successed! Current ser_OpenValue:  %d\n",servo_motor.open_value);
_exit:
    return result;
}
MSH_CMD_EXPORT(servo_motor_openvalue_set,ag: servo_motor_OpenValue_set 160);




/*【机械臂】舵机 修改 【关闭值】 MSH方法 */
static int servo_motor_closevalue_set(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        rt_kprintf("Usage: servo_motor_CloseValue_set 115\n");
				result = -RT_ERROR;
        goto _exit;
    }
		servo_motor.close_value = atoi(argv[1]);

		W25QXX_Write(&servo_motor.close_value,1,1);	//从1个地址处写入数据
		
		rt_kprintf("Write_Successed! Current ser_CloseValue:  %d\n",servo_motor.close_value);
_exit:
    return result;
}
MSH_CMD_EXPORT(servo_motor_closevalue_set,ag: servo_motor_CloseValue_set 115);




/* list 相关重要参数 */
void list_value(void)
{
		W25QXX_Read(&servo_motor.open_value,0,1);		// 地址0
		W25QXX_Read(&servo_motor.close_value,1,1);	// 地址1
	
		rt_kprintf("variable name    value\n");
    rt_kprintf("--------------  ---------\n");
	  rt_kprintf("ser_OpenValue  	 %d\n",servo_motor.open_value);
	  rt_kprintf("ser_CloseValue   %d\n",servo_motor.close_value);
    rt_kprintf("                         \n");
}
MSH_CMD_EXPORT(list_value,list some important values);



//TIM1 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数

void TIM1_PWM_Init(u32 arr,u32 psc)
{		 					 
	//此部分需手动修改IO口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//使能PORTE时钟	
	
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource9, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;//GPIOE
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //初始化PF9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器14
	
	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_Cmd(TIM1, ENABLE);  //使能TIM14
}  


///*---------------------- Constant / Macro Definitions -----------------------*/

//#define TIM1_CH1_PIN 		60 //	E9
//#define TIM1_CH2_PIN 		64 //	E11
//#define TIM1_CH3_PIN 		66 //	E13
//#define TIM1_CH4_PIN 		67 //	E14

//#define PWM_DEV_NAME        "pwm"  /* PWM设备名称 */
//#define PWM_DEV_CHANNEL     1       /* PWM通道 */


///*----------------------- Variable Declarations -----------------------------*/

//struct rt_device_pwm *pwm_dev;      /* PWM设备句柄 */
///*----------------------- Function Implement --------------------------------*/

//void pwm_thread_entry(void *parameter)
//{
//		rt_uint32_t period = 50000, pulse = 0;
//	
//    rt_pin_mode(TIM1_CH1_PIN, PIN_MODE_OUTPUT);
//	  rt_pin_mode(TIM1_CH2_PIN, PIN_MODE_OUTPUT);
//	  rt_pin_mode(TIM1_CH3_PIN, PIN_MODE_OUTPUT);
//	  rt_pin_mode(TIM1_CH4_PIN, PIN_MODE_OUTPUT);
//	
//		/* 查找设备 */
//    pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
//	  if (pwm_dev == RT_NULL)
//    {
//        rt_kprintf("pwm sample run failed! can't find %s device!\n", PWM_DEV_NAME);
//    }

//		/* 设置PWM周期和脉冲宽度默认值 */
//    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, 	 period, pulse);
//		rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL+1, period, pulse);
//		rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL+2, period, pulse);
//		rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL+3, period, pulse);
//	
//    /* 使能设备 */
//    rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL	);
//		rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL+1);
//		rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL+2);
//		rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL+3);
//    while (1)
//    {	
//				rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, 20000);
//				rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, 40000);
//				rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, 60000);
//				rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, 80000);
//			
//				rt_thread_mdelay(500);
//    }
//}





