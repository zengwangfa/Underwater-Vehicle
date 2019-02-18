#include"USB_CDC.h"



/* CDC Global Structures */
CDC_Line_Coding com_cfg;
uint8 u8CDCState    = WAITING_FOR_ENUMERATION;
uint8 CDC_OUT_Data[CDC_BUFFER_SIZE];



/*!
 *  @brief      CDC类初始化函数
 *  @since      v5.0
 */
void CDC_Init(void)
{
    //配置串口特性
    com_cfg.DTERate      = 9600;                //波特率
    com_cfg.CharFormat   = 0;                   //字符格式
    com_cfg.ParityType   = 0;                   //极性类型
    com_cfg.Databits     = 0x08;                //数据位数
}

/**********************************************************/
void CDC_Engine(void)
{
    //uint16 u8RecData;
    /* control Stage */
    switch(u8CDCState)
    {
    case WAITING_FOR_ENUMERATION:
        /* 等待 USB 枚举 */
        while(gu8USB_State != uENUMERATED)
        {
            (void)u8CDCState;
        };


        u8CDCState = 0;
        break;

    case SET_LINE_CODING:
        if(FLAG_CHK(EP0, gu8USB_Flags))
        {
            FLAG_CLR(EP0, gu8USB_Flags);
            (void)EP_OUT_Transfer(EP0, (uint8 *)&com_cfg);
            EP_IN_Transfer(EP0, 0, 0);
        }
        break;

    case SET_CONTROL_LINE_STATE:
        EP_IN_Transfer(EP0, 0, 0);
        break;

    }
}

/*!
 *  @brief      当前接口请求为CDC类请求 的 中断复位函数
 *  @note       在 USB_Setup_Handler 中断里调用
 *  @since      v5.0
 */
uint8 CDC_InterfaceReq_Handler(void)
{
    uint8 u8State = uSETUP;

    switch(Setup_Pkt->bRequest)
    {
    case GET_INTERFACE:
        EP_IN_Transfer(EP0, &gu8Interface, 1);
        break;

    case GET_LINE_CODING:
        EP_IN_Transfer(EP0, (uint8 *)&com_cfg, 7);
        break;

    case SET_LINE_CODING:
        u8CDCState = SET_LINE_CODING;
        u8State = uDATA;
        break;

    case SET_CONTROL_LINE_STATE:
        u8CDCState = SET_CONTROL_LINE_STATE;
        u8State = uSETUP;
        break;

    case LOADER_MODE:
        FLAG_SET(LOADER, gu8USB_Flags);
        CDC_OUT_Data[0] = 0xFF;
        break;
    default:
        break;
    }
    return(u8State);
}

