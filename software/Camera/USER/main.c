#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "usmart.h"  
#include "usart2.h"  
#include "timer.h" 
#include "ov2640.h" 
#include "dcmi.h" 
//ALIENTEK 探索者STM32F407开发板 实验35
//摄像头 实验 -库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

u8 ov2640_mode=1;						//工作模式:0,RGB565模式;1,JPEG模式

#define jpeg_buf_size 10*1024//31*1024  			//定义JPEG数据缓存jpeg_buf的大小(*4字节)
__align(4) u32 jpeg_buf[jpeg_buf_size];	//JPEG数据缓存buf
volatile u32 jpeg_data_len=0; 			//buf中的JPEG有效数据长度 
volatile u8 jpeg_data_ok=0;				//JPEG数据采集完成标志 
										//0,数据没有采集完;
										//1,数据采集完了,但是还没处理;
										//2,数据已经处理完成了,可以开始下一帧接收
//JPEG尺寸支持列表
const u16 jpeg_img_size_tbl[][2]=
{
	176,144,	//QCIF
	160,120,	//QQVGA
	352,288,	//CIF
	320,240,	//QVGA
	640,480,	//VGA
	800,600,	//SVGA
	1024,768,	//XGA
	1280,1024,	//SXGA
	1600,1200,	//UXGA
}; 
const u8*EFFECTS_TBL[7]={"Normal","Negative","B&W","Redish","Greenish","Bluish","Antique"};	//7种特效 
const u8*JPEG_SIZE_TBL[9]={"QCIF","QQVGA","CIF","QVGA","VGA","SVGA","XGA","SXGA","UXGA"};	//JPEG图片 9种尺寸 


//处理JPEG数据
//当采集完一帧JPEG数据后,调用此函数,切换JPEG BUF.开始下一帧采集.
void jpeg_data_process(void)
{
	if(ov2640_mode)//只有在JPEG格式下,才需要做处理.
	{
		if(jpeg_data_ok==0)	//jpeg数据还未采集完?
		{	
			DMA_Cmd(DMA2_Stream1, DISABLE);//停止当前传输 
			while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}//等待DMA2_Stream1可配置  
			jpeg_data_len=jpeg_buf_size-DMA_GetCurrDataCounter(DMA2_Stream1);//得到此次数据传输的长度
				
			jpeg_data_ok=1; 				//标记JPEG数据采集完按成,等待其他函数处理
		}
		if(jpeg_data_ok==2)	//上一次的jpeg数据已经被处理了
		{
				DMA2_Stream1->NDTR=jpeg_buf_size;	
				DMA_SetCurrDataCounter(DMA2_Stream1,jpeg_buf_size);//传输长度为jpeg_buf_size*4字节
				DMA_Cmd(DMA2_Stream1, ENABLE);			//重新传输
				jpeg_data_ok=0;						//标记数据未采集
		}
	}
} 
//JPEG测试
//JPEG数据,通过串口2发送给电脑.
void jpeg_test(void)
{
	u32 i; 
	u8 *p;
	u8 effect=0,saturation=2,contrast=2;
	u8 size=4;		//默认是QVGA 320*240尺寸
	u8 msgbuf[15];	//消息缓存区 

	sprintf((char*)msgbuf,"JPEG Size:%s",JPEG_SIZE_TBL[size]);
	printf("%s\r\n",msgbuf);
	
 	OV2640_JPEG_Mode();		//JPEG模式
	My_DCMI_Init();			//DCMI配置
	DCMI_DMA_Init((u32)&jpeg_buf,jpeg_buf_size,DMA_MemoryDataSize_Word,DMA_MemoryInc_Enable);//DCMI DMA配置   
	OV2640_OutSize_Set(jpeg_img_size_tbl[size][0],jpeg_img_size_tbl[size][1]);//设置输出尺寸 
	DCMI_Start(); 		//启动传输
	while(1)
	{
		if(jpeg_data_ok==1)	//已经采集完一帧图像了
		{  
			p=(u8*)jpeg_buf;
			//LCD_ShowString(30,210,210,16,16,"Sending JPEG data..."); //提示正在传输数据
			for(i=0;i<jpeg_data_len*4;i++)		//dma传输1次等于4字节,所以乘以4.
			{
        while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);	//循环发送,直到发送完毕  		
				USART_SendData(USART3,p[i]); 

			} 
			if(KEY_Scan(0))	//有按键按下,需要处理
			{  
				//LCD_ShowString(30,210,210,16,16,"Quit Sending data   ");//提示退出数据传输
					printf("Quit Sending data\r\n ");
					effect++;
					if(effect>6)effect=0;
					OV2640_Special_Effects(effect);//设置特效
					sprintf((char*)msgbuf,"%s",EFFECTS_TBL[effect]);
					printf("--------------------\r\n         effect:%s\r\n --------------------\r\n",EFFECTS_TBL[effect]);
		
			//LCD_Fill(30,180,239,190+16,WHITE);
			//LCD_ShowString(30,180,210,16,16,msgbuf);//显示提示内容
				delay_ms(300); 				  
			}//else printf("Send data complete!!\r\n ");//提示传输结束设置 
			jpeg_data_ok=2;	//标记jpeg数据处理完了,可以让DMA去采集下一帧了.
		}		
	}    
} 

int main(void)
{ 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	usart2_init(42,115200);		//初始化串口2波特率为115200
	usart3_init(460800);
	uart4_init(115200);
	LED_Init();					//初始化LED 
 
 	KEY_Init();					//按键初始化 
	TIM3_Int_Init(10000-1,8400-1);//10Khz计数,1秒钟中断一次
	
 	usmart_dev.init(84);		//初始化USMART
 
	while(OV2640_Init())//初始化OV2640
	{
		delay_ms(200);
		printf("OV2640 ERR\r\n");
		delay_ms(200);
	}
	printf("OV2640 OK\r\n");	  
 	while(1)
	{	
		jpeg_test();
	}


}
