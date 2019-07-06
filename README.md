# 1、ROV/AUV航行器控制中心

[软件说明](./software/README.md) |
[硬件说明](./hardware/README.md) |
[机械结构说明](./graphic_model/README.md)

## 该航控 基于国产RT-Thread实时操作系统
[![Badge](https://img.shields.io/badge/link-996.icu-%23FF4D5B.svg)](https://996.icu/#/en_US)
[![LICENSE](https://img.shields.io/badge/license-Anti%20996-blue.svg)](https://github.com/996icu/996.ICU/blob/master/LICENSE)
[![Slack](https://img.shields.io/badge/slack-996icu-green.svg)](https://join.slack.com/t/996icu/shared_invite/enQtNTg4MjA3MzA1MzgxLWQyYzM5M2IyZmIyMTVjMzU5NTE5MGI5Y2Y2YjgwMmJiMWMxMWMzNGU3NDJmOTdhNmRlYjJlNjk5ZWZhNWIwZGM)

---

| 硬件 | 描述 |
| -- | -- |
|芯片型号| STM32F407ZGT6 |
|CPU| Cortex-M4 |
|主频| 168MHz |
|FLASH| 1MB |
|SRAM| 192KB |
|单元| FPU、DSP |

- 系统架构简图：
![构建图](/docs/pictures/构建图.jpg "构建图")




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
|            ├──rt-thread-master
|                 └──bsp
|                    └──stm32f40x 【主要系统控制程序】
|--------+ README.md
```

## 3、机械结构
![ROV 3D](/docs/pictures/ROV-Model.jpg "ROV-Model.jpg")

<table>
    <tr>
        <td ><center><img src="/docs/pictures/ROV-Model.jpg" >  3维模型 </center></td>
        <td ><center><img src="/docs/pictures/ROV-Model.jpg"  > 实物图 </center></td>
    </tr>
</table>

## 4、软件结构

```mermaid
graph TD
A[RT-Thread系统初始化] --> B(外设初始化)
B --> C{是否初始化成功}
C -->|YES| D[采集各个传感器数据并进行数据处理]
C -->|YES| E[树莓派通信 上行与下行]
C -->|NO| F[软件异常中断 卡死 亮红灯]
```


- Computer Connect【C#】--Ethernet - Optical Fiber -- Ethernet--> Raspberry Pi【Python】 ---> MCU Control 【C】


## 5、硬件结构
![Controller PCB 3D](/docs/pictures/Controller_3D.jpg "Controller 3D.jpg")
- 拥有外设：

| 外设名称 | RGB LED | Key | Buzzer | Dial Switch | MS5837 | OLED | W25Q128 | Voltage Detection | Current Detection | Zigbee |  JY901 | USR-C216 | CP2102 | OV2640 Camera | Servo Motor | Searchlights | Perpellers |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |  --- | --- |  --- |  --- | --- |
| 控制方式 | GPIO | GPIO | GPIO | GPIO | I2C | SPI | SPI | ADC | ADC | USART | USART | USART  | USART | DCMI-DMA | PWM | PWM | PWM |
| 功能描述 | 指示灯 | 按键 | 蜂鸣器 | 拨码开关 | 深度传感器 | 显示屏 | 闪存芯片 | 电压检测 | 电流检测 | 2.4G无线通信 | 九轴 | WiFi模块 | 串口转USB | 摄像头 | 舵机  | 探照灯 | 推进器 |





## 6、Underwater Vehicle 的进展
- [X] 电路设计
	- [X] Underwater_Vehicle核心控制板 by [@zengwangfa](https://github.com/zengwangfa)	
	- [X] Underwater_Vehicle底板 by [@zengwangfa](https://github.com/zengwangfa)	
	- [X] 变焦摄像头驱动控制板 by [@张静](https://gitee.com/xiaojing_1)	
	- [X] 电源主板设计 by [@李浩丞]	🆕
	
- [X] 控制程序
    - [X] 外设驱动 by [@zengwangfa](https://github.com/zengwangfa)
    - [X] 内置MSH方法 by [@zengwangfa](https://github.com/zengwangfa)
    - [X] debug上位机:(Vcan【山外】/Ano【匿名】) by [@zengwangfa](https://github.com/zengwangfa)
	- [X] 控制简易框架 by [@zengwangfa](https://github.com/zengwangfa)
	- [X] 通信控制 by [@zengwangfa](https://github.com/zengwangfa)🆕

- [X] 上位机
	- [X] C#上位机界面 by [@陈信任] 🆕
	- [X] 遥控器通信控制 by [@陈信任]	
	
- [X] 机械结构
	- [X] 框架式结构CAD设计 by [@林麟亮] 🆕
	- [X] 电子仓、电源仓设计 by [@林麟亮]
	- [X] 云台3D打印模型 by [@洪华霖]

- [X] 树莓派
	- [X] 数据Server服务器 by [@FatihBo](https://github.com/FatihBo)🆕





#### 使用说明

- 暂无添加

#### 参与贡献

- Fork 本仓库
- 新建 Feat_xxx 分支
- 提交代码
- 新建 Pull Request





