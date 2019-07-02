- server.py 
    - 为以树莓派作为服务器，树莓派实现接收上位机的控制数据包通过USB转发给STM32
    - 并将STM32采集到的数据【CPU温度、水深、水温、航向角、俯仰角、横滚角、航速、提示字符等】回传给上位机
	
- autostart文件夹为 放置在树莓派 /home/pi/.config 文件夹下 **实现开机自启动**【树莓派服务器】以及【MJPG视频推流】
	- Server.desktop :【树莓派服务器】
    - MJPG.desktop :【MJPG视频推流】
