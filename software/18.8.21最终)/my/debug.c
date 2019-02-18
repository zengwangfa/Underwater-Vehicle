#include "include.h"


extern uint8 page;
extern uint8 row;
extern uint8 unit;

extern CAR_STATUS_e car_mode;
extern int16 Servo_Duty;

extern uint8 system_clock;
extern uint8 camera_frequency;
extern uint8 camera_count;
extern uint8 edge_start[80][2];
extern uint8 Line_sum[2];
extern uint8 circle_edge_start[4];

extern float S_P;
extern float S_D;
extern float M_L_P;
extern float M_L_I;
extern float M_R_P;
extern float M_R_I;
extern float C_P;
extern float C_D;

extern uint16 var_left;
extern uint16 var_right;
extern uint16 var_middle;

extern uint8 L_bubble;
extern uint8 R_bubble;
extern uint8 M_bubble;

extern uint8 var_max[3];
extern uint8 var_min[3];

extern int16 deviation;

extern int16 Cam_duty;
extern int16 Mag_duty;
extern int16 Servo_Offset;
extern int16 Servo_Turn;

extern uint16 normal_speed;
extern uint16 min_speed;

extern float Circle_W_In;                //判断到圆环  摄像头权重
extern float Cam2Mag_Weight;


extern uint8 Circle_ON_OFF;
extern uint8 Meet_ON_OFF;
extern MEET_PLACE_e meet_choose;
extern uint8 meet_ring_number;

extern uint8 img[CAMERA_H][CAMERA_W]; 

extern uint8 edge_sum_left;
extern uint8 edge_sum_right;
extern uint16 edge_black[2];
extern uint8 ring_pass_number;
extern ring_missage ring_info;

extern uint8 meet_flag;
extern uint8 FootWalk_Flag;
extern uint8 MSG_out[4]; //通讯发出
extern uint8 MSG_in[4];  //通讯接收
extern MEET_CAR_NUMBER_e car_number; //前车后车标志位
extern uint8 circle_point[2];

extern uint16 smoothness_left;
extern uint16 smoothness_right;
extern uint16 variance_left;       //方差
extern uint16 variance_right;       //方差
extern uint16 variance_center;       //方差
extern uint16 corner_meet_rn;
extern uint16 corner_meet_ln;

extern uint16 ring_size;
extern uint16 long_count_cjx;
extern uint16 add_deviation_cjx;

extern uint8 polarization_ON_OFF;  //直道会车偏置开关
/*******************************************************

显示图像

********************************************************/
void picture_show_OLED()
{
	LCD_P6x8Str(0,0,"page");
	Dis_SNum(37,0,page,2);
	LCD_P6x8Str(70,0,"unit");
	Dis_SNum(45,0,unit,2);
	
	
	Dis_SNum(45,1,circle_point[0],3);
	Dis_SNum(45,2,circle_point[1],3);
	Dis_SNum(45,4,Cam_duty,4);    
	
	Dis_SNum(45,5,edge_black[1],2);
	Dis_SNum(45,6,edge_sum_right,2);          
        
        
        
	if(ring_info.ring_flag==no_ring)
	{
		LCD_P6x8Str(100,3," no  ");
	}
	else if(ring_info.ring_flag==near_ring)
	{
		LCD_P6x8Str(100,3,"near ");
	}
	else if(ring_info.ring_flag==reach_ring_point)
	{
		LCD_P6x8Str(100,3,"leave");
	}
	else if(ring_info.ring_flag==at_ring)
	{
		LCD_P6x8Str(100,3," at  ");
	}
	else if(ring_info.ring_flag==ring_meet)
	{
		LCD_P6x8Str(100,3,"meet ");
	}

	MY_OLED_IMAGE(img);
	
	
	
	
}


