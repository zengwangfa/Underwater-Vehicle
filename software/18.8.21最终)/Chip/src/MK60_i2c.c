/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_i2c.c
 * @brief      i2c驱动函数
 * @author     山外科技
 * @version    v5.0
 * @date       2013-07-12
 */

#include "common.h"
#include "MK60_port.h"
#include "MK60_i2c.h"

unsigned char MasterTransmission;
unsigned char SlaveID;

I2C_MemMapPtr I2CN[2] = {I2C0_BASE_PTR, I2C1_BASE_PTR}; //定义两个指针数组保存 I2CN 的地址


/*
 *  把I2C通信的每个小步骤都用宏定义来实现，方便编写顶层函数
 *  此宏定义参考飞思卡尔公司例程修改所得
 */
//启动信号
#define i2c_Start(I2Cn)             I2C_C1_REG(I2CN[I2Cn]) |= (I2C_C1_TX_MASK | I2C_C1_MST_MASK)    //MST 由0变1，产生起始信号，TX = 1 进入发送模式

//停止信号
#define i2c_Stop(I2Cn)              I2C_C1_REG(I2CN[I2Cn]) &= ~(I2C_C1_MST_MASK | I2C_C1_TX_MASK)   //MST 由1变0，产生停止信号，TX = 0 进入接收模式

//重复启动
#define i2c_RepeatedStart(I2Cn)     I2C_C1_REG(I2CN[I2Cn]) |= I2C_C1_RSTA_MASK

//进入接收模式(应答,需要接收多个数据，接收最后一个字节前需要禁用应答i2c_DisableAck)
#define i2c_EnterRxMode(I2Cn)       I2C_C1_REG(I2CN[I2Cn]) &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK)  //

//进入接收模式(不应答,只接收一个字节)
#define i2c_PutinRxMode(I2Cn)       I2C_C1_REG(I2CN[I2Cn]) &= ~I2C_C1_TX_MASK;I2C_C1_REG(I2CN[I2Cn]) |= I2C_C1_TXAK_MASK

//禁用应答(接收最后一个字节)
#define i2c_DisableAck(I2Cn)        I2C_C1_REG(I2CN[I2Cn]) |= I2C_C1_TXAK_MASK

//等待 I2C_S
#define i2c_Wait(I2Cn)              while(( I2C_S_REG(I2CN[I2Cn]) & I2C_S_IICIF_MASK)==0) {} \
                                    I2C_S_REG(I2CN[I2Cn]) |= I2C_S_IICIF_MASK;

//写一个字节
#define i2c_write_byte(I2Cn,data)   (I2C_D_REG(I2CN[I2Cn]) = (data));i2c_Wait(I2Cn)

/*!
 *  @brief      I2C初始化，设置波特率
 *  @param      I2Cn_e      I2C模块(I2C0、I2C1)
 *  @param      baud        期待的波特率
 *  @return                 实际的波特率
 *  @since      v5.0
 *  Sample usage:       i2c_init(I2C0,400*1000);     // 初始化I2C0，期待的波特率为400k
 */
