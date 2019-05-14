#include "stm8s_gpio.h"
#include "bsp_timer.h"
#include "motor.h"
#include "key.h"
#include "stm8s_it.h"
#include "bsp_flash.h"
#include "bsp_uart.h"

extern uint8_t motor_data[6];//接收数据包
extern bool recvl_data_ok;//数据接收成功

uint8_t WriteBuf[6]={'1','1','1','1','1','1'};
//uint8_t Camera_Limit_Zoom_Data[6]={0xAA,0x55,0x02,0xFF,0x00,0x00};
//uint8_t Camera_Limit_Focus_Data[6]={0xAA,0x55,0x02,0x00,0xFF,0x00};
bool rotate_Zoom=FALSE; 
bool rotate_Focus=FALSE; 
bool scope_ok=FALSE;

void data_handle();

void main(void)
{
  TIMER_Initializes();
  STEP_Init();
  UART_Initializes();
 
  while(1)
  {
   data_handle();  
  }
}

void data_handle(void)
{

 if(recvl_data_ok)
   {

    // UART1_SendNByte(WriteBuf,6);
    if(motor_data[3]==0x01||motor_data[3]==0x02)     //选择不同步进机
    {
      switch(motor_data[3])
      {
         case 0x01:rotate_Zoom=FALSE;WriteBuf[0]-=1;break;  //控制是正转还是反转
         case 0x02:rotate_Zoom=TRUE;WriteBuf[0]+=1;break;
         default: break;
      }
      
      if(WriteBuf[0]=='0')
      {
        if(WriteBuf[1]=='1'&&WriteBuf[2]=='1')
        {
          WriteBuf[0]='1';
        }
        else
       {
         WriteBuf[0]='8';
         WriteBuf[1]-=1;
         if(WriteBuf[1]=='0')
         {
           WriteBuf[1]='8';
           WriteBuf[2]-=1;          
         }
         scope_ok=TRUE;       
       }
      }
      else if(WriteBuf[0]=='9')
      {

        if(WriteBuf[1]=='8'&&WriteBuf[2]=='8')
        {
          WriteBuf[0]='8';      
        }
        else
        {
          WriteBuf[0]='1';
          WriteBuf[1]+=1;
          if(WriteBuf[1]=='9')
         {
           WriteBuf[1]='1';
           WriteBuf[2]+=1;          
         }
         scope_ok=TRUE;          
        }
      }
      else scope_ok=TRUE;
      
      if(scope_ok) 
      {
        MotorNAngle(ZOOM_MOTOR,1,rotate_Zoom);
        UART1_SendNByte(WriteBuf,6);
        motor_data[3]=0x00;
      }
    }
    else if(motor_data[3]==0x88)
    {
     MotorNCircle(ZOOM_MOTOR,1,TRUE);
     WriteBuf[0]='8';
     WriteBuf[1]='8';
     WriteBuf[2]='8';
     UART1_SendNByte(WriteBuf,6);
     motor_data[3]=0x00;
    }

    
    if(motor_data[4]==0x01||motor_data[4]==0x02)
    {
      switch(motor_data[4])
      {
         case 0x01:rotate_Focus=FALSE;WriteBuf[3]-=1;break;  //控制是正转还是反转
         case 0x02:rotate_Focus=TRUE;WriteBuf[3]+=1;break;
         default: break;
      }
      
      if(WriteBuf[3]=='0')
      {
        if(WriteBuf[4]=='1'&&WriteBuf[5]=='1')
        {
          WriteBuf[3]='1';
        }
        else
       {
         WriteBuf[3]='8';
         WriteBuf[4]-=1;
         if(WriteBuf[4]=='0')
         {
           WriteBuf[4]='8';
           WriteBuf[5]-=1;          
         }
         scope_ok=TRUE;       
       }
      }
      else if(WriteBuf[3]=='9')
      {
        if(WriteBuf[4]=='8'&&WriteBuf[5]=='8')
        {
          WriteBuf[3]='8';    
        }
        else
        {
          WriteBuf[3]='1';
          WriteBuf[4]+=1;
          if(WriteBuf[4]=='9')
         {
           WriteBuf[4]='1';
           WriteBuf[5]+=1;          
         }
         scope_ok=TRUE;          
        }
      }
      else scope_ok=TRUE;
      
      if(scope_ok) 
       {
          MotorNAngle(FOCUS_MOTOR,1,rotate_Focus);
          UART1_SendNByte(WriteBuf,6);
          motor_data[4]=0x00;
       }
    }
    else if(motor_data[4]==0x88)
    {
     MotorNCircle(FOCUS_MOTOR,1,TRUE);
     WriteBuf[3]='8';
     WriteBuf[4]='8';
     WriteBuf[5]='8';
     UART1_SendNByte(WriteBuf,6);
     motor_data[4]=0x00;
    }
   }
}


#ifdef USE_FULL_ASSERT                           //断言(参考官方)

void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif


/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
