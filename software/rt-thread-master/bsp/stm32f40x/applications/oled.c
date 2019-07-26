/*
 * oled.c
 *
 *  Created on: 2019年2月30日
 *      Author: zengwangfa
 *      Notes:  OLED状态、参数指示
 */
#define LOG_TAG    "oled"

#include "oled.h"
#include <rtthread.h>
#include <math.h>
#include <elog.h>
#include <stdio.h>
#include "sys.h"

#include "led.h"
#include "rc_data.h"
#include "drv_cpu_temp.h"
#include "drv_cpuusage.h"
#include "drv_oled.h"
#include "drv_adc.h"
#include "sensor.h"

#include "ioDevices.h"

#include "gyroscope.h"
#include "sensor.h"
#include "filter.h"

#include "EasyThread.h"
/* 自定义OLED 坐标系如下: 

	127 ↑y
			---------
			|	      |
			|				|
			|				|
			|				|
			|				|
			|				|
		(0,0)-----→x  
							63
*/
/*---------------------- Static / Constant / Macro Definitions ---------------------*/
static float slope = 0.0; //东北天坐标系下 航向斜率 slope
static uint32 total_mem,used_mem,max_used_mem;

/*--------------------------- Variable Declarations --------------------------------*/

char *VehicleModeName[2] = {"4_AXIS","6_AXIS"}; //定义用于显示的 模式字符 0->AUV  1->ROV
volatile MENU_LIST_Enum MENU = StatusPage;//OLED初始页面为 状态页. volatile是一种类型修饰符。
																				  //volatile 的作用 是作为指令关键字，确保本条指令不会因编译器的优化而省略，且要求每次直接在其内存中读值。


extern struct rt_event init_event;/* ALL_init 事件控制块 */


/* OLED 变量 初始化. */
Oled_Type oled = {	 
									 .pagenum = LockPage,		 //页码 pagenum
									 .pagechange = LockPage, //暂存页码 检测页码是否改变 pagechange
									 .pagechange_flag = 0,     //页码改变标志位 pagechange flag
									 .pagename = //页名定义 pagename
										{	
												"StatusPage",
												"GyroscopePage",
												"LockPage",
												"PicturePage"} 							
};
/*----------------------- Function Implement --------------------------------*/

/* OLED 线程初始化 */
int oled_thread_init(void)
{
    rt_thread_t oled_tid;
		/*创建动态线程*/
    oled_tid = rt_thread_create("oled", //线程名称
                    oled_thread_entry,	//线程入口函数【entry】
                    RT_NULL,				    //线程入口函数参数【parameter】
                    2048,							  //线程栈大小，单位是字节【byte】
                    15,								  //线程优先级【priority】
                    10);							  //线程的时间片大小【tick】= 100ms

    if (oled_tid != RT_NULL){
				OLED_Init();
				log_i("OLED_Init()");
				rt_thread_startup(oled_tid);
				//rt_event_send(&init_event, OLED_EVENT);
				oled.pagechange = oled.pagenum;  //初始化暂存页面
		}
		return 0;
}
INIT_APP_EXPORT(oled_thread_init);

/*******************************************
* 函 数 名：menu_define
* 功    能：菜单选择
* 输入参数：none
* 返 回 值：none
* 注    意：页码切换时，蜂鸣器响一声
********************************************/
void menu_define(void) //菜单定义
{
	if(oled.pagenum >= OLED_Page_MAX || oled.pagenum < StatusPage) oled.pagenum = StatusPage; //超出页面范围 则为第一页
	if(oled.pagechange != oled.pagenum){ //当页码改变
			Buzzer_Set(&Beep,1,1);
			rt_kprintf("Current Menu_Page: %s \n",oled.pagename[oled.pagenum-1]);
			OLED_Clear(); //清屏
			oled.pagechange_flag = 1;
	}
	else {oled.pagechange_flag = 0;}
	oled.pagechange = oled.pagenum;
	
	if(ControlCmd.All_Lock == LOCK){// 当拨码不是强制解锁 锁定页面
			oled.pagenum = LockPage;
	}
	switch(oled.pagenum){
			case 1:{
					MENU = StatusPage;	 OLED_StatusPage();		break;
			}
			case 2:{
					MENU = GyroscopePage;OLED_GyroscopePage();break;
			}
			case 3:{
					MENU = LockPage;		 OLED_LockPage(); 	  break; //锁定界面
			}
			case 4:{
					MENU = PicturePage;	 OLED_PicturePage(); break;
			}	
	}
}


