#include "include.h"
/*******************************************
* 功    能：直道会车识别
* 触    发：当为直道(中线方差小) && 边线方差相对大 && 边线平滑度相对大  && 边线凹陷点较多
********************************************/

//--------------------通讯--------------------//
extern uint8 CMD_in[5];  //通讯接收
extern uint8 MSG_in[4];  //通讯接收
extern uint8 MSG_out[4]; //通讯发出
extern uint8 last_send_signal; //上次发出信号简写（用于判断是否可以关断发给另一辆车的flag）
extern uint8 OK_send_flag;
extern uint8 send_to_other_car_flag; //开关向外发送的flag
extern MEET_CAR_NUMBER_e car_number; //前车后车标志位
extern uint8 recv_data_p;

extern uint8 to_the_side;
extern CAR_STATUS_e car_mode;  //车子状态
extern uint8 edge[60][2];
extern uint8 center_line[60];
extern uint8 Cam2Mag_Weight;

extern uint8 img[CAMERA_H][CAMERA_W];

extern uint16 Servo_Duty;
//************************会车区************************//

//---------------------平滑度-------------------//
extern int16 Servo_Turn;
extern uint16 smoothness_left;
extern uint16 smoothness_right;
extern uint16 smoothness_sum;

//---------------------左线方差-------------------//
extern uint16 meet_edge_left[19];  //区边线数组
extern uint32 variance_sum_left;   //方差样本累加量
extern uint16 variance_aver_left;  //样本平均数
extern uint16 variance_left;       //方差

//---------------------右线方差-------------------//
extern uint16 meet_edge_right[19];//会车区边线数组
extern uint32 variance_sum_right;   //方差样本累加量
extern uint16 variance_aver_right;  //样本平均数
extern uint16 variance_right;       //方差

//---------------------中线方差-------------------//
extern uint16 variance_sum_center;   //方差样本累加量
extern uint16 variance_aver_center;  //样本平均数
extern uint16 variance_center;       //方差

//---------------------边线凹陷-----------------//
extern uint16 corner_dian_r;
extern uint16 corner_dian_l;
extern uint16 corner_meet_rn;
extern uint16 corner_meet_ln;

//---------------------距离-----------------//
extern uint16 Pulse_Counter_R;
extern uint16 Pulse_Counter_L;
extern uint16 Length_L;  //计算长度
extern uint16 Length_R;
extern uint16 long_count_cjx;

//---------------------标志位及开关-----------------//
extern int16 Servo_Turn;
extern uint8 meet_flag;         //会车状态位
extern int16 meet_turn;  

extern uint8 Meet_ON_OFF;       //会车总开关
extern uint8 Meet_Action_flag;  //会车标志位
extern uint8 real_stop_flag;     //真正停车标志位

extern uint8 FootWalk_Flag;      //斑马线标志位
extern uint8 FootWalk_ON_OFF;   //斑马线开关
extern uint8 WhiteDot_Count;  //斑马线黑黑黑白白白起始白点
extern uint8 WhiteDot_CountLn;  //斑马线黑黑黑白白白起始白点


//---------------------圆环会车标志位及开关-----------------//

