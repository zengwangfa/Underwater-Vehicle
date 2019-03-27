#include "drv_MS5837.h"
#include "math.h"
#include "init.h"

//气压计状态机
#define SCTemperature    0x01	  //开始温度转换
#define CTemperatureing  0x02   //正在转换温度
#define SCPressure  		 0x03	  //开始气压转换
#define SCPressureing    0x04	  //正在转换气压


 
/*
C1 压力灵敏度 SENS|T1
C2  压力补偿  OFF|T1
C3	温度压力灵敏度系数 TCS
C4	温度系数的压力补偿 TCO
C5	参考温度 T|REF
C6 	温度系数的温度 TEMPSENS
*/
uint32_t  Cal_C[7];	        //用于存放PROM中的6组数据1-6

double OFF_;
float Aux;
/*
dT 实际和参考温度之间的差异
TEMP_DEPTH 实际温度	
*/
uint64_t dT,TEMP_DEPTH;
/*
OFF 实际温度补偿
SENS 实际温度灵敏度
*/
uint64_t OFf,SENS;
uint32_t D1_Pres,D2_Temp;	// 数字压力值,数字温度值

uint32_t Pressure,Pressure_old,qqp,Wdodo;				//大气压
uint32_t TEMP2,T2,OFF2,SENS2;	//温度校验值
uint32_t Pres_BUFFER[20];     //数据组
uint32_t Temp_BUFFER[10];     //数据组
uint32_t depth;




/*******************************************************************************
  * @函数名称	MS583730BA_RESET
  * @函数说明   复位MS5611
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void MS583703BA_RESET(void)
{
		IIC_Start();
		IIC_Send_Byte(0xEC);//CSB接地，主机地址：0XEE，否则 0X77
	  IIC_Wait_Ack();
    IIC_Send_Byte(0x1E);//发送复位命令
	  IIC_Wait_Ack();
    IIC_Stop();
	
}
/*******************************************************************************
  * @函数名称	MS5611_init
  * @函数说明   初始化5611
  * @输入参数  	无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
u8 MS5837_init(void)
 {	 
  u8  inth,intl;
  int i;
  for (i=1;i<=6;i++) 
	{
 
		IIC_Start();
    IIC_Send_Byte(0xEC);
		IIC_Wait_Ack();
		IIC_Send_Byte(0xA0 + (i*2));
		IIC_Wait_Ack();
    IIC_Stop();
		rt_hw_us_delay(5);
		IIC_Start();
		IIC_Send_Byte(0xEC+0x01);  //进入接收模式
		rt_hw_us_delay(1);
		IIC_Wait_Ack();
		inth = IIC_Read_Byte(1);  		//带ACK的读数据
		rt_hw_us_delay(1);
		intl = IIC_Read_Byte(0); 			//最后一个字节NACK
		IIC_Stop();
    Cal_C[i] = (((uint16_t)inth << 8) | intl);
	}
	 return !Cal_C[0];
 }


/**************************实现函数********************************************
*函数原型:unsigned long MS561101BA_getConversion(void)
*功　　能:    读取 MS5837 的转换结果 
*******************************************************************************/
unsigned long MS583703BA_getConversion(uint8_t command)
{
 
			unsigned long conversion = 0;
			u8 temp[3];
	
	    IIC_Start();
			IIC_Send_Byte(0xEC); 		//写地址
			IIC_Wait_Ack();
			IIC_Send_Byte(command); //写转换命令
			IIC_Wait_Ack();
			IIC_Stop();

			rt_hw_us_delay(10);
			IIC_Start();
			IIC_Send_Byte(0xEC); 		//写地址
			IIC_Wait_Ack();
			IIC_Send_Byte(0);				// start read sequence
			IIC_Wait_Ack();
			IIC_Stop();
		 
			IIC_Start();
			IIC_Send_Byte(0xEC+0x01);  //进入接收模式
			IIC_Wait_Ack();
			temp[0] = IIC_Read_Byte(1);  //带ACK的读数据  bit 23-16
			temp[1] = IIC_Read_Byte(1);  //带ACK的读数据  bit 8-15
			temp[2] = IIC_Read_Byte(0);  //带NACK的读数据 bit 0-7
			IIC_Stop();
			
			conversion = (unsigned long)temp[0] * 65536 + (unsigned long)temp[1] * 256 + (unsigned long)temp[2];
			return conversion;
 
}


/**************************实现函数********************************************
*函数原型:void MS561101BA_GetTemperature(void)
*功　　能:    读取 温度转换结果 
*******************************************************************************/

void MS583703BA_getTemperature(void)
{
	
	D2_Temp = MS583703BA_getConversion(0x58);
	rt_thread_mdelay(10);
	dT=D2_Temp - (((uint32_t)Cal_C[5])*256);
	TEMP_DEPTH=2000+dT*((uint32_t)Cal_C[6])/8388608;
}

///***********************************************
//  * @brief  读取气压
//  * @param  None
//  * @retval None
//************************************************/
void MS583703BA_getPressure(void)
{
	D1_Pres= MS583703BA_getConversion(0x48);
	rt_thread_mdelay(10);
	
	OFF_=(uint32_t)Cal_C[2]*65536+((uint32_t)Cal_C[4]*dT)/128;
	SENS=(uint32_t)Cal_C[1]*32768+((uint32_t)Cal_C[3]*dT)/256;

	if(TEMP_DEPTH<2000)  // low temp
	{
		Aux = (2000-TEMP_DEPTH)*(2000-TEMP_DEPTH);
		T2 = 3*(dT*dT) /0x80000000; 
		OFF2 = (float)1.5*Aux;
		SENS2 = 5*Aux/8;
		
	
		OFF_ = OFF_ - OFF2;
		SENS = SENS - SENS2;	
	}
	 else{
	  T2=2*(dT*dT)/137438953472;
		OFF2 = 1*Aux/16;
		SENS2 = 0;
		OFF_ = OFF_ - OFF2;
		SENS = SENS - SENS2;	
		 
	 }
  Pressure= ((D1_Pres*SENS/2097152-OFF_)/8192)/10;
	TEMP_DEPTH=(TEMP_DEPTH-T2)/100;
	Wdodo=4095*(Pressure-900)/2600;
	DAC_SetChannel1Data(DAC_Align_12b_R,Wdodo); 
}


