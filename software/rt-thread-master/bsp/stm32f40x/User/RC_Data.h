#ifndef __RC_DATA_H_
#define __RC_DATA_H_

#include "DataType.h"


typedef struct{
		uint16  THR;
		uint16  YAW;
		uint16  ROL;
		uint16  PIT;
	
}ReceiveDataType;

typedef struct{
		uint8  Depth_Lock;     //【深度】	 0x01 深度锁定、0x02 人工控制
		uint8  Direction_Lock; //【方向】	 0x01 方向锁定、0x02 随水动
		uint8  Move;      		 //【运动】	 0x01 前进、    0x02 后退、  0x00 不动作（默认）
		uint8  Translation;    //【平移】	 0x01 左平移、  0x02 右平移、0x00 不动作（默认）
	  uint8  Vertical;       //【垂直】	 0x01 向上、    0x02 向下、  0x00 不动作（默认）
		uint8  Rotate;         //【旋转】	 0x01 左旋、    0x02 右旋、  0x00表示不动作（默认）
		uint8  Power;          //【油门】	 0x00~0xff (0~255) 表示的油门大小：4档位可调，LB加档，LT减档，可分别设置4个档位油门大小
		uint8  Light;          //【灯光】	 0x01 打开、    0x00 关闭（默认）
		uint8  Focus;          //【聚焦】  0x01 聚焦、			0x02 放焦、  0x00表示不动作（默认）
		uint8  Zoom;           //【变焦】  0x01 放大、			0x02 缩小、  0x00表示不动作（默认）
		uint8  Yuntai;         //【云台】	 0x01 向上、    0x02 向下、  0x00表示不动作（默认）
		uint8  Arm;            //【机械臂】0x01 张开、     0x02 关闭、  0x00表示不动作（默认）
		uint8  Reserve;            //【机械臂】0x01 张开、     0x02 关闭、  0x00表示不动作（默认）
}ControlDataType;





extern ReceiveDataType ReceiveData;



#endif 




