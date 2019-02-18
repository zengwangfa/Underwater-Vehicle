/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_uart.c
 * @brief      uart串口函数
 * @author     山外科技
 * @version    v5.0
 * @date       2013-06-26
 */


#include "common.h"
#include "MK60_uart.h"

UART_MemMapPtr UARTN[UART_MAX] = {UART0_BASE_PTR, UART1_BASE_PTR, UART2_BASE_PTR, UART3_BASE_PTR, UART4_BASE_PTR, UART5_BASE_PTR}; //定义五个指针数组保存 UARTN 的地址


/*!
 *  @brief      初始化串口，设置波特率
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @param      baud        波特率，如9600、19200、56000、115200等
 *  @since      v5.0
 *  @note       UART所用的管脚在 vcan_drivers_cfg.h 里进行配置，
                printf所用的管脚和波特率在 k60_vcan.h 里进行配置
 *  Sample usage:       uart_init (UART3, 9600);        //初始化串口3，波特率为9600
 */
void uart_init (UARTn_e uratn, uint32 baud)
{
    register uint16 sbr, brfa;
    uint8 temp;
    uint32 sysclk;     //时钟

    /* 配置 UART功能的 复用管脚 */
    switch(uratn)
    {
    case UART0:
        SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;      //使能 UART0 时钟

        if(UART0_RX == PTA1)
        {
            port_init( PTA1, ALT2);             //在PTA1上使能UART0_RXD
        }
        else if(UART0_RX == PTA15)
        {
            port_init( PTA15, ALT3);            //在PTA15上使能UART0_RXD
        }
        else if(UART0_RX == PTB16)
        {
            port_init( PTB16, ALT3);            //在PTB16上使能UART0_RXD
        }
        else if(UART0_RX == PTD6)
        {
            port_init( PTD6, ALT3);             //在PTD6上使能UART0_RXD
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }

        if(UART0_TX == PTA2)
        {
            port_init( PTA2, ALT2);             //在PTA2上使能UART0_RXD
        }
        else if(UART0_TX == PTA14)
        {
            port_init( PTA14, ALT3);            //在PTA14上使能UART0_RXD
        }
        else if(UART0_TX == PTB17)
        {
            port_init( PTB17, ALT3);            //在PTB17上使能UART0_RXD
        }
        else if(UART0_TX == PTD7)
        {
            port_init( PTD7, ALT3);             //在PTD7上使能UART0_RXD
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }

        break;

    case UART1:
        SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;

        if(UART1_RX == PTC3)
        {
            port_init( PTC3, ALT3);             //在PTC3上使能UART1_RXD
        }
        else if(UART1_RX == PTE1)
        {
            port_init( PTE1, ALT3);             //在PTE1上使能UART1_RXD
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }

        if(UART1_TX == PTC4)
        {
            port_init( PTC4, ALT3);             //在PTC4上使能UART1_RXD
        }
        else if(UART1_TX == PTE0)
        {
            port_init( PTE0, ALT3);             //在PTE0上使能UART1_RXD
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }

        break;

    case UART2:
        SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
        port_init( PTD3, ALT3);                 //在PTD3上使能UART2_TXD功能
        port_init( PTD2, ALT3);                 //在PTD2上使能UART2_RXD
        break;

    case UART3:
        SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;

        if(UART3_RX == PTB10)
        {
            port_init( PTB10, ALT3);              //在PTB10上使能UART3_RXD
        }
        else if(UART3_RX == PTC16)
        {
            port_init( PTC16, ALT3);             //在PTC16上使能UART3_RXD
        }
        else if(UART3_RX == PTE5)
        {
            port_init( PTE5, ALT3);             //在PTE5上使能UART3_RXD
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }

        if(UART3_TX == PTB11)
        {
            port_init( PTB11, ALT3);             //在PTB11上使能UART3_RXD
        }
        else if(UART3_TX == PTC17)
        {
            port_init( PTC17, ALT3);            //在PTC17上使能UART3_RXD
        }
        else if(UART3_TX == PTE4)
        {
            port_init( PTE4, ALT3);             //在PTE4上使能UART3_RXD
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }
        break;

    case UART4:
        SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;

        if(UART4_RX == PTC14)
        {
            port_init( PTC14, ALT3);            //在PTC14上使能UART4_RXD
        }
        else if(UART4_RX == PTE25)
        {
            port_init( PTE25, ALT3);            //在PTE25上使能UART4_RXD
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }

        if(UART4_TX == PTC15)
        {
            port_init( PTC15, ALT3);            //在PTC15上使能UART4_RXD
        }
        else if(UART4_TX == PTE24)
        {
            port_init( PTE24, ALT3);            //在PTE24上使能UART4_RXD
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }
        break;

    case UART5:
        SIM_SCGC1 |= SIM_SCGC1_UART5_MASK;

        if(UART5_RX == PTD8)
        {
            port_init( PTD8, ALT3);             //在PTD8上使能UART5_RXD
        }
        else if(UART5_RX == PTE9)
        {
            port_init( PTE9, ALT3);             //在PTE9上使能UART5_RXD
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }

        if(UART5_TX == PTD9)
        {
            port_init( PTD9, ALT3);             //在PTD9上使能UART5_RXD
        }
        else if(UART5_TX == PTE8)
        {
            port_init( PTE8, ALT3);             //在PTE8上使能UART5_RXD
        }
        else
        {
            ASSERT(0);                           //上诉条件都不满足，直接断言失败了，设置管脚有误？
        }
        break;
    default:
        break;
    }

    //设置的时候，应该禁止发送接受
    UART_C2_REG(UARTN[uratn]) &= ~(0
                                   | UART_C2_TE_MASK
                                   | UART_C2_RE_MASK
                                  );


    //配置成8位无校验模式
    //设置 UART 数据格式、校验方式和停止位位数。通过设置 UART 模块控制寄存器 C1 实现；
    UART_C1_REG(UARTN[uratn]) |= (0
                                  //| UART_C2_M_MASK                    //9 位或 8 位模式选择 : 0 为 8位 ，1 为 9位（注释了表示0，即8位） （如果是9位，位8在UARTx_C3里）
                                  //| UART_C2_PE_MASK                   //奇偶校验使能（注释了表示禁用）
                                  //| UART_C2_PT_MASK                   //校验位类型 : 0 为 偶校验 ，1 为 奇校验
                                 );

    //计算波特率，串口0、1使用内核时钟，其它串口使用bus时钟
    if ((uratn == UART0) || (uratn == UART1))
    {
        sysclk = core_clk_khz * 1000;                                   //内核时钟
    }
    else
    {
        sysclk = bus_clk_khz * 1000;                                    //bus时钟
    }

    //UART 波特率 = UART 模块时钟 / (16 × (SBR[12:0] + BRFA))
    //不考虑 BRFA 的情况下， SBR = UART 模块时钟 / (16 * UART 波特率)
    sbr = (uint16)(sysclk / (baud * 16));
    if(sbr > 0x1FFF)sbr = 0x1FFF;                                       //SBR 是 13bit，最大为 0x1FFF

    //已知 SBR ，则 BRFA =  = UART 模块时钟 / UART 波特率 - 16 ×SBR[12:0]
    brfa = (sysclk / baud)  - (sbr * 16);
    ASSERT( brfa <= 0x1F);                  //断言，如果此值不符合条件，则设置的条件不满足寄存器的设置
                                            //可以通过增大波特率来解决这个问题

    //写 SBR
    temp = UART_BDH_REG(UARTN[uratn]) & (~UART_BDH_SBR_MASK);           //缓存 清空 SBR 的 UARTx_BDH的值
    UART_BDH_REG(UARTN[uratn]) = temp |  UART_BDH_SBR(sbr >> 8);        //先写入SBR高位
    UART_BDL_REG(UARTN[uratn]) = UART_BDL_SBR(sbr);                     //再写入SBR低位

    //写 BRFD
    temp = UART_C4_REG(UARTN[uratn]) & (~UART_C4_BRFA_MASK) ;           //缓存 清空 BRFA 的 UARTx_C4 的值
    UART_C4_REG(UARTN[uratn]) = temp |  UART_C4_BRFA(brfa);             //写入BRFA



    //设置FIFO(FIFO的深度是由硬件决定的，软件不能设置)
    UART_PFIFO_REG(UARTN[uratn]) |= (0
                                     | UART_PFIFO_TXFE_MASK               //使能TX FIFO(注释表示禁止)
                                     //| UART_PFIFO_TXFIFOSIZE(0)         //（只读）TX FIFO 大小，0为1字节，1~6为 2^(n+1)字节
                                     | UART_PFIFO_RXFE_MASK               //使能RX FIFO(注释表示禁止)
                                     //| UART_PFIFO_RXFIFOSIZE(0)         //（只读）RX FIFO 大小，0为1字节，1~6为 2^(n+1)字节
                                    );

    /* 允许发送和接收 */
    UART_C2_REG(UARTN[uratn]) |= (0
                                  | UART_C2_TE_MASK                     //发送使能
                                  | UART_C2_RE_MASK                     //接收使能
                                  //| UART_C2_TIE_MASK                  //发送中断或DMA传输请求使能（注释了表示禁用）
                                  //| UART_C2_TCIE_MASK                 //发送完成中断使能（注释了表示禁用）
                                  //| UART_C2_RIE_MASK                  //接收满中断或DMA传输请求使能（注释了表示禁用）
                                 );

    //设置是否允许接收和发送中断。通过设置 UART 模块的 C2 寄存器的
    //RIE 和 TIE 位实现。如果使能中断，必须首先实现中断服务程序；
}

