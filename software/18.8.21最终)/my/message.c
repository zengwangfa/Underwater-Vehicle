#include "include.h"

//--------------------通讯--------------------//
extern uint8 CMD_in[5];  //通讯接收
extern uint8 MSG_in[4];  //通讯接收
extern uint8 MSG_out[4]; //通讯发出
extern uint8 last_send_signal; //上次发出信号简写（用于判断是否可以关断发给另一辆车的flag）
extern uint8 send_to_other_car_flag; //开关向外发送的flag
extern MEET_CAR_NUMBER_e car_number; //前车后车标志位
extern uint8 recv_data_p;
//--------------------通讯--------------------//
extern uint8 Meet_ON_OFF;
extern int16 Servo_Turn;
extern uint8 meet_flag;         //会车状态位
extern int16 meet_turn;  

extern CAR_STATUS_e car_mode;  //车子状态
extern uint8 check_send_flag;
extern MEET_PLACE_e meet_choose;
extern uint8 to_the_side;
extern uint8 one_car_mode;
extern uint8 FootWalk_ON_OFF;   //斑马线开关
extern uint8 first_meet_flag;
extern uint8 OK_send_flag;
extern ring_missage ring_info;
extern ROAD_STATUS_e road_flag;
extern uint8 meet_ring_number;
extern uint8 meet_seq;
extern uint8 meet_ring_seq;
extern uint16 ring_size;
extern uint8 ring_size_plus;
extern uint8 yy_flag;
void Check_Communication()
{
	if(check_send_flag==1)
	{
		MSG_out[0]='M';
		MSG_out[1]='I';
		MSG_out[2]='I';
		MSG_out[3]='.';
		uart_putbuff(UART3,MSG_out,sizeof(MSG_out));
	}
	
	
}
void Communication_In()
{

	char buff=0 ;
	if(UART_S1_REG(UARTN[UART3]) & UART_S1_RDRF_MASK)   //接收数据寄存器满
	{
		uart_getchar (UART3,&buff);
		MSG_in[recv_data_p]=buff;
		if(MSG_in[0]=='M')//正确包头字节
		{
			recv_data_p++;
		}
		else {recv_data_p=0;}
		if(recv_data_p==4)  //接收4个
		{
			recv_data_p=0;//清零
			if(MSG_in[0]=='M'&& MSG_in[3]=='.')//包头字节与结束字符校验   ---> 接收正确
			{
				recv_data_p=0;//清零
				if(MSG_in[1]!='O'&& MSG_in[1]==MSG_in[2])  //接收信号为非O（应答），为最新信息
				{
					switch(MSG_in[1])
					{
						case 'G':  // 后车接收到的) 前车到达会车区    开始区分前后车(后车收到)
						{
							if(first_meet_flag==1 && meet_flag!=5)
							{
								last_send_signal='O';
								MSG_out[0]='M';
								MSG_out[1]='O';
								MSG_out[2]='G';
								MSG_out[3]='.';
								OK_send_flag=1;//发送开启flag
								send_to_other_car_flag=0;//停止广播
								one_car_mode=1;
								FootWalk_ON_OFF=1;
								car_mode=run;
                                                                meet_ring_seq=7;
                                                                meet_seq=7;								
							}
							break;   
							
						}
						case 'I':  // 后车接收到的) 前车到达会车区    开始区分前后车
						{
							if(check_send_flag==0)
							{
								MSG_in[0]=0;
								MSG_in[1]=0;
								MSG_in[2]=0;
								MSG_in[3]=0;
								MSG_out[0]=0;
								MSG_out[1]=0;
								MSG_out[2]=0;
								MSG_out[3]=0;
								gpio_set(PTD15,1);
								
							}
							gpio_turn(PTD15);    // LED点亮
							
							break;
						}
						case 'A':  // 后车接收到的) 前车到达会车区    开始区分前后车(后车收到)
						{
							last_send_signal='O';
							MSG_out[0]='M';
							MSG_out[1]='O';
							MSG_out[2]='A';
							MSG_out[3]='.';
							OK_send_flag=1;//发送开启flag
							send_to_other_car_flag=0;//停止广播
							car_number=after_car;
							buzzer_bibi(1,1);  
							break;
							
							
						}
						case 'B': //  (前车接收到的) 79 70  后车到达会车区 (前车收到)
						{
							last_send_signal='O';
							MSG_out[0]='M';
							MSG_out[1]='O';
							MSG_out[2]='B';
							MSG_out[3]='.';
							OK_send_flag=1;//发送开启flag
							send_to_other_car_flag=0;//停止广播
							to_the_side=1;  
							car_mode=run; 
							meet_flag=1;
							buzzer_bibi(1,1);
							break; 
							
							
							
						}
						case 'C': // 接收CC进行回应OC (后车接收到的) 79 71    提示后车启动
						{
							last_send_signal='O';
							MSG_out[0]='M';
							MSG_out[1]='O';
							MSG_out[2]='C';
							MSG_out[3]='.';
							OK_send_flag=1;//发送开启flag
							send_to_other_car_flag=0;
							car_number=after_car;
							meet_flag=3;    
							buzzer_bibi(1,1);
							break; 
							
							
						}
						
						case 'D': // 接收DD进行回应OD  69 (后车接收到的)   后车得知前者已经到达终点
						{
							last_send_signal='O';
							MSG_out[0]='M';
							MSG_out[1]='O';
							MSG_out[2]='D';
							MSG_out[3]='.';
							OK_send_flag=1;//发送开启flag
							send_to_other_car_flag=0;//停止广播
							to_the_side=2;  
							car_mode=run;//前车启动
							meet_flag=4;    
							buzzer_bibi(1,1);
							break; 
							
							
						}
						
						case 'R': // 接收EE进行回应OE  69 (前车接收到的)    遥控器发车
						{
							last_send_signal='O';
							MSG_out[0]='M';
							MSG_out[1]='O';
							MSG_out[2]='R';
							MSG_out[3]='.';
							OK_send_flag=1;//发送开启flag
							send_to_other_car_flag=0;//停止广播
							if(Meet_ON_OFF==1)
							{
								car_mode=run;
								buzzer_bibi(1,1);
							}
							break;
						}
						
						case 'X': // 接收EE进行回应OE  69 (前车接收到的)   遥控器停车
						{
							last_send_signal='O';
							MSG_out[0]='M';
							MSG_out[1]='O';
							MSG_out[2]='X';
							MSG_out[3]='.';
							OK_send_flag=1;//发送开启flag
							send_to_other_car_flag=0;//停止广播
							car_number=after_car;

							buzzer_bibi(1,2);  
							break; 
						}
						case 'Y': // 接收EE进行回应OE  69 (前车接收到的)   遥控器停车
						{
							last_send_signal='O';
							MSG_out[0]='M';
							MSG_out[1]='O';
							MSG_out[2]='Y';
							MSG_out[3]='.';
							OK_send_flag=1;//发送开启flag
                                                        yy_flag=1;
							car_mode=run;
                                                        
							meet_seq=3;
							send_to_other_car_flag=0;//停止广播
							break;
							
						}
						case 'Z': // 接收EE进行回应OE  69 (前车接收到的)   遥控器停车
						{
							last_send_signal='O';
							MSG_out[0]='M';
							MSG_out[1]='O';
							MSG_out[2]='Z';
							MSG_out[3]='.';
							OK_send_flag=1;//发送开启flag
							send_to_other_car_flag=0;//停止广播
							meet_seq=2;
							meet_flag=2;
							FootWalk_ON_OFF=1;
							Meet_ON_OFF=0;
							break; 
						}
						
						default:
						{
							break;
						}
					}
				}
				else if(MSG_in[1]=='O') //接收到对方对我方发出信号的回应
				{
					switch(MSG_in[2])
					{
						
						case 'A'://(前车接收到的)
						{
							if(last_send_signal=='A')
							{
								send_to_other_car_flag=0;//停止广播
								
							}
							break;
							
						}
						case 'B'://(后车接收到的)
						{
							if(last_send_signal=='B')
							{
								
								send_to_other_car_flag=0;//停止广播
								
							}
							break;
						}
						case 'C'://(前车接收到的)
						{
							if(last_send_signal=='C')
							{
								send_to_other_car_flag=0;//停止广播
								
							}
							break;
						}
						
						case 'D'://(前车接收到的)
						{
							if(last_send_signal=='D')
							{
								send_to_other_car_flag=0;//停止广播
								
							}
							break;
						}
						
						case 'E'://(后车接收到的)
						{
							if(last_send_signal=='E')
							{
								send_to_other_car_flag=0;//停止广播
							}
							break;
						}
						case 'R'://(后车接收到的)
						{
							if(last_send_signal=='R')
							{
								send_to_other_car_flag=0;//停止广播
							}
							break;
						}
						
						case 'G'://(后车接收到的)
						{
							if(last_send_signal=='G')
							{
								send_to_other_car_flag=0;//停止广播
							}
							break;
						}
						case 'X'://(后车接收到的)
						{
							if(last_send_signal=='X')
							{
								send_to_other_car_flag=0;//停止广播
							}
							break;
						}
						case 'Y'://(后车接收到的)
						{
							if(last_send_signal=='Y')
							{
								send_to_other_car_flag=0;//停止广播
							}
							break;
						}
						case 'Z'://(后车接收到的)
						{
							if(last_send_signal=='Z')
							{
								send_to_other_car_flag=0;//停止广播
							}
							break;
						}
						default:
						{
							break;
						}
						
					}
				}
				
			}
			else {recv_data_p=0;}
		}
	}
	
}

void Communication_Out()  
{
	static uint8 send_count=0;
	if(send_to_other_car_flag==1)
	{
		uart_putbuff(UART3,MSG_out,sizeof(MSG_out));
		
	}
	if(OK_send_flag==1)
	{
		uart_putbuff(UART3,MSG_out,sizeof(MSG_out));
		send_count+=1;
		if(send_count==8)
		{
			OK_send_flag=0;
			send_count=0;
		}
		
	}
}
