#include "include.h"

extern int16 camera_center;
extern int16 camera_last_center; 
extern int16 Servo_Duty_Last;
extern int16 Servo_Duty;
extern int16 deviation;
extern int16 last_deviation;
extern float Cam2Mag_Weight;
extern float S_P;
extern float S_D;
extern float M_L_P;
extern float M_L_I;
extern float M_R_P;
extern float M_R_I;
extern float C_P;
extern float C_D;

extern int16 Servo_Offset;
extern int16 Servo_Turn;

extern int16 Cam_duty;
extern int16 Mag_duty;

extern CAR_STATUS_e car_mode;
extern to_speed speed_expect;

extern uint16 var_left;
extern uint16 var_right;
extern uint16 var_middle;

extern uint16 normal_speed;
extern uint16 min_speed;

extern uint8 BM_L; 
extern uint8 BM_R;
extern uint16 Pulse_Counter_R;
extern uint16 Pulse_Counter_L;
extern int16 Get_SpeedL; //编码器采集到的速度
extern int16 Get_SpeedR;
extern moty_duty run_duty;

extern uint8 Circle_fill_line_flag;
extern ring_missage ring_info;
extern ROAD_STATUS_e road_flag;

extern uint8 Meet_ON_OFF;
extern uint8 Circle_ON_OFF;

extern uint8 straight_go_flag;
extern uint8 Om_turn_flag;

extern int16 camera_integral_center;


extern uint8 real_stop_flag;         //真正停车标志位
extern uint8 ramp_flag;
extern uint8 MSG_out[4]; //通讯发出
extern uint8 last_send_signal; //上次发出信号简写（用于判断是否可以关断发给另一辆车的flag） ring_weight
extern uint8 OK_send_flag;

extern uint8 to_the_side;
extern uint8 circle_point[2];//圆环切入标志角纵坐标0,,横坐标1

extern uint8 Circle_go_flag;
extern uint8 Mag_insurance;



extern uint8 Go_back_flag;
extern uint8 meet_ring_seq;
extern uint8 meet_seq;
extern uint8 meet_zk_count;
extern uint8 send_to_other_car_flag;

extern uint16 ring_size;
extern int16 Servo_dir;
extern int16 Servo_dir_n[20];

extern uint8 Insurance_dir;
extern uint8 Insurance_flag;

extern uint16 add_deviation_cjx;
extern uint8 death_time_flag;

