#include "magnetic.h"
#include "include.h"

extern int16 deviation;  //偏差
extern int16 last_deviation;

extern uint8 real_stop_flag;     //真正停车标志位
extern uint16 var_left;
extern uint16 var_right;
extern uint16 var_middle;

extern uint16 var_left_last;
extern uint16 var_right_last;
extern uint16 var_middle_last;
extern uint8 L_bubble;
extern uint8 R_bubble;
extern uint8 M_bubble;

extern uint8 var_max[3];
extern uint8 var_min[3];

extern int16 camera_center;
extern int16 camera_last_center; 

extern int16 camera_integral_center;
extern uint8 camera_integral_flag;
extern CAR_STATUS_e car_mode;



extern float S_P;
extern int16 Servo_Duty;

extern ring_missage ring_info;
extern ROAD_STATUS_e road_flag;

extern uint8 edge_sum_left;
extern uint8 edge_sum_right;
extern uint16 edge_black[2];
extern uint8 edge_start[80][2];
extern uint8 circle_edge_start[4];
extern uint8 buzzer_flag;//蜂鸣器

extern uint8 Circle_fill_line_flag;
extern uint8 circle_point[2];//圆环切入标志角纵坐标0,,横坐标1

extern float Cam2Mag_Weight;
extern float Circle_W_In;                //判断到圆环  摄像头权重
//圆环多少行打脚

extern uint8 test_flag;
extern uint8 Meet_ON_OFF;
extern uint8 Circle_ON_OFF;


extern uint8 straight_go_flag;

extern MEET_CAR_NUMBER_e car_number; //前车后车标志位
extern uint8 meet_flag; 
extern MEET_PLACE_e meet_choose;
extern uint8 ring_pass_number;
extern uint8 meet_ring_number;

extern uint16 temp_time_c;
extern uint8 Circle_go_flag;
extern uint8 Mag_insurance;
extern uint8 Circle_size;//0无1小2大

extern uint16 ring_size;
extern uint8 ring_size_plus;
extern uint8 white_proportion;   //白点占比

extern uint8 meet_stop_flag;
extern uint8 Go_back_flag;
extern uint8 meet_ring_seq;
extern uint8 meet_seq;

extern uint16 Pulse_Counter_R;
extern uint16 Pulse_Counter_L;

extern uint8 meet_zk_count;
extern uint8 yy_flag;
extern uint8 zz_flag;


extern int16 Servo_dir_n[20];
extern int16  Servo_dir;

extern uint8 Insurance_flag;
extern uint8 Insurance_dir;

extern uint8 death_time_flag;
extern uint8 one_car_mode;
/*************************归一化采集********************************/

void magnetic_information() //ad采集最大最小值
{
  uint8 var1[10];
  uint8 var2[10];
  uint8 var3[10];
  uint8 i;
  for(i=0;i<10;i++)
  {
    var1[i]=(uint8)adc_once(AD_left, ADC_8bit);  //left
    var2[i]=(uint8)adc_once(AD_right, ADC_8bit);  //right
    var3[i]=(uint8)adc_once(AD_middle, ADC_8bit); //middle
    
  }
  if(var_max[0]<Bubble(var1))
  {
    var_max[0]=Bubble(var1);
  }
  if(var_min[0]>Bubble(var1))
  {
    var_min[0]=Bubble(var1);
  }
  if(var_max[1]<Bubble(var2))
  {
    var_max[1]=Bubble(var2);
  }
  if(var_min[1]>Bubble(var2))
  {
    var_min[1]=Bubble(var2);
  }
  if(var_max[2]<Bubble(var3))
  {
    var_max[2]=Bubble(var3);
  }
  if(var_min[2]>Bubble(var3))
  {
    var_min[2]=Bubble(var3);
  }
  
}

/*******************************冒泡***********************************/
uint8 Bubble(uint8 *v)     
{
  
  
  uint8 i,j;
  uint8 temp,k;
  for(j=0;j<10;j++)
  {			
    for(i=0;i<9-j;i++)
    {
      
      if(v[i]>v[i+1])
      {
        temp=v[i];
        v[i]=v[i+1];
        v[i+1]=temp;
      }
    }
  }
  
  k=(v[4]+v[5]+v[6])/3;  //取中间三个值得平均值
  
  
  return k;
  
}



