#include "sys.h"
#include "init.h"
#include "led.h"
#include "key.h"
#include "drv_ov2640.h" 
#include "drv_dcmi.h" 

#include "string.h"
/*---------------------- Constant / Macro Definitions -----------------------*/

#define jpeg_buf_size 20 * 1024   //31*1024  定义JPEG数据缓存jpeg_buf的大小(*4字节)

/*----------------------- Variable Declarations -----------------------------*/
/* ALL_init 事件控制块. */
extern struct rt_event init_event;
extern rt_device_t debug_uart_device;	

u8 ov2640_mode=1;						//工作模式:0,RGB565模式;1,JPEG模式

__align(4) u32 jpeg_buf[jpeg_buf_size];	//JPEG数据缓存buf
volatile u32 jpeg_data_len=0; 	//buf中的JPEG有效数据长度 
volatile u8 jpeg_data_ok=0;			//JPEG数据采集完成标志 
																//0,数据没有采集完;
																//1,数据采集完了,但是还没处理;
																//2,数据已经处理完成了,可以开始下一帧接收
/*----------------------- Function Implement --------------------------------*/

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

//特效设置
enum OV_Effect{ 
		OV_Normal,  //0:普通模式 
		OV_Negative,//1,负片
		OV_BW,			//2,黑白 
		OV_Redish,  //3,偏红色
		OV_Greenish,//4,偏绿色
		OV_Bluish,	//5,偏蓝色
		OV_Antique  //6,复古	
};

//大小设置
enum OV_Size{ 
		OV_QCIF,  
		OV_QQVGA,
		OV_CIF,		
		OV_QVGA,  
		OV_VGA,
		OV_SVGA,	
		OV_XGA,
		OV_SXGA,
		OV_UXGA  //6,复古	
};
const char *EFFECTS_TBL[7]={"Normal","Negative","B&W","Redish","Greenish","Bluish","Antique"};	//7种特效 
const char *JPEG_SIZE_TBL[9]={"QCIF","QQVGA","CIF","QVGA","VGA","SVGA","XGA","SXGA","UXGA"};	//JPEG图片 9种尺寸 


//处理JPEG数据
//当采集完一帧JPEG数据后,调用此函数,切换JPEG BUF.开始下一帧采集.
void jpeg_data_process(void)
{
		if(ov2640_mode)//只有在JPEG格式下,才需要做处理.
		{
				if(jpeg_data_ok==0)	//jpeg数据还未采集完
				{	
						DMA_Cmd(DMA2_Stream1, DISABLE);//停止当前传输 
						while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}//等待DMA2_Stream1可配置  
						jpeg_data_len = jpeg_buf_size - DMA_GetCurrDataCounter(DMA2_Stream1);//得到此次数据传输的长度

						jpeg_data_ok=1; 				//标记JPEG数据采集完按成,等待其他函数处理
				}
				if(jpeg_data_ok==2)	//上一次的jpeg数据已经被处理了
				{
						DMA2_Stream1->NDTR=jpeg_buf_size;	
						DMA_SetCurrDataCounter(DMA2_Stream1,jpeg_buf_size);//传输长度为jpeg_buf_size*4字节
						DMA_Cmd(DMA2_Stream1, ENABLE);			//重新传输
						jpeg_data_ok=0;										  //标记数据未采集
				}
		}
} 

//JPEG数据,通过WIFI发送给电脑.
void camera_thread_entry(void* paramter)
{
	  u32 i,jpgstart,jpglen; 
		u8 headok=0;
		u8 *p;
		//u8 effect=0,saturation=2,contrast=2;
		u8 size=1;			//默认是QVGA 320*240尺寸

		log_v("JPEG Size:%s ",JPEG_SIZE_TBL[size]);
		
		OV2640_JPEG_Mode();		//JPEG模式
		My_DCMI_Init();			//DCMI配置
		DCMI_DMA_Init((u32)&jpeg_buf,jpeg_buf_size,DMA_MemoryDataSize_Word,DMA_MemoryInc_Enable);//DCMI DMA配置   
		OV2640_OutSize_Set(jpeg_img_size_tbl[size][0],jpeg_img_size_tbl[size][1]);//设置输出尺寸 
		DCMI_Start(); 		//启动传输
	
		while(1)
		{
	
				if(jpeg_data_ok == 1)	//已经采集完一帧图像了
				{
				  	p = (u8*)jpeg_buf;
						jpglen = 0;	//设置jpg文件大小为0
						headok = 0;	//清除jpg头标记
						for(i = 0;i < jpeg_data_len*4;i++)	//dma传输1次等于4字节,所以乘以4.
						{
								if((p[i]==0xFF)&&(p[i+1]==0xD8))//找到FF D8
								{
										jpgstart = i;
										headok = 1;	//标记找到jpg头(FF D8)
								}
								if((p[i]==0xFF)&&(p[i+1]==0xD9)&&headok)//找到头以后,再找FF D9
								{
										jpglen = i - jpgstart + 2;
										//rt_kprintf("jpglen:%d\n",jpglen);
										break;
								}
						} 
						
						if(jpglen)	//正常的jpeg数据 
						{
								p += jpgstart;			  //偏移到0XFF,0XD8处 
								for(i = 0;i < jpglen;i++)	//发送整个jpg文件
								{
//										while(USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);	//循环发送,直到发送完毕  		
//										USART_SendData(USART3,p[i]); 		
								}  
						}	
						jpeg_data_ok=2;	//标记jpeg数据处理完了,可以让DMA去采集下一帧了.
				}
				rt_thread_mdelay(1);
	
		}    
} 



