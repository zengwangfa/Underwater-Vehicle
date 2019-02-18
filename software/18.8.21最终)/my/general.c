#include "include.h"



uint8 img[CAMERA_H][CAMERA_W]; 
uint8 imgbuff[CAMERA_SIZE];//摄像头数组
uint8 send_picture[CAMERA_SIZE];
uint8 sending_flag=0;//摄像头发送完成


float S_P; //舵机P
float S_D; //舵机D
float M_L_P=M_P; //左电机P
float M_L_I=M_I; //左电机D
float M_R_P=M_P; //右电机P 
float M_R_I=M_I; //右电机D
float C_P; //cameraP
float C_D; //cameraD
float Cam2Mag_Weight;//摄像头对电磁权重//10为满值

volatile MENU_LIST_e MENU =picture; //菜单
volatile CAR_STATUS_e car_mode = stop;  //车子状态
volatile ROAD_STATUS_e road_flag=straight; // 车子位置（圆环，十字，弯道，直道）

to_speed speed_expect={0}; //电磁、摄像头期望速度

ring_missage ring_info={0};

uint8 buzzer_time=0;//蜂鸣器
uint8 buzzer_count=0;//蜂鸣器
uint8 buzzer_number=0;//蜂鸣器

uint8 page =1; // LCD 页
uint8 row=1;  // LCD 行
uint8 unit=1;//档位

uint16 integer=0,decimal=0;//整数、小数部分（用于flash）

uint16 var_left=0; //归一化后相对值
uint16 var_right=0;
uint16 var_middle=0;
uint16 var_left_last=0;
uint16 var_right_last=0;
uint16 var_middle_last=0;

uint8 var_max[3]={0,0,0}; //归一化最大值
uint8 var_min[3]={0,0,0};//归一化最小值

int16 Cam_duty=0;//偏差*P D//监控量
int16 Mag_duty=0;//偏差*P D//监控量

int16 Servo_Offset=0;

int16 deviation=0;  //电磁偏差
int16 last_deviation=0; //上次电磁偏差

uint8 L_bubble=0; //AD实际采集值（0~255）
uint8 R_bubble=0; 
uint8 M_bubble=0;

uint8 BM_L;   //左1   为正转  
uint8 BM_R;   //右0   为正转

int16 Get_SpeedL=0; //编码器采集到的速度
int16 Get_SpeedR=0;

uint16 normal_speed; //最高期望速度
uint16 min_speed=200; //最小期望速度//圆环速度
moty_duty run_duty={0};//两轮ftm输出速度
  
int16 Servo_Duty_Last=Servo_Duty_Md; //舵机打角
int16 Servo_Duty=Servo_Duty_Md;

uint8 A_Car=1;
uint8 edge[60][2]; //摄像头边线
uint8 center_line[60]; // 摄像头中线（线）
int16 camera_center=0; // 摄像头中线（偏差值）//带远近权重
int16 camera_last_center=0; 
int16 camera_integral_center=0; //摄像头中线一次积分
//int16 camera_integral_center_2=0;  //摄像头中线二次积分
            //圆环多少行打脚
float Circle_W_In=7.5;                //判断到圆环  摄像头权重
int8 edge_der1[2][40]={0};//边沿一次导数
int8 edge_der2[2][40]={0};//边沿二次导数
uint8 edge_start[80][2]={0};  //边沿白色切入行数
uint8 circle_edge_start[4];//圆环切入标志角纵坐标0,2,横坐标1,3
uint8 Line_sum[2];
uint8 straight_go_flag;
uint8 Circle_fill_line_flag=0;;
uint8 circle_point[2]={0};//圆环切入标志角纵坐标0,,横坐标1
uint8 ring_pass_number=0;


uint8 edge_sum_left;//边沿黑色数量
uint8 edge_sum_right;//边沿黑色数量
uint16 edge_black[2];
uint16 edge_Int_R=0;//边沿粗糙度
uint16 edge_Int_L=0;//边沿粗糙度


uint8 camera_integral_flag=0;//圆环方向
uint8 Om_turn_flag=0;

uint8 system_clock=0;//计时
uint8 system_old_clock=0; //计时
uint8 camera_frequency=0;//计帧率用
uint8 camera_count=0;//计帧率用

uint8 fitting_over_flag=1;
uint8 Circle_ON_OFF=0;
MEET_PLACE_e meet_choose;
uint8 meet_ring_number;


uint8 Circle_go_flag=0;

MEET_CAR_NUMBER_e car_number=un_know; //前车后车标志位
//**********************************会车区******************************//

//---------------------平滑度-------------------//
uint16 smoothness_left=0;
uint16 smoothness_right=0;
uint16 smoothness_sum=0;

//---------------------左线方差-------------------//
uint16 meet_edge_left[19]={0};  //区边线数组
uint32 variance_sum_left=0;   //方差样本累加量
uint16 variance_aver_left=0;  //样本平均数
uint16 variance_left=0;       //方差

//---------------------右线方差-------------------//
uint16 meet_edge_right[19]={0};//会车区边线数组
uint32 variance_sum_right=0;   //方差样本累加量
uint16 variance_aver_right=0;  //样本平均数
uint16 variance_right=0;       //方差

