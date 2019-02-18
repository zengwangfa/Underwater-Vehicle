#ifndef _MEETCAR_H_     
#define _MEETCAR_H_


//--------------会车
void First_Meet(void);

void Smoothness_Judge(void);
void variance_judge_left(void);//左边线  方差判断   
void variance_judge_right(void);//右边线  方差判断   
void variance_judge_center(void);//中线方差判断
void sunken_edge(void);          //边线凹陷

void Straight_Meet_Judge(void);
void Meet_Action(void);
void Out_Straight_Meet(void);

void FootWalk_scanning(void);

void circle_reverse();






#endif