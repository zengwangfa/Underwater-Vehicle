# 航行器控制中心

该航控 基于国产 RT-Thread 实时操作系统，目前完成外设驱动及数据采集，后续为姿态算法 与 抗洋流算法.


```gantt

    title 项目开发流程
    section 项目确定
        需求分析       :a1, 2018-12-22, 3d
        可行性方案     :after a1, 5d
        
    section 项目实施
        核心控制板      :2019-01-05  , 5d
        添加底层硬件驱动      :2019-01-08, 10d
        算法          :2019-01-15, 10d
        测试          :2019-01-22, 5d
    section 发布验收
        发布: 2d
        验收: 3d
```


#### 软件架构
软件架构说明
```
RT-Thread 
├──STM32F407ZGT6                  
│   └──Peripherals
│       ├── RGB [IO]   
│       ├── KEY [IO] 
│       ├── BUZZER [IO]             
│       ├── OLED [Software SPI]       
│       ├── Voltage Detection [ADC]             
│       ├── Wifi [UART]             
│       ├── Bluetooth [UART]       
│       ├── Nine axis gyroscope [UART]
│       ├── USB to serial port [CP2102 UART Bridge]
│       ├── OV2640 Camera [unfinished]
│       └── Be continued     
├── hardware
├── README.md  
├── docs           //documents      
└── Be continued  

```

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




