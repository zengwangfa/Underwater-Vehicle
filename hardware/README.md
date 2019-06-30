[软件说明](./../software/README.md)|
[硬件说明](./../hardware/README.md)| 
[机械结构说明](./graphic_model/README.md)


# 1.航行器控制中心

| 硬件 | 描述 |
| -- | -- |
|芯片型号| STM32F407ZGT6 |
|CPU| Cortex-M4 |
|主频| 84MHz-168MHz |
|FLASH| 1MB |
|SRAM| 192KB |
|单元| FPU、DSP |


![输入图片说明](https://images.gitee.com/uploads/images/2019/0630/203052_a928c81e_2330851.jpeg "Controller 3D.jpg")

# 2.硬件架构

- 拥有外设：
| 外设名称 | RGB LED | Key | Buzzer | Dial Switch | MS5837 | OLED | W25Q128 | Voltage Detection | Current Detection | Zigbee |  JY901 | USR-C216 | CP2102 | OV2640 Camera | Servo Motor | Searchlights | Perpellers |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |  --- | --- |  --- |  --- | --- |
| 控制方式 | GPIO | GPIO | GPIO | GPIO | I2C | SPI | SPI | ADC | ADC | USART | USART | USART  | USART | DCMI-DMA | PWM | PWM | PWM |
| 功能描述 | 指示灯 | 按键 | 蜂鸣器 | 拨码开关 | 深度传感器 | 显示屏 | 闪存芯片 | 电压检测 | 电流检测 | 2.4G无线通信 | 九轴 | WiFi模块 | 串口转USB | 摄像头 | 舵机  | 探照灯 | 推进器 |

- 指示灯：显示系统工作状态[绿色闪烁为正常、红色常亮为异常]
- 按键：目前为切换OLED界面
- 蜂鸣器：报警提示作用
- 拨码开关：切换状态
- 深度传感器：采集水压、水温,并转换为深度值
- OLED显示屏：显示相关数据及状态
- 闪存芯片：存储相关变量及参数
- 电压检测：检测电源电压
- 电流检测：检测总电流大小,用于过流保护
- 2.4G通信模块：用于与操作系统交互
- 九轴：采集加速度、角速度、磁场、角度[偏航角Yaw、横滚角Roll、俯仰角Pitch]
- WiFi模块：用于上位机数据示波器显示及调参
- 串口转USB：用于与**树莓派**通信
- 摄像头：STM32自带摄像头,可用于扩展
- 舵机：分为云台舵机、机械手舵机
- 探照灯：用于水下照明
- 推进器：ROV/AUV动力来源




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
![输入图片说明](https://images.gitee.com/uploads/images/2019/0630/202225_428ce6a4_2330851.jpeg "Controller原理图.jpg")
![输入图片说明](https://images.gitee.com/uploads/images/2019/0630/203041_d9d9f3a8_2330851.jpeg "Controller PCB.jpg")
------
### 2.1 电源及 USB
核心板可以通过 Micro-USB 或 5V 电源供电。使用 Micro-USB 接到电脑时，电脑弹出串口CP210x USB to UART Bridge。连接该串口后，可以通过 shell 命令行工具进行交互，也可以查看实时的日志信息。

### 2.2 功能接口
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

### 2.3 其他接口

|接口|数量|备注|
|:-:|:-:|:-:|
|POWER IN|1路|电源电压接入口|
|Bluetooth|1路|UART4|
|SWD调试接口|1路|SWD|
|气压计|1路|I2C|
|OLED|1路| SPI|
|BOOT|1路|BOOT0|
|GND|2路|电源地接入口|
|+5V|1路|+5V电压接入口|
|+3.3V|1路|+3.3V电压接入口|

### 2.4 指示灯
- Power-LED:+3.3V电源指示灯
- RGB: User 自定义指示灯
- Wifi-LED: Wifi连接指示灯

### 2.5 按键
- WRST:Wifi RESET 恢复出厂设置按键【长按3s】
- SRST:STM32 RESET 复位
- D10:GPIO-PD10 自定义【OLED下一页】