uint32 i2c_init(I2Cn_e i2cn, uint32 baud)
{
    if(i2cn == I2C0)
    {
        /* 开启时钟 */
#if defined(MK60DZ10)
        SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;           //开启 I2C0时钟
#elif defined( MK60F15)
        SIM_SCGC4 |= SIM_SCGC4_IIC0_MASK;           //开启 I2C0时钟
#endif

        /* 配置 I2C0功能的 GPIO 接口 */
        if(I2C0_SCL == PTB0)
            port_init (PTB0, ALT2 | ODO | PULLUP );
        else if(I2C0_SCL == PTB2)
            port_init (PTB2, ALT2 | ODO | PULLUP );
        else if(I2C0_SCL == PTD8)
            port_init (PTD8, ALT2 | ODO | PULLUP );
        else
            ASSERT(0);                              //上诉条件都不满足，直接断言失败了，设置管脚有误？

        if(I2C0_SDA == PTB1)
            port_init (PTB1, ALT2 | ODO | PULLUP );
        else if(I2C0_SDA == PTB3)
            port_init (PTB3, ALT2 | ODO | PULLUP );
        else if(I2C0_SDA == PTD9)
            port_init (PTD9, ALT2 | ODO | PULLUP );
        else
            ASSERT(0);                              //上诉条件都不满足，直接断言失败了，设置管脚有误？
    }
    else
    {
        /* 开启时钟 */
#if defined(MK60DZ10)
        SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;           //开启 I2C1时钟
#elif defined(MK60F15)
        SIM_SCGC4 |= SIM_SCGC4_IIC1_MASK;           //开启 I2C1时钟
#endif
        /* 配置 I2C1功能的 GPIO 接口 */
        if(I2C1_SCL == PTE1)
            port_init (PTE1, ALT6 | ODO | PULLUP );
        else if(I2C1_SCL == PTC10)
            port_init (PTC10, ALT2 | ODO | PULLUP );
        else
            ASSERT(0);                          //上诉条件都不满足，直接断言失败了，设置管脚有误？

        if(I2C1_SDA == PTE0)
            port_init (PTE0, ALT6 | ODO | PULLUP );
        else if (I2C1_SDA == PTC11)
            port_init (PTC11, ALT2 | ODO | PULLUP );
        else
            ASSERT(0);                          //上诉条件都不满足，直接断言失败了，设置管脚有误？
    }

    /* 设置频率 */

    // I2C baud rate = bus speed (Hz)/(mul × SCL divider)  即这里 50MHz/(1 ×128)=390.625kHz
    // SDA hold time = bus period (s) × mul × SDA hold value
    // SCL start hold time = bus period (s) × mul × SCL start hold value
    // SCL stop hold time = bus period (s) × mul × SCL stop hold value

    //查表 ICR 对应的  SCL_divider ，见 《K60P144M100SF2RM.pdf》第1468页的 I2C Divider and Hold Values
    uint16 ICR_2_SCL_divider[0x40]  =
    {
        20, 22, 24, 26, 28, 30, 34, 40, 28, 32, 36, 40, 44, 48, 56, 68,
        48, 56, 64, 72, 80, 88, 104, 128, 80, 96, 112, 128, 144, 160, 192, 240,
        160, 192, 224, 256, 288, 320, 384, 480, 320, 384, 448, 512, 576, 640, 768, 960,
        640, 768, 896, 1024, 1152, 1280, 1536, 1920, 1280, 1536, 1792, 2048, 2304, 2560, 3072, 3840
    };

    uint8 mult;
    if(bus_clk_khz <= 50000)mult = 0;         //bus 一分频
    else  if(bus_clk_khz <= 100000)mult = 1;  //bus 二分频
    else      mult = 2;                       //bus 四分频

    uint16 scldiv =  bus_clk_khz * 1000 / ( (mult + 1) * baud );  //最佳的分频系数

    //需要从 ICR_2_SCL_divider 里找到 与最佳分频系数scldiv最相近的 分频系数
    uint8 icr, n = 0x40;
    uint16 min_Dvalue = ~0, Dvalue;

    while(n)                                            //循环里逐个扫描，找出最接近的 分频系数
    {
        n--;
        Dvalue = abs(scldiv - ICR_2_SCL_divider[n]);
        if(Dvalue == 0)
        {
            icr = n;
            break;                                      //退出while循环
        }

        if(Dvalue < min_Dvalue)
        {
            icr = n;
            min_Dvalue = Dvalue;
        }
    }

    I2C_F_REG(I2CN[i2cn])  = ( 0                        // I2C Frequency Divider register (I2Cx_F)  I2C分频寄存器   I2C最大波特率为 400k
                               | I2C_F_MULT(mult)        // 乘数因子 mul = MULT + 1
                               | I2C_F_ICR(icr)          // 时钟速率 = ICR_2_SCL_divider[ICR] ，查表获得 ICR 与 SCL_divider 映射关系
                             );

    /* 使能 IIC1 */
    I2C_C1_REG(I2CN[i2cn]) = ( 0
                               | I2C_C1_IICEN_MASK       //使能I2C
                               //| I2C_C1_IICIE_MASK       //使能中断
                             );

    return (  bus_clk_khz * 1000 / ( (mult + 1) * ICR_2_SCL_divider[icr])  );
}


