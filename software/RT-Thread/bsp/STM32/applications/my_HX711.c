#include "my_init.h"
/*
说明：
			首先5s初始化采集 重量零点(即最小值)，后串口1 打印出“Please place 1kg!”  然后在蜂箱上
			放置1kg的重物，系统采集 0到1kg的线性K值后。
			按下按键KEY3，表示归一化采集成功。
*/



extern rt_uint32_t Count;
extern rt_uint32_t HX711_Date[2];

extern rt_uint8_t Init_flag;
extern rt_uint8_t first_ON_OFF;
extern rt_uint16_t Test_count;

extern rt_int32_t Init_Value[2];
extern rt_int32_t AD_Value[2];
extern rt_int32_t Last_AD_Value[2];
extern rt_int32_t side_weight[2];
extern rt_int32_t Weight[3];

extern rt_int32_t temp[2];
extern rt_int32_t var_max[2];    //8632784        //比这个值大 记为归一化最大值
extern rt_int32_t var_min[2];  //比这个值小 记为归一化最小值  -2147483648～+2141483647
extern rt_int32_t var0[10];
extern rt_int32_t var1[10];
extern rt_int32_t Real_Weight;

extern rt_uint8_t set1kg_flag ;
extern float Normalized[2];

void Hx711_Normalized(void);//归一化

rt_uint32_t HX711_ReadAD1(void)                               //读取HX711模块采集重量值	
{
  unsigned char i;
  HX711_SCK1_LOW;                                                //使能AD(PD_SCK 拉低)
  Count=0;
  while(HX711_DT1);                                           //AD转换未结束则等待,否则开始读取
  for(i=0;i<24;i++)
   {
      HX711_SCK1_HIGH;                                            //PD_SCK 置高(发送脉冲)
      Count=Count<<1;                                    //下降沿来临时变量Count左移一位,右则补零
      HX711_SCK1_LOW;                                            //PD_SCK 拉低
      if(HX711_DT1) Count++;
     }
  HX711_SCK1_HIGH;
  Count=Count^0x800000;                                  //第25个脉冲下降沿来时,转换数据完成
  HX711_SCK1_LOW;
  return(Count);
}

rt_uint32_t HX711_ReadAD2(void)                               //读取HX711模块采集重量值	
{
  unsigned char i;
  HX711_SCK2_LOW;                                                //使能AD(PD_SCK 拉低)
  Count=0;
  while(HX711_DT2);                                           //AD转换未结束则等待,否则开始读取
  for(i=0;i<24;i++)
   {
      HX711_SCK2_HIGH;                                            //PD_SCK 置高(发送脉冲)
      Count=Count<<1;                                    //下降沿来临时变量Count左移一位,右则补零
      HX711_SCK2_LOW;                                            //PD_SCK 拉低
      if(HX711_DT2) Count++;
     }
  HX711_SCK2_HIGH;
  Count=Count^0x800000;                                  //第25个脉冲下降沿来时,转换数据完成
  HX711_SCK2_LOW;
  return(Count);
}



void weight_get_val(void* parameter)
{
		static rt_uint8_t i=50;
		rt_pin_mode(HX711_DT1_PIN, PIN_MODE_INPUT_PULLUP);  
		rt_pin_mode(HX711_SCK1_PIN, PIN_MODE_OUTPUT);
		rt_pin_write(HX711_SCK1_PIN, PIN_HIGH);
	
		rt_pin_mode(HX711_DT2_PIN, PIN_MODE_INPUT_PULLUP);  
		rt_pin_mode(HX711_SCK2_PIN, PIN_MODE_OUTPUT);
		rt_pin_write(HX711_SCK2_PIN, PIN_HIGH);

		Hx711_Normalized(); //归一化采集重量数据
	
		while(1)
		{
			Last_AD_Value[0]=AD_Value[0];
			Last_AD_Value[1]=AD_Value[1];
			for(i=0;i<10;i++){
				 var0[i]=HX711_ReadAD1();
				 var1[i]=HX711_ReadAD2();
			}
			AD_Value[0]=Bubble(var0);
			AD_Value[1]=Bubble(var1);
			
			
			
			side_weight[0] = Normalized[0]*(AD_Value[0]-var_min[0])*10;  //放大10倍  //精度可扩大
			side_weight[1] = Normalized[1]*(AD_Value[1]-var_min[1])*10;  //放大10倍
			
			Weight[0] = (side_weight[0]+side_weight[1])/2;   //真实重量
			
			Weight[1] = (Weight[0])/10;   //取整数位  单位 1kg 
			Weight[2] = (Weight[0])%10;   //取小数位  单位 10g
			
			if(side_weight[0]>0){
				rt_kprintf("\r\nside_Weight[0] :%d   ",side_weight[0]);
			}
			else {rt_kprintf("\r\nside_Weight[0] :%d   ",side_weight[0]);}
			
			if(side_weight[1]>0){
				rt_kprintf("nside_Weight[1] :%d   ",side_weight[1]);
			}
			else {rt_kprintf("nside_Weight[1] :%d   ",side_weight[1]);}
			
			rt_kprintf("\r\nWeight[0] :%d.%d kg",Weight[1],Weight[2]);
			rt_thread_delay(100);     
			
		}
}



int hx711_thread_init()
{
		rt_thread_t hx711_tid;
    hx711_tid = rt_thread_create("hx711",
                    weight_get_val,
                    (void*)5,
                    1024,
                    25,
                    10);
    if (hx711_tid != RT_NULL){
			rt_thread_startup(hx711_tid);}
		return 0;
}
INIT_APP_EXPORT(hx711_thread_init);



void Hx711_Normalized()//归一化
{

	rt_uint8_t i = 0;
	static rt_uint8_t j = 5;
  rt_kprintf( "Hx711_Normalized...");

	while(j--)
	{
		for(i=0;i<10;i++){
			 var0[i]=HX711_ReadAD1();
			 var1[i]=HX711_ReadAD2();
		}
		temp[0] = Bubble(var0);
		temp[1] = Bubble(var1);
		if(var_min[0]>temp[0]){ var_min[0]=temp[0];}//采集最小值
		if(var_min[1]>temp[1]){ var_min[1]=temp[1];}
	}
	

	rt_kprintf( "Please place 1kg!");

	while(set1kg_flag == 0){  //如果重量突变表示放上1kg 重物
		
		for(i=0;i<10;i++){
		 var0[i]=HX711_ReadAD1();
		 var1[i]=HX711_ReadAD2();
		}
		temp[0] = Bubble(var0);
		temp[1] = Bubble(var1);


			Normalized[0] =(float) 1/(temp[0]-var_min[0]);
			Normalized[1] =(float) 1/(temp[1]-var_min[1]);
	}
	rt_thread_mdelay(100);
	rt_kprintf( "Successed...");

  rt_thread_mdelay(300);

}


