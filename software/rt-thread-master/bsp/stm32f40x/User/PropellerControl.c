#include "init.h"
#include <math.h>
#include "PropellerControl.h"

PropellerParamter_Type PropellerParamter = {//初始化推进器参数值
		 1500,//中值
		 1800,//最大值
		 1200,//最小值
	   100	//死区值
}; 

ActionTypeEnum       Posture_Flag; //机器人姿态标志位
PropellerPower_Type  PropellerPower; //推进器推理控制器
PropellerError_Type  PropellerError; //推进器偏差值

u8 turnAngle = 45;    //转向角度

unsigned int myabs(int absValue)
{
		return absValue > 0 ? absValue : -absValue;
}



/*******************************************
* 函 数 名：askResultant
* 功    能：求出合力大小及方向
* 输入参数：输入需要的角度和大小 forceSize 为需要的合力的大小  angle为转向的角度 
* 返 回 值：force 根据力和行进角度算出的单个推进器的推力大小
* 注    意：none
********************************************/
double askResultant(double angle,double forceSize)
{
		double force=0;
		force = forceSize*cos(Deg2Rad(angle));
		return force;
}


/*******************************************
* 函 数 名：Output_Limit
* 功    能：输出限制
* 输入参数：PowerValue
* 返 回 值：限幅后的 PowerValue
* 注    意：最大值为Propeller.PowerMax 初始化为1800
						最小值为Propeller.PowerMin 初始化为1300
********************************************/
u16 Output_Limit(u16 PowerValue)
{
		PowerValue = PowerValue > PropellerParamter.PowerMax ? PropellerParamter.PowerMax : PowerValue;//正向限幅
		PowerValue = PowerValue < PropellerParamter.PowerMin ? PropellerParamter.PowerMin : PowerValue;//反向限幅
	
		return PowerValue;
}




/*******************************************
* 函 数 名：propeller_control
* 功    能：水平轴推进器的控制
* 输入参数：none
* 返 回 值：none
* 注    意：none
********************************************/
void Propeller_Control(void)
{
		int MoveValue = 0;
		
		switch(Posture_Flag)
		{
				case  Forward  :  ;break;  //前进
				case  BackAway :  ;break;	 //后退
				case  TurnLeft :  ;break;  //左转
				case  TurnRight:  ;break;  //右转
				case  MoveLeft :	;break;  //左移
				case  MoveRight:  ;break;  //右移
				case  Stop     :	;break;  //停止
				
				default:break;
		}
		Propeller_Output_Limit(MoveValue);  //推进器输出限幅
}

/*******************************************
* 函 数 名：Propeller_Output_Limit
* 功    能：推进器输出限制
* 输入参数：运动值：MoveValue
* 返 回 值：none
* 注    意：最大值为Propeller.PowerMax 初始化为1800
						最小值为Propeller.PowerMin 初始化为1300
********************************************/
void Propeller_Output_Limit(int MoveValue)
{
		
		#if VehicleMode == AUV_Mode
			MoveValue = PropellerParamter.PowerMed + PropellerPower.leftUp;
			MoveValue = Output_Limit(MoveValue);
			TIM_SetCompare1(TIM1,MoveValue);			 //左上
		#else
			MoveValue = PropellerParamter.PowerMed  + PropellerPower.rightDown;
			MoveValue = Output_Limit(MoveValue);
			TIM_SetCompare1(TIM1,MoveValue);			//右下00000000
		#endif
//---------------------------------------------------------------------------//
		#if VehicleMode == AUV_Mode
			MoveValue = PropellerParamter.PowerMed  + PropellerPower.rightUp;
			MoveValue = Output_Limit(MoveValue);
			TIM_SetCompare2(TIM1,MoveValue);			//右上
		#else
			MoveValue = PropellerParamter.PowerMed  + PropellerPower.leftUp;
			MoveValue = Output_Limit(MoveValue);
			TIM_SetCompare2(TIM1,MoveValue);		 //左上0000000000
		#endif
//---------------------------------------------------------------------------//
		#if VehicleMode == AUV_Mode
			MoveValue = PropellerParamter.PowerMed  + PropellerPower.leftDown;
			MoveValue = Output_Limit(MoveValue);
			TIM_SetCompare3(TIM1,MoveValue);			//左下
		#else
			MoveValue = PropellerParamter.PowerMed  + PropellerPower.leftDown;
			MoveValue = Output_Limit(MoveValue);
			TIM_SetCompare2(TIM12,MoveValue);	   //	左下 0000000
		#endif
//---------------------------------------------------------------------------//
		#if VehicleMode == AUV_Mode
			MoveValue = PropellerParamter.PowerMed + PropellerPower.rightDown;
			MoveValue = Output_Limit(MoveValue);
			TIM_SetCompare4(TIM1,MoveValue);			//右下
		#else
			MoveValue = PropellerParamter.PowerMed + PropellerPower.rightUp;
			MoveValue = Output_Limit(MoveValue);
			TIM_SetCompare4(TIM1,MoveValue);			//右上00000000
		#endif
}



