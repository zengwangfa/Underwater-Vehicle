#ifndef __BUZZER_H
#define __BUZZER_H

#include "init.h"


void Buzzer_Init(void);
	
void buzzer_once(void);

/* count为响的次数  length响的时间长度  */
void buzzer_bibi(u8 count,u8 length);

void buzzer_ring_task(void);


#endif



