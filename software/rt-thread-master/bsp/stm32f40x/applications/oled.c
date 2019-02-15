#include "init.h"
#include "math.h"
	
/*---------------------- Constant / Macro Definitions -----------------------*/
extern struct SAcc stcAcc;

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
		//显示温度  get_vol  	  OLED_ShowString(0,16,(u8 *) "Vol:",12); 	
		
		sprintf(str,"Voltage:%.2f v\r\n",get_vol());
		OLED_ShowString(0,16,(u8 *)str,12); 
		sprintf(str,"Temperature:%.2f C\r\n",(float)stcAcc.T/0x100);
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
			0———→x  
							63
*/
/* 开机动画 */
void Boot_Animation(void)
{
		static u8 x=0,y=0;

		for(y = 59;y <= 67;y++){
				OLED_DrawPoint(y,0,1);
				OLED_DrawPoint(y,63,1);
		}

		for(x = 0;x <= 63;x++){
				y = sqrt(pow(32,2)-pow(x-31,2))+64; //圆方程  x,y反置
				OLED_DrawPoint(y,x,1);
				OLED_DrawPoint(127-y,x,1);
		}
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


void draw_circle(u8 x,u8 y,u8 R) //圆心(x,y),半径R
{
		for(x = 0;x <= 63;x++){
				y = sqrt(pow(R,2)-pow(x-31,2))+64; //圆方程  x,y反置
				OLED_DrawPoint(y,x,1);
				OLED_DrawPoint(127-y,x,1);
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