/*!
 *  @brief      I2C通信结束后需要调用的函数函数
 *  @since      v5.0
 *  @note       如果通信失败，可尝试增大此延时值，确认是否延时导致的
 */
void Pause(void)
{
    volatile uint16 n = 80;     //注意，这个数据太小，会导致读取错误。

    while(n--)
    {
        asm("nop");
    }
}

/*!
 *  @brief      读取I2C设备指定地址寄存器的数据
 *  @param      I2Cn_e        I2C模块(I2C0、I2C1)
 *  @param      SlaveID     从机地址(7位地址)
 *  @param      reg         从机寄存器地址
 *  @return                 读取的寄存器值
 *  @since      v5.0
 *  Sample usage:       uint8 value = i2c_read_reg(I2C0, 0x1D, 1);
 */
uint8 i2c_read_reg(I2Cn_e i2cn, uint8 SlaveID, uint8 reg)
{

    //先写入寄存器地址,再读取数据,因此此过程是 I2C 的复合格式,改变数据方向时需要重新启动
    uint8 result;

    ASSERT((SlaveID & 0x80) == 0);                      //断言，我们要求的7位地址的值仅仅是7bit,不是通信时要求的高7位
    //有些手册，给出的7位地址指的是8bit里的高7位
    //有些手册，给出的7位地址指的是7bit
    //请自行确认，可以尝试是否通信正常来确认

    i2c_Start(i2cn);                                    //发送启动信号

    i2c_write_byte(i2cn, ( SlaveID << 1 ) | MWSR);      //发送从机地址和写位

    i2c_write_byte(i2cn, reg);                          //发送从机里的寄存器地址

    i2c_RepeatedStart(i2cn);                            //复合格式，发送重新启动信号

    i2c_write_byte(i2cn, ( SlaveID << 1) | MRSW );      //发送从机地址和读位

    i2c_PutinRxMode(i2cn);                              //进入接收模式(不应答,只接收一个字节)
    result = I2C_D_REG(I2CN[i2cn]);                     //虚假读取一次，启动接收数据
    i2c_Wait(i2cn);                                     //等待接收完成

    i2c_Stop(i2cn);                                     //发送停止信号

    result = I2C_D_REG(I2CN[i2cn]);                     //读取数据

    Pause();                                            //必须延时一下，否则出错

    return result;
}


/*!
 *  @brief      写入一个字节数据到I2C设备指定寄存器地址
 *  @param      I2Cn_e        I2C模块(I2C0、I2C1)
 *  @param      SlaveID     从机地址(7位地址)
 *  @param      reg         从机寄存器地址
 *  @param      Data        数据
 *  @since      v5.0
 *  Sample usage:       i2c_write_reg(I2C0, 0x1D, 1,2);     //向从机0x1D 的寄存器 1 写入数据 2
 */

void i2c_write_reg(I2Cn_e i2cn, uint8 SlaveID, uint8 reg, uint8 Data)
{

    i2c_Start(i2cn);                                    //发送启动信号

    i2c_write_byte(i2cn, ( SlaveID << 1 ) | MWSR);      //发送从机地址和写位

    i2c_write_byte(i2cn, reg);                         //发送从机里的寄存器地址

    i2c_write_byte(i2cn, Data);                         //发送需要写入的数据

    i2c_Stop(i2cn);

    Pause();                                            //延时太短的话，可能写出错
}