extern uint8 add_deviation;
void servo_control_AD()
{
	//角度控制中心
	//	static uint8 count;
	float fuzzy_p_i;
	float fuzzy_p_j;
	uint8 i=0;
	fuzzy_p_j=(float)(circle_point[1]-80);
	fuzzy_p_i=-(float)(circle_point[0]-30);
	
	
	Servo_Duty_Last=Servo_Duty;
	//打脚
	
	
	
	if(ring_info.ring_turn==1)//left
	{
		Servo_Duty=Servo_Duty_Lf-10; 
		
		FTM_PWM_Duty(FTM1,FTM_CH0,Servo_Duty); 
		return;  
	}
	
	
	if(ring_info.ring_turn==2)//right
	{
		Servo_Duty=Servo_Duty_Rt+10; 
		FTM_PWM_Duty(FTM1,FTM_CH0,Servo_Duty); 
		return;
	}
	
	
	//全局电磁保险
	
	
	
	if(Insurance_flag==1)
	{
		if(Insurance_dir==0)//0left ，1 right
		{		
			
			Servo_Duty=Servo_Duty_Lf-10; 
			FTM_PWM_Duty(FTM1,FTM_CH0,Servo_Duty); 
			return;
		}
		if(Insurance_dir==1)//0left ，1 right
		{		
			
			Servo_Duty=Servo_Duty_Rt+10; 
			FTM_PWM_Duty(FTM1,FTM_CH0,Servo_Duty); 
			return;
		}
	}	
	
	if(to_the_side==1)
	{
		deviation+=add_deviation_cjx;
		Servo_Duty=Servo_Duty_Md-4*deviation;
		if(Servo_Duty>=Servo_Duty_Lf)    {Servo_Duty=Servo_Duty_Lf;}//限制到左打死
		if(Servo_Duty<=Servo_Duty_Rt)    {Servo_Duty=Servo_Duty_Rt;}//限制到右打死
		FTM_PWM_Duty(FTM1,FTM_CH0,Servo_Duty);
		return;
	}
	if(to_the_side==2)
	{
		deviation+=add_deviation_cjx-5;
		Servo_Duty=Servo_Duty_Md-3.5*deviation;
		if(Servo_Duty>=Servo_Duty_Lf)    {Servo_Duty=Servo_Duty_Lf;}//限制到左打死
		if(Servo_Duty<=Servo_Duty_Rt)    {Servo_Duty=Servo_Duty_Rt;}//限制到右打死
		FTM_PWM_Duty(FTM1,FTM_CH0,Servo_Duty);
		return;
	}	
	
	if(meet_seq!=0)
	{	
		if(ring_info.ring_flag==reach_ring_point )  
		{		
			if(camera_integral_center>0)//右
			{
				Servo_Duty=Servo_Duty_Md-Fuzzy(&fuzzy_p_j,&fuzzy_p_i);
			}
			if(camera_integral_center<0)//左
			{
				fuzzy_p_j=-fuzzy_p_j;
				Servo_Duty=Servo_Duty_Md+Fuzzy(&fuzzy_p_j,&fuzzy_p_i);
			}
			FTM_PWM_Duty(FTM1,FTM_CH0,Servo_Duty);
			return;
		}	
	}
	
	
	
	
	Cam_duty= (C_P*camera_center-C_D*(camera_last_center-camera_center));//原始量
	Mag_duty=(S_P*deviation-S_D*(last_deviation-deviation));//原始量
	
	if(abs(Mag_duty)<=100 && straight_go_flag==1){straight_go_flag=2;} 
	
	
	Servo_Offset=Cam2Mag_Weight*Cam_duty+(10-Cam2Mag_Weight)*Mag_duty;//10倍量
	Servo_Offset/=10;
	
	if(abs(Servo_Offset)>=200){Om_turn_flag=1;} else{Om_turn_flag=0;}//欧姆弯降速
	
	
	if(meet_seq!=0)
	{
		//入环保险	
		if(Circle_go_flag==1){Servo_Offset=Servo_Offset<=-30?Servo_Offset:-30;}
		if(Circle_go_flag==2){Servo_Offset=Servo_Offset>=30 ?Servo_Offset:30;}
		//环内电磁保险
		if(Mag_insurance==1&&camera_integral_center<0){Servo_Offset=Servo_Offset<=-20?Servo_Offset:-20;}  //2018.8.10 改大为20  （之前为15）
		if(Mag_insurance==1&&camera_integral_center>0){Servo_Offset=Servo_Offset>= 20?Servo_Offset: 20;}     
	}
	
	
	Servo_Duty=(int16)(Servo_Duty_Md-Servo_Offset+Servo_Turn); 
	
	
	if(meet_ring_seq==5)//锁角//倒开	
	{
		Servo_Duty=(int16)(Servo_Duty_Md+Servo_Offset/2); 
	}
	
	//////////电磁保险↓↓↓↓↓↓↓//
	//	Servo_dir=0;
	//	for(i=0;i<=18;i++)
	//	{Servo_dir_n[i]=Servo_dir_n[i+1];}
	//	
	//	Servo_dir_n[19]=-(Servo_Duty-Servo_Duty_Md)/10;//左负右正
	//	for(i=0;i<=19;i++)
	//	{Servo_dir+=Servo_dir_n[i];}
	//	
	//	if(Servo_dir<0){Insurance_dir=0;}
	//	else if (Servo_dir>=0){Insurance_dir=1;}
	//	
	//	
	//////////电磁保险/↑↑↑↑↑↑↑/
	
	
	if(Servo_Duty>=Servo_Duty_Lf)    {Servo_Duty=Servo_Duty_Lf;}//限制到左打死
	if(Servo_Duty<=Servo_Duty_Rt)    {Servo_Duty=Servo_Duty_Rt;}//限制到右打死	
	
	
	
	FTM_PWM_Duty(FTM1,FTM_CH0,Servo_Duty);
	
	
	
	
	
}


void OUT_STOP()
{
	static uint8 good_game_count=0;
	if(car_mode==run && var_left+var_right<=6 && Mag_insurance==0)
	{
		good_game_count++;
		if(good_game_count==50)
		{
			good_game_count=0;
			car_mode=stop;
			last_send_signal='G';
			MSG_out[0]='M';       
			MSG_out[1]='G';     
			MSG_out[2]='G';       
			MSG_out[3]='.';       
			send_to_other_car_flag=1;
			OK_send_flag=0;
		}
		
	}
	else
	{
		good_game_count=0;
	}
	
}