extern uint16 var_left;
extern uint16 var_right;
extern uint16 var_middle;
extern ROAD_STATUS_e road_flag;
extern uint8 delay_run_flag;
extern uint8 one_car_mode;
extern uint8 first_meet_flag;
extern int16 camera_integral_center;
extern MEET_PLACE_e meet_choose;
extern uint8 zz_flag;
extern uint8 meet_seq;
extern uint8 polarization_ON_OFF; 
//---------------------------会车--------------------------------//
/*******************************************
* 函 数 名：First_Meet
* 功    能：起点发车动作
* 输入参数：none
* 返 回 值：none
* 目    的：关闭斑马线与会车区识别，延时打脚发车
********************************************/
void First_Meet()   //第一次(发车)会车   延时会车
{
	static uint16 delay_count=0;//延时4s发车
	static uint8 first_meet_count=0;
	static uint8 first_run_flag=0;
	static uint8 MEET_STATUS=0;
	if(Meet_ON_OFF==1){MEET_STATUS=1;}
	if(first_meet_flag==0 && car_mode==run)
	{
		if(delay_run_flag==0)
		{
			delay_count++;
			LED_ALL_ON();
			last_send_signal='R';
			MSG_out[0]='M';
			MSG_out[1]='R';
			MSG_out[2]='R';
			MSG_out[3]='.';
			buzzer_bibi(3,2);
			send_to_other_car_flag=1;//发送开启flag
			OK_send_flag=0;
		}
		if(delay_count>=100)
		{
			delay_run_flag=1;
		}
		if(delay_run_flag==1)
		{
			LED_ALL_OFF();
			to_the_side=1; 
			first_meet_count++;
			if(first_run_flag==0)//给另一辆车发送RUN
			{
				first_run_flag=1;
			}
			if(first_meet_count<=40)
			{
				Cam2Mag_Weight=0;  //关闭摄像头
				Meet_ON_OFF=0;     //强制关闭会车
				FootWalk_ON_OFF=0;
			}
			else if(first_meet_count>40 && first_meet_count<150)//等完全出起点会车区  防误判
			{
				Cam2Mag_Weight=flash_read(servo_sector , 76, uint8)+((float)(flash_read(servo_sector , 80, uint8))*(0.1));;  //开启摄像头
				to_the_side=0;
				Meet_ON_OFF=0;     //强制关闭会车
				FootWalk_ON_OFF=0;
			}
			else 
			{
				if(MEET_STATUS==1){Meet_ON_OFF=1;}
				else {Meet_ON_OFF=0;}
				first_meet_flag=1;//一生只用一次
				to_the_side=0;
				Cam2Mag_Weight=flash_read(servo_sector , 76, uint8)+((float)(flash_read(servo_sector , 80, uint8))*(0.1));;  //开启摄像头 
			}
		}
		
	}
	
}
/*******************************************
* 函 数 名：Straight_Meet_Judge
* 功    能：直道会车区判断
* 输入参数：none
* 返 回 值：none
* 目    的：判断直道会车区，消抖防止误判
********************************************/
void Straight_Meet_Judge()
{
	static uint8 Meet_Action_flag_count=0;
	if(variance_center<=50)    //接近直道
	{
		if(variance_left>critical_val && variance_left<500 &&variance_right>critical_val && variance_right<500)   
		{
			if(smoothness_left>critical_val && smoothness_left<400 && smoothness_right>critical_val &&smoothness_right<400&&smoothness_sum<800)  //直道与直道会车区的  区别
			{   
				if(corner_meet_rn>=2 && corner_meet_rn<=5 && corner_meet_ln>=2 && corner_meet_ln<=5)
				{
					Meet_Action_flag_count++;
					if(Meet_Action_flag_count>=2)  //消抖 多次防误判
					{
						Meet_Action_flag=1;
						Meet_Action_flag_count=0;
					}
				}
				else{Meet_Action_flag=0;}//清零
			}
			else{Meet_Action_flag=0;Meet_Action_flag_count=0;}
		}
		else{Meet_Action_flag=0;Meet_Action_flag_count=0;}
	}
}

/*******************************************
* 函 数 名：Meet_Action
* 功    能：直道会车区判断
* 输入参数：none
* 返 回 值：none
* 目    的：判断直道会车区，并消抖防止误判
********************************************/
void Meet_Action()
{
	if(car_number==un_know  && meet_flag==0)//为  前车时检测到 停止
	{
                Cam2Mag_Weight=0;
		gpio_set (PTC0, 0);    // LED点亮
		last_send_signal='A';
		MSG_out[0]='M';        //给B车发送已经到达
		MSG_out[1]='A';        //给B车发送已经到达65
		MSG_out[2]='A';        //给B车发送已经到达65
		MSG_out[3]='.';        //给B车发送已经到达
		send_to_other_car_flag=1;
		OK_send_flag=0;
		car_number=front_car;  //变为前车
                
                if(polarization_ON_OFF==1)  //当开关打开  开启偏置
                {
                  to_the_side=1;
                }
		car_mode=meet_reversal;      //停车等待
		Meet_ON_OFF=0;         //关闭直到会车判断
	}
	
	if(car_number==after_car && meet_flag==0)
	{ 
                Cam2Mag_Weight=0;
		gpio_set (PTC0, 0);    // LED点亮
		car_mode=meet_reversal;
		meet_flag=1;    
		Meet_ON_OFF=0;      //关闭直到会车判断
	}
	
}