/*******************************************
* 函 数 名：robotUp
* 功    能：机器人前进
* 输入参数：none
* 返 回 值：none
* 注    意：none
********************************************/
void robotForward(void)  //前进
{
		#if VehicleMode == AUV_Mode
			PropellerPower.leftUp = -askResultant(45,PropellerPower.Power/2)-23-   PropellerError.leftUpError;
			PropellerPower.rightUp = askResultant(45,PropellerPower.Power/2)+3+    PropellerError.rightUpError;
			PropellerPower.leftDown = askResultant(45,PropellerPower.Power/2)+     PropellerError.leftDownError;
			PropellerPower.rightDown = -askResultant(45,PropellerPower.Power/2)-28-PropellerError.rightDownError;
		#else
			PropellerPower.Power = PropellerPower.Power * 5;
			PropellerPower.leftUp = -PropellerPower.Power-32;
			PropellerPower.rightUp = PropellerPower.Power+5;
			PropellerPower.leftDown = -PropellerPower.Power-32;
			PropellerPower.rightDown = PropellerPower.Power+5;
		#endif
}

void robotBackAway(void)  //后退
{
		#if VehicleMode == AUV_Mode	
			PropellerPower.leftUp = askResultant(45,PropellerPower.Power/2)+3+PropellerError.leftUpError;
			PropellerPower.rightUp = -askResultant(45,PropellerPower.Power/2)-26-PropellerError.rightUpError;
			PropellerPower.leftDown = -askResultant(45,PropellerPower.Power/2)-28-PropellerError.leftDownError;
			PropellerPower.rightDown = askResultant(45,PropellerPower.Power/2)+PropellerError.rightDownError;
		#else
			PropellerPower.Power = PropellerPower.Power * 5;
			PropellerPower.leftUp = PropellerPower.Power+2+PropellerError.leftUpError;
			PropellerPower.rightUp = -PropellerPower.Power-30-PropellerError.rightUpError;
			PropellerPower.leftDown = PropellerPower.Power+2+PropellerError.leftDownError;
			PropellerPower.rightDown = -PropellerPower.Power-30-PropellerError.rightDownError;
		#endif
}


void turnRight(void)  //右转
{
		#if VehicleMode == AUV_Mode
			PropellerPower.leftUp = -askResultant(45,PropellerPower.Power/4)-28;
			PropellerPower.rightUp = -askResultant(45,PropellerPower.Power/2)-20;
			PropellerPower.leftDown = askResultant(45,PropellerPower.Power/4)+3;
			PropellerPower.rightDown = askResultant(45,PropellerPower.Power/4)+1;
		#else
			PropellerPower.Power = PropellerPower.Power * 3;
			PropellerPower.leftUp = -PropellerPower.Power-30-PropellerError.leftUpError;
			PropellerPower.rightUp = -PropellerPower.Power-30-PropellerError.rightUpError;
			PropellerPower.leftDown = -PropellerPower.Power-30-PropellerError.leftDownError;
			PropellerPower.rightDown = -PropellerPower.Power-30-PropellerError.rightDownError;
		#endif
}

