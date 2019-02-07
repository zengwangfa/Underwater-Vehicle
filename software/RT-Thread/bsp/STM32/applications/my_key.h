#ifndef MY_KEY_H
#define MY_KEY_H

#define KEY1 28  	//PB2
#define KEY2 29		//PB10
#define KEY3 45		//PA12

#define BOMA1_PIN 37
#define BOMA2_PIN 38
#define BOMA3_PIN 39
#define BOMA4_PIN 40

#define BOMA1 rt_pin_read(BOMA1_PIN)
#define BOMA2 rt_pin_read(BOMA2_PIN)
#define BOMA3 rt_pin_read(BOMA3_PIN)
#define BOMA4 rt_pin_read(BOMA4_PIN)


int key_thread_init(void);
void key_thread_entry(void* parameter);







#endif 


