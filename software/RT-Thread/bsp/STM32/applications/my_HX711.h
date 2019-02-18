#ifndef MY_HX711_H
#define MY_HX711_H


#define HX711_DT1_PIN			10
#define HX711_SCK1_PIN		11

#define HX711_DT2_PIN			14
#define HX711_SCK2_PIN		15


   	
#define HX711_DT1					rt_pin_read(HX711_DT1_PIN)// PC0为HX711模块的DT引脚  
#define HX711_SCK1_HIGH 	rt_pin_write(HX711_SCK1_PIN, PIN_HIGH)
#define HX711_SCK1_LOW 		rt_pin_write(HX711_SCK1_PIN, PIN_LOW)

#define HX711_DT2					rt_pin_read(HX711_DT2_PIN)// PC0为HX711模块的DT引脚  
#define HX711_SCK2_HIGH 	rt_pin_write(HX711_SCK2_PIN, PIN_HIGH)
#define HX711_SCK2_LOW 		rt_pin_write(HX711_SCK2_PIN, PIN_LOW)

void my_hx711_thread_init(void);

rt_uint32_t HX711_ReadAD1(void);                              //读取HX711模块采集重量值	
rt_uint32_t HX711_ReadAD2(void);                               //读取HX711模块采集重量值	



#endif 


