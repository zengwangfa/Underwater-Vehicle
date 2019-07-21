#ifndef __KEY_H
#define __KEY_H	 

#define KEY0  GPIO_ReadInputPin(GPIOC,GPIO_PIN_3)//读取按键0
#define KEY1  GPIO_ReadInputPin(GPIOB,GPIO_PIN_4)//读取按键1
#define KEY2 GPIO_ReadInputPin(GPIOB,GPIO_PIN_5)//读取按键3(WK_UP) 

 

#define KEY0_PRES   1	//KEY0按下
#define KEY1_PRES   2	//KEY1按下
#define KEY2_PRES   3	//KEY_UP按下(即WK_UP/KEY_UP)


void KEY_Init(void);//IO初始化
char KEY_Scan(char);  	//按键扫描函数					    
#endif
