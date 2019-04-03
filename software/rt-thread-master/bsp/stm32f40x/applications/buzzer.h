#ifndef __BUZZER_H
#define __BUZZER_H

#include "DataType.h"


void Buzzer_Init(void);
	
void buzzer_once(void);

/* count为响的次数  length响的时间长度  */
void buzzer_bibi(uint8 count,uint8 length);

void buzzer_ring_task(void);


#endif