/*******************************************
* 函 数 名：Out_Straight_Meet
* 功    能：出直道会车区动作
* 输入参数：none
* 返 回 值：none
* 目    的：编码器记距离，大概0.8M，打脚回正 
********************************************/
void Out_Straight_Meet()// 出了知道会车区，提示后车启动  编码器采集距离 
{
  static uint8 clear_zero_flag=0;
	//--------------编码器采集 到大概要出中间会车区  meet_flag 置为2 ----------//
	if(meet_flag==1 && meet_choose==meet_straight)  // 后车出会车区
	{
		Length_L+=(Pulse_Counter_L*20/1165);
		Length_R+=(Pulse_Counter_R*20/1165);
		Cam2Mag_Weight=0;	
		if(car_number==after_car )  // 提示后车一起走
		{
			if(real_stop_flag==1)
			{
				last_send_signal='B';
				MSG_out[0]='M';        //给B车发送已经到达
				MSG_out[1]='B';        //给B车发送已经到达
				MSG_out[2]='B';        //给B车发送已经到达
				MSG_out[3]='.';        //给B车发送已经到达
				send_to_other_car_flag=1;
				OK_send_flag=0;
				car_mode=run;  //启动
			}
			to_the_side=1;
		}
		
		if(((Length_R+Length_L)/2) >=90)
		{
			to_the_side=0;
			buzzer_bibi(3,3);
			Cam2Mag_Weight=flash_read(servo_sector , 76, uint8)+((float)(flash_read(servo_sector , 80, uint8))*(0.1));  //摄像头
			Length_L=0; 
			Length_R=0;  //记距离清零
			meet_flag=2;  //    用于终点会车区用 
			FootWalk_ON_OFF=1;   //打开斑马线
		}  
	}  
	
	//--------------编码器采集 到大概要出终点会车区  meet_flag 置为5 ----------//
	if(meet_flag==4)  // 
	{
          
            if(clear_zero_flag==0)
            {
              Length_L=0;
              Length_R=0;
              clear_zero_flag=1;   
            }
		Length_L+=(Pulse_Counter_L*20/1165);
		Length_R+=(Pulse_Counter_R*20/1165);
		Cam2Mag_Weight=0;
		if(car_number==after_car)  // 前车提示后车一起走
		{
			car_mode=run;  //启动  //关闭摄像头
		}
		if(((Length_R+Length_L)/2) >long_count_cjx)    
		{
			to_the_side=0;
			Cam2Mag_Weight=flash_read(servo_sector , 76, uint8)+((float)(flash_read(servo_sector , 80, uint8))*(0.1));  //摄像头
			buzzer_bibi(3,3);
		}
		if(((Length_R+Length_L)/2) >130)    
		{
			to_the_side=0;
			meet_flag=5;  //    用于终点会车区用 
			car_mode=meet_reversal;  
			
		}   
	}
}

/*******************************************
* 函 数 名：sunken_edge
* 功    能：扫描边线凹陷
* 输入参数：none
* 返 回 值：none
* 目    的：直道会车区有明显凹陷跳变点，防止与其它元素误判 
********************************************/
void sunken_edge()   //边线凹陷
{
	corner_meet_ln=0;
	corner_meet_rn=0;
	corner_dian_r=0;
	corner_dian_l=0;
	//-----------------------------数会车区横线点----------------------------------//
	for(int i =0; i<19; i++)   //
	{
		corner_dian_r=abs(meet_edge_right[i+1]-meet_edge_right[i]);
		
		if(corner_dian_r>=6)
		{corner_meet_rn++;}
		
		corner_dian_l=abs(meet_edge_left[i]-meet_edge_left[i+1]);
		
		if(corner_dian_l>=6)
		{corner_meet_ln++;}	
	}
	//-----------------------------数会车区横线点----------------------------------//
}

/*******************************************
* 函 数 名：variance_judge_center
* 功    能：中线方差
* 输入参数：none
* 返 回 值：none
* 目    的：判断是否大概为直道 
********************************************/
void variance_judge_center()//中线方差判断
{
	uint8 n=17;     //样本个数
	variance_sum_center=0;   //方差样本累加量
	variance_aver_center=0;  //样本平均数
	variance_center=0;       //方差
	for(int i=13;i<30;i++)   //有效判断区域   //  >覆盖区<   >13-30(有效区)<   >丢线区<
	{
		variance_sum_center+=center_line[i];  //样本累加
	}
	variance_aver_center = variance_sum_center/n;  //求取平均 
	for(int i=13;i<30;i++)
	{
		variance_center+=(  (abs(center_line[i]-variance_aver_center))  *  (abs(center_line[i]-variance_aver_center)) );
	}
	variance_center/=(n);
	
}