/*******************************************
* 函 数 名：OLED_StatusPage
* 功    能：显示系统状态页面
* 输入参数：none
* 返 回 值：none
* 注    意：系统第一页 【状态页】
********************************************/
void OLED_StatusPage(void)
{
		static char str[50];
		
		if(is_raspi_start()){
				OLED_ShowMyChar(100,0,0,16,1); //3G数据图标2
		}
		else {OLED_ShowMyChar(119,0,2,16,1);} //清空图标0
		if(is_wifi_connect()){
				OLED_ShowMyChar(119,0,1,16,1);} //Wifi图标1
		else {OLED_ShowMyChar(119,0,2,16,1);} //清空图标0
	
		sprintf(str,"Mode:[%s-NO.%d]",VehicleModeName[VehicleMode],get_boma_value()); //获取本机为ROV or AUV
		OLED_ShowString(0,0, (uint8 *)str,12); 
	
		sprintf(str,"Vol:%.2fV",Sensor.PowerSource.Voltage);//电压
		OLED_ShowString(0,16,(uint8 *)str,12); 
		
		sprintf(str,"Cur:%.2fA",Sensor.PowerSource.Current);//电流
		OLED_ShowString(70,16,(uint8 *)str,12); 	
		
	  sprintf(str,"CPU:%.2f%% ",Sensor.CPU.Usage);//%字符的转义字符是%%  %这个字符在输出语句是向后匹配的原则
		OLED_ShowString(0,32,(uint8 *)str,12); 
		
		sprintf(str,"Temp:%.1fC ",Sensor.CPU.Temperature);//显示的温度
		OLED_ShowString(70,32,(uint8 *)str,12);
		
		sprintf(str,"Depth:%.2f cm      ",Sensor.DepthSensor.Depth);//显示的温度
		OLED_ShowString(0,48,(uint8 *)str,12);
		OLED_Refresh_Gram();//更新显示到OLED
}


