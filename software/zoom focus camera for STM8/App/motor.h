#ifndef __MOTOR_H
#define __MOTOR_H  

#define ZOOM_MOTOR  0   //定义变焦步进电机ID
#define FOCUS_MOTOR 1

/*定义 变焦摄像头 引脚信息*/ 
#define ZOOM_A1 GPIO_PIN_2     //PA8对应步进电机的A相   ZOOM  A+ PD2
#define ZOOM_A0 GPIO_PIN_3     //PA9对应步进电机的B相   ZOOM  A- PD3
#define ZOOM_B1 GPIO_PIN_3    //PA10对应步进电机的C相  ZOOM  B+  PA3
#define ZOOM_B0 GPIO_PIN_7    //PA11对应步进电机的D相  ZOOM  B-  PC7

  

/*定义 聚焦摄像头 引脚信息*/
#define FOCUS_STEP_PORT GPIOC   //步进电机的控制端口为A端口  
#define FOCUS_A1 GPIO_PIN_3	// FOCUS  A+   PC3
#define FOCUS_A0 GPIO_PIN_4	// FOCUS  A-   PC4
#define FOCUS_B1 GPIO_PIN_5	// FOCUS  B+   PC5
#define FOCUS_B0 GPIO_PIN_6	// FOCUS  B-   PC6

void STEP_Stop(void);
void STEP_Init(void);                      //步进电机引脚初始化

void SetMotor(unsigned char Motor_ID,unsigned char InputData);    //根据传送的数据Inputdata设置脉冲引脚输出

void MotorNCircle(unsigned char Motor_ID,int n,bool position);    //n为步进电机转动的圈数，position为正转或者反转

void MotorNAngle(unsigned char Motor_ID,int n,bool position);    //n为步进电机转动的角度为n*120，position为正转或者反转
#endif
