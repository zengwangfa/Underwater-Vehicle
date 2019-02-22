# 1.航行器控制中心

该航控 基于国产 RT-Thread 实时操作系统，目前完成外设驱动及数据采集，后续为姿态算法 与 抗洋流算法.

| 硬件 | 描述 |
| -- | -- |
|芯片型号| STM32F407ZGT6 |
|CPU| Cortex-M4 |
|主频| 84MHz-168MHz |

# 2.目录说明
```
+——Underwater_vehicle
|--------+ docs: 一些设计参考文档
|--------+ hardware:相关电路设计      
|            └──.SchDoc
|            ├──.PcbDoc
|            └──.pdf
|--------+ software:相关软件设计
|            └──rt-thread-master
|               └──bsp
|                   └──stm32f40x
|--------+ README.md
```


# 3.硬件架构
```
+——Underwater_vehicle
|--------+──DC 24V/48V 【Power management】 
|--------+──STM32F407ZGT6 【Control-Center】                  
|        |       └──【Normal Peripherals】
|        │       ├── RGB [GPIO]   
|        │       ├── KEY [GPIO] 
|        │       ├── BUZZER [GPIO]      
|        │       ├── Dial Switch [GPIO]   
|        │       ├── OLED [Software SPI]       
|        │       ├── Voltage Detection [ADC]             
|        │       ├── Bluetooth [UART]       
|        │       ├── W25Q128 FLASH [SPI]
|        │       ├── USB to serial port CP2102 UART Bridge [UART]
|        │       └──......  
|        |       └── 【Important Peripherals】
|        |       ├── Nine axis gyroscope [UART]
|        |       ├── USR-C216 Wifi [UART]   
|        |       ├── OV2640 Camera [unfinished]
|        |       └──...... 
|        +──Vehicle Devices
|        └── Searchlights [GPIO]
|        ├── Propellers [PWM]
|        ├── Servo Motor - Mechanical arm [PWM]
|        ├── CAMERAs 
|        └──...... 
```

------
## 3.1 控制中心

![Underwater Vehicle](https://images.gitee.com/uploads/images/2019/0222/201120_1db060f5_2330851.png "控制中心")

------
## 3.1 电源及 USB
核心板可以通过 Micro-USB 或 5V 电源供电。使用 Micro-USB 接到电脑时，电脑上虚拟出一个串口。连接该串口后，可以通过 shell 命令行工具进行交互，也可以查看实时的日志信息。

## 3.2 功能接口
|接口|数量|备注|
|:-:|:-:|:-:|
|GPIO|2路|其他路均可复用为GPIO|
|DAC|1路|DAC2|
|ADC|5路|ADC123|
|TIM|16路|TIM4等|
|UART|2路|UART4 —— UART5|
|SPI|1路|SPI1|
|CAN|1路|CAN1|
|OV-Camera|1路|DCMI|

## 3.3 其他接口

|接口|数量|备注|
|:-:|:-:|:-:|
|POWER IN|1路|电源电压接入口|
|Bluetooth|1路|UART4|
|SWD调试接口|1路|SWD|
|气压计|1路|soft I2C|
|OLED|1路|soft SPI|
|BOOT|1路|BOOT0|
|GND|2路|电源地接入口|
|+5V|1路|+5V电压接入口|
|+3.3V|1路|+3.3V电压接入口|

## 3.4 指示灯
- Power-LED:+3.3V电源指示灯
- RGB: User 自定义指示灯
- Wifi-LED: Wifi连接指示灯

## 3.5 按键
- WRST:Wifi RESET 恢复出厂设置按键【长按3s】
- SRST:STM32 RESET 复位
- D10:GPIO-PD10 User自定义【OLED下一页】



# 4.软件架构
```
+——RT-Thread
|    └──Kernel 【RT-Thread内核初始化】                  
|    ├── Normal Peripherals Init 【普通外设初始化】
|    ├── System self-check 【系统自检:检测是否所有外设初始化完成】 
|           └── Read Gyroscope data 【读取JY901 九轴数据】 
|           ├── Detection input devices 【检测输入设别】 
|    └──...... 
```



# 5.Underwater Vehicle 的进展

- [X] 基础功能
    - [X] 普通外设初始化 by [@zengwangfa](https://github.com/zengwangfa)
    - [X] 普通外设驱动 by [@zengwangfa](https://github.com/zengwangfa)
        - [X] Add LED/KEY/BUZZER by [@zengwangfa](https://github.com/zengwangfa) 
        - [X] Add OLED/Gyroscope/ADC/PWM/FLASH by [@zengwangfa](https://github.com/zengwangfa) 
    - [X] 内置MSH方法 by [@zengwangfa](https://github.com/zengwangfa)


#### 安装教程

1. Keil5
2. STM32F4XX package
3. RT-Thread

#### 使用说明

- NULL available


#### 参与贡献

1. Fork 本仓库
2. 新建 Feat_xxx 分支
3. 提交代码
4. 新建 Pull Request





