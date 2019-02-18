#include "common.h"
#include "include.h"

extern uint8  *ov7725_eagle_img_buff;
extern uint8 imgbuff[CAMERA_SIZE];
extern uint8 img[CAMERA_H][CAMERA_W];
extern CAR_STATUS_e car_mode;

extern int16 Get_SpeedL;
extern int16 Get_SpeedR;

extern uint8 camera_open_flag;
extern moty_duty run_duty;

extern uint8 buzzer_time;
extern uint8 buzzer_count;
extern uint8 buzzer_number;

extern uint8 edge_sum_left;
extern uint8 edge_sum_right;
extern uint16 edge_black[2];
extern int16 Servo_Duty;
extern uint8 meet_flag;

extern uint8 system_clock;
extern uint8 camera_times;

extern uint8 send_picture[CAMERA_SIZE];
extern uint8 sending_flag;

extern uint8 Circle_ON_OFF;
extern uint8 Meet_ON_OFF;
extern uint8 Meet_Action_flag;
extern uint8 FootWalk_ON_OFF;   //斑马线开关

extern uint8 check_send_flag;
extern MEET_PLACE_e meet_choose;
extern uint8 meet_ring_number;
extern uint8 ring_pass_number;

extern int16 ramp_count;
extern uint8 delay_run_flag;
extern uint8 one_car_mode;

extern  ROAD_STATUS_e road_flag;

void PORTA_IRQHandler()
{
	
	uint8 n=24;
	if(PORTA_ISFR & (1<<n))  
	{
		PORTA_ISFR |=1<<n;         //清除VSYNC标志位
		disable_irq(PORTA_IRQn);   //关闭中断
		PORTA_ISFR = 1<<PTA19;      //清除PCLK标志位
		DMA_IRQ_CLEAN(DMA_CH4);
		DMA_EN(DMA_CH4);
		DMA_IRQ_EN(DMA_CH4);
		DMA_DADDR(DMA_CH4) =(uint32)ov7725_eagle_img_buff;
		PORTA_ISFR = 1<<PTA19;       
	}
}

void DMA4_IRQHandler()
{
	
	DMA_IRQ_CLEAN(DMA_CH4);
	DMA_IRQ_DIS(DMA_CH4);
	DMA_DIS(DMA_CH4);
	
	img_extract(img, imgbuff, CAMERA_SIZE);             //解压
	
	/*初级检测处理*/
	
	cover_image();//覆盖前10行//前瞻70—80cm  
	find_edge();//扫边沿//入线start行数  
	find_circle_point();
	/*中心线优化*/
	
	/*坡道处理*/

	get_white_proportion();
	Edge_sum();//求黑点 
	/*算法*/
	Camera_center();   //中心线偏差 
	/*圆环处理算法*/
	if(Circle_ON_OFF==1)  
	{
		// Edge_sum();//黑边，圆环用//需要优化       
		Circle_deal_last();
		//    magnetic_circle();
	}
	
	if(FootWalk_ON_OFF==1)
	{
		FootWalk_scanning();//垂直扫描
	}
	
	/* 汇车处理算法  */
	if(meet_choose!=no_meet)
	{
		First_Meet();
		if(meet_choose==meet_straight && Meet_ON_OFF==1)
		{
			Smoothness_Judge();
			variance_judge_left();//左边线  方差判断   
			variance_judge_right();//右边线  方差判断  

			sunken_edge();
			if(one_car_mode==0)
			{
				if(car_mode!=stop)  //停车状态不识别会车区
				{
					Straight_Meet_Judge();//汇车判断
				}				
			}
		}
	}
	else {delay_run_flag=1;}
	if(Meet_Action_flag==1 )
	{
		Meet_Action();//汇车逻辑
	}
	
	
	/*辅助函数*/
	//	dma_count();
        variance_judge_center();//中线  方差判断   
	draw_center_line();
	draw_car_line();//画车位线  //直道加速
	Three_line_mode();
	
	/***************************/
	img_compress(imgbuff, img, CAMERA_SIZE);  
	if(sending_flag==0)
	{
		memcpy(send_picture,imgbuff,CAMERA_SIZE);
	}
	
	PORTA_ISFR  |= (1 << 24);
	enable_irq(PORTA_IRQn);
	
	
	
	
}


void PIT0_IRQHandler()
{
	if(boma5==1)
	{
		buzzer_ring();
	}
	if(boma3==0)
	{
		check_send_flag=0;
		
	}
        if(check_send_flag==0)
	{
                Communication_Out();
	}
	
	PIT_Flag_Clear(PIT0);
}


void PIT1_IRQHandler()//电磁中断//5ms采集10次
{
	
	//基础函数
	time_count();
	ad_collect();
//        if(road_flag==circle)
//        {
//          Insurance_Mag();
//        }
	OUT_STOP();
	//角度处理  
	if(Circle_ON_OFF==1)
	{
		magnetic_circle();
	}
	
	//角度上传  
	servo_control_AD();
	
	
	PIT_Flag_Clear(PIT1);//清中断标志位
	
	
}



void PIT2_IRQHandler()//pi中断10ms
{
        if(ramp_count>0){ramp_count--;}
	
	Counter_collect();//编码器采集  
	if(Meet_ON_OFF==1 && meet_choose==meet_circle)
	{
		circle_reverse();
	}
	road_speed_control(); //路型变换期望速度
	

        Go_back(50);//flag1飞倒车0结束
        
	Out_Straight_Meet();//出回车去
	PI_control();    

	if(delay_run_flag==1)//延时上传
	{
		FTM_updata(run_duty);//速度传到电机 
	}
	
	PIT_Flag_Clear(PIT2);//清中断标志位
	
	
	
	
}


void UART3_IRQHandler()
{
	Communication_In();
}



void HardFault()
{
	car_mode=error;
	printf("hardfault");
}