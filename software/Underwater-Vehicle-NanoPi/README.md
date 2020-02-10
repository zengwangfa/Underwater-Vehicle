# [Underwater-Vehicle-NanoPi](https://github.com/chenxiqiyuan/Underwater-Vehicle-NanoPi.git)

# 1. 介绍

本项目基于 [Underwater-Vehicle](https://github.com/zengwangfa/Underwater-Vehicle) 修改，使用NanoPi NEO Core进行开发。

- 本项目用到了以下软件包：
  - [WiringNP](https://github.com/chenxiqiyuan/WiringNP.git) This is a GPIO access library for NanoPi. It is based on the WiringOP for Orange PI which is based on original WiringPi for Raspberry Pi.
  - [EasyLogger](https://github.com/armink/EasyLogger) 是一款超轻量级(ROM<1.6K, RAM<0.3K)、高性能的 C/C++ 日志库，非常适合对资源敏感的软件项目。

# 2. 使用

## 2.1 动态库设置

```shell
sudo cp easylogger/libeasylogger.so /usr/lib/
```

## 2.2 编译与执行

```shell
make && sudo ./NanoPiMain
```

### 2.3 注意事项

有时修改会后编译执行会出问题，未出现修改部分效果，此时`make clean` 后再编译运行即可。

# 3. 进度

- [ ] 模块进度
  - [x] ADC
    - [x] 电压检测
    - [x] 电流检测
  - [x] RGB LED
  - [ ] 探照灯
  - [ ] Key
  - [x] Buzzer
  - [ ] 拨码开关
  - [x] 深度传感器
  - [x] 显示屏
  - [x] 九轴
  - [ ] PWM
    - [x] 电压
    - [ ] 频率
  - [x] CPU状况
    - [x] CPU温度
    - [x] CPU使用率
- [ ] 整体进度
  - [x] 传感器整合
  - [ ] 系统自检
  - [ ] 通信协议