/*******************************************
* 函 数 名：OLED_GyroscopePage
* 功    能：显示九轴模块参数【加速度、角速度、欧拉角、磁场】
* 输入参数：none
* 返 回 值：none
* 注    意：OLED第二页 【九轴参数页】
********************************************/
void OLED_GyroscopePage(void)
{
		char str[100];
		sprintf(str,"Acc:%.2f %.2f %.2f  ",Sensor.JY901.Acc.x,Sensor.JY901.Acc.y,Sensor.JY901.Acc.z);
		OLED_ShowString(0,0,(uint8 *)str,12); 	
		
		sprintf(str,"Gyro:%.1f %.1f %.1f ",Sensor.JY901.Gyro.x,Sensor.JY901.Gyro.y,Sensor.JY901.Gyro.z);
		OLED_ShowString(0,16,(uint8 *)str,12); 	
		
		sprintf(str,"Ang:%.1f %.1f %.1f  ",Sensor.JY901.Euler.Roll,Sensor.JY901.Euler.Pitch,Sensor.JY901.Euler.Yaw);
		OLED_ShowString(0,32,(uint8 *)str,12); 	
		 
		sprintf(str,"Mag:%d %d %d  ",Sensor.JY901.Mag.x,Sensor.JY901.Mag.y,Sensor.JY901.Mag.z);
		OLED_ShowString(0,48,(uint8 *)str,12); 
		
	  OLED_Refresh_Gram();//更新显示到OLED
}
/*******************************************
* 函 数 OLED_LockPage
* 功    能：显示
* 输入参数：none
* 返 回 值：none
* 注    意：OLED第三页 
********************************************/
void OLED_LockPage(void)
{		

		static char str[30] = {0};   //暂存OLED字符串
		static uint8 vol_box = 0; 	 //电压框 格子数
		static uint8 vol_percent = 0;//电压百分比
		
		if(Sensor.PowerSource.Capacity != 0){ //判定非0
				vol_box = (Sensor.PowerSource.Voltage-(Sensor.PowerSource.Capacity*STANDARD_VOLTAGE/FULL_VOLTAGE))*12/
								  (Sensor.PowerSource.Capacity/(2*FULL_VOLTAGE)); //oled电量显示 = 真实电压值*12格/最大电池容量的电压
			
				vol_percent = (Sensor.PowerSource.Voltage-(Sensor.PowerSource.Capacity*STANDARD_VOLTAGE/FULL_VOLTAGE))/
					            (Sensor.PowerSource.Capacity/(2*FULL_VOLTAGE))*100;  //电量百分比 = （真实电压值-安全电压值）*100%
			
				vol_box = vol_box > 12 ? 12 : vol_box; 						  //电压格子数 限幅
				vol_box = vol_box <= 0  ? 0  : vol_box;
				vol_percent = vol_percent > 100 ? 100 : vol_percent;//电压百分比限幅
				vol_percent = vol_percent <= 0   ? 0   : vol_percent;
			
				Sensor.PowerSource.Percent = vol_percent;
		}		
		else{ //如果未设定,则不计算 <电压框格子数> 和 <电量百分比> ,并提示设定电池容量参数 
				log_w("not yet set_battery_capacity parameter!");
				Buzzer_Set(&Beep,1,1);	
				rt_thread_mdelay(5000); //5s
		}
		
		if(is_raspi_start()){//树莓派是否启动服务器程序
				Buzzer_Set(&Beep,3,1);
				OLED_ShowPicture(0,28,raspberry_logo,28,33);//显示树莓派LOGO
		}

		sprintf(str,"Vol:%.2fV  \r\n",Sensor.PowerSource.Voltage);//电压
		OLED_ShowString(0,0, (uint8 *)str,12);
		
		sprintf(str,"Cur:%.2f A  \r\n",Sensor.PowerSource.Current);//电流
		OLED_ShowString(0,12,(uint8 *)str,12); 	
		
		sprintf(str,"%d%% ",vol_percent);//当前电量百分比 %
		OLED_ShowString(80,0, (uint8 *)str,12);

		OLED_ShowPicture(107,0,bmp_battery[vol_box],10,16);//显示电量
		OLED_ShowPicture(49,43-15,bmp_lock[ControlCmd.All_Lock-1],30,30);//锁屏
		
		OLED_Refresh_Gram();//更新显示到OLED
}

/*******************************************
* 函 数 名：OLED_PicturePage
* 功    能：显示OLED电子罗盘图像页面
* 输入参数：none
* 返 回 值：none
* 注    意：OLED第四页 【图像页】
********************************************/
void OLED_PicturePage(void)
{
		static uint8 y=0;
		static char str[100];
		static int Angle_x = 0,Angle_y = 0;
		
		draw_fill_circle(31+Angle_x,31+Angle_y,6,0); //清空实心圆，用于刷新坐标
		draw_line(31,31,slope,0); //清除上一次画的线 进行刷新
	
		OLED_Refresh_Gram();//更新显示到OLED
	
		Angle_x = Sensor.JY901.Euler.Roll/5;
		Angle_y = Sensor.JY901.Euler.Pitch/5;
		if(fabs(Sensor.JY901.Euler.Yaw) != 90) //90 deg无效
				slope = tan((float)Deg2Rad(Sensor.JY901.Euler.Yaw));  //转化弧度制 解算东北天坐标系下 航向斜率slope
	
		for(y = 28;y <= 36;y++){ //补圆顶、底部的缺失点
				OLED_DrawPoint(y,0,1);
				OLED_DrawPoint(y,63,1);
		}

		draw_line(31,31,slope,1);
		
		sprintf(str,"Rol:%3.1f  ",Sensor.JY901.Euler.Roll); //横滚角Roll 
		OLED_ShowString(65,0, (uint8 *)str,12);
		
		sprintf(str,"Pit:%3.1f  ",Sensor.JY901.Euler.Pitch);//俯仰角Pitch
		OLED_ShowString(65,16, (uint8 *)str,12);
		
		sprintf(str,"Yaw:%3.1f  ",Sensor.JY901.Euler.Yaw); //偏航角Yaw
		OLED_ShowString(65,32, (uint8 *)str,12);
		
		sprintf(str,"k:%.1f   ",slope);
		OLED_ShowString(65,48,(uint8 *)str,12); 
		

		OLED_ShowString(29,2 ,(uint8 *)"N",12);
		OLED_ShowString(29,51,(uint8 *)"S",12);
		OLED_ShowString(3	,28,(uint8 *)"W",12);
		OLED_ShowString(55,28,(uint8 *)"E",12);
		
		draw_circle(31,31,32);//画固定圆
		draw_fill_circle(31+Angle_x,31+Angle_y,6,1); //画实心圆
	
		OLED_Refresh_Gram();//更新显示到OLED						
}



