#include "bsp_timer.h"
#include "motor.h"
#include "bsp_timer.h"

//unsigned char F_Rotation[8]={0x08,0x0c,0x04,0x06,0x02,0x03,0x01,0x09} ;  //单双8拍反转表 五线四相步进电机表
//unsigned char R_Rotation[8]={0x09,0x01,0x03,0x02,0x06,0x04,0x0c,0x08} ;  //单双8拍正转表

unsigned char F_Rotation[4]={0x09,0x0A,0x06,0x05} ;  //4拍正转表  Forward
unsigned char R_Rotation[4]={0x05,0x06,0x0A,0x09} ;  //4拍反转表  Reverse


/******************************************************************************
* Name: 	 STEP_Init
*
* Desc: 	 初始化两个两相四线步进电机引脚
* Param:   无
* Return:  无
* Note:    本函数为 变焦、聚焦控制的步进电机引脚初始化
* Author:  Dwfish
* -------------------------------------
* Log: 	 2018/12/27, Create this function by Dwfish
 ******************************************************************************/
void STEP_Init()
{
     CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); //HSI = 16M (1分频)  //时钟配置
     
     GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)ZOOM_A1, GPIO_MODE_OUT_PP_LOW_FAST); //初始化两个两相四线步进电机引脚
     GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)ZOOM_A0, GPIO_MODE_OUT_PP_LOW_FAST);
     GPIO_Init(GPIOA, (GPIO_Pin_TypeDef)ZOOM_B1, GPIO_MODE_OUT_PP_LOW_FAST);
     GPIO_Init(GPIOC, (GPIO_Pin_TypeDef)ZOOM_B0, GPIO_MODE_OUT_PP_LOW_FAST);
     
     GPIO_Init(FOCUS_STEP_PORT, (GPIO_Pin_TypeDef)FOCUS_A1, GPIO_MODE_OUT_PP_LOW_FAST);
     GPIO_Init(FOCUS_STEP_PORT, (GPIO_Pin_TypeDef)FOCUS_A0, GPIO_MODE_OUT_PP_LOW_FAST);
     GPIO_Init(FOCUS_STEP_PORT, (GPIO_Pin_TypeDef)FOCUS_B1, GPIO_MODE_OUT_PP_LOW_FAST);
     GPIO_Init(FOCUS_STEP_PORT, (GPIO_Pin_TypeDef)FOCUS_B0, GPIO_MODE_OUT_PP_LOW_FAST);
     
     GPIO_WriteLow(GPIOD,ZOOM_A1);   //输出低
     GPIO_WriteLow(GPIOD,ZOOM_A0);
     GPIO_WriteLow(GPIOA,ZOOM_B1);
     GPIO_WriteLow(GPIOC,ZOOM_B0);
     
     GPIO_WriteLow(FOCUS_STEP_PORT,FOCUS_A1);
     GPIO_WriteLow(FOCUS_STEP_PORT,FOCUS_A0);
     GPIO_WriteLow(FOCUS_STEP_PORT,FOCUS_B1);
     GPIO_WriteLow(FOCUS_STEP_PORT,FOCUS_B0);
     
}

/******************************************************************************
* Name: 	 SetMotor
*
* Desc: 	 使步进电机转动
* Param:   电机编号:Motor_ID[0为变焦、1为聚焦]     传入:InputData选择相位使能
* Return:  无
* Note:    根据自己定义的节拍表输出相应脉冲
* Author:  Dwfish
* -------------------------------------
* Log: 	 2018/12/27, Create this function by Dwfish
 ******************************************************************************/
