#include "init.h"
#include "math.h"
	
/*---------------------- Constant / Macro Definitions -----------------------*/
	
float slope = 0.0;
	
/*----------------------- Variable Declarations -----------------------------*/

/* ALL_init 事件控制块 */
extern struct rt_event init_event;
extern struct SAcc stcAcc;
extern struct SAngle 	stcAngle;

extern u8 Flash_Logo[];
extern int page_num ;

typedef enum
{
		StatusPage,
		GyroscopePage,
		FlashPage,   
		PicturePage

}
MENU_LIST_e; //菜单

char *PageName[20] = {"StatusPage","GyroscopePage","FlashPage","PicturePage"};

volatile MENU_LIST_e MENU = StatusPage; //OLED初始页面为 状态页. volatile是一种类型修饰符。
//volatile 的作用 是作为指令关键字，确保本条指令不会因编译器的优化而省略，且要求每次直接读值。
/*----------------------- Function Implement --------------------------------*/

void menu_define(void) //菜单定义
{
	static int page_change = 0; //暂存页码

	if(page_num > 3) page_num = 0;
	if(page_num < 0) page_num = 0;
	if(page_change != page_num){
			buzzer_bibi(1,1);
			rt_kprintf("\n Current Menu_Page: %s \n",PageName[page_num]);
			OLED_Clear();
	}
	page_change = page_num;

	switch(page_num){
			case 0:{
				MENU = StatusPage;OLED_StatusPage();			break;
			}
			case 1:{
				MENU = GyroscopePage;OLED_GyroscopePage();break;
			}
			case 2:{
				MENU = FlashPage;		 											break;
			}
			case 3:{
				MENU = PicturePage;Boot_Animation();		  break;
			}	
	}
}

	
void oled_thread_entry(void* parameter)
{


	Boot_Animation();	//开机动画
	OLED_Clear();
	page_num = 3;
	while(1)
	{	
			
			menu_define();
			rt_thread_mdelay(100);
	}
	
}

void OLED_StatusPage(void)
{
		char str[100];
		OLED_ShowString(0,0, (u8 *)"Mode:",12); 

	
		if(boma_value_get() != AUV_Mode){
				OLED_ShowString(50,0,(u8 *)"[ ROV ]",12);
		}
		else {
  			OLED_ShowString(50,0,(u8 *)"[ AUV ]",12);			
		}		
		
		sprintf(str,"Voltage:%.2f v\r\n",get_vol());
		OLED_ShowString(0,16,(u8 *)str,12); 
		sprintf(str,"Temperature:%.2f C\r\n",(float)stcAcc.T/100);
		OLED_ShowString(0,48,(u8 *)str,12);
		OLED_Refresh_Gram();//更新显示到OLED
}


/* OLED 坐标系如下: */
/*
	127 ↑y
			---------
			|	      |
			|				|
			|				|
			|				|
			|				|
			|				|
			0-------→x  
							63
*/
/* 开机动画 */
void Boot_Animation(void)
{
		char str[100];
		static u8 x=0,y=0;
		slope = tan((float)(stcAngle.Angle[2]*3.14/32768));
		sprintf(str,"Angle z:%.3f\r\n",(float)stcAngle.Angle[2]/32768*180);
		rt_kprintf(str);	
	
		sprintf(str,"pai:%.3f\r\n",(float)(stcAngle.Angle[2]*3.14/32768));
		rt_kprintf(str);	
	
		sprintf(str,"slope :%.3f\r\n",slope);
		rt_kprintf(str);		
		for(y = 59;y <= 67;y++){
				OLED_DrawPoint(y,0,1);
				OLED_DrawPoint(y,63,1);
		}
		draw_circle(31,63,32);
		
		draw_line(31,63,slope);

		
//		for(x = 0;x <= 63;x++){
//				y = sqrt(pow(32,2)-pow(x-31,2))+64; //圆方程  x,y反置
//				OLED_DrawPoint(y,x,1);
//				OLED_DrawPoint(127-y,x,1);
//		}
//		for(x = 63;x>=18;x--){
//				OLED_DrawPoint(108-0.7*x,x,1);//画斜线 x,y反置
//				OLED_DrawPoint(17 +0.7*x,x,1);
//				y = 64-x;
//				OLED_DrawPoint(64-0.7*y,y,1);
//				OLED_DrawPoint(64+0.7*y,y,1);
//				rt_thread_delay(2);
//			  OLED_Refresh_Gram();//更新显示到OLED
//		}
//		
//		for(x = 30;x <= 94;x++){
//				OLED_DrawPoint(125-x,47,1);
//				OLED_DrawPoint(x,18,1);
//				rt_thread_delay(2);
//			  OLED_Refresh_Gram();//更新显示到OLED
//		}

//		OLED_ShowString(60,20,(u8 *)"E",16);
	  OLED_Refresh_Gram();//更新显示到OLED
		rt_thread_delay(100);
		
}

void get_slope(void)
{
		char str[100];
		sprintf(str,"k= %f\n",(double)slope);
		rt_kprintf(str);
}
MSH_CMD_EXPORT(get_slope,get_slope[k]);

void draw_circle(u8 x0,u8 y0,u8 r) //圆心(x,y),半径r
{
		u8 x,y;
		for(x = 0;x <= 63;x++){
				y = sqrt(pow(r,2)-pow(x-x0,2))+y0+1; //圆方程  x,y反置
				OLED_DrawPoint(y,x,1);
				OLED_DrawPoint(127-y,x,1);
		}
		
//		for(x = 0;x <= 63;x++){
//			y = sqrt(pow(32,2)-pow(x-31,2))+64; //圆方程  x,y反置
//			OLED_DrawPoint(y,x,1);
//			OLED_DrawPoint(127-y,x,1);
//		}
}

void draw_line(u8 x0,u8 y0,float k) //过固定点(x0,y0),斜率k
{
	
		u8 x,y;
		y = sqrt(pow(32,2)-pow(x-31,2))+63+1; //圆方程  x,y反置
		//y = (u8)(k*(x-x0)+y0);
		for(x = 0;x <= 63;x++){
				if((k*(x-x0)+y0)< y ){//点限制在 园内
						OLED_DrawPoint((k*(x-x0)+y0),x,1);}
		}

}
/* OLED 线程初始化 */
int oled_thread_init(void)
{
    rt_thread_t oled_tid;
		/*创建动态线程*/
    oled_tid = rt_thread_create("oled", //线程名称
                    oled_thread_entry,	//线程入口函数【entry】
                    RT_NULL,				    //线程入口函数参数【parameter】
                    1024,							  //线程栈大小，单位是字节【byte】
                    10,								  //线程优先级【priority】
                    10);							  //线程的时间片大小【tick】= 100ms

    if (oled_tid != RT_NULL){
				OLED_Init();
				LOG_I("OLED_Init()");
				rt_thread_startup(oled_tid);
				rt_event_send(&init_event, OLED_EVENT);
		}
		return 0;
}
INIT_APP_EXPORT(oled_thread_init);



/* OLED 下一页 */
void next_oled_page(void)
{
		//数据打包成string型
		buzzer_once();	
		page_num ++;
}
MSH_CMD_EXPORT(next_oled_page,next_oled_page[page++]);

/* OLED 上一页 */
void last_oled_page(void)
{
		buzzer_once();	
		page_num --;
}
MSH_CMD_EXPORT(last_oled_page,last_oled_page[page--]);



