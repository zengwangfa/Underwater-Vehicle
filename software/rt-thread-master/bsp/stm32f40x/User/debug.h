#ifndef __DEBUG_
#define __DEBUG_

#include "DataType.h"


/* VCAN山外上位机调试 BEGIN */
void Vcan_Send_Cmd(void *wareaddr, unsigned int waresize);
void Vcan_Send_Data(void);

void Debug_Mode(int button);

extern char *debug_tool_name[3];

extern uint8 debug_tool ; //山外 / 匿名上位机 调试标志位

#endif