void FTM_updata(moty_duty duty)
{
	
	if(car_mode==run)
	{
		if(duty.Speed_Duty_R>=0)
		{
			FTM_PWM_Duty(FTM0, FTM_CH0, duty.Speed_Duty_R);
			FTM_PWM_Duty(FTM0, FTM_CH2, 0);
		}
		
		else if(duty.Speed_Duty_R<0)
		{
			FTM_PWM_Duty(FTM0, FTM_CH0, 0);      
			FTM_PWM_Duty(FTM0, FTM_CH2, abs(duty.Speed_Duty_R));
		}      
		
		if(duty.Speed_Duty_L>=0)
		{  
			FTM_PWM_Duty(FTM0, FTM_CH1, 0);
			FTM_PWM_Duty(FTM0, FTM_CH3, duty.Speed_Duty_L);
		}
		else if(duty.Speed_Duty_L<0)
		{  
			FTM_PWM_Duty(FTM0, FTM_CH1, abs(duty.Speed_Duty_L));
			FTM_PWM_Duty(FTM0, FTM_CH3, 0);
		}
		
		
		
	}
	else if(car_mode==stop)
	{
		FTM_PWM_Duty(FTM0, FTM_CH0, 0);
		FTM_PWM_Duty(FTM0, FTM_CH1, 0);
		FTM_PWM_Duty(FTM0, FTM_CH2, 0);
		FTM_PWM_Duty(FTM0, FTM_CH3, 0);
	}
	else if(car_mode==reversal)
	{
		FTM_PWM_Duty(FTM0, FTM_CH0, 0);
		FTM_PWM_Duty(FTM0, FTM_CH1,2000);
		FTM_PWM_Duty(FTM0, FTM_CH2,2000);
		FTM_PWM_Duty(FTM0, FTM_CH3, 0);
	}
	else if(car_mode==meet_reversal)
	{
		if((Get_SpeedL+Get_SpeedR)/2>=0)
		{
			FTM_PWM_Duty(FTM0, FTM_CH0, 0);
			FTM_PWM_Duty(FTM0, FTM_CH1, abs(Get_SpeedL*50)<6000?abs(Get_SpeedL*50):6000);//反左轮
			FTM_PWM_Duty(FTM0, FTM_CH2, abs(Get_SpeedR*50)<6000?abs(Get_SpeedR*50):6000);//反右轮
			FTM_PWM_Duty(FTM0, FTM_CH3, 0);
		}
		
		
		if((Get_SpeedL+Get_SpeedR)/2<=0)
		{
			
			FTM_PWM_Duty(FTM0, FTM_CH0, abs(Get_SpeedR*50)<6000?abs(Get_SpeedR*50):6000);//正左轮
			FTM_PWM_Duty(FTM0, FTM_CH1, 0);
			FTM_PWM_Duty(FTM0, FTM_CH2, 0);
			FTM_PWM_Duty(FTM0, FTM_CH3, abs(Get_SpeedL*50)<6000?abs(Get_SpeedL*50):6000);//正右轮
		}
	}
}


void Counter_collect()
{
	BM_L=gpio_get(PTD10);  //左1   为正转  
	BM_R=gpio_get(PTD12);  //右0   为正转
	
	Pulse_Counter_L =abs(FTM_QUAD_get(FTM2)); //采集时间不能太长
	FTM_QUAD_clean(FTM2);
	Pulse_Counter_R = lptmr_pulse_get();
	lptmr_pulse_clean();
	
	Get_SpeedL=(int16)(Pulse_Counter_L*50*20/1165);   //20cm为轮子一圈距离，1165为轮子一圈脉冲，单位为cm/s
	Get_SpeedR=(int16)(Pulse_Counter_R*50*20/1165);   //单位cm/s
	
	if(BM_L==0)Get_SpeedL=-Get_SpeedL;
	if(BM_R==1)Get_SpeedR=-Get_SpeedR;
	if((Get_SpeedL+Get_SpeedR)/2 <=3)
	{
		real_stop_flag=1;
		
	}
	else {real_stop_flag=0;}
	
	
	if(meet_ring_seq==4)
	{
		meet_zk_count+=((Pulse_Counter_L*20/1165)+(Pulse_Counter_R*20/1165))/2;	
	}
	
	
	
	
}

