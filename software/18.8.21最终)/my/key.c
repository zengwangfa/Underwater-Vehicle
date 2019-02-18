#include "include.h"

extern MENU_LIST_e MENU;
extern CAR_STATUS_e car_mode;

extern uint16 normal_speed;
extern uint16 min_speed;

extern uint8 page;
extern uint8 row;
extern uint8 unit;
extern int16 Servo_Duty;
extern float M_L_P;
extern float M_L_I;
extern float M_R_P;
extern float M_R_I;
extern float S_P;
extern float S_D;
extern float C_P;
extern float C_D;

extern uint16 integer;//整数
extern uint16 decimal;//小数

extern float Circle_W_In;                //判断到圆环  摄像头权重            //圆环多少行打脚
extern float Cam2Mag_Weight;

extern uint8 Circle_ON_OFF;
extern uint8 Meet_ON_OFF;
extern MEET_PLACE_e meet_choose;
extern uint8 meet_ring_number;
extern uint16 long_count_cjx;
extern uint16 add_deviation_cjx;

extern uint8 polarization_ON_OFF; 
uint8 key_scan(void) //按键扫描
{
	if (KeyUp==0) 
	{
		
		DELAY_MS(5);
		if (KeyUp==0)
		{
			while (!KeyUp) ;
			return 8;
		}
	}
	if (KeyDm==0)  
	{
		
		DELAY_MS(5);
		if (KeyDm==0)
		{
			while (!KeyDm) ;
			return 2;
		}
	}
	if (KeyMd==0)  
	{
		
		DELAY_MS(5);
		if (KeyMd==0)
		{
			while (!KeyMd) ;
			return 5;
		}
	}
	if (KeyLt==0)  
	{
		DELAY_MS(5);
		if (KeyLt==0)
		{
			while (!KeyLt) ;
			return 4;
		}
	}
	if (KeyRt==0)  
	{
		DELAY_MS(5);
		if (KeyRt==0)
		{
			while (!KeyRt) ;
			return 6;
		}
	}
	return 0;
}


void Clear_screen() //清屏
{
	static  uint8 LAST_MANU=6;
	
	if(LAST_MANU !=MENU)
	{
		LCD_Fill(0x00);
		page=1;
		row=1;
		unit=1;
		LAST_MANU = MENU;
		
	}
}


void menu_define(void) //菜单定义
{
	uint8 mode=0;
	mode=boma1*1+boma2*2;
	
	if(boma6==0)//强制菜单
	{
		MENU=mode_screen;
		Clear_screen();  
		return;
	}
	
	switch(mode)
	{
		case 1:
		{
			MENU=car_run;
			Clear_screen();
			break;
		}
		case 2:
		{
			MENU=flash;
			Clear_screen();
			break;
		}
		case 3:
		{
			MENU=normaliz;
			Clear_screen();
			break; 
		}
		default: 
		{
			MENU=picture;
			Clear_screen();
		}
	}
	
}



void Boma_OLED() // 读取拨码并显示在OLED上
{
	menu_define();
	Clear_screen();
	
	/*******************************************************
	
	显示图像	
	
	********************************************************/	
	
	if(MENU==picture)   
	{
		picture_show_OLED();
		return;
	}
	
	/*******************************************************
	
	归一化
	
	********************************************************/
	
	if(MENU==normaliz)
	{
		normaliz_OLED();
		return;
		
	}
	
	/*******************************************************
	
	flash菜单
	
	********************************************************/
	
	else if(MENU==flash)
	{
		flash_OLED();
		return;
	}
	
	/*******************************************************
	
	正常发车/电感查看/调速
	
	********************************************************/
	
	else if(MENU==car_run)
	{
		car_run_OLED();
		return;
	}
	
	/*******************************************************
	
	模式调整菜单//单双车//圆环
	
	********************************************************/        
	else if(MENU==mode_screen)
	{
		mode_screen_OLED();
		return;
	}
	
	/*******************************************************
	
	待定、模板
	
	********************************************************/
	
}


