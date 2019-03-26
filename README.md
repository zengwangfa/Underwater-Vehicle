## 1、ROV航行器控制中心

该航控 基于RT-Thread实时操作系统

| 硬件 | 描述 |
| -- | -- |
|芯片型号| STM32F407ZGT6 |
|CPU| Cortex-M4 |
|主频| 84MHz-168MHz |
|FLASH| 1MB |
|SRAM| 192KB |
|单元| FPU、DSP |

![ROV](https://images.gitee.com/uploads/images/2019/0222/201120_1db060f5_2330851.png "控制中心")

![构建图](https://images.gitee.com/uploads/images/2019/0314/162025_6336b3f6_2330851.png "构建图")

## 2、目录说明
```
+——Underwater_vehicle
|--------+ docs: 【设计参考文档】
|--------+ hardware:【相关电路设计】      
|            └──README.md
|            ├──.SchDoc
			 ├──.PcbDoc
|            └──.pdf
|--------+ software:【相关软件设计】
|            └──README.md
			 ├──rt-thread-master
|               └──bsp
|                   └──stm32f40x
|--------+ README.md
```

## 3、软件结构
```
+——RT-Thread
|    └──Kernel 【RT-Thread内核初始化】                  
|    ├── Normal Peripherals Init 【普通外设初始化】
|    ├── System self-check 【系统自检:检测是否所有外设初始化完成】 
|           └── Read Gyroscope data 【读取JY901 九轴数据】 
|           ├── Detection input devices 【检测输入设备】 
|    └──...... 
```

## 4、硬件结构
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
|        |       ├── OV2640 Camera [DCMI-DMA]
|        |       └──...... 
|        +──Vehicle Devices
|        └── Searchlights [GPIO]
|        ├── Propellers [PWM]
|        ├── Servo Motor for Mechanical Arm [PWM]
|        ├── CAMERAs 
|        └──...... 
```






## 5、Underwater Vehicle 的进展

- [X] 基础功能
    - [X] 普通外设驱动 by [@zengwangfa](https://github.com/zengwangfa)
        - [X] Add LED/KEY/BUZZER by [@zengwangfa](https://github.com/zengwangfa) 
        - [X] Add OLED/Gyroscope/ADC/PWM/FLASH by [@zengwangfa](https://github.com/zengwangfa) 
    - [X] 内置MSH方法 by [@zengwangfa](https://github.com/zengwangfa)
    - [X] PID调试方法 by [@zengwangfa](https://github.com/zengwangfa)


	
	
#### 使用说明

- 暂无添加

#### 参与贡献

- Fork 本仓库
- 新建 Feat_xxx 分支
- 提交代码
- 新建 Pull Request