/***************************AD采集******************************/

void ad_collect()
{       
  static uint8 C_var_flag=0;
  float var_sum;
  
  uint8 var1[10]={0};
  uint8 var2[10]={0};
  uint8 var3[10]={0};
  uint8 i;
  
  
  last_deviation=deviation;
  
  var_left_last=var_left;
  var_right_last=var_right;
  var_middle_last=var_middle;
  
  for(i=0;i<10;i++)
  {
    var1[i]=adc_once(AD_left, ADC_8bit);  //left
    var2[i]=adc_once(AD_right, ADC_8bit);  //right
    var3[i]=adc_once(AD_middle, ADC_8bit); //middle
  }
  L_bubble=Bubble(var1);
  R_bubble=Bubble(var2);
  M_bubble=Bubble(var3);
  //】】】】】】】】归一化及	低于最小值超出最大值判断
  //逻辑：先判断状态标志位，再计算var值，小于最小值为1，爆表不做限制
  
  if(var_min[0]>=L_bubble)  
  {var_left=1;}//计算var
  else
  {var_left=(uint16)(100*(L_bubble-var_min[0])/(var_max[0]-var_min[0]));}	
  
  if(var_min[1]>=R_bubble)  
  {var_right=1;}//计算var
  else
  {var_right=(uint16)(100*(R_bubble-var_min[1])/(var_max[1]-var_min[1]));}	
  
  if(var_min[2]>=M_bubble)  
  {var_middle=1;}//计算var
  else  
  {var_middle=(uint16)(100*(M_bubble-var_min[2])/(var_max[2]-var_min[2]));}	
  
  
  if(car_mode==run)
  {
    C_var_flag=1;
  }
  
  if(C_var_flag==1)
  {
    if(abs(var_left_last-var_left)>=60){var_left=var_left_last;}
    if(abs(var_right_last-var_right)>=60){var_right=var_right_last;}
    if(abs(var_middle_last-var_middle)>=60){var_middle=var_middle_last;}
    C_var_flag=1;//自锁
  }
  var_sum=var_left+var_right+var_middle+1;  //求归一化偏差
  
  var_sum=powf(var_sum ,1.5);
  deviation=(int16)((1000*(var_right-var_left))/var_sum)+add_bias;//+meet_flag*60;
  deviation=(int16)(last_deviation*2+deviation*8)/10;//低通滤波
  
  
  if(deviation>100)     {deviation=100; }        //限幅
  if(deviation<-100){deviation=-100;}  
  
  
}      



void Insurance_Mag()
{
  
  if(var_left+var_right+var_middle<50)
  {
    Insurance_flag=1;
    buzzer_once();
  }
  else
  {Insurance_flag=0;}
  
}





///*************************************************************

