#ifndef CONTROL_H_H
#define CONTROL_H_H


#define max_output 6000


typedef struct D
{
	int16 Speed_Duty_L;
	int16 Speed_Duty_R;
}moty_duty;

void servo_control_AD();
void FTM_updata(moty_duty duty);
void PI_control();
void road_speed_control();

void Ackerman();
void Counter_collect();
void OUT_STOP();
void Go_back(int16 dist);



#endif