/*******************************************************

正常发车

********************************************************/
void car_run_OLED()
{
	LCD_P6x8Str(0,0,"page");
	Dis_SNum(37,0,page,2);
	LCD_P6x8Str(70,0,"unit");
	Dis_SNum(45,0,unit,2);
	if(page==1)
	{
		
		Dis_Num(0,1,L_bubble,3);
		LCD_Print(30,1,"var_L");
		Dis_Num(43,1,var_left,3);
		
		
		Dis_Num(0,3,R_bubble,3);
		LCD_Print(30,3,"var_R");
		Dis_Num(43,3,var_right,3);
		
		
		
		Dis_Num(0,5,M_bubble,3);
		LCD_Print(30,5,"var_M");
		Dis_Num(43,5,var_middle,3);
		
		
		
		
		LCD_P6x8Str(0,7,"Deviation");
		if(deviation>=0)
		{
			LCD_P6x8Str(80,7,"+");
		}
		else
		{
			LCD_P6x8Str(80,7,"-");
		}
		Dis_SNum(43,7,abs(deviation),4);
		
	}
	if(page==2)
	{
		
		
		uint8 Show_IN[3]={'\0','\0','\0'};
		uint8 Show_OUT[3]={'\0','\0','\0'};
		Show_IN[0]=MSG_in[1];
		Show_IN[1]=MSG_in[2];
		Show_OUT[0]=MSG_out[1];
		Show_OUT[1]=MSG_out[2];

		
		
		LCD_Print(0,1,"speed");
		Dis_Num(0,3,min_speed,3);
		LCD_P6x8Str(40,4,"normal");
		Dis_Num(43,3,normal_speed,3);
		
		////----------------标志位----------------////
		LCD_P6x8Str(9,6,"in");
		
		LCD_P6x8Str(9,7,Show_IN);


		LCD_P6x8Str(42,6,"out");
		
		LCD_P6x8Str(42,7,Show_OUT);

		if(car_number==front_car)       //变为前车
			LCD_Print(70,6,".front");
		else if(car_number==after_car)       //变为前车
			LCD_Print(70,6,".after");
		else  LCD_Print(70,6,"unknow");
		Dis_Num(40,1,meet_flag,1); //会车标志
		
	}
	if(page==3)
	{
		if(car_mode==run) 
		{
			
			LCD_Print(0,2,"run   "); 
			LCD_Print(0,4,"run   ");
			//LCD_Fill(0x00);
		}

		else              
		{
			LCD_Print(0,2,"stop  ");
			LCD_Print(0,4,"stop  ");
			//LCD_Fill(0x00);
		}
		
		
		
	}
	if(page==4)
	{
          
          Dis_Num(0,1,variance_left,4);
          LCD_Print(35,1,"Meet_L");
          Dis_Num(43,1,smoothness_left,4);
          
          
          Dis_Num(0,3,variance_center,4);
          LCD_Print(35,3,"Meet_M");
          Dis_Num(43,3,000,4);

          Dis_Num(0,5,variance_right,4);
          LCD_Print(35,5,"Meet_R");
          Dis_Num(43,5,smoothness_right,4);
          
          Dis_SNum(35,7,corner_meet_ln,2);
          Dis_SNum(40,7,corner_meet_rn,2);
          LCD_P6x8Str(0,7,"va");
          LCD_P6x8Str(100,7,"sm");
//		LCD_Print(15,1,"Sys_Clc");
//		Dis_Num(43,1,system_clock,3);
//		
//		LCD_Print(15,3,"Cam_f");
//		Dis_Num(43,3,camera_frequency,3);
//		
//		LCD_Print(15,5,"count");
//		Dis_Num(43,5,camera_count,3);
		
		
		
		
		
	}
	if(page==5)
	{
		LCD_Print(15,1,"Cam_duty");
		if(Cam_duty>=0)
		{
			LCD_P6x8Str(80,1,"+");
		}
		else
		{
			LCD_P6x8Str(80,1,"-");
		}
		Dis_Num(43,1,abs(Cam_duty),3);                
		
		
		
		LCD_Print(15,3,"Mag_duty");
		if(Mag_duty>=0)
		{
			LCD_P6x8Str(80,3,"+");
		}
		else
		{
			LCD_P6x8Str(80,3,"-");
		}
		Dis_Num(43,3,abs(Mag_duty),3);  
		
		
		LCD_Print(15,5,"Ser_OFF");               
		if(Servo_Offset>=0)
		{
			LCD_P6x8Str(80,5,"+");
		}
		else
		{
			LCD_P6x8Str(80,5,"-");
		}
		Dis_Num(43,5,abs(Servo_Offset),3);  
		
		
	}
        
	
	
	
	
}


/*******************************************************

归一化

********************************************************/

void normaliz_OLED()
{
	LCD_Print(0,0,"Normalized");
	LCD_Print(22,2,"min Lf max");
	Dis_SNum(45,3,var_max[0],3);
	Dis_SNum(0,3,var_min[0],3);
	LCD_Print(22,4,"min Rt max");
	Dis_SNum(45,5,var_max[1],3);
	Dis_SNum(0,5,var_min[1],3);
	LCD_Print(22,6,"min Md max");
	Dis_SNum(45,7,var_max[2],3);
	Dis_SNum(0,7,var_min[2],3);
	
	
}