void Flash_Key()
{
	switch(key_scan())
	{
		case 4:  //加页
		{
			page+=1;
			LCD_Fill(0x00);
			if(page==5) page=1;
			break;
		}
		case 5:  //换挡
		{
			unit+=1;
			LCD_Fill(0x00);
			if(unit==4) unit=1;
			break;
			
			
		}
		case 6:  //换行
		{
			row++;
			LCD_Fill(0x00);
			if(row==4) row=1;
			break;
		}
		case 8:  //加
		{
			if(page==1)
			{
				if(row==1)
				{
					switch(unit)
					{
						case 1:{S_P+=1;break;}
						case 2:{S_P+=0.5;break;}        
						case 3:{S_P+=0.1;break;}
					}
					flash_updata();
					break;
				}
				else if(row==2)
				{
					switch(unit)
					{
						case 1:{S_D+=1;break;}
						case 2:{S_D+=0.5;break;}        
						case 3:{S_D+=0.1;break;}
					}
					flash_updata();
					break;
				}
				else if(row==3)
				{
					
					break;
				}
				
				
			}
			if(page==2)
			{
                          if(row==1)
                          {
				long_count_cjx+=5;
                                flash_updata();
                                break;
                          }
                          else if(row==2)
                          {
                            add_deviation_cjx+=5;
                             flash_updata();
                              break;
                          }
				
			}
			if(page==3)
			{
				if(row==1)
				{
					switch(unit)
					{
						case 1:{C_P+=1;break;}
						case 2:{C_P+=0.5;break;}        
						case 3:{C_P+=0.1;break;}
					}           
					flash_updata();         
					break;         
				}
				else if(row==2)
				{
					switch(unit)
					{
						case 1:{C_D+=1;break;}
						case 2:{C_D+=0.5;break;}        
						case 3:{C_D+=0.1;break;}
					}  
					flash_updata();                  
					break;
				}
				else if(row==3)
				{
					switch(unit)
					{
						case 1:{Cam2Mag_Weight+=1;break;}
						case 2:{Cam2Mag_Weight+=0.5;break;}        
						case 3:{Cam2Mag_Weight+=0.1;break;}
					}            
					Cam2Mag_Weight=Cam2Mag_Weight<10?Cam2Mag_Weight:10;
					flash_updata();
					break;
				}                                  
			}
			if(page==4)
			{
				if(row==1)
				{
					Servo_Duty+=5;
					break;
				}
				else if(row==2)
				{
					break;
				}
				else if(row==3)
				{
					break;
				}                                      
			}
			if(page==5)
			{
				if(row==1)
				{
					break;
				}
				else if(row==2)
				{
					break;
				}
				else if(row==3)
				{
					break;
				}                                     
			}
		}
		case 2:  //减
		{
			if(page==1)
			{
				if(row==1)
				{
					switch(unit){
						case 1:{S_P-=1;break;}
						case 2:{S_P-=0.5;break;}        
						case 3:{S_P-=0.1;break;}
					}
					if(S_P<=0){S_P=0;}
					flash_updata();
					break;
				}
				else if(row==2)
				{
					switch(unit){
						case 1:{S_D-=1;break;}
						case 2:{S_D-=0.5;break;}        
						case 3:{S_D-=0.1;break;}
					}
					if(S_D<=0){S_D=0;}
					flash_updata();
					break;
				}
				else if(row==3)
				{
					break;
				}
				
				
			}
			if(page==2)
			{
                          if(row==1)
                          {
				long_count_cjx-=5;
                                flash_updata();
                                break;
                          }
                          else if(row==2)
                          {
                            add_deviation_cjx-=5;
                             flash_updata();
                              break;
                          }
				
			}
			if(page==3)
			{
				if(row==1)
				{
					switch(unit)
					{
						case 1:{C_P-=1;break;}
						case 2:{C_P-=0.5;break;}        
						case 3:{C_P-=0.1;break;}
					}  
					if(C_P<=0){C_P=0;}
					flash_updata();         
					break;
				}
				else if(row==2)
				{
					switch(unit)
					{
						case 1:{C_D-=1;break;}
						case 2:{C_D-=0.5;break;}        
						case 3:{C_D-=0.1;break;}
					}  
					if(C_D<=0){C_D=0;}
					flash_updata();         
					break;
					
				}
				else if(row==3)
				{
					switch(unit)
					{
						case 1:{Cam2Mag_Weight-=1;break;}
						case 2:{Cam2Mag_Weight-=0.5;break;}        
						case 3:{Cam2Mag_Weight-=0.1;break;}
					}            
					Cam2Mag_Weight=Cam2Mag_Weight>0?Cam2Mag_Weight:0;
					flash_updata();
					break;
				}                     
			}
			if(page==4)
			{
				if(row==1)
				{
					Servo_Duty-=5;
					break;
				}
				else if(row==2)
				{
					break;
				}
				else if(row==3)
				{
					break;
				}                      
			}
			if(page==5)
			{
				if(row==1)
				{
					break;
				}
				else if(row==2)
				{
					break;
				}
				else if(row==3)
				{
					break;
				}                                     
			}
		}
	}
}



