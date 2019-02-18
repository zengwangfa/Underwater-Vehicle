/*
   ------------  Smart Hive System  ---------------
 * Date           Author             Notes                                              		 Defects 
 * 2018-11-30      Drfish       create -> ADD LED_thread
 * 2018-12-01      Drfish       create -> ADD KEY Control_IO
 * 2018-12-02      Drfish       create -> ADD DS18B20                                  The temperature seems to be a little high
 * 2018-12-03      Drfish       create -> ADD HX711 Weight_collect                     The weight hasn't been fitted yet
 * 2018-12-04      Drfish       create -> WIFI Connect & Voltage_AD_collect      
 * 2018-12-05      Drfish       create -> Upper Computer Communication Normal      		 Temperature data is unstable on the Upper Computer
 * 2018-12-05      Drfish       create -> ADD BEEP & AD_Voltage Filtering
 * 2018-12-06      Drfish       create -> ADD OLED Display  &  filter_temp 
 * 2018-12-07      Drfish       create -> Improve variable struct
 * 2018-12-10      Drfish       create -> ADD  SFUD  FLASH存储 【蜂门状态 & 蜂箱号数】

 Unfinished tasks:1.  ADD FLASH 存储  蜂王状态，脾数 蜂门状态    蜂箱号数【√】
									2.  ADD Servo Motor Control     
									
*/
 

#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"
#include "my_init.h"
#include "sfud.h"
#include "string.h"
#include "spi_flash.h"
#include "spi_flash_sfud.h"

extern rt_int32_t Weight[3];
extern rt_uint16_t Temperature[3]; //温度
extern rt_uint16_t voltage[3];     //电压
extern rt_uint8_t DataBuffer[8];   //数据包

extern rt_uint8_t FW_status_flag;
extern rt_uint8_t PiShu;
extern bool   HEAT_START_FLAG;          //开始控制加热丝加热标志位
extern bool   WASHING_START_FLAG;       //开始清洗蜂箱标志位
extern bool   FEED_START_FLAG;          //开始喂食蜂箱标志位

rt_spi_flash_device_t W25Q128_dev = NULL;
const sfud_flash *flash = NULL;



int main(void)
{
	rt_uint8_t temp;  
	
	W25Q128_dev = rt_sfud_flash_probe("flash0", "norspi");
	flash = (sfud_flash_t)W25Q128_dev->user_data;
	sfud_read(flash,0,sizeof(FW_status_flag), &FW_status_flag);
	sfud_read(flash,1,sizeof(PiShu), &PiShu);      //初始化后从 W25Q128 FLASH读出【蜂王状态】【脾数】
								

	while(1)
	{
		DataBuffer[0]=(Weight[0]*100);                  //2字节的蜂箱重量数据放入数组
		DataBuffer[1]=(Weight[0]*100)>>8;
		DataBuffer[2]=0xFF;                               //2字节的饲料重量数据放入数组(ffff代表饲料充足)
		DataBuffer[3]=0xFF;
		DataBuffer[5]=Temperature[0]+500;                     //2字节的温度数据放入数组中
		DataBuffer[4]=(Temperature[0]+500)>>8;
	
		DataBuffer[6]=HEAT_START_FLAG;                     //将加热标志位存入数组，具体看通信数据格式
		DataBuffer[6]=DataBuffer[6]<<1;                    //00000010代表正在加热
		DataBuffer[6]=DataBuffer[6]+FEED_START_FLAG;        //将喂食标志位存入数组，具体看通信数据格式
		temp=WASHING_START_FLAG;
		DataBuffer[6]=DataBuffer[6]+(temp<<2);
	
		rt_thread_mdelay(10); 
	}

}



void WIFI_SEND_BUFF(void* parameter)  //WIFI与上位机通信
{

	while(1)
	{	
		Uart2DataAnalyzing(); //WIFI数据包解析
		rt_thread_delay(5);  //10ms
	}
	
}

int wifi_send_init()
{
	rt_thread_t tid;
        tid = rt_thread_create("WIFI_SEND_BUFF",WIFI_SEND_BUFF, RT_NULL,1024,2, 20);
        if (tid != RT_NULL)
                rt_thread_startup(tid);
        
        return 0;
}
INIT_APP_EXPORT(wifi_send_init);


