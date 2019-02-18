#ifndef __USB_CDC_H__
#define __USB_CDC_H__

/* 头文件 */
#include "USB.h"
#include "FslTypes.h"

extern uint8 u8CDCState;

#define CDC_INPointer   gu8EP2_IN_ODD_Buffer
#define CDC_OUTPointer  gu8EP3_OUT_ODD_Buffer

/* Defines */
#define CDC_BUFFER_SIZE 128
#define EP_OUT          EP3
#define EP_IN           EP2

/* USB_CDC Definitions */
#define WAITING_FOR_ENUMERATION 0x00
#define SET_LINE_CODING         0x20
#define GET_LINE_CODING         0x21
#define SET_CONTROL_LINE_STATE  0x22
#define LOADER_MODE             0xAA
#define GET_INTERFACE           0x0A
#define GET_STATUS              0x00
#define CLEAR_FEATURE           0x01
#define SET_FEATURE             0x03

/* TypeDefs */
typedef struct
{
    uint32  DTERate;
    uint8   CharFormat;
    uint8   ParityType;
    uint8   Databits;
} CDC_Line_Coding;


/* 外部调用的变量 */
extern uint8 CDC_OUT_Data[];
extern CDC_Line_Coding com_cfg;


/* 函数 */
void    CDC_Init(void);
void    CDC_Engine(void);
uint8   CDC_InterfaceReq_Handler(void);

#endif /* __CDC__*/