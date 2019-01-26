#include "usmart.h"
#include "usmart_str.h"
////////////////////////////用户配置区///////////////////////////////////////////////
//这下面要包含所用到的函数所申明的头文件(用户自己添加) 
#include "delay.h"	
#include "ov2640.h"   
#include "dcmi.h"    
#include "sccb.h"   
//函数名列表初始化(用户自己添加)
//用户直接在这里输入要执行的函数名及其查找串
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//如果使能了读写操作
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif		   
	(void*)delay_ms,"void delay_ms(u16 nms)",
 	(void*)delay_us,"void delay_us(u32 nus)",
		
  	(void*)SCCB_WR_Reg,"u8 SCCB_WR_Reg(u8 reg,u8 data)",
  	(void*)SCCB_RD_Reg,"u8 SCCB_RD_Reg(u8 reg)",
	(void*)DCMI_CR_Set,"void DCMI_CR_Set(u8 pclk,u8 hsync,u8 vsync)",
  	(void*)DCMI_Set_Window,"void DCMI_Set_Window(u16 sx,u16 sy,u16 width,u16 height)", 

	(void*)OV2640_Auto_Exposure,"void OV2640_Auto_Exposure(u8 level)",
  	(void*)OV2640_Light_Mode,"OV2640_Light_Mode(u8 mode)",
 	(void*)OV2640_Color_Saturation,"void OV2640_Color_Saturation(u8 sat)",
	(void*)OV2640_Brightness,"void OV2640_Brightness(u8 bright)",
 	(void*)OV2640_Contrast,"void OV2640_Contrast(u8 contrast)",
 	(void*)OV2640_Special_Effects,"void OV2640_Special_Effects(u8 eft)",
 	(void*)OV2640_Color_Bar,"void OV2640_Color_Bar(u8 sw)",
 	(void*)OV2640_Window_Set,"void OV2640_Window_Set(u16 sx,u16 sy,u16 width,u16 height)",
 	(void*)OV2640_OutSize_Set,"u8 OV2640_OutSize_Set(u16 width,u16 height)",
 	(void*)OV2640_ImageWin_Set,"u8 OV2640_ImageWin_Set(u16 offx,u16 offy,u16 width,u16 height)",
 	(void*)OV2640_ImageSize_Set,"u8 OV2640_ImageSize_Set(u16 width,u16 height)",  
		
 	(void*)OV2640_JPEG_Mode,"void OV2640_JPEG_Mode(void)",  
 	(void*)OV2640_RGB565_Mode,"void OV2640_RGB565_Mode(void)", 		  	    
};						  
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//函数控制管理器初始化
//得到各个受控函数的名字
//得到函数总数量
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//函数数量
	0,	  	//参数数量
	0,	 	//函数ID
	1,		//参数显示类型,0,10进制;1,16进制
	0,		//参数类型.bitx:,0,数字;1,字符串	    
	0,	  	//每个参数的长度暂存表,需要MAX_PARM个0初始化
	0,		//函数的参数,需要PARM_LEN个0初始化
};   



















