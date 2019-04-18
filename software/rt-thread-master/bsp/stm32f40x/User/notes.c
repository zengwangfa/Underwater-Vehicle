/*********************************************************************************
*      notes.c    调试笔记
*                                     	  0.log_a[A]：断言(Assert)        
* File          : notes.c             	  1.log_e[E]：错误(Error)
* Version       : V1.0          		  	  2.log_w[W]：警告(Warn)
* Author        : zengwangfa				   		3.log_i[I]：信息(Info)
																					4.log_d[D]：调试(Debug)
* History       :													5.log_v[V]：详细(Verbose)
* Date          : 2019.01.25							调试log【优先级】按序号
*******************************************************************************/

/*

程序笔记：
  1.①当拨码值=1（即拨码都拨下）为正常模式
正常模式：
				开启摄像头
				开启LED系统运行 闪烁指示灯（绿LED 约0.5s闪烁）
				开启开机 蜂鸣器响三声指示

    ②当拨码值!=1 为 调试模式
调试模式：
				关闭正常模式下的几个属性
				
------------------------------------------------------		

	2.FLASH读取有 普通参数与PID参数
	





*/

/*

Notes:

重点：编译环境需为c99，旧版本Keil5系统默认为c89，c89编制只能将定义放在函数内最前面

如何添加c99环境：
				Options -> C/C++ -> Misc Controls ->里面输入【--c99】 ->保存即可



1.JY901 方位角数据漂移

①:使用前需要校准
②校准时不能受周围磁场干扰
使用常见问题指南:
		http://wiki.wit-motion.com/doku.php?id=wt901常见问题



2.RT-Thread如何输出浮点型：

sample:
		//数据打包成string型       因为RT-Thread rt_kprintf()函数无法输出浮点型，因此现将数据打包成String型发出.
		char str[100];
		sprintf(str,"Time:20%d-%d-%d %d:%d:%.3f\r\n",stcTime.ucYear,stcTime.ucMonth,stcTime.ucDay,stcTime.ucHour,stcTime.ucMinute,(float)stcTime.ucSecond+(float)stcTime.usMiliSecond/1000);
		rt_kprintf(str);




3.W25Q128 ID读取不正确：

SPI初始化配置时:
		< SPI_BaudRatePrescaler_64 >  -> 定义波特率预分频的值  不能太大也不能太小.



4.内存占用记录：

Code是代码占用的空间;
RO-data是 Read Only 只读常量的大小，如const型;
RW-data是（Read Write） 初始化了的可读写变量的大小;
ZI-data是（Zero Initialize） 没有初始化的可读写变量的大小。ZI-data不会被算做代码里因为不会被初始化;


Program Size: Code=157602 RO-data=27178 RW-data=1684 ZI-data=105348    【2019.3.4】


【2019.3.9】
    Total RO  Size (Code + RO Data)               190236 ( 185.78kB)
    Total RW  Size (RW Data + ZI Data)            108336 ( 105.80kB)
    Total ROM Size (Code + RO Data + RW Data)     190652 ( 186.18kB)

【2019.3.23】
		Total RO  Size (Code + RO Data)               194360 ( 189.80kB)
		Total RW  Size (RW Data + ZI Data)            108392 ( 105.85kB)
		Total ROM Size (Code + RO Data + RW Data)     194788 ( 190.22kB)
	
【2019.4.1】	
		Total RO  Size (Code + RO Data)               199912 ( 195.23kB)
    Total RW  Size (RW Data + ZI Data)            108880 ( 106.33kB)
    Total ROM Size (Code + RO Data + RW Data)     200360 ( 195.66kB)
		
【2019.4.3】
    Total RO  Size (Code + RO Data)               201512 ( 196.79kB)
    Total RW  Size (RW Data + ZI Data)            109024 ( 106.47kB)
    Total ROM Size (Code + RO Data + RW Data)     201964 ( 197.23kB)
		
【2019.4.18】		
    Total RO  Size (Code + RO Data)               168260 ( 164.32kB)
    Total RW  Size (RW Data + ZI Data)             26048 (  25.44kB)
    Total ROM Size (Code + RO Data + RW Data)     168664 ( 164.71kB)
		
		
		
		
		
*/

