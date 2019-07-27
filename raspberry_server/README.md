- server.py 
    - 为以树莓派作为服务器，树莓派实现接收上位机的控制数据包通过USB转发给STM32
    - 并将STM32采集到的数据【CPU温度、水深、水温、航向角、俯仰角、横滚角、航速、提示字符等】回传给上位机
	
	
	
# 自启动方式
- 一、系统脚本式自启动(推荐)
- 二、桌面式自启动


## 一、系统脚本式自启动

- 1.将/etc/rc.local中的rc.local替换掉 
- 2.runserver.sh 和 server.py 放 /home/pi 下

- 3.shell输入：
`
chmod +x runserver.sh
`
使脚本生效（可执行）
	
## 二、桌面式启动器	
- 1.server.py 放 /home/pi 下
- 2.autostart文件夹为 放置在树莓派 /home/pi/.config 文件夹下 **实现开机自启动**【树莓派服务器】以及【MJPG视频推流】
	- Server.desktop :【树莓派服务器】
    - MJPG0.desktop :【MJPG视频0推流】
	- MJPG1.desktop :【MJPG视频1推流】
> 只有当桌面打开,该服务程序才会打开