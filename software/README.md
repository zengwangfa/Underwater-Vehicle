[软件说明](./../software/README.md)|
[硬件说明](./../hardware/README.md)| 
[机械结构说明](./graphic_model/README.md)


- 本项目用用到了两个实用的软件包：
	- [EasyFlash](https://github.com/armink/EasyFlash)一款开源的轻量级嵌入式Flash存储器库
	- [EasyLogger](https://github.com/armink/EasyLogger)一款超轻量级、高性能的 C/C++ 日志库


# 1.目录说明

**software:【相关软件设计】**
```c
   └──Camera 【OV2640例程】
   ├──RT-Thread 【RTT示例】
   ├──rt-thread-master
        └──bsp
            └──stm32f40x 【控制中心程序】

```			

| 目录组 | 描述 |
| -- | -- |
|Kernel| 它用于存放RT-Thread内核源文件 |
|User| 它用于存放用户应用文件 |
|Applications| 它用于存放外设应用程序 |
|Drivers|  它用于存放相关外设驱动 |
|STM32_StdPeriph| 它用于存放STM32固件库文件 |
|cpu|  它用于存放内核相关文件 |
|Fliesystem| 它用于存放虚拟文件系统 |
|DeviceDrivers|  它用于存放RT-Thread驱动 |
|finsh| 它用于存放RT-Thread Finsh组件 |
|libc| 它用于存放RT-Thread 相关文件 |
|Easylogger| 它用于存放RT-Thread Easylogger组件 |
|EasyFlash| 它用于存放RT-Thread EasyFlash |
|Utilities| 它用于存放相关工具与滤波算法 |


# 2.软件结构
```
+——RT-Thread
|    └──Kernel 【RT-Thread内核初始化】                  
|    ├── Normal Peripherals Init 【普通外设初始化】
|    ├── System self-check 【系统自检:检测是否所有外设初始化完成】 
|           └── Read Gyroscope data 【读取JY901 九轴数据】 
|           ├── Detection input devices 【检测输入设别】 
|           ├── Attitude Control 【姿态控制】 	
|    └──...... 
```







