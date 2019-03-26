# 1.航行器控制中心

| 硬件 | 描述 |
| -- | -- |
|芯片型号| STM32F407ZGT6 |
|CPU| Cortex-M4 |
|主频| 84MHz-168MHz |
|FLASH| 1MB |
|SRAM| 192KB |
|单元| FPU、DSP |

![Underwater Vehicle](https://images.gitee.com/uploads/images/2019/0222/201120_1db060f5_2330851.png "控制中心")

![ROV](https://images.gitee.com/uploads/images/2019/0314/160752_50b3c61c_2330851.jpeg "ROV")

# 2.硬件架构
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
|气压计|1路|soft I2C|
|OLED|1路|soft SPI|
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
- D10:GPIO-PD10 User自定义【OLED下一页】