void Car_Run_Key()
{
	switch(key_scan())
	{
		case 4:  //加页
		{
			page+=1;
			LCD_Fill(0x00);
			if(page==6) page=1;
			break;
		}
		case 5:  //权重
		{
			unit+=1;
			LCD_Fill(0x00);
			if(unit==4) unit=1;
			break;
			
		}
		//		case 6:  //换行
		//			{
		//				row++;
		//				LCD_Fill(0x00);
		//				if(row==4) row=1;
		//			   break;
		//			}
		
		
		case 8:  //加
		{
			if(page==1)
			{
				
				
			}
			if(page==2)
			{
				switch(unit)
				{
					case 1:{normal_speed+=5;break;}
					case 2:{normal_speed+=10;break;}        
					case 3:{normal_speed+=20;break;}
				}  
				flash_updata();
				break;
			}
			if(page==3)
			{
				if(car_mode==stop)
				{
					car_mode=run;
					break;
				}
				if(car_mode==run)
				{
					car_mode=stop;
					break;
				}
			}
			if(page==4)
			{
				break;
				
			}
		}
		case 2:  //减
		{
			if(page==1)
			{
				
			}
			if(page==2)
			{
				switch(unit)
				{
					case 1:{normal_speed-=5;break;}
					case 2:{normal_speed-=10;break;}        
					case 3:{normal_speed-=20;break;}
				}  
				flash_updata();
				break;                    
			}
			if(page==3)
			{
				if(car_mode==stop)
				{
					car_mode=run;
					break;
				}
				if(car_mode==run)
				{
					car_mode=stop;
					break;
				}
				
				
			}
			if(page==4)
			{
				break;
			}
		}
	}
}

void  Mode_Screen_Key()
{
	switch(key_scan())
	{
		case 4:  //加页
		{
			page+=1;
			LCD_Fill(0x00);
			if(page==5) page=1;
			break;
		}
		case 5:  //权重
		{
			unit+=1;
			LCD_Fill(0x00);
			if(unit==4) unit=1;
			break;
			
		}
		case 6:  //换行
			{
				row++;
				LCD_Fill(0x00);
				if(row==3) row=1;
			   break;
			}

		
		case 8:  //加
		{
			if(page==1)
			{
				if(Meet_ON_OFF==1)
				{
					Meet_ON_OFF=0;
					meet_choose=no_meet;
					meet_ring_number=0;
					flash_updata(); 
					break;
				}
				if(Meet_ON_OFF==0)
				{
					Meet_ON_OFF=1;
					flash_updata();                                                
					break;
				} 
			}
			if(page==2)
			{
				
				if(Circle_ON_OFF==1)
				{
					Circle_ON_OFF=0;
					flash_updata(); 
					break;
				}
				if(Circle_ON_OFF==0)
				{
					Circle_ON_OFF=1;
					flash_updata();                                                
					break;
				} 
			}
			if(page==3)
			{
				
				if(Meet_ON_OFF==1)
				{
					if(meet_choose==meet_straight)
					{
						meet_choose=meet_circle;
						flash_updata(); 
						break;
					}
					if(meet_choose==meet_circle)
					{
						meet_choose=meet_straight;
						flash_updata(); 
						break;
					}
					else
					{
						meet_choose=meet_straight;
						flash_updata();
						break;
					}	
				}
			}
			if(page==4)
			{
				if(row==1)
				{
                                  
                                        if(Meet_ON_OFF==1 && meet_choose==meet_straight)
					{
                                              if(polarization_ON_OFF==1)
                                              {
                                                polarization_ON_OFF=0;
                                                flash_updata(); 
                                                break;
                                              }
                                              if(polarization_ON_OFF==0)
                                              {
                                                polarization_ON_OFF=1;
                                                flash_updata();                                                
                                                break;
                                              }
                                          
					}
					if(Meet_ON_OFF==1 && meet_choose==meet_circle)
					{
						meet_ring_number+=1;
						if(meet_ring_number==5)
						{
							meet_ring_number=1;
						}
						flash_updata(); 
						break;
					}
				}
				else if(row==2)
				{
					break;
				
				}
			}
			
		}
		case 2:  //减
		{
			if(page==1)
			{
				if(Meet_ON_OFF==1)
				{
					Meet_ON_OFF=0;
					flash_updata(); 
					break;
				}
				if(Meet_ON_OFF==0)
				{
					Meet_ON_OFF=1;
					flash_updata();                                                
					break;
				} 
			}
			if(page==2)
			{
				
				if(Circle_ON_OFF==1)
				{
					Circle_ON_OFF=0;
					flash_updata(); 
					break;
				}
				if(Circle_ON_OFF==0)
				{
					Circle_ON_OFF=1;
					flash_updata();                                                
					break;
				} 
			}
			if(page==3)
			{
				
				if(Meet_ON_OFF==1)
				{
					if(meet_choose==meet_straight)
					{
						meet_choose=meet_circle;
						flash_updata(); 
						break;
					}
					if(meet_choose==meet_circle)
					{
						meet_choose=meet_straight;
						flash_updata(); 
						break;
					}
					else
					{
						meet_choose=meet_straight;
						flash_updata();
						break;
					}
				}
			}
			if(page==4)
			{
				if(row==1)
				{
                                 
                                        if(Meet_ON_OFF==1 && meet_choose==meet_straight)
					{
                                              if(polarization_ON_OFF==1)
                                              {
                                                polarization_ON_OFF=0;
                                                flash_updata(); 
                                                break;
                                              }
                                              if(polarization_ON_OFF==0)
                                              {
                                                polarization_ON_OFF=1;
                                                flash_updata();                                                
                                                break;
                                              }
                                          
					}
					if(Meet_ON_OFF==1 && meet_choose==meet_circle)
					{
						if(meet_ring_number==0)
						{
							break;
						}
						meet_ring_number-=1;
						if(meet_ring_number==0)
						{
							meet_ring_number=1;
						}
						flash_updata(); 
						break;
					}
				}
				else if(row==2)
				{
					break;
				
				}
				
			}
			
			
		}
		
//		case 5:
//		{
//			if(page==5)
//			{
//				
//				Reflash();      
//				
//			}
//		}
	}
}


