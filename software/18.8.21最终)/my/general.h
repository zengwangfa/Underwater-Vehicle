#ifndef H_GENERAL_H
#define H_GENERAL_H
#include "include.h"

typedef enum
{
	
	normaliz=1,
	flash=2,   
	car_run=3,
	picture=4,
	mode_screen=5,
}
MENU_LIST_e; //菜单

typedef enum
{
	meet_reversal=4,
	reversal=3,
	run=2,
	stop=1,
	error=0,
}
CAR_STATUS_e;  //车子状态 


typedef enum
{
	in_contorl=0, //AD或摄像头控制中
	enforce_left=1, //强制左
	enforce_right=2, //强制右
	
}
TURN_STATUS_e; //强制打角

typedef enum
{   
	straight=1, //直道
	corner=2, //弯道
	circle=3, //圆环
	cross=4, //十字
	meet=5,//汇车
	ramp=6,
}
ROAD_STATUS_e;  //赛道状态



typedef enum
{
	no_ring=0, //附近没有圆环
	near_ring=1, //靠近圆环
	reach_ring_point=2, //到达圆环直道分离点
	at_ring=3, //在环内
//	out_ring=4, //出环
	ring_meet=4,
}
RING_FLAG_e;


typedef enum
{
	no_meet=0,
	meet_straight=1,
	meet_circle=2,
}
MEET_PLACE_e;



typedef struct A
{
	int16 goal_speed_L;
	int16 goal_speed_R;
}to_speed;


typedef struct R
{
	RING_FLAG_e ring_flag;
	TURN_STATUS_e ring_turn;
}ring_missage;

typedef enum
{
	un_know=0,
	front_car=1,
	after_car=2,
}
MEET_CAR_NUMBER_e;


void ware_test();
void buzzer_once();
void buzzer_bibi(uint8 t,uint8 l);
void buzzer_ring();
void time_count();
void dma_count();
void LED_ALL_ON();
void LED_ALL_OFF();
#define servo_sector (FLASH_SECTOR_NUM-1)
#define magnetic_sector (FLASH_SECTOR_NUM-2)





#endif