//---------------------中线方差-------------------//
uint16 variance_sum_center=0;   //方差样本累加量
uint16 variance_aver_center=0;  //样本平均数
uint16 variance_center=0;       //方差

//---------------------边线凹陷-----------------//
uint16 corner_dian_r=0;
uint16 corner_dian_l=0;
uint16 corner_meet_rn=0;
uint16 corner_meet_ln=0;

//---------------------距离-----------------//
uint16 Pulse_Counter_R=0;
uint16 Pulse_Counter_L=0;
uint16 Length_L=0;  //计算长度
uint16 Length_R=0;

//---------------------标志位及开关-----------------//
int16 Servo_Turn=0;
uint8 meet_flag=0;         //会车状态位
int16 meet_turn=0;  


uint8 Meet_ON_OFF=0;       //会车总开关
uint8 Meet_Action_flag=0;  //会车标志位


uint8 FootWalk_Flag=0;      //斑马线标志位
uint8 FootWalk_ON_OFF=0;   //斑马线开关
uint8 WhiteDot_Count=0;  //斑马线黑黑黑白白白起始白点

uint8 WhiteDot_CountLn=0;  //斑马线黑黑黑白白白起始白点
uint8 real_stop_flag=0;    //真正停车标志位

//--------------------通讯--------------------//
uint8 CMD_in[5]={0};  //通讯接收
uint8 MSG_in[4]={0};  //通讯接收
uint8 MSG_out[4]={0}; //通讯发出
uint8 last_send_signal='O'; //上次发出信号简写（用于判断是否可以关断发给另一辆车的flag）
uint8 send_to_other_car_flag=0; //开关向外发送的flag
uint8 recv_data_p=0;
uint8 check_send_flag=1; //校验zigbee发射是否有问题



uint16 white_num=0;
uint8 white_proportion=0;   //白点占比
uint8 ramp_flag=0;
int16 ramp_count=0;
uint8 to_the_side=0;//靠边
uint8 side_high[160]={0};//0-60
uint8 delay_run_flag=0;
uint8 one_car_mode=0;
uint8 first_meet_flag=0;
uint8 OK_send_flag=0;

int16 var_camera_center=0;
int16 var_last_camera_center=0;

uint16 temp_time_c=0;
uint8 Mag_insurance; //电磁保险
uint8 Circle_size=0;//0无1小2大
uint16 ring_size=0;

uint8 Go_back_flag=0;
uint8 meet_ring_seq=0;
uint8 meet_seq=6;
uint8 meet_zk_count=0;
uint8 yy_flag=0;
uint8 zz_flag=0;
uint8 ring_size_plus=0;

int16 Servo_dir_n[20]={0};
int16  Servo_dir=0;
uint8 Insurance_flag=0;
uint8 Insurance_dir=3;

uint16 long_count_cjx;
uint16 add_deviation_cjx;

//--------------------十字--------------------//
uint16 crossl_f=0;
uint16 crossr_f=0;
uint16 crossl_b=0;
uint16 crossr_b=0;

uint8 death_time_flag=0;
uint8 polarization_ON_OFF=0;  //直道会车偏置开关

void ware_test()
{   

	static int16 t_list[8]= {0};
	t_list[0] = Cam_duty;//edge_sum_left;//edge_Int_R+edge_Int_L;//camera_center;
	t_list[1] = Mag_duty;//edge_Int_R;//(edge_sum_left-edge_sum_right)*7;
	t_list[2] = 180;//Servo_Offset;
	t_list[3] = white_proportion*2;//-(Servo_Duty-Servo_Duty_Md);
	t_list[4] = var_left+var_right+var_middle;//corner_meet_rn;//edge_start[1];//
	t_list[5] = var_left;
	t_list[6] = var_middle;//camera_forecast_center;//
	t_list[7] = var_right;//camera_center;
	
        
        
	vcan_sendware(t_list,sizeof(t_list));
}




void buzzer_once()
{
buzzer_time=5;
buzzer_number=5;
buzzer_count=1;

}  
void buzzer_bibi(uint8 t,uint8 l)
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
          if(buzzer_count%2==1)   {gpio_set(PTB23,0);}
          else                    {gpio_set(PTB23,1);}
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
  gpio_set(PTB23,1);
  buzzer_time=0;
  buzzer_count=0;
}

}


void time_count()
{

 static uint8 system_count=0;
  
  system_count++;
  
  if(system_count>=201) 
  { 
    
    camera_frequency=camera_count;//当前显示帧率
    camera_count=0;
    system_count=0;
    system_clock++;
  }
  if(system_clock==61)
  {
    system_clock=0;
  }
}

void dma_count()
{
  camera_count++;
}


void LED_ALL_ON()
{
  	//LED
        gpio_set (PTC0,0);    
	gpio_set (PTD15,0);    
	gpio_set(PTE26,0);   
	gpio_set (PTA17,0);    
}

void LED_ALL_OFF()
{
        gpio_set (PTC0,1);    
	gpio_set (PTD15,1);    
	gpio_set(PTE26,1);   
	gpio_set (PTA17,1);    
}