/*******************************************
* 函 数 名：Smoothness_Judge
* 功    能：边线平滑度判断
* 输入参数：none
* 返 回 值：none
* 目    的：判断边线是否粗糙
********************************************/
void Smoothness_Judge()//相对平滑度
{ 
	uint16 temp_l=0,temp_r=0;
	smoothness_left=0;
	smoothness_right=0;
	smoothness_sum=0;
	for(int i=11;i<=30;i++)   //有效判断区域
	{
		if(edge[i][0]!=0)
		{		
			temp_l=abs(edge[i][0]-edge[i+1][0]);
		}
		if(edge[i][1]!=159)
		{
			temp_r=abs(edge[i][1]-edge[i+1][1]);
		}
		smoothness_left +=(temp_l*temp_l);  
		smoothness_right+=(temp_r*temp_r);
	}
	smoothness_sum=smoothness_left+smoothness_right;
}

void  variance_judge_left()//左边线  方差判断   
{
	uint8 n=19;               //样本个数
	
	variance_sum_left=0;   //方差样本累加量
	variance_aver_left=0;  //样本平均数
	variance_left=0;       //方差
	for(int i=11;i<30;i++)   //有效判断区域   //  >覆盖区<   >11-30(有效区)<   >会车丢线区<
	{
		meet_edge_left[i-11]=edge[i][0];  //数据降维
		// if(meet_edge_left[i-13]==0){n--;}
	}
	
	for(int i=11;i<30;i++)   //有效判断区域   //  >覆盖区<   >11-30(有效区)<   >会车丢线区<
	{
		variance_sum_left+=meet_edge_left[i-11];  //样本累加
	}
	variance_aver_left = variance_sum_left/n;
	
	for(int i=0;i<19;i++)
	{
		variance_left+=(  (abs(meet_edge_left[i]-variance_aver_left)) * (abs(meet_edge_left[i]-variance_aver_left))  );
	}
	// variance_left-=((variance_aver_left*variance_aver_left)*(17-n));//减去坏值
	variance_left/=(n);
	
}


void  variance_judge_right()//右边线  方差判断   
{
	uint8 n=19;               //样本个数
	
	variance_sum_right=0;   //方差样本累加量
	variance_aver_right=0;  //样本平均数
	variance_right=0;       //方差
	
	for(int i=11;i<30;i++)   //有效判断区域   //  >覆盖区<   >11-30(有效区)<   >丢线区<
	{
		meet_edge_right[i-11]=edge[i][1];  //数据降维
		//  if(meet_edge_right[i-13]==79){meet_edge_right[i-13]=0;n--;}
	}
	
	for(int i=11;i<30;i++)   //有效判断区域   //  >覆盖区<   >11-30(有效区)<   >丢线区<
	{
		variance_sum_right+=meet_edge_right[i-11];  //样本累加
	}
	variance_aver_right = variance_sum_right/n;//样本平均
	
	for(int i=0;i<19;i++)
	{
		variance_right+=(  (abs(meet_edge_right[i]-variance_aver_right)) * (abs(meet_edge_right[i]-variance_aver_right))  );//差值的平方
	}
	// variance_right-=(  ((variance_aver_right)*(variance_aver_right)) * (17-n));//减去坏值
	variance_right/=(n);
	
	
	
}