void SetMotor(unsigned char Motor_ID,unsigned char InputData)    //根据传送的数据Inputdata设置PB12-PB15引脚输出
{
    if(InputData & 0x08){
				switch(Motor_ID){
					case ZOOM_MOTOR : GPIO_WriteHigh(GPIOD, ZOOM_A1 );	break;
					case FOCUS_MOTOR : GPIO_WriteHigh(FOCUS_STEP_PORT, FOCUS_A1 );break;
				}					
    }
    else {
		  	switch(Motor_ID){
					case ZOOM_MOTOR : GPIO_WriteLow(GPIOD, ZOOM_A1 );	break;
					case FOCUS_MOTOR : GPIO_WriteLow(FOCUS_STEP_PORT, FOCUS_A1 );break;
				}	
    }
	 
    if(InputData & 0x04){
				switch(Motor_ID){
					case ZOOM_MOTOR : GPIO_WriteHigh(GPIOD, ZOOM_A0 );	break;
					case FOCUS_MOTOR : GPIO_WriteHigh(FOCUS_STEP_PORT, FOCUS_A0 );break;
				}	  
    }
   else{
		  	switch(Motor_ID){
					case ZOOM_MOTOR : GPIO_WriteLow(GPIOD, ZOOM_A0 );	break;
					case FOCUS_MOTOR : GPIO_WriteLow(FOCUS_STEP_PORT, FOCUS_A0 );break;
				}	
    }
		
    if(InputData & 0x02){
				switch(Motor_ID){
					case ZOOM_MOTOR : GPIO_WriteHigh(GPIOA, ZOOM_B1 );	break;
					case FOCUS_MOTOR : GPIO_WriteHigh(FOCUS_STEP_PORT, FOCUS_B1 );break;
				}	   
    }
    else{
		  	switch(Motor_ID){
					case ZOOM_MOTOR : GPIO_WriteLow(GPIOA, ZOOM_B1 );	break;
					case FOCUS_MOTOR : GPIO_WriteLow(FOCUS_STEP_PORT, FOCUS_B1 );break;
				}	
    }
    if(InputData & 0x01){
				switch(Motor_ID){
					case ZOOM_MOTOR : GPIO_WriteHigh(GPIOC, ZOOM_B0 );	break;
					case FOCUS_MOTOR : GPIO_WriteHigh(FOCUS_STEP_PORT, FOCUS_B0 );break;
				}	  
    }
    else{
				switch(Motor_ID){
					case ZOOM_MOTOR : GPIO_WriteLow(GPIOC, ZOOM_B0 );	break;
					case FOCUS_MOTOR : GPIO_WriteLow(FOCUS_STEP_PORT, FOCUS_B0 );break;
				}	
    }
}

/******************************************************************************
* Name: 	 MotorNCircle
*
* Desc: 	 电机转动 N 圈
* Param:   电机编号:Motor_ID     转动n圈     正反：position【0正1反】
* Return:  无
* Note:    无
* Author:  Dwfish
* -------------------------------------
* Log: 	 2018/12/27, Create this function by Dwfish
 ******************************************************************************/
void MotorNCircle(unsigned char Motor_ID,int n,bool position)
{
   int i=0;
   int j=0;
   int k=0;
   for(j=0;j<n;j++)
   {
      for(i=0;i<4*128;i++)   //极限是4*128
		 {
          for(k=0;k<4;k++)
				 {
              if(TRUE == position)
                 SetMotor(Motor_ID,R_Rotation[k]);  //1反转
             else
                 SetMotor(Motor_ID,F_Rotation[k]);    
                TIMDelay_N10us(100);
          }
      }
    }
}

/******************************************************************************
* Name: 	 MotorNAngle
*
* Desc: 	 电机转动 N °
* Param:   电机编号:Motor_ID     转动n°     正反：position【0正1反】
* Return:  无
* Note:    第二个for循环中i所对应的i<1中"1"参数，需要根据自己实际步进电机调整
* Author:  Dwfish
* -------------------------------------
* Log: 	 2018/12/27, Create this function by Dwfish
 ******************************************************************************/

void MotorNAngle(unsigned char Motor_ID,int n,bool position) //n为步进电机转动的角度为n*120，position为正转或者反转
{
   int i=0;
   int j=0;
   int k=0;
   for(j=0;j<n;j++)
   {
      for(i=0;i<1;i++)     
      {
          for(k=0;k<4;k++)   
          {
              if(TRUE == position)
                 SetMotor(Motor_ID,R_Rotation[k]);  
              else
                 SetMotor(Motor_ID,F_Rotation[k]);  
             	TIMDelay_N10us(100);      
          }
      }
    }
}
