# 1.航行器控制中心

该航控 基于国产 RT-Thread 实时操作系统，目前完成外设驱动及数据采集，后续为姿态算法 与 抗洋流算法.


# 2.目录说明
```
+——Underwater_vehicle
|--------+ docs 
|--------+ hardware       
|            └──.SchDoc
|            ├──.PcbDoc
|            └──.pdf
|--------+ sofrware
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
|OLED|1路|soft SPI|
|气压计|1路|soft I2C|
|SWD下载接口|1路|SWD|
|Bluetooth|1路|UART4|
|BOOT|1路|BOOT0|
|POWER IN|1路|电源电压接入口|
|+5V|1路|+5V电压接入口|
|+3.3V|1路|+3.3V电压接入口|
|GND|2路|电源地接入口|


## 3.4 指示灯
- Power-LED:+3.3V电源指示灯
- RGB: User 自定义指示灯
- Wifi-LED: Wifi连接指示灯





# 4.软件

#### 安装教程

1. KEIL5
2. STM32F4XX package
3. RT-Thread

#### 使用说明

1. NULL available
2. NULL available
3. NULL available

#### 参与贡献

1. Fork 本仓库
2. 新建 Feat_xxx 分支
3. 提交代码
4. 新建 Pull Request