void  key_control() //按键控制
{ 
	if(MENU==flash)      Flash_Key();
	if(MENU==car_run)     Car_Run_Key();
	if(MENU==mode_screen) Mode_Screen_Key();
	
}

void  Reflash()//重新写入flash
{
	//  S_P=5;
	//  S_D=20;
	//  M_L_I=6;
	//  M_L_P=40;
	//  M_R_I=6;
	//  M_R_P=40;
	//  C_P=4;
	//  C_D=20;
	//  
	//  normal_speed=200;
	//  Circle_ON_OFF=0;	
	//  Meet_ON_OFF=0;
	//  Cam2Mag_Weight=4;
	//  flash_updata();
	//  buzzer_bibi(4,1);  
}


void  flash_updata()
{
	
	flash_erase_sector(servo_sector);
	
	integer=((uint16)(S_P*10))/10;
	decimal=((uint16)(S_P*10))%10;
	flash_write(servo_sector, 0, integer);
	flash_write(servo_sector, 4, decimal);
	
	integer=((uint16)(S_D*10))/10;
	decimal=((uint16)(S_D*10))%10;
	flash_write(servo_sector, 8, integer);
	flash_write(servo_sector, 12, decimal);

	flash_write(servo_sector, 16, long_count_cjx);
        flash_write(servo_sector, 24, add_deviation_cjx);
        
//	flash_write(servo_sector, 20, decimal);
//	
//	integer=((uint16)(M_L_P*10))/10;
//	decimal=((uint16)(M_L_P*10))%10;
//	flash_write(servo_sector, 24, integer);
//	flash_write(servo_sector, 28, decimal);
//	
//	integer=((uint16)(M_R_I*10))/10;
//	decimal=((uint16)(M_R_I*10))%10;
//	flash_write(servo_sector, 32, integer);
//	flash_write(servo_sector, 36, decimal);
//	
//	integer=((uint16)(M_R_P*10))/10;
//	decimal=((uint16)(M_R_P*10))%10;
//	flash_write(servo_sector, 40, integer);
//	flash_write(servo_sector, 44, decimal);
	
	integer=((uint16)(C_P*10))/10;
	decimal=((uint16)(C_P*10))%10;
	flash_write(servo_sector, 48, integer);
	flash_write(servo_sector, 52, decimal);  
	
	integer=((uint16)(C_D*10))/10;
	decimal=((uint16)(C_D*10))%10;
	flash_write(servo_sector, 56, integer);
	flash_write(servo_sector, 60, decimal);  
	
	flash_write(servo_sector, 64, normal_speed);
	
	flash_write(servo_sector, 68,Meet_ON_OFF);
	
	flash_write(servo_sector, 72, Circle_ON_OFF);
	
	
	integer=((uint16)(Cam2Mag_Weight*10))/10;
	decimal=((uint16)(Cam2Mag_Weight*10))%10;
	flash_write(servo_sector, 76, integer);
	flash_write(servo_sector, 80, decimal);  
	flash_write(servo_sector, 84, meet_choose);
	flash_write(servo_sector, 88, meet_ring_number); 
	
        flash_write(servo_sector, 92, polarization_ON_OFF);
        
}