/*******************************************************

参数整定

********************************************************/

void flash_OLED()
{
	LCD_P6x8Str(0,0,"page");
	Dis_SNum(37,0,page,2);
	LCD_P6x8Str(0,1,"row");
	Dis_SNum(37,1,row,2);
	LCD_P6x8Str(70,1,"unit");
	Dis_SNum(45,1,unit,2);
	if(page==1)
	{
		LCD_Print(0,2,"S_P"); 
		Dis_Float(36,2,S_P,2);
		LCD_Print(0,4,"S_D"); 
		Dis_Float(36,4,S_D,2);
		
	}
	if(page==2)
	{
//		LCD_Print(0,2,"moty_P_I");
//		LCD_Print(31,4,"I Rt P");
//		Dis_Float(0,4,M_R_I,1);
//		Dis_Float(43,4,M_R_P,1);
//		LCD_Print(31,6,"I Lf P");
//		Dis_Float(0,6,M_L_I,1);
//		Dis_Float(43,6,M_L_P,1);
                LCD_Print(0,2,"long_count");
		Dis_Num(45,2,long_count_cjx,2);
                LCD_Print(0,4,"deviation+");
		Dis_Num(45,4,add_deviation_cjx,2);
                LCD_Print(0,6,"ring_size");
		Dis_Num(43,6,ring_size,4);
	}
	
	
	if(page==3)
	{
		LCD_Print(0,2,"C_P"); 
		Dis_Float(36,2,C_P,2);
		LCD_Print(0,4,"C_D"); 
		Dis_Float(36,4,C_D,2);	
		LCD_Print(0,6,"C2M"); 
		Dis_Float(36,6,Cam2Mag_Weight,2);
		
	} 
	
	if(page==4)
	{
		
		LCD_Print(10,2,"Servo_Duty");               
		Dis_Num(43,5,Servo_Duty,5);  
		
	}

	
}


/*******************************************************

模式菜单//单双车//圆环

********************************************************/

void mode_screen_OLED()
{
	
	LCD_P6x8Str(0,0,"page");
	Dis_SNum(37,0,page,2);
	LCD_P6x8Str(0,1,"row");
	Dis_SNum(37,1,row,2);
	LCD_P6x8Str(70,1,"unit");
	Dis_SNum(45,1,unit,2);
	
	if(page==1)
	{
		if(Meet_ON_OFF==1)
		{
			LCD_Print(0,2,"Meet_ON "); 
			LCD_Print(0,4,"Meet_ON ");
		}
		else
		{
			LCD_Print(0,2,"Meet_OFF"); 
			LCD_Print(0,4,"Meet_OFF");
		}  
	}
	if(page==2)
	{
		if(Circle_ON_OFF==1)
		{
			LCD_Print(0,2,"Circle_ON "); 
			LCD_Print(0,4,"Circle_ON ");
		}
		else
		{
			LCD_Print(0,2,"Circle_OFF"); 
			LCD_Print(0,4,"Circle_OFF");
		}  
		
	}
	if(page==3)
	{
		if(Meet_ON_OFF==1)
		{
			LCD_Print(0,2,"Meet_at:");
			if(meet_choose==meet_straight)
			{
				LCD_Print(0,4,"straight");
			}
			else if(meet_choose==meet_circle)
			{
				LCD_Print(0,4," circle ");
			}
			else
			{
				LCD_Print(0,4,"no meet ");
			}
		}
	}
	if(page==4)
	{
          	if(Meet_ON_OFF==1 && meet_choose==meet_straight)
		{
			LCD_Print(0,2,"polarization:");
			if(polarization_ON_OFF==0)
                        {
                          LCD_Print(45,4,"OFF");
                        }
                        else if(polarization_ON_OFF==1)
                        {
                          LCD_Print(45,4,"ON ");
                        }
		}
                
                
		if(Meet_ON_OFF==1 && meet_choose==meet_circle)
		{
			LCD_Print(0,2,"Meet_Ring_at:");
			Dis_Num(45,4,meet_ring_number,2);
		}

	}
		if(page==5)
	{
		
//		LCD_Print(0,2,"DO NOT TOUCH"); 
//		LCD_Print(0,4,"THIS!!");
		
		
		
	}
	
	
	
}