/*!
 *  @brief      等待接受1个字节
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @param      ch          接收地址
 *  @since      v5.0
 *  @note       如果需要查询接收状态，可用 uart_query ，
                如果需要查询接收数据，可用 uart_querychar
 *  Sample usage:       char ch = uart_getchar (UART3);   //等待接受1个字节，保存到 ch里
 */
void uart_getchar (UARTn_e uratn, char *ch)
{
    //等待接收到数据（使能了FIFO后，以下两种方法都可行，都是一样的意思）
    //while( !UART_RCFIFO_REG(UARTN[uratn]) );                                //等待接收到数据数量非0
    //while( (UART_SFIFO_REG(UARTN[uratn]) & UART_SFIFO_RXEMPT_MASK)) ;     //等待接收缓冲区/FIFO 非空的

    //如下方法是飞思卡尔官方例程提供的方法
    while (!(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK));       //等待接收满了

    // 获取接收到的8位数据
    *ch =  UART_D_REG(UARTN[uratn]);

    // 获取 9位数据，应该是（需要修改函数的返回类型）：
    // *ch =   ((( UARTx_C3_REG(UARTN[uratn]) & UART_C3_R8_MASK ) >> UART_C3_R8_SHIFT ) << 8)   |   UART_D_REG(UARTN[uratn]);  //返回9bit

}


