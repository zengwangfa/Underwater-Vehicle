/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_flexbus.c
 * @brief      flexbus函数库
 * @author     山外科技
 * @version    v5.0
 * @date       2013-09-02
 */

#include "common.h"
#include "MK60_flexbus.h"


/*!
 *  @brief      flexbus初始化为8080协议
 *  @since      v5.0
 */
void flexbus_8080_init()
{
    //flexbus 本身不直接支持 8080协议，而是支持 6800协议
    //我们可以配置 cmd 与 data 数据的地址不同，从而通过地址线来区分命令和数据

    SIM_SOPT2 |= SIM_SOPT2_FBSL(3);             //FlexBus安全水平:允许指令和数据访问
    SIM_SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;        //使能时钟

    //配置数据管脚复用
    port_init(PTD6  , ALT5 | HDS);              //PTD6 复用为 fb_ad[0] ,配置为 输出高驱动能力
    port_init(PTD5  , ALT5 | HDS);              //PTD5 复用为 fb_ad[1] ,配置为 输出高驱动能力
    port_init(PTD4  , ALT5 | HDS);              //PTD4 复用为 fb_ad[2] ,配置为 输出高驱动能力
    port_init(PTD3  , ALT5 | HDS);              //PTD3 复用为 fb_ad[3] ,配置为 输出高驱动能力
    port_init(PTD2  , ALT5 | HDS);              //PTD2 复用为 fb_ad[4] ,配置为 输出高驱动能力
    port_init(PTC10 , ALT5 | HDS);              //PTC10复用为 fb_ad[5] ,配置为 输出高驱动能力
    port_init(PTC9  , ALT5 | HDS);              //PTC9 复用为 fb_ad[6] ,配置为 输出高驱动能力
    port_init(PTC8  , ALT5 | HDS);              //PTC8 复用为 fb_ad[7] ,配置为 输出高驱动能力
    port_init(PTC7  , ALT5 | HDS);              //PTC7 复用为 fb_ad[8] ,配置为 输出高驱动能力
    port_init(PTC6  , ALT5 | HDS);              //PTC6 复用为 fb_ad[9] ,配置为 输出高驱动能力
    port_init(PTC5  , ALT5 | HDS);              //PTC5 复用为 fb_ad[10],配置为 输出高驱动能力
    port_init(PTC4  , ALT5 | HDS);              //PTC4 复用为 fb_ad[11],配置为 输出高驱动能力
    port_init(PTC2  , ALT5 | HDS);              //PTC2 复用为 fb_ad[12],配置为 输出高驱动能力
    port_init(PTC1  , ALT5 | HDS);              //PTC1 复用为 fb_ad[13],配置为 输出高驱动能力
    port_init(PTC0  , ALT5 | HDS);              //PTC0 复用为 fb_ad[14],配置为 输出高驱动能力
    port_init(PTB18 , ALT5 | HDS);              //PTB18复用为 fb_ad[15],配置为 输出高驱动能力

    //配置控制管脚复用
    //8080总线,即Intel总线,需要四根线控制线：RD写使能, WR读使能, RS数据/指令选择, CS片选
    port_init(PTB19 , ALT5 | HDS);              //PTB19复用为 fb_oe_b  ,配置为 输出高驱动能力 , fb_oe_b 时序符合 8080 总线的 RD写使能
    port_init(PTD1  , ALT5 | HDS);              //PTD1 复用为 fb_cs0_b ,配置为 输出高驱动能力 , fb_cs0_b时序符合 8080 总线的 CS片选
    port_init(PTC11 , ALT5 | HDS);              //PTC11复用为 fb_r/w   ,配置为 输出高驱动能力 , fb_r/w  时序符合 8080 总线的 WR读使能

    //目前还缺 8080 总线的 RS数据/指令选择线
    //flexbus可配置成两块地址不连续的块（高16位地址不相同），从而利用地址线来符合 8080 总线的 RS数据/指令选择线
    //高16位的地址，FB_BA 我们配置为 0x6000 ，因而可选择的 地址线有 fb_ad[16] ~ fb_ad[28]
    //FB_BAM配置为 0x0800 ，即选择 fb_ad[27] 作为 RS 。(0x0800 0000 == 1<<27 )
    // RS == 1 时 传输数据，RS == 0 时传输命令
    //因而 0x6000 0000 为 命令地址 ， 0x6800 0000 为数据端口
    port_init(PTC12 , ALT5 | HDS);              //PTC12复用为 fb_ad[27],配置为 输出高驱动能力 , fb_ad[27]作为8080 总线的 RS数据/指令选择线

    FB_CSAR(0) = FB_CSAR_BA(FB_BA);             // 基地址 Base address
    FB_CSMR(0) =  ( 0
                    | FB_CSMR_BAM(FB_BAM)       // BAM = 0x0800 ,基地地址掩码 为 0x,800 FFFF ,即 片选有效的地址为 基地址 ~ （基地址 + 基地地址掩码） ,0x0800 0000 对应与 FB_AD27
                    | FB_CSMR_V_MASK            // 使用片选信号 FB_CS0
                  );
    FB_CSCR(0) =    FB_CSCR_BLS_MASK            //右对齐
                    | FB_CSCR_PS(2)             //16Byte数据
                    | FB_CSCR_AA_MASK           // 自动应答
                    ;

}