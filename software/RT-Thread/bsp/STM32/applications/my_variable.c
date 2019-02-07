#include "my_init.h"

//---------------User Variable 用户变量--------------------//


fx_date fx_data1={0};
bool   HEAT_START_FLAG=FALSE;          //开始控制加热丝加热标志位
bool   WASHING_START_FLAG=FALSE;       //开始清洗蜂箱标志位
bool   FEED_START_FLAG=FALSE;          //开始喂食蜂箱标志位

char *FW_status_string[4]={"ZhongWang","WangTai","ShiWang","ChuNuWang"}; //蜂王状态名 字符串

rt_uint32_t Count = 0;

rt_uint8_t FX_NUMBER = 0;
rt_uint8_t FW_status_flag;  //写入FLASH的参数  初始化不赋初值，直接读取FLASH数值
rt_uint8_t PiShu;

rt_int32_t Weight[3]={0};
rt_uint16_t voltage[3] ={0};
rt_uint16_t Temperature[3] = {0};

rt_uint32_t Flow_Count = 0;



rt_uint8_t buzzer_time=0;//蜂鸣器
rt_uint8_t buzzer_count=0;//蜂鸣器
rt_uint8_t buzzer_number=0;//蜂鸣器


rt_uint8_t TEXT_Buffer[2]={0x04};    //第一个字节为蜂箱的开口状态，0x04表示开口最大，0x02居中，0x01最小

rt_uint8_t DataBuffer[8];   //蜂箱数据暂存数组，包括2字节的重量+2字节的饲料重量+2字节的温度+1字节的(清洗、加热、喂食)标志位+1字节的蜂王状态
rt_uint8_t send_buffer[(MAX_DATA_LENS)]; //串口1发送数据缓冲变量,
rt_uint8_t recv_buffer[MAX_DATA_LENS]; //WIFI 串口2接收数据缓冲变量,
rt_uint8_t recv_data_p=0x00;              //WIFI   串口2接收数据指针
rt_uint8_t recv_data_ok=0;                //接收到正确的数据包
rt_uint8_t sum_check=0;

rt_uint16_t WASHING_count;                             //定时器清洗，计数值
rt_uint16_t HEAT_count;                                //定时加热，计数值
rt_uint16_t FEED_count;                                //定时喂食，计数值

rt_uint16_t Washing_Time=0;                 //清洗时间，由上位机给定
rt_uint16_t Heat_Time=0;                    //加热时间，由上位机给定，单位秒
rt_uint16_t Feed_Time=0;                    //喂食时间，由上位机给定，单位秒



rt_uint8_t Init_flag=0;
rt_uint8_t first_ON_OFF = 0;
rt_uint16_t Test_count = 0;

rt_int32_t Init_Value[2]={0};
rt_int32_t AD_Value[2]={0};
rt_int32_t Last_AD_Value[2]={0};
rt_int32_t side_weight[2]={0};


rt_int32_t temp[2]={0};
rt_int32_t var_max[2]={0};    //8632784        //比这个值大 记为归一化最大值
rt_int32_t var_min[2]={99999999,99999999};  //比这个值小 记为归一化最小值  -2147483648～+2141483647
rt_int32_t var0[10]={0};
rt_int32_t var1[10]={0};


rt_uint8_t set1kg_flag = 0;
float Normalized[2]={0};