/*******************************************
* 函 数 名：Boot_Animation
* 功    能：开机动画
* 输入参数：none
* 返 回 值：none
* 注    意：none
* 说    明：绘制ETA简易LOGO
********************************************/
void Boot_Animation(void)
{
		static uint8 x=0,y=0;

		for(x = 63;x>=18;x--){
				OLED_DrawPoint(108-0.7*x,x,1);//画斜线 x,y反置
				OLED_DrawPoint(17 +0.7*x,x,1);
				y = 64-x;
				OLED_DrawPoint(64-0.7*y,y,1);
				OLED_DrawPoint(64+0.7*y,y,1);
				rt_thread_delay(2);
			  OLED_Refresh_Gram();//更新显示到OLED
		}
		
		for(x = 30;x <= 94;x++){
				OLED_DrawPoint(125-x,47,1);
				OLED_DrawPoint(x,18,1);
				rt_thread_delay(2);
			  OLED_Refresh_Gram();//更新显示到OLED
		}

		OLED_ShowString(60,20,(uint8 *)"E",16);
	  OLED_Refresh_Gram();//更新显示到OLED
		rt_thread_delay(100);
		
}



/*******************************************
* 函 数 名：draw_fill_circle
* 功    能：绘制填充圆
* 输入参数：(x0,y0)圆心坐标、r为半径
						dot为1时 画实心圆，dot为0时 清空实心圆
* 返 回 值：none
* 注    意：none
********************************************/
void draw_fill_circle(uint8 x0,uint8 y0,uint8 r,uint8 dot)//写画实心圆心(x0,y0),半径r
{	
		uint8 x = 0,y = 0,R = 0;
		for(x = x0-r;x <= x0+r;x++){
				for(y = y0-r; y <= y0+r ;y++ ){
						R = sqrt(pow(r,2)-pow(x-x0,2))+y0; //圆方程  x,y反置		
						if( (y >= y0 && y <= R) || (y < y0 && y >= 2*y0-R )|| 0 == dot ) {  //点限制在 圆方程内	
								OLED_DrawPoint(y,x,dot);
						}	
				}
		}
}

/*******************************************
* 函 数 名：draw_circle
* 功    能：绘制空心圆
* 输入参数：(x0,y0)圆心坐标、r为半径
* 返 回 值：none
* 注    意：none
********************************************/
void draw_circle(uint8 x0,uint8 y0,uint8 r) //圆心(x0,y0),半径r
{
		uint8 x,y;

		for(x = 0;x <= 63;x++){
				y = sqrt(pow(r,2)-pow(x-x0,2))+y0; //圆方程  x,y反置
				OLED_DrawPoint(y,x,1);      //上半圆
				OLED_DrawPoint(63-y,x,1);  //下半圆
		}
		
}


