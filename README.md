# ROV/AUV航行器控制中心

<p align="center">

[个人主页🕺](https://zengwangfa.top/) | 
[软件说明💻](./software/README.md) |
[硬件说明📚](./hardware/README.md) |
[机械说明🎮](./graphic_model/README.md)
</p>

---

<p align="center">
  <a href="https://zh.wikipedia.org/wiki/C%E8%AF%AD%E8%A8%80"><img src="https://img.shields.io/badge/language-C-brigreen.svg?style=flat-square" alt="C"></a>
  <a href="https://zh.wikipedia.org/wiki/Python"><img src="https://img.shields.io/badge/language-Python-ff69b4.svg?style=flat-square" alt="Python"></a>
  <a href="https://img.shields.io"><img src="https://img.shields.io/github/repo-size/zengwangfa/Underwater-Vehicle?style=flat-square" alt="Size"></a>
  <a href="https://zh.wikipedia.org/wiki/%E6%A0%91%E8%8E%93%E6%B4%BE"><img src="https://img.shields.io/badge/-Raspberry%20Pi-pink.svg?style=flat-square" alt="Python"></a>
</p>

## 1、简介



| 硬件 | 主控制器(STM32) | 视频与指令服务器(Raspberry Pi) | 
| :--: | :--: | :--: |
|芯片型号| [STM32F407ZGT6](/docs/DataSheet/STM32F407ZGT6.pdf) | [BCM2837B0](/docs/DataSheet/BCM2835-ARM-Peripherals.pdf) |
|CPU| 32位 Cortex-M4(ARMv7) | 64位 Cortex-A53(ARMv8) |
|主频| 168MHz | 1.4GHz |
|RAM| 192KB | 1GB |
|外存| 1MB(FLASH) | SD Card(MAX:128GB) |
|单元| FPU、DSP | 千兆以太网 |

- 系统结构简图：
![系统结构简图](/docs/pictures/Structure.jpg "系统结构简图")


## 2、目录说明
```
+——Underwater_vehicle
|---- docs: 【设计文档】
|---- hardware:【电路设计】      
|---+ software:【软件设计】
       ├──README.md
       ├──rt-thread-master
            └──bsp
               └──stm32f407【控制程序】
|---- graphic_model:【机械模型】
|---- raspberry_server:【树莓派服务】
|---- tools:【调试工具】
|---- README.md
```

## 3、机械结构
- 3维模型
![ROV 3D](/docs/pictures/ROVMasetr_Model.png "ROVMaser Model")

- 实物图
![Entity](/docs/pictures/Entity.jpg "ROV Entity")

## 4、软件结构
![Connect Flow](/docs/pictures/Connect_Flow.jpg "Connect Flow")

- Computer Connect![language](https://img.shields.io/badge/language-C%23-blue.svg)
- Raspberry Pi    ![language](https://img.shields.io/badge/language-Python-ff69b4.svg)
- MCU Controller  ![language](https://img.shields.io/badge/language-C-brightgreen.svg)


## 5、硬件结构
![Controller PCB 3D](/docs/pictures/Controller_3D.jpg "Controller 3D")

- 核心板(Core Controller)拥有外设：

| 外设名称 | RGB LED | Key | Buzzer | Dial Switch | MS5837 | OLED | W25Q128 | Voltage Detection | Current Detection | Zigbee |  JY901 | USR-C216 | CP2102 | OV2640 Camera | Servo Motor | Searchlights | Perpellers |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 控制(通讯)方式 | GPIO | GPIO | GPIO | GPIO | I2C | SPI | SPI | ADC | ADC | USART | USART | USART  | USART | DCMI-DMA | PWM | PWM | PWM |
| 功能描述 | 指示灯 | 按键 | 蜂鸣器 | 拨码开关 | 深度传感器 | 显示屏 | 闪存芯片 | 电压检测 | 电流检测 | 2.4G无线通信 | 九轴 | WiFi模块 | 串口转USB | 摄像头 | 舵机  | 探照灯 | ESC |

![Hardware PCB 3D](/docs/pictures/hardware.jpg "Hardware PCB 3D")

---

- 树莓派拥有有外设：

| 外设名称 | Camera 1 | Camera 2 | Core Controller | PLC / Optical-to-electrical Transducer |
| :---: | :---: | :---: | :---: | :---: |
| 控制(通讯)方式 | USB | USB | USB | Ethernet |
| 功能描述 | 外置光学变焦摄像头 | 内置云台摄像头 | 核心控制器 | 电力载波模块/光电转换器 | 





## 6、Underwater Vehicle 的进展
- [X] 电路设计
    - [X] Core Controller核心控制板 by [@Ian](https://github.com/zengwangfa)	
	- [X] Underwater_Vehicle主板 by [@Ian](https://github.com/zengwangfa)	
	- [X] 电源主板设计 by [@Ian](https://github.com/zengwangfa)
	
- [X] 控制程序
    - [X] 外设驱动 by [@Ian](https://github.com/zengwangfa)
	- [X] 控制简易框架 by [@Ian](https://github.com/zengwangfa)
	- [X] 通信控制 by [@Ian](https://github.com/zengwangfa)
	- [X] 定向控制算法 by [@Okurarisona](https://github.com/Okurarisona)🆕
- [X] 上位机
	- [X] C#上位机 by [@陈信任] 🆕

- [X] 机械结构
	- [X] 框架式结构CAD设计 by [@JackyLin](https://github.com/JackyLin1205) 🆕
	- [X] 电子仓、电源仓设计 by [@JackyLin](https://github.com/JackyLin1205)

- [X] 树莓派
	- [X] 数据Server服务器 by [@FatihBo](https://github.com/FatihBo)


## 7、使用说明

- propeller_power_set [parameter]
> 设置推进器动力 <0~300%>
- set_depth_sensor_type [parameter]
> 修改深度传感器 类型<ms5737/spl1301/null>

#### 参与贡献

- Fork 本仓库
- 新建 Feat_xxx 分支
- 提交代码
- 新建 Pull Request
- [点击联系我](Mailto:zengwangfa@outlook.com)