/*!
 *  @brief      查询接收1个字符
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @param      ch          接收地址
 *  @return     1为接收成功，0为接收失败
 *  @since      v5.0
 *  @note       如果需要等待接收，可用 uart_getchar
 *  Sample usage:       char ch ;
                        if( uart_querychar (UART3,&ch) == 1)     //查询接收1个字符，保存到 ch里
                        {
                            printf("成功接收到一个字节");
                        }
 */
char uart_querychar (UARTn_e uratn, char *ch)
{
    if( UART_RCFIFO_REG(UARTN[uratn]) )         //查询是否接受到数据
    {
        *ch  =   UART_D_REG(UARTN[uratn]);      //接受到8位的数据
        return  1;                              //返回 1 表示接收成功
    }

    *ch = 0;                                    //接收不到，应该清空了接收区
    return 0;                                   //返回0表示接收失败
}


/*!
 *  @brief      查询接收字符串
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @param      str         接收地址
 *  @param      max_len     最大接收长度
 *  @return     接收到的字节数目
 *  @since      v5.0
 *  Sample usage:       char str[100];
                        uint32 num;
                        num = uart_pendstr (UART3,&str,100);
                        if( num != 0 )
                        {
                            printf("成功接收到%d个字节:%s",num,str);
                        }
 */
char uart_querystr (UARTn_e uratn, char *str, uint32 max_len)
{
    uint32 i = 0;
    while(uart_querychar(uratn, str + i)  )
    {
        if( *(str + i) == NULL )    //接收到字符串结束符
        {
            return i;
        }

        i++;
        if(i >= max_len)            //超过设定的最大值，退出
        {
            return i;
        }
    };

    return i;
}


/*!
 *  @brief      查询接收buff
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @param      str         接收地址
 *  @param      max_len     最大接收长度
 *  @return     接收到的字节数目
 *  @since      v5.0
 *  Sample usage:       char buff[100];
                        uint32 num;
                        num = uart_pendbuff (UART3,&buff,100);
                        if( num != 0 )
                        {
                            printf("成功接收到%d个字节:%s",num,buff);
                        }
 */
char uart_querybuff (UARTn_e uratn, char *buff, uint32 max_len)
{
    uint32 i = 0;
    while(uart_querychar(uratn, buff + i)  )
    {
        i++;
        if(i >= max_len)            //超过设定的最大值，退出
        {
            return i;
        }
    };

    return i;
}


/*!
 *  @brief      串口发送一个字节
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @param      ch          需要发送的字节
 *  @since      v5.0
 *  @note       printf需要用到此函数
 *  @see        fputc
 *  Sample usage:       uart_putchar (UART3, 'A');  //发送字节'A'
 */
void uart_putchar (UARTn_e uratn, char ch)
{
    //等待发送缓冲区空
    while(!(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK));
 
    //发送数据
    UART_D_REG(UARTN[uratn]) = (uint8)ch;
}