/*******************************************
* 函 数 名：FootWalk_scanning
* 功    能：垂直扫描 斑马线跳变  
* 输入参数：none
* 返 回 值：none
* 目    的：扫描斑马线  完成终点会车
********************************************/
void FootWalk_scanning()
{
	uint8 i=0,j=0;   //加宽版本
	for(i=45;i>=11;i--)
	{
                WhiteDot_Count=0;//--
		WhiteDot_CountLn=0;
		for(j=40;j<120;j++)
		{
			if(img[i][j]==0 && img[i][j+1]==0 && img[i][j+2]==0 && img[i][j+3]==1 && img[i][j+4]==1 && img[i][j+5]==1 )  //嘿嘿嘿白白白
			{
				WhiteDot_Count++;  //斑马点
				j+=4;
			}
                   
			if(WhiteDot_Count>=6)
			{
                                WhiteDot_CountLn++; //斑马行
				gpio_set (PTA17, 0);    // LED点亮
				//FootWalk_Flag=1;
			}
                        
                        //--
                        if(WhiteDot_CountLn>=3)   //超过两行为正确识别
                        {
                          FootWalk_Flag=1;
                          buzzer_once();
                        }
                        //--
		} 
	}
          //边线方案edge[i][0]+edge[i][1]  从边线扫描
        /*uint8 i=0,j=0; 
	for(i=50;i>=11;i--)
	{
		WhiteDot_CountLn=0;
		for(j=edge[i][0];j<edge[i][1];j++)
		{
			if(img[i][j]==0 && img[i][j+1]==0 && img[i][j+2]==0 && img[i][j+3]==1 && img[i][j+4]==1 && img[i][j+5]==1 )  //嘿嘿嘿白白白
			{
				WhiteDot_CountLn++;
				j+=4;
			}
			if(WhiteDot_CountLn>=5)
			{
				gpio_set (PTA17, 0);    // LED点亮
				FootWalk_Flag=1;
                                buzzer_once();
			}
		} 
	}*/
             
        /*  //原始函数
        uint8 i=0,j=0; 
	for(i=18;i>=11;i--)
	{
		WhiteDot_CountLn=0;
		for(j=40;j<120;j++)
		{
			if(img[i][j]==0 && img[i][j+1]==0 && img[i][j+2]==0 && img[i][j+3]==1 && img[i][j+4]==1 && img[i][j+5]==1 )  //嘿嘿嘿白白白
			{
				WhiteDot_CountLn++;
				j+=4;
			}
			if(WhiteDot_CountLn>=6)
			{
				gpio_set (PTA17, 0);    // LED点亮
				FootWalk_Flag=1;
			}
		} 
	}
        */
	
	if(FootWalk_Flag == 1&& one_car_mode==0)
	{
		Cam2Mag_Weight=0;
		if(meet_flag==2)   //谁最后到  终点会车区   END结束
		{ 
			gpio_set (PTC0, 1);    // LED熄灭
			last_send_signal='C';
			MSG_out[0]='M';        //给B车发送已经到达
			MSG_out[1]='C';        //给B车发送已经到达67
			MSG_out[2]='C';        //给B车发送已经到达67
			MSG_out[3]='.';        //给B车发送已经到达
			send_to_other_car_flag=1;
			OK_send_flag=0;
                        Cam2Mag_Weight=0;
			car_number=front_car;       //变为前车
			to_the_side=2;
			car_mode=meet_reversal;
			meet_flag=3;
		}
		
		if(car_number==after_car && meet_flag==3)//后车到达终点会车区
		{ 
			gpio_set (PTC0, 1);  // LED熄灭
			last_send_signal='D';
			MSG_out[0]='M';        //给B车发送已经到达
			MSG_out[1]='D';        //给B车发送已经到达67
			MSG_out[2]='D';        //给B车发送已经到达67
			MSG_out[3]='.';        //给B车发送已经到达
			send_to_other_car_flag=1;
			OK_send_flag=0;
                        Cam2Mag_Weight=0;
			meet_flag=4;
			to_the_side=2;
			car_mode=stop;
		}         
	}
	if(FootWalk_Flag == 1&& one_car_mode==1)
	{
		Length_L+=(Pulse_Counter_L*20/1165);
		Length_R+=(Pulse_Counter_R*20/1165);
                OK_send_flag=0;
		if(((Length_R+Length_L)/2) >=100)    
		{
			car_mode=meet_reversal;  		
		}   
	}
}



void circle_reverse()
{
	if(real_stop_flag==1 && car_number==front_car  && meet_seq==4 ) //（前车动作）
	{
		last_send_signal='X';
		MSG_out[0]='M';        
		MSG_out[1]='X';        
		MSG_out[2]='X';        
		MSG_out[3]='.';        
		send_to_other_car_flag=1;
		OK_send_flag=0;
		
	}
	if(car_number==after_car && real_stop_flag==1 &&meet_seq==1) //(后车动作)
	{
		last_send_signal='Y';
		MSG_out[0]='M';        
		MSG_out[1]='Y';        
		MSG_out[2]='Y';       
		MSG_out[3]='.';        
		send_to_other_car_flag=1;
		OK_send_flag=0;
	}
	

	
	if(car_number==front_car && zz_flag==1)  //（前车动作）
	{
		
			last_send_signal='Z';
			MSG_out[0]='M';       
			MSG_out[1]='Z';     
			MSG_out[2]='Z';       
			MSG_out[3]='.';       
			send_to_other_car_flag=1;
			OK_send_flag=0;
			car_number=un_know;
			meet_flag=2;
			FootWalk_ON_OFF=1;
			Meet_ON_OFF=0;
			camera_circle_clear();


	}
}