#ifndef __DEBUG_
#define __DEBUG_

#include "sys.h"


extern char *debug_tool_name[3];


/* VCAN山外上位机调试 BEGIN */
void Vcan_Send_Cmd(void *wareaddr, unsigned int waresize);
void Vcan_Send_Data(void);


#endif


