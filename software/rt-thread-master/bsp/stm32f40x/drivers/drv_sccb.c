#include "sys.h"
#include "drv_sccb.h"
#include "init.h"


//初始化SCCB接口 
void SCCB_Init(void)
{				
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOD时钟
  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;//PD6,7 推挽输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //PD6,7 推挽输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
 
	GPIO_SetBits(GPIOD,GPIO_Pin_6|GPIO_Pin_7);
	SCCB_SDA_OUT();	   
}			 

//SCCB起始信号
//当时钟为高的时候,数据线的高到低,为SCCB起始信号
//在激活状态下,SDA和SCL均为低电平
void SCCB_Start(void)
{
    SCCB_SDA=1;     //数据线高电平	   
    SCCB_SCL=1;	    //在时钟线高的时候数据线由高至低
    rt_hw_us_delay(50);  
    SCCB_SDA=0;
    rt_hw_us_delay(50);	 
    SCCB_SCL=0;	    //数据线恢复低电平，单操作函数必要	  
}

//SCCB停止信号
//当时钟为高的时候,数据线的低到高,为SCCB停止信号
//空闲状况下,SDA,SCL均为高电平
void SCCB_Stop(void)
{
    SCCB_SDA=0;
    rt_hw_us_delay(50);	 
    SCCB_SCL=1;	
    rt_hw_us_delay(50); 
    SCCB_SDA=1;	
    rt_hw_us_delay(50);
}  
//产生NA信号
void SCCB_No_Ack(void)
{
	rt_hw_us_delay(50);
	SCCB_SDA=1;	
	SCCB_SCL=1;	
	rt_hw_us_delay(50);
	SCCB_SCL=0;	
	rt_hw_us_delay(50);
	SCCB_SDA=0;	
	rt_hw_us_delay(50);
}
//SCCB,写入一个字节
//返回值:0,成功;1,失败. 
u8 SCCB_WR_Byte(u8 dat)
{
	u8 j,res;	 
	for(j=0;j<8;j++) //循环8次发送数据
	{
		if(dat&0x80)SCCB_SDA=1;	
		else SCCB_SDA=0;
		dat<<=1;
		rt_hw_us_delay(50);
		SCCB_SCL=1;	
		rt_hw_us_delay(50);
		SCCB_SCL=0;		   
	}			 
	SCCB_SDA_IN();		//设置SDA为输入 
	rt_hw_us_delay(50);
	SCCB_SCL=1;			//接收第九位,以判断是否发送成功
	rt_hw_us_delay(50);
	if(SCCB_READ_SDA)res=1;  //SDA=1发送失败，返回1
	else res=0;         //SDA=0发送成功，返回0
	SCCB_SCL=0;		 
	SCCB_SDA_OUT();		//设置SDA为输出    
	return res;  
}	 
//SCCB 读取一个字节
//在SCL的上升沿,数据锁存
//返回值:读到的数据
u8 SCCB_RD_Byte(void)
{
	u8 temp=0,j;    
	SCCB_SDA_IN();		//设置SDA为输入  
	for(j=8;j>0;j--) 	//循环8次接收数据
	{		     	  
		rt_hw_us_delay(50);
		SCCB_SCL=1;
		temp=temp<<1;
		if(SCCB_READ_SDA)temp++;   
		rt_hw_us_delay(50);
		SCCB_SCL=0;
	}	
	SCCB_SDA_OUT();		//设置SDA为输出    
	return temp;
} 							    
//写寄存器
//返回值:0,成功;1,失败.
u8 SCCB_WR_Reg(u8 reg,u8 data)
{
	u8 res=0;
	SCCB_Start(); 					//启动SCCB传输
	if(SCCB_WR_Byte(SCCB_ID))res=1;	//写器件ID	  
	rt_hw_us_delay(100);
  	if(SCCB_WR_Byte(reg))res=1;		//写寄存器地址	  
	rt_hw_us_delay(100);
  	if(SCCB_WR_Byte(data))res=1; 	//写数据	 
  	SCCB_Stop();	  
  	return	res;
}		  					    
//读寄存器
//返回值:读到的寄存器值
u8 SCCB_RD_Reg(u8 reg)
{
	u8 val=0;
	SCCB_Start(); 				//启动SCCB传输
	SCCB_WR_Byte(SCCB_ID);		//写器件ID	  
	rt_hw_us_delay(100);	 
  	SCCB_WR_Byte(reg);			//写寄存器地址	  
	rt_hw_us_delay(100);	  
	SCCB_Stop();   
	rt_hw_us_delay(100);	   
	//设置寄存器地址后，才是读
	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID|0X01);	//发送读命令	  
	rt_hw_us_delay(100);
  	val=SCCB_RD_Byte();		 	//读取数据
  	SCCB_No_Ack();
  	SCCB_Stop();
  	return val;
}