int camera_thread_init(void)
{
		static u8 ErrorCount = 0;//错误计数  用于防止摄像头初始化卡死
    rt_thread_t camera_tid;
		/*创建动态线程*/
    camera_tid = rt_thread_create("camera",	 //线程名称
                    camera_thread_entry,		 //线程入口函数【entry】
                    RT_NULL,							   //线程入口函数参数【parameter】
                    512,										 //线程栈大小，单位是字节【byte】
                    20,										 	 //线程优先级【priority】
                    10);										 //线程的时间片大小【tick】= 100ms

    if (camera_tid != RT_NULL){
				if(boma_value_get() == System_NORMAL_STATUS)//系统状态
				{
						while(OV2640_Init() && ErrorCount <5)//初始化OV2640
						{
								ErrorCount ++;
								log_e("OV2640_Init_Error\r\n");
						}
								log_i("OV2640_Init()");
								rt_event_send(&init_event, CAM_EVENT);
								rt_thread_startup(camera_tid);
				 }
				else {log_w("Not Open OV2640 Camera!");}
		}
		return 0;
}
INIT_APP_EXPORT(camera_thread_init);




/* OV2640 特效设置 */
static int ov2640_set_effect(int argc,char **argv)
{
		int result = 0;
    if (argc != 2){
			log_e("Proper Usage: ov2640_set_effect bw / normal");//用法: 设置【黑白】【普通】
				result = -RT_ERROR;
        goto _exit;
    }

		if( !strcmp(argv[1],"normal") ){ //设置为普通
				OV2640_Special_Effects(OV_Normal);//设置特效
				log_i("OV_2640 Effcet: normal");
		}
		else if( !strcmp(argv[1],"bw") ){ //设置黑白
				OV2640_Special_Effects(OV_BW);//设置特效
				log_i("OV_2640 Effcet: B&W");
		}
		else {
				log_e("Proper Usage: ov2640_set_effect bw / normal");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(ov2640_set_effect,ov2640 set effect [bw / normal]);


/* OV2640 分辨率设置 */
static int ov2640_set_size(int argc,char **argv)
{
		int result = 0;
    if (argc != 2){
				log_e("Proper Usage: ov2640_set_size 0/1/2/3/4/5");//用法: 设置分辨率
				rt_kprintf("0.QCIF:176*144\n1.QQVGA:160*120\n2.CIF:352*288\n3.QVGA:320*240\n4.VGA:640*480\n5.SVGA:800*600\n");//用法: 设置分辨率
				result = -RT_ERROR;
        goto _exit;
    }

		if( atoi(argv[1]) < 6 ){ //设置为普通
				OV2640_OutSize_Set(jpeg_img_size_tbl[atoi(argv[1])][0],jpeg_img_size_tbl[atoi(argv[1])][1]);//设置输出尺寸 
				log_i("Success! OV_2640 Size: %s",JPEG_SIZE_TBL[atoi(argv[1])]);
		}
		else {
				log_e("Proper Usage: ov2640_set_size 0/1/2/3/4/5");
				rt_kprintf("0.QCIF:176*144\n1.QQVGA:160*120\n2.CIF:352*288\n3.QVGA:320*240\n4.VGA:640*480\n5.SVGA:800*600\n");//用法: 设置分辨率
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(ov2640_set_size,ov2640 set size [QQVGA...]);