void turnLeft(void)  //左转
{
		#if VehicleMode == AUV_Mode
			PropellerPower.leftUp = askResultant(45,PropellerPower.Power/2)-5;
			PropellerPower.rightUp = askResultant(45,PropellerPower.Power/3);
			PropellerPower.leftDown = -askResultant(45,PropellerPower.Power/4)-30;
			PropellerPower.rightDown = -askResultant(45,PropellerPower.Power/4)-30;
		#else
			PropellerPower.Power = PropellerPower.Power * 3;
			PropellerPower.leftUp = PropellerPower.Power+0+PropellerError.leftUpError;
			PropellerPower.rightUp = PropellerPower.Power+0+PropellerError.rightUpError;
			PropellerPower.leftDown = PropellerPower.Power+0+PropellerError.leftDownError;
			PropellerPower.rightDown = PropellerPower.Power+0+PropellerError.rightDownError;
		#endif
}


void moveLeft(void)  //左移
{
		#if VehicleMode == AUV_Mode
			PropellerPower.leftUp = askResultant(45,PropellerPower.Power/2)+5+PropellerError.leftUpError;
			PropellerPower.rightUp = askResultant(45,PropellerPower.Power/2)+6+PropellerError.rightUpError;
			PropellerPower.leftDown = askResultant(45,PropellerPower.Power/2)+3+PropellerError.leftDownError;
			PropellerPower.rightDown = askResultant(45,PropellerPower.Power/2)+3+PropellerError.rightDownError;
		#else
			PropellerPower.Power = PropellerPower.Power * 4;
			PropellerPower.leftUp = PropellerPower.Power+5+PropellerError.leftUpError;
			PropellerPower.rightUp = PropellerPower.Power+5+PropellerError.rightUpError;
			PropellerPower.leftDown = -PropellerPower.Power-32-PropellerError.leftDownError;
			PropellerPower.rightDown = -PropellerPower.Power-32-PropellerError.rightDownError;
		#endif
}



void moveRight(void)  //右移
{
		#if VehicleMode == AUV_Mode
			PropellerPower.leftUp = -askResultant(45,PropellerPower.Power/2)-25-PropellerError.leftUpError;
			PropellerPower.rightUp = -askResultant(45,PropellerPower.Power/2)-25-PropellerError.rightUpError;
			PropellerPower.leftDown = -askResultant(45,PropellerPower.Power/2)-25-PropellerError.leftDownError;
			PropellerPower.rightDown = -askResultant(45,PropellerPower.Power/2)-27-PropellerError.rightDownError;

		#else
			PropellerPower.Power = PropellerPower.Power * 4;
			PropellerPower.leftUp = -PropellerPower.Power-32-PropellerError.leftUpError;
			PropellerPower.rightUp = -PropellerPower.Power-32-PropellerError.rightUpError;
			PropellerPower.leftDown = PropellerPower.Power+5+PropellerError.leftDownError;
			PropellerPower.rightDown = PropellerPower.Power+5+PropellerError.rightDownError;
		#endif
}



/*******************************************
* 函 数 名：Propeller_Init
* 功    能：推进器的初始化
* 输入参数：none
* 返 回 值：none
* 注    意：初始化流程：
*           1,接线,上电，哔-哔-哔三声,表示开机正常
*           2,给电调2ms或1ms最高转速信号,哔一声
*           3,给电调1.5ms停转信号,哔一声
*           4,初始化完成，可以开始控制
********************************************/
void Propeller_Init(void)
{
		TIM_SetCompare1(TIM12,1000); 	 	//最高转速信号  	垂直推进器1号
		TIM_SetCompare2(TIM12,1000);	  //最高转速信号  	垂直推进器2号
		TIM_SetCompare1(TIM1,1000);  		//最高转速信号   	水平推进器1号
		TIM_SetCompare2(TIM1,1000);  		//最高转速信号    水平推进器2号
		TIM_SetCompare3(TIM1,1000); 		//最高转速信号    水平推进器3号
		TIM_SetCompare4(TIM1,1000);  		//最高转速信号    水平推进器4号
	
		rt_thread_mdelay(1000);   						 	//1s
		TIM_SetCompare1(TIM12,PropellerParamter.PowerMed);		//停转信号
		TIM_SetCompare2(TIM12,PropellerParamter.PowerMed);		//停转信号
		TIM_SetCompare1(TIM1, PropellerParamter.PowerMed);			//停转信号
		TIM_SetCompare2(TIM1, PropellerParamter.PowerMed);			//停转信号
		TIM_SetCompare3(TIM1, PropellerParamter.PowerMed);			//停转信号
		TIM_SetCompare4(TIM1, PropellerParamter.PowerMed);			//停转信号
		rt_thread_mdelay(1000);  
}