/*!
 *  @brief      查询是否接受到一个字节
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @return     接收到数据返回1，没接收到数据返回0
 *  @since      v5.0
 *  Sample usage:       char ch;
                        if(uart_query (UART3) == 1)     //查询是否接收到数据
                        {
                            ch = uart_getchar (UART3);  //等待接收一个数据，保存到 ch里
                        }
 */
char uart_query (UARTn_e uratn)
{
    if(UART_RCFIFO_REG(UARTN[uratn]))                 //接收到数据数量非0
        //if(!(UART_SFIFO_REG(UARTN[uratn]) & UART_SFIFO_RXEMPT_MASK))
        //if(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    //return UART_RCFIFO_REG(UARTN[uratn]);
}

/*!
 *  @brief      发送指定len个字节长度数组 （包括 NULL 也会发送）
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @param      buff        数组地址
 *  @param      len         发送数组的长度
 *  @since      v5.0
 *  Sample usage:       uart_putbuff (UART3,"1234567", 3); //实际发送了3个字节'1','2','3'
 */
void uart_putbuff (UARTn_e uratn, uint8 *buff, uint32 len)
{
    while(len--)
    {
        uart_putchar(uratn, *buff);
        buff++;
    }
}


/*!
 *  @brief      发送字符串(遇 NULL 停止发送)
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @param      str         字符串地址
 *  @since      v5.0
 *  Sample usage:       uart_putstr (UART3,"1234567"); //实际发送了7个字节
 */
void uart_putstr (UARTn_e uratn, const uint8 *str)
{
    while(*str)
    {
        uart_putchar(uratn, *str++);
    }
}

/*!
 *  @brief      开串口接收中断
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @since      v5.0
 *  Sample usage:       uart_rx_irq_en(UART3);         //开串口3接收中断
 */
void uart_rx_irq_en(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) |= UART_C2_RIE_MASK;                          //使能UART接收中断
    enable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));                  //使能IRQ中断
}

/*!
 *  @brief      开串口发送中断
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @since      v5.0
 *  Sample usage:       uart_tx_irq_en(UART3);         //开串口3发送中断
 */
void uart_tx_irq_en(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) |= UART_C2_TIE_MASK;                          //使能UART发送中断
    enable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));                  //使能IRQ中断
}

/*!
 *  @brief      开串口发送完成中断
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @since      v5.0
 *  Sample usage:       uart_tx_irq_en(UART3);         //开串口3发送中断
 */
void uart_txc_irq_en(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) |= UART_C2_TCIE_MASK;                         //使能UART发送中断
    enable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));                  //使能IRQ中断
}

/*!
 *  @brief      关串口接收中断
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @since      v5.0
 *  Sample usage:       uart_rx_irq_dis(UART3);         //关串口3接收中断
 */
void uart_rx_irq_dis(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) &= ~UART_C2_RIE_MASK;                         //禁止UART接收中断

    //如果发送中断还没有关，则不关闭IRQ
    if(!(UART_C2_REG(UARTN[uratn]) & (UART_C2_TIE_MASK | UART_C2_TCIE_MASK)) )
    {
        disable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));             //关IRQ中断
    }
}

/*!
 *  @brief      关串口发送中断
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @since      v5.0
 *  Sample usage:       uart_tx_irq_dis(UART3);         //关串口3发送中断
 */
void uart_txc_irq_dis(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) &= ~UART_C2_TCIE_MASK;                        //禁止UART发送完成中断

    //如果接收中断还没有关，则不关闭IRQ
    if(!(UART_C2_REG(UARTN[uratn]) & UART_C2_RIE_MASK) )
    {
        disable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));             //关IRQ中断
    }
}

/*!
 *  @brief      关串口发送中断
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @since      v5.0
 *  Sample usage:       uart_tx_irq_dis(UART3);         //关串口3发送中断
 */
void uart_tx_irq_dis(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) &= ~UART_C2_TIE_MASK;                         //禁止UART发送中断

    //如果接收中断还没有关，则不关闭IRQ
    if(!(UART_C2_REG(UARTN[uratn]) & UART_C2_RIE_MASK) )
    {
        disable_irq((IRQn_t)((uratn << 1) + UART0_RX_TX_IRQn));             //关IRQ中断
    }
}

/*!
 *  @brief      UART3测试中断服务函数
 *  @since      v5.0
 *  @warning    此函数需要用户根据自己需求完成，这里仅仅是提供一个模版
 *  Sample usage:       set_vector_handler(UART3_RX_TX_VECTORn , uart3_test_handler);    //把 uart3_handler 函数添加到中断向量表，不需要我们手动调用
 */
void uart3_test_handler(void)
{
    UARTn_e uratn = UART3;

    if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //接收数据寄存器满
    {
        //用户需要处理接收数据

    }

    if(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK )  //发送数据寄存器空
    {
        //用户需要处理发送数据

    }
}