void road_speed_control()//路况决定预判速、度等
{
	int16 speed_plus=0;
	
	//】】】】】】】】】直道、弯道正常
	//阿克曼角
	//////////预期速度控制中心/////buzzer_once();//////
	
	if(meet_seq!=0&& road_flag==circle)
	{
		if(Circle_ON_OFF==1 )     
		{
			speed_expect.goal_speed_L=140;
			speed_expect.goal_speed_R=140; 
			if(ring_info.ring_flag==reach_ring_point)
			{
				speed_expect.goal_speed_L=120;
				speed_expect.goal_speed_R=120; 
			}
			
		}
		if(ring_size>=500)
		{
			speed_expect.goal_speed_L=180;
			speed_expect.goal_speed_R=180;
		}
		if(death_time_flag==1)
		{
			
			speed_expect.goal_speed_L=normal_speed<200?normal_speed:200;
                        speed_expect.goal_speed_R=normal_speed<200?normal_speed:200;
		}
		Ackerman();//阿克曼角处理速度
		return;
	}
	
	if(ramp_flag==1)     
        {
          speed_plus=-80;
          buzzer_once();
        }
	if(road_flag==straight)
	{
		if(Om_turn_flag==1)     {speed_plus=-30;}
	}
	if(to_the_side!=0)
	{
		speed_plus=-40;
	}
	
	
	
	speed_expect.goal_speed_L=normal_speed+speed_plus;
	speed_expect.goal_speed_R=normal_speed+speed_plus; 
	
	Ackerman();//阿克曼角处理速度
	
	
	
}

void Ackerman()
{
	if(Om_turn_flag==1)
	{
		speed_expect.goal_speed_L-=((Servo_Duty-Servo_Duty_Md)*0.5*(speed_expect.goal_speed_L))/150;
		speed_expect.goal_speed_R+=((Servo_Duty-Servo_Duty_Md)*0.5*(speed_expect.goal_speed_R))/150;
		return;
	}
	else
	{
	speed_expect.goal_speed_L-=((Servo_Duty-Servo_Duty_Md)*0.25*(speed_expect.goal_speed_L))/150;
	speed_expect.goal_speed_R+=((Servo_Duty-Servo_Duty_Md)*0.25*(speed_expect.goal_speed_R))/150;
	}
	//	speed_expect.goal_speed_R=speed_expect.goal_speed_R>0?speed_expect.goal_speed_R:0;
	//	speed_expect.goal_speed_L=speed_expect.goal_speed_L>0?speed_expect.goal_speed_L:0;
	
	
}




void PI_control()
{
	int16 Speed_errL_New=0;
	int16 Speed_errR_New=0;
	
	static int16 Speed_errL_Last=0;
	static int16 Speed_errR_Last=0;
	
	int16 Add_Moty_L;
	int16 Add_Moty_R;
	
	Speed_errL_New = speed_expect.goal_speed_L-Get_SpeedL;
	Speed_errR_New = speed_expect.goal_speed_R-Get_SpeedR;
	
	Add_Moty_L=(int16)(M_L_P*(Speed_errL_New-Speed_errL_Last)+M_L_I*(Speed_errL_New));
	Add_Moty_R=(int16)(M_R_P*(Speed_errR_New-Speed_errR_Last)+M_R_I*(Speed_errR_New));
	Speed_errL_Last = Speed_errL_New;
	Speed_errR_Last = Speed_errR_New;	
	run_duty.Speed_Duty_L+=Add_Moty_L;  
	run_duty.Speed_Duty_R+=Add_Moty_R;
	
	//左边FTM波//限幅	
	if(run_duty.Speed_Duty_L<-6000)              {run_duty.Speed_Duty_L=-6000;}
	if(run_duty.Speed_Duty_L>=max_output)    {run_duty.Speed_Duty_L=max_output;}	
	//右边FTM波//限幅
	if(run_duty.Speed_Duty_R<-6000)               {run_duty.Speed_Duty_R=-6000;}
	if(run_duty.Speed_Duty_R>=max_output)    {run_duty.Speed_Duty_R=max_output;}
	
	
	
	
}


void Go_back(int16 dist)//倒退xx的编码器//自锁
{
	static int16 counter=0;
	
	
	if(Go_back_flag==1)
	{
		car_mode=reversal;
		counter+=((Pulse_Counter_L*20/1165)+(Pulse_Counter_R*20/1165))/2;//编码器获取
		
	}
	
	if(Go_back_flag==1&&(var_left+var_right+var_middle)>=400)//if(counter>=dist) 
	{ 	
		car_mode=meet_reversal;
		counter=0;
		Go_back_flag=2;
	}
	
	
}






