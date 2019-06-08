## 1、ROV航行器控制中心

该航控 基于RT-Thread实时操作系统

| 硬件 | 描述 |
| -- | -- |
|芯片型号| STM32F407ZGT6 |
|CPU| Cortex-M4 |
|主频| 168MHz |
|FLASH| 1MB |
|SRAM| 192KB |
|单元| FPU、DSP |


- ![构建图](https://images.gitee.com/uploads/images/2019/0314/162025_6336b3f6_2330851.png "构建图")

- ![ROV 3D](https://images.gitee.com/uploads/images/2019/0527/140412_e23ebd0f_2330851.jpeg "ROV_3D Model.jpg")

- ![ROV Controler 3D](https://images.gitee.com/uploads/images/2019/0527/140434_ed5fdc50_2330851.jpeg "ROV_Controler_3D Model.jpg")


## 2、目录说明
```
+——Underwater_vehicle
|--------+ docs: 【设计参考文档】
|--------+ hardware:【相关电路设计】      
|            ├──README.md
|            ├──.SchDoc
|            ├──.PcbDoc
|            └──.pdf
|--------+ software:【相关软件设计】
|            ├──README.md
|			 └──rt-thread-master
|               └──bsp
|                   └──stm32f40x 【主要系统控制程序】
|--------+ README.md
```

## 3、软件结构
```
+——RT-Thread
|    ├── Kernel 【RT-Thread内核初始化】                  
|    ├── Normal Peripherals Init 【普通外设初始化】
|    └── System self-check 【系统自检:检测是否所有外设初始化完成】 
|           ├── Read Gyroscope data 【读取JY901 九轴数据】 
|           ├── Detection input devices 【检测输入设备】 
|           ├── ....
|           └── ....
|    └──  Computer Connect【C#】--Ethernet - Optical Fiber -- Ethernet--> Raspberry Pi【Python】 ---> MCU Control 【C】
```

## 4、硬件结构
```
+——Underwater_vehicle
|--------+──DC 24V/48V 【Power management】 
|--------+──STM32F407ZGT6 【Control-Center】                  
|        │       └──【Normal Peripherals】
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
|        │       └── 【Important Peripherals】
|        │       ├── Nine axis gyroscope [UART]
|        │       ├── USR-C216 Wifi [UART]   
|        │       ├── OV2640 Camera [DCMI-DMA]
|        │       └──...... 
|        +──Vehicle Devices
|        └── Searchlights [GPIO]
|        ├── Propellers [PWM]
|        ├── Servo Motor for Mechanical Arm [PWM]
|        ├── CAMERAs 
|        └──...... 
```






## 5、Underwater Vehicle 的进展
- [X] 电路设计
	- [X] Underwater_Vehicle核心控制板 by [@zengwangfa](https://github.com/zengwangfa)	
	- [X] Underwater_Vehicle底板 by [@zengwangfa](https://github.com/zengwangfa)	
	- [X] 变焦摄像头驱动控制板 by [@张静]	
	- [X] 电源主板设计 by [@李浩辰]	
	
- [X] 控制程序
    - [X] 普通外设驱动 by [@zengwangfa](https://github.com/zengwangfa)
        - [X] Add LED/KEY/BUZZER by [@zengwangfa](https://github.com/zengwangfa) 
        - [X] Add OLED/Gyroscope/ADC/PWM/FLASH by [@zengwangfa](https://github.com/zengwangfa) 
    - [X] 内置MSH方法 by [@zengwangfa](https://github.com/zengwangfa)
    - [X] debug上位机、PID调试方法 by [@zengwangfa](https://github.com/zengwangfa)
	- [X] PID定深控制 by [@zengwangfa](https://github.com/zengwangfa)
	- [X] 通信控制 by [@zengwangfa](https://github.com/zengwangfa)

- [X] 上位机
	- [X] C#上位机界面 by [@陈信任]
	- [X] 遥控器通信控制 by [@陈信任]	
	
- [X] 机械结构
	- [X] 框架式结构CAD设计 by [@林麟亮]
	- [X] 电子仓、电源仓设计 by [@林麟亮]
	- [X] 云台3D打印模型 by [@洪华霖]
	
- [X] 其它





#### 使用说明

- 暂无添加

#### 参与贡献

- Fork 本仓库
- 新建 Feat_xxx 分支
- 提交代码
- 新建 Pull Request