/*******************************************
* 函 数 名：draw_line
* 功    能：画一过固定点线段
* 输入参数：(x0,y0)为固定点坐标、k为斜率
* 返 回 值：none
* 注    意：使用不同坐标系 为了解决函数上 x映射y时只能多对一的关系
						该线段长度为圆方程的半径，将线段长度限制在圆内
********************************************/
void draw_line(uint8 x0,uint8 y0,float k,uint8 dot) //过固定点(x0,y0),斜率k   dot:0,清空;1,填充	  
{
		uint8 x,y;
/* 以下函数使用该坐标系: 
	
	                127 ↑y
			-----------------
			|								|
			|								|
			|								|
			|								|
		←---------------(0,0)x  
			63
*/
		for(x = 0;x <= 63;x++){
				y = sqrt(pow(20,2)-pow(x-31,2))+31+1; //圆方程  x,y反置
			
				if( (Sensor.JY901.Euler.Yaw >-135 && Sensor.JY901.Euler.Yaw <-90 ) ||(Sensor.JY901.Euler.Yaw >90 && Sensor.JY901.Euler.Yaw < 145 ) || 0 == dot ){ //上半圆
						if(  ((x-x0)/k+y0) >= 31 && ((x-x0)/k+y0) < y ) {  //点限制在 圆方程内
								OLED_DrawPoint(x,((x-x0)/k+y0),dot);}
				}
				
				if( (Sensor.JY901.Euler.Yaw < -45 && Sensor.JY901.Euler.Yaw > -90) || (Sensor.JY901.Euler.Yaw < 90 && Sensor.JY901.Euler.Yaw> 45) || 0 == dot  ){ //上半圆
						if(  ((x-x0)/k+y0) <= 31 && ((x-x0)/k+y0)> 63-y ) {  //点限制在 圆方程内
								OLED_DrawPoint(x,((x-x0)/k+y0),dot);}
				}
		}
/* 以下函数使用该坐标系: 
	127 ↑y
			---------
			|	      |
			|				|
			|				|
			|				|
			|				|
			|				|
			|				|
		(0,0)-----→x  
							63
*/
		for(x = 0;x <= 63;x++){
				y = sqrt(pow(20,2)-pow(x-31,2))+31+1; //圆方程  x,y反置
		
				if( (Sensor.JY901.Euler.Yaw>=-45 && Sensor.JY901.Euler.Yaw <= 0) || (Sensor.JY901.Euler.Yaw >=-180 && Sensor.JY901.Euler.Yaw <= -135)  || 0 == dot ){  // Sensor.JY901.Angle[2] < 0
						if( (k*(x-x0)+y0) >= 31 && (k*(x-x0)+y0) < y ) {  //点限制在 圆方程内   上半圆
								OLED_DrawPoint((k*(x-x0)+y0),x,dot);}
				}
				
				if( (Sensor.JY901.Euler.Yaw > 0 && Sensor.JY901.Euler.Yaw <= 45) || (Sensor.JY901.Euler.Yaw >=135 && Sensor.JY901.Euler.Yaw <= 180)  || 0 == dot ){  // Sensor.JY901.Angle[2] < 0
						if(((k*(x-x0)+y0)< 31 && (k*(x-x0)+y0) > 63-y)) {  //点限制在 圆方程内  下半圆
								OLED_DrawPoint((k*(x-x0)+y0),x,dot);} 
				}
		}

}



/* OLED 下一页 */
void next_oled_page(void)
{
		oled.pagenum ++;
}
MSH_CMD_EXPORT(next_oled_page,next_oled_page[page++]);

/* OLED 上一页 */
void last_oled_page(void)
{
		oled.pagenum --;
}
MSH_CMD_EXPORT(last_oled_page,last_oled_page[page--]);


void get_slope(void)
{
		char str[100];
		sprintf(str,"k= %f\n",slope);
		rt_kprintf(str);
}
MSH_CMD_EXPORT(get_slope,get_slope[k]);


void get_cpuusage(void)
{
		rt_memory_info(&total_mem, &used_mem, &max_used_mem);
    rt_kprintf("Total_Mem:%ld  Used_Mem:%ld  Max_Used_Mem:%ld\n",total_mem,used_mem,max_used_mem);

}
MSH_CMD_EXPORT(get_cpuusage,get_cpuusage);
//STM32F405RGT6   Total_Mem:131048  Used_Mem:22044  Max_Used_Mem:22368