void magnetic_circle()//环岛处理   
{     
  
  static uint16 ring_count=0;
  static uint16  reach_flag=0;
  static uint16  circle_edge_flag=0;
  
  
  static uint16  start_count=0;
  static uint8  fake_in_circle=0;
  static uint8  y_flag=0;
  static uint16 at_ring_count=0;
  
  static uint8 death_time_count=0;
  
  if(yy_flag==1&&y_flag==0)
  {
    y_flag=1;
    ring_size=150;
    ring_size_plus=1;
  }
  
  if(Mag_insurance==1 && var_left+var_right+var_middle<=40)//圆环电磁保险&&
  {  
    buzzer_once();
    if(camera_integral_center<0)
    {ring_info.ring_turn=enforce_left;} 
    else if(camera_integral_center>0)
    {ring_info.ring_turn=enforce_right;} 
    else{ring_info.ring_turn=in_contorl;}
  }                              
  //初始判断圆环
  if(road_flag==straight && var_left>70 && var_right>70 && var_middle>100 && (var_left+var_right+var_middle)>=420 &&  death_time_flag==0)//上升沿
  {
    start_count++; 
  }
  else {start_count=0;}
  
  if(circle_point[0]<=40 && ring_info.ring_flag==reach_ring_point && meet_seq==0)
  {
    meet_ring_seq=4;//后车往前滑行
    ring_size_plus=0;
  }
  
  
  if(meet_zk_count>=30)//bmq
  {	
    car_mode=meet_reversal;//抱死
    
    meet_zk_count=0;
    
  } 
  
  if(real_stop_flag==1&&meet_seq==0)
  {
    meet_seq=1;		
    road_flag=straight;
    ring_info.ring_flag=no_ring; 
  }
  
  
  if(real_stop_flag==1&&meet_ring_seq==4&&meet_seq==2)
  { 
    meet_ring_seq=5;
    Go_back_flag=1;
  }
  if(real_stop_flag==1&&Go_back_flag==2&&meet_ring_seq==5)//假装入环
  {
    car_mode=run;
    meet_ring_seq=6;
    fake_in_circle=1;
  }
  
  if((start_count>=4 && road_flag==straight)||fake_in_circle==1)
  {
    
    if(fake_in_circle==1)
    {fake_in_circle=0;}
    
    start_count=0;
    ring_size=0;
    ring_size_plus=1; 
    road_flag=circle;
    ring_info.ring_flag=near_ring;
    ring_pass_number+=1;
    
    if(meet_choose==meet_circle && one_car_mode==0)
    {
      if(ring_pass_number==meet_ring_number && car_number==un_know)
      {
        meet_seq=5;
      }
      if(ring_pass_number==meet_ring_number && car_number==after_car)
      {
        meet_seq=0;
      }
    }
    buzzer_bibi(2,2); //----
    
  }
  
  
  if(road_flag==circle)
  {
    ring_size+=ring_size_plus;//环内时间
    
    if(ring_info.ring_flag==near_ring) // near_ring靠近环
    {
      
      if(camera_integral_flag>=4)//if(var_left+var_right+var_middle<390)//下降沿/100     
      { 
        
        if(circle_edge_start[0]<=circle_line_in &&camera_integral_center<0)  //47
        {       
          ring_info.ring_flag =reach_ring_point;             
        }
        if(circle_edge_start[2]<=circle_line_in &&camera_integral_center>0)  //47
        {       
          ring_info.ring_flag =reach_ring_point;            
        }
      }
      
      
    }                
    
    if(ring_info.ring_flag==reach_ring_point)
    { 
      
      Mag_insurance=1;//开启保险
      reach_flag++;
      
      if(reach_flag<3){
        if(camera_integral_center<0)       //方向判断//
        {
          Circle_fill_line_flag=1;
          Circle_go_flag=1;	
          //补线巡线
          //ring_info.ring_turn=enforce_left;
        }
        else if(camera_integral_center>0)
        {
          Circle_fill_line_flag=2;
          Circle_go_flag=2;
          //补线巡线
          //ring_info.ring_turn=enforce_right;
        }
      }
      if(circle_edge_flag==0&&camera_integral_center>0&&edge_start[20][0]>=43)
        
      {circle_edge_flag=1;}
      if( circle_edge_flag==1&&edge_start[20][0]<=35)//关闭打脚//摄像头巡线
      {
        // Cam2Mag_Weight=5;
        //ring_info.ring_turn=in_contorl;  
        Circle_fill_line_flag=0;
        circle_edge_flag=0;
        ring_count=0;
        reach_flag=0;
        ring_info.ring_flag=at_ring;     
      }  
      
      if(circle_edge_flag==0&&camera_integral_center<0&&edge_start[20][1]>=43)
      {circle_edge_flag=1;}
      if(circle_edge_flag==1&&edge_start[20][1]<=35)//关闭打脚  
      {
        
        // Cam2Mag_Weight=5;
        //  ring_info.ring_turn=in_contorl;
        Circle_fill_line_flag=0;
        circle_edge_flag=0;
        ring_count=0;
        reach_flag=0;
        ring_info.ring_flag=at_ring;     
      }          		
    } 
    
    if(meet_ring_seq==1&&ring_info.ring_flag==at_ring)
    {
      car_mode=meet_reversal;
      ring_size_plus=0;
    }	
    if(meet_ring_seq==2&&ring_info.ring_flag==at_ring)
    {
      car_mode=run;
      ring_size_plus=1;
    }
    if(ring_info.ring_flag==at_ring)//at_ring在环内
    { 
      // if(meet_seq==0){meet_ring_seq=1;}
      //buzzer_once();	
      //Cam2Mag_Weight=0;
      ring_count++;  
      at_ring_count++;
      
      if(ring_count%7==0)
      {
        Cam2Mag_Weight-=0.6;
        Cam2Mag_Weight=Cam2Mag_Weight<0?0:Cam2Mag_Weight;
      }
      //if(Cam2Mag_Weight==0)  {buzzer_once();}
      if( meet_choose==meet_circle  &&car_number==un_know && meet_seq==5)
      {
        meet_seq=4;
        car_number=front_car;
      }
      if(meet_seq==4&&ring_count>=30)
      {car_mode=meet_reversal;}
      
      
      if(at_ring_count>=150&&((camera_integral_center>0&&edge_start[20][0]>=40)||(camera_integral_center<0&&edge_start[20][1]>=40)))//((camera_center==0))//||(ring_count>=700))//3.5s
      {
        Circle_go_flag=0;
        ring_info.ring_flag=no_ring;  
        ring_count=0;
        //Cam2Mag_Weight=1;//出环电磁巡线
        //camera_circle_clear();//摄像头积分清零
        //buzzer_bibi(1,1);
        ring_size_plus=0;
      }        
    }
    
    if(ring_info.ring_flag==no_ring)//no_ring出环（无环）
    {
      at_ring_count=0;
      ring_count++;
      
      //大小环判断
      if(ring_size<=500){Circle_size=1;}
      else if(ring_size>550){Circle_size=2;}
      
      if(Circle_size==2)
      {
        Mag_insurance=0;//关闭保险
        
      }
      
      /*//         //出环补角
      if(Circle_size==1)//小环出环补jio
      { 
      if(ring_count<=(ring_size*10)/75) 
      {
      
      if(white_number[0]-white_number[1]<20&&camera_integral_center<0)
      {ring_info.ring_turn=enforce_left;}
      if(white_number[0]-white_number[1]>-20&&camera_integral_center>0)
      {ring_info.ring_turn=enforce_right;}
    }
      else
      {
      ring_info.ring_turn=in_contorl;
      Mag_insurance=0;//关闭保险
    }*/
      
      
      if(ring_count>(ring_size*10)/75) 
      {      
        ring_info.ring_turn=in_contorl;
        Mag_insurance=0;//关闭保险
      }
      
      
      if(ring_count>=(ring_size*10)/25)// && Circle_size==1)//(straight_go_flag==2||((var_left+var_right+var_middle)<=300))//(ring_count>=150)//||((var_left+var_right+var_middle)<=300)) 
      {
        if(meet_seq==3 && car_number==front_car)
        {	
          zz_flag=1;
        }
        
        buzzer_bibi(1,1);   //----
        camera_circle_clear();//摄像头积分清零
        ring_count=0;
        ring_size_plus=0;
        Circle_size=0;
        death_time_flag=1;
      }
      
      //			if(Circle_size==2 && var_left>90 && var_right>90 && var_middle>100 && (var_left+var_right+var_middle)>=430)//(straight_go_flag==2||((var_left+var_right+var_middle)<=300))//(ring_count>=150)//||((var_left+var_right+var_middle)<=300)) 
      //			{
      //				if(meet_seq==3 && car_number==front_car)
      //				{	
      //					zz_flag=1;
      //				}
      //
      //				buzzer_bibi(1,1);
      //				camera_circle_clear();//摄像头积分清零
      //				ring_count=0;
      //				ring_size_plus=0;
      //				Circle_size=0;
      //				death_time_flag=1;
      //				
      //			}
      
      if( death_time_flag==1)
      {
        death_time_count++;
        if(death_time_count>=50)
        {
          death_time_flag=0;
          road_flag=straight;
          Cam2Mag_Weight=flash_read(servo_sector , 76, uint8)+((float)(flash_read(servo_sector , 80, uint8))*(0.1));//出环后恢复摄像头&电磁巡线
          //				    buzzer_bibi(2,1);
        }
      }
      
    }
    
  }
  
}