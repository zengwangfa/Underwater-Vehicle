#include "include.h"

extern uint8 imgbuff[CAMERA_SIZE];
extern uint8 img[CAMERA_H][CAMERA_W];
extern uint8 edge[60][2];
extern uint8 center_line[60];
extern int16 camera_center;
extern int16 camera_last_center; 
extern CAR_STATUS_e car_mode;
extern int16 camera_integral_center;
extern uint8 Cam2Mag_Weight;

extern ring_missage ring_info;
extern  ROAD_STATUS_e road_flag;
extern uint8 edge_sum_left;
extern uint8 edge_sum_right;
extern uint16 edge_black[2];
extern uint16 edge_Int_R;
extern uint16 edge_Int_L;
extern int16 deviation;

extern uint8 camera_integral_flag;

extern int8 edge_der1[2][40];
extern int8 edge_der2[2][40];
extern uint8 edge_start[80][2];

extern uint8 Circle_fill_line_flag;
extern uint8 circle_point[2];//圆环切入标志角纵坐标0,,横坐标1
extern uint8 circle_edge_start[4];//圆环切入标志角纵坐标0,2,横坐标1,3
extern uint8 Line_sum[2];X
extern uint8 straight_go_flag;

extern int16 Get_SpeedL; //编码器采集到的速度
extern int16 Get_SpeedR;
extern uint16 normal_speed;

extern uint8 A_Car;

extern uint16 white_num;
extern uint8 white_proportion;   //白点占比
extern uint8 ramp_flag;
extern int16 ramp_count;
extern uint8 side_high[160];//0-60
extern int16 var_camera_center;
extern int16 var_last_camera_center;


extern uint16 crossl_f;
extern uint16 crossr_f;
extern uint16 crossl_b;
extern uint16 crossr_b;

extern uint16 var_left;
extern uint16 var_right;
extern uint16 var_middle;
/**************************/
extern uint16 variance_center;

void Camera_center()//中线偏差
{
	static float camera_line_w=0;	
	static int16 var_camera[5];
	camera_last_center=camera_center;
	var_last_camera_center=var_camera_center;

	camera_center=0;   
	for (uint8 i=11;i<50;i++)//中线取求和作为偏差，忽略前n行
	{
		if(i<20) camera_line_w=2;
		if(20<=i&&i<30) camera_line_w=3;//2
		if(30<=i&&i<40) camera_line_w=2;//2.5
		if(40<=i&&i<50) camera_line_w=0;//3                  
		
		camera_center+=camera_line_w*(center_line[i]-79)/20;
	}
	
	var_camera[0]=var_camera[1];
	var_camera[1]=var_camera[2];
	var_camera[2]=var_camera[3];
	var_camera[3]=var_camera[4];
	var_camera[4]=camera_center;
	
	var_camera_center=(var_camera[0]+var_camera[1]+var_camera[2]+var_camera[3]+var_camera[4])/5;
	
	// camera_center=(camera_last_center*3+camera_center*7)/10;//低通滤波   
	dma_count();
	
}

/*******************************************
* 函 数 名：get_white_proportion
* 功    能：得到白点占总图像的比列（proportion）
* 输入参数：none
* 返 回 值：none
* 目    的：判断坡道 
********************************************/
void get_white_proportion()
{ 
	uint8 i;
	uint8 j;
        static uint8 ramp_count=0;
	white_num=0;
	for(i=11;i<60;i++)
	{
		for(j=0;j<160;j++)//  50*160共8000像素
		{
			if(img[i][j]==0)
			{
				white_num++;
			}
		}
	}
	white_proportion=white_num/80;
	if(white_proportion>=90 && var_middle>=120) //占比例大于90%   中间电感变大 判定坡道
	{
          ramp_flag=1;
          ramp_count=0;
	}
        if(ramp_flag==1)
        {
          ramp_count++;
        }
        if(ramp_flag==1 && ramp_count>=25)  //延时解除坡道
        {
          ramp_count=0;
          ramp_flag=0;
        }	
}


void Circle_deal_last()   //圆环积分//环内开始判断//转弯检测      
{
  static uint8 count;
  static uint8 n,p;
  int16 i,j;
  
  uint8 side_line[60]={0};//0-160

  
  uint8 search_s=0; //0-160
  uint8 search_h=0;//0-60
  uint8 search_w=0;//0-160
  uint8 search_o=0;//0-160
  
  if(ring_info.ring_flag==near_ring) 
  { 
    count=0;
    if(edge_black[0]<=6||edge_black[1]<=6)
    {camera_integral_flag++;}
    
    if(camera_integral_flag==2)//略过前n次判断消抖
    {	    
      //buzzer_bibi(2,1);     
      camera_integral_center=edge_sum_left-edge_sum_right;//边沿黑块相减作方向
    }
    
  }
  else   {camera_integral_flag=0;}
  
  if(ring_info.ring_flag==reach_ring_point)
  {
    
    for(j=0;j<=159;j++)
    { side_high[j]=59;}
    for(j=11;j<=149;j++)//从上往下寻全局点高
    {
      for(i=14;i<=49;i++)
      {
       
        if(img[i-3][j]+img[i-2][j]-img[i-1][j]-img[i][j]==2 )
        {side_high[j]=59-(i-2); break;}
      }
    }
    search_h=side_high[80];
    
    
    //抓左点
    if(Circle_fill_line_flag==2) //右拐
    {
       for(i=0;i<=59;i++){side_line[i]=0;}
       
      for(i=11;i<=40;i++)
      {
        for(j=0;j<=60;j++)
        {
          if((img[i][j]+img[i][j+1]-img[i][j+2]-img[i][j+3])==2){side_line[i]=j+3; break;}//从边扫
        }
      }
      
      search_s=side_line[11];//找最右值(大)
      search_o=140-edge_sum_right/3;
      search_o=search_o>120?search_o:120;
      
      for(i=11;i<=40;i++)
      {
        if(search_s<side_line[i])  search_s=side_line[i];
      }
      
      
      if(count==0&&search_s<=15) {count=2;}
     
      if(count==2)  {search_s=15; search_o=130;}
      
       
      for(j=search_s+5;j<=search_o;j++)//寻拐角高度值
      {
        if(search_h>=side_high[j])
        {search_h=side_high[j]; search_w=j;}
      }

      
    }
    
        
     //抓右点
    if(Circle_fill_line_flag==1) //左拐
    {
      for(i=0;i<=59;i++){side_line[i]=159;}
      
      
      for(i=11;i<=40;i++)
      {
        for(j=159;j>=100;j--)
        {
          if((img[i][j]+img[i][j-1]-img[i][j-2]-img[i][j-3])==2){side_line[i]=j-3; break;}//从边扫
        }
      }
      
      search_s=side_line[11];//找最左值(小)
      
      search_o=20+edge_sum_left/3;
      search_o=search_o<40?search_o:30;
        
        
        
      for(i=11;i<=40;i++)
      {
        
        if(search_s>side_line[i])  search_s=side_line[i];
      }
      
      if(count==0&&search_s>=144) {count=2;}
     
      if(count==2)  {search_s=144;}
      

      for(j=search_s-5;j>=search_o;j--)//寻拐角高度值
      {
        if(search_h>=side_high[j])
        {search_h=side_high[j]; search_w=j;}
      }

      
    }
    
    
    
    //传输
    
    circle_point[0]=search_h;  
    circle_point[1]=search_w;
    n=circle_point[0];
    p=circle_point[1];
    
    
    
   /*
    //画竖线
    for(i=11;i<=59;i++)
    {
      img[i][search_s]=1;
      img[i][search_o]=1;      
    }
    
    
    
    if(Circle_fill_line_flag==1) //补左线(向右补)//操作img  
    { 
      for(i=n+2;i>=1;i--)    
      {
        j=p+((159-p)*(n-i))/n;   
        img[60-i][j]=1;
        img[60-i][j-1]=1;
      } 
    }
    
    if(Circle_fill_line_flag==2)   //补右线(向左补)//操作img 
    { 
      for(i=n+2;i>=1;i--)    
      {
        j=p+((0-p)*(n-i))/n;   
        img[60-i][j]=1;
        img[60-i][j+1]=1;
      } 
    }
    
    
    //重新巡线
    find_edge();//扫边沿//入线start行数 
    Camera_center();   //中心线偏差
    
    */
  }
  else{count=0;}
 
}



void camera_circle_clear()//清空方向，圆环用
{
	// camera_center=0;//变量作持续更新变量
	camera_integral_center=0;//只在判断方向时启用的标志
	
}

void Edge_integral()//边线一次导数平方的积分//汇车判断//直线粗糙度
{
	
	int16 temp;  
	edge_Int_R=0;
	edge_Int_L=0;
	
	
	for(int i=45;i>12;i--)
	{
		if(edge[i][0]!=0)
		{		
			temp=abs(edge[i][0]-edge[i-1][0]);
			edge_Int_L+=temp*temp;
			
		}  
		
		if  (edge[i][1]!=79)
		{			
			temp=abs(edge[i][1]-edge[i-1][1]);
			edge_Int_R+=temp*temp;			
		}		
		if(edge_Int_R+edge_Int_L>=200 && road_flag==straight && abs(deviation)<=3);
		//buzzer_once();
	}
}
void Edge_sum()////边沿黑色数量//圆环用
{
	int i=0;
	edge_sum_left=0;
	edge_sum_right=0;
	
        edge_black[0]=0;
        edge_black[1]=0;
        
	for(i=11;i<=40;i++)
	{
		edge_sum_left+=img[i][10];
		edge_sum_left+=img[i][11];
		
		edge_sum_right+=img[i][149];
		edge_sum_right+=img[i][148];
	}
        
        	for(i=15;i<=40;i++)
	{
		edge_black[0]+=img[i][0];
		edge_black[0]+=img[i][1];
                edge_black[0]+=img[i][2];

		
		edge_black[1]+=img[i][157];
		edge_black[1]+=img[i][158];
                edge_black[1]+=img[i][159];
	}
        
        
	
	
}

void find_edge()//边线提取//通用
{
	int16 i=0;//行
	int16 j=0; //列
	int16 temp;
	int16 n;
	//  int16 k;
	
	for(i=58;i>=11;i--)
	{
		n=0;
		center_line[59]=79;//优化
		
		for(j=center_line[i+1];j>=1;j--)
		{		
			if(img[i][j-1]!=img[i][j]&&img[i][j-1]==1)
			{				
				edge[i][n]=j-1;
				n++;  
				break;
			}
		}
		if(n==0&&img[i][0]==0) 
		{
			edge[i][n]=0;
			n++;
		}//补点
		
		for(j=center_line[i+1];j<=158;j++)
		{                 
			if(img[i][j+1]!=img[i][j]&&img[i][j+1]==1)
			{                   
				edge[i][n]=j+1;
				n++; 
				break;
			}
		} 
		if(n==1&&img[i][159]==0) 
		{
			edge[i][n]=159; 
			n++; 
		}//补点
		
		center_line[i]=(edge[i][0]+edge[i][1])/2;//中线//初步采集
		// if(abs(center_line[i+1]-center_line[i])>=50){center_line[i]=center_line[i+1];}//滤波
	}//边沿采集结束
	
	
	
	  for(i=58;i>=11;i--)//中线偏差限幅
	  {
	    if(center_line[i]<=30)//偏边修复补线
	    {
	      for(j=i-1;j>=11;j--)
	      {
	       temp=center_line[i]-((i-j)*((79*100)/i))/100 ;
	       temp=temp>0?temp:0;
	      center_line[j]=temp;
	      edge[j][0]=temp;
	      edge[j][1]=temp;  
	      }
	      break;
	    }
	  }
	    for(i=58;i>=11;i--)//中线偏差限幅
	  {
	    
	    if(center_line[i]>=130)//偏边修复补线
	    {
	      for(j=i-1;j>=11;j--)
	      {
	       temp=center_line[i]+((i-j)*((79*100)/i))/100 ;
	       temp=temp<159?temp:159;
	      center_line[j]=temp;
	      edge[j][0]=temp;
	      edge[j][1]=temp;  
	      }
	      break;
	    }
	  }
	//
	////
	// for(i=11;i<=55;i++)    //边沿滤波优化//修复影子问题//未考虑环岛
	//  {
	//    if(edge[i][0]+edge[i+1][0]==0)//从上往下找边沿起始点
	//    {
	//   temp=0;
	//   for(n=0;n<=3;n++)
	//   {
	//   temp+=img[i+n][0]+img[i+n][1];//
	//   }
	//    
	//    if(temp==0)
	//    {
	//      for(j=i;j<=58;j++) {  edge[j][0]=0; }// img[j][0]=1; img[j][1]=1;}
	//    break;    
	//    }
	//    }
	//   
	//  }  
	//  for(i=15;i<=55;i++)
	//  {    
	//    if(edge[i][1]+edge[i+1][1]==318)//从上往下找边沿起始点
	//    {
	//    temp=0;
	//   for(n=0;n<=3;n++)
	//   {
	//   temp+=img[i+n][159]+img[i+n][158];//起始点往下是否黑点
	//   }
	//    if(temp==0)//确认黑点数量=0//强行改写edge为贴边
	//    {
	//      for(j=i;j<=59;j++) {edge[j][1]=159; }//img[j][158]=1; img[j][159]=1;}
	//    break; 
	//    }   
	//    }
	//  }  
	////  
	//  for(i=58;i>=11;i--)//中线优化后采集
	//  {
	//    center_line[i]=(edge[i][0]+edge[i][1])/2;              
	//    
	//  } 
	
}    
void find_circle_point()
{
	int16 i;
	
	/*白线起始点*/
	edge_start[0][0]=find_edge_start(0,0);	
	edge_start[0][1]=find_edge_start(0,1);	    
	//edge_20_start[0];//左右第二十列的白色起点
	edge_start[20][0]=find_edge_start(20,0);	
	edge_start[20][1]=find_edge_start(20,1);  
	/////////////circle judge抓点
	
	
	if(road_flag==circle)
	{
		circle_edge_start[0]=edge_start[0][0];
		circle_edge_start[2]=edge_start[0][1];
		for(i=25;i<=78;i++)
		{ 
			edge_start[i][0]=find_edge_start(i,0);
			if(edge_start[i][0]<=circle_edge_start[0])
			{
				circle_edge_start[0]=edge_start[i][0];
				circle_edge_start[1]=i;    
			} //找最小值 
		}   
		for(i=25;i<=78;i++)
		{ 
			edge_start[i][1]=find_edge_start(i,1);
			if(edge_start[i][1]<=circle_edge_start[2])
			{
				circle_edge_start[2]=edge_start[i][1];
				circle_edge_start[3]=160-i;
			} //找最小值 
		}   
	}
	
	
}

void Three_line_mode()
{
	int16 i,j,temp;
	if(boma4)//路肩显示模式	
	{
		for(i=58;i>=0;i--)//全黑
		{for(j=159;j>=0;j--)
		{
			img[i][j]=1;
		}
		}
		
		for(i=58;i>=0;i--)//显示边沿和中线
		{
			
			temp=center_line[i];
			img[i][temp]=0;  
			
			temp=edge[i][0];
			img[i][temp]=0; 
			
			temp=edge[i][1];
			img[i][temp]=0;              
			
		}
		
		
	}   
	
}

void draw_center_line()
{
	uint8 i,temp;
	
	for(i=58;i>=11;i--)
	{
		temp=center_line[i];
		img[i][temp]=1;
	}
}


uint8 find_edge_start(uint8 a,uint8 b)//a<80 b=0左1右
{
	uint8 i=0;
	
	if(b==0)
	{
		for (i=57;i>=2;i--)
		{
			if(img[i][0+a]+img[i][1+a]+img[i-1][0+a]+img[i-1][1+a]==4)
			{ return(59-i); }     
		}
	}
	
	if(b==1)
	{
		for (i=57;i>=2;i--)
		{
			if(img[i][159-a]+img[i][158-a]+img[i-1][159-a]+img[i-1][158-a]==4)
			{ return(59-i); }     
		}
	}
	
	
	
}

void P_transf()//透视变换//辅助观察用
{
	uint8 imgout[60][80];
	int16 i;
	int16 j;
	int16 cl;
	int16 k=0;
	int16 l=0;
	
	for(i=0;i<=59;i++)
	{
		for(j=0;j<=159;j++)
		{                    
			imgout[i][j]=1;
		}
	}
	
	for(l=0;l<60;l++)
	{
		for(k=0;k<160;k++)
		{                
			i=l; 
			
			cl=i-10;
			cl=cl>20?20:cl;
			
			j=((10+(float)(1.5*cl))/10)*(k-40)+40;
			
			if(i<60&&i>=0&&j<160&&j>=0) 
			{
				imgout[l][k]=img[i][j];
			}
			
			
		}
	}
	
	
	memcpy(img,imgout,sizeof(imgout));
	
	
	
}

void cover_image()//覆盖无用图像//前十行去掉
{
	int16 i;
	int16 j;     
	for(i=0;i<=10;i++)
	{
		for(j=0;j<=159;j++)
		{
			img[i][j]=1;
		}}
	
}

/*draw a straight car line */
void draw_car_line()//辅助上位机划车位线，不参与计算,做直冲判断//
{
	
	uint8 i;
	uint8 h;
	uint8 k;//10times
	uint8 l_R,l_L;
	
	Line_sum[0]=0;//初始化
	Line_sum[1]=0; 
	straight_go_flag=0;
	
	if(edge_sum_right>=10&&edge_sum_left>=10)//中位线显示条件
	{
		
		for(i=59;i>=11;i--)//划线
		{
			h=60-i;//上下坐标翻转
			k=10;//车位线斜率
			
			l_L=79-(68-(k*h)/10);//横坐标
			l_R=79+(68-(k*h)/10);
			
			//   img[i][l_R]=1; //划线 
			//   img[i][l_L]=1; 
			
			Line_sum[0]+= img[i][l_R+1]+img[i][l_R+2];//找切点
			Line_sum[1]+= img[i][l_L-1]+img[i][l_L-2];  
				
		}
		
		if(Line_sum[0]+Line_sum[1]<=4)
		{
			straight_go_flag=1;
			//buzzer_once();
		}
		
	}
	
	
}





void removeNoise(uint8 imginput[][80])//腐蚀膨胀//useless
{
	uint8 imgout[60][80];
	int16 i;
	int16 j;
	
	
	for(i=0;i<80;i++)
	{
		imgout[0][i]=imginput[0][i];
		imgout[59][i]=imginput[59][i];
	}
	for(i=0;i<60;i++)
	{
		imgout[i][0]=imginput[i][0];
		imgout[i][79]=imginput[i][79];
	}    
	
	
	//腐蚀  一黑全黑，去噪点，先腐蚀    
	
	for(i=1;i<59;i++)
	{
		for(j=1;j<79;j++)
		{
			imgout[i][j]=imginput[i][j];
			if(imginput[i][j+1]==1)
				imgout[i][j]=1;
			
			if(imginput[i+1][j+1]==1)
				imgout[i][j]=1;
			
			if(imginput[i+1][j]==1)
				imgout[i][j]=1;
			
			if(imginput[i+1][j-1]==1)
				imgout[i][j]=1;
			
			if(imginput[i][j-1]==1)
				imgout[i][j]=1;
			
			if(imginput[i-1][j-1]==1)
				imgout[i][j]=1;
			
			if(imginput[i-1][j]==1)
				imgout[i][j]=1;
			
			if(imginput[i-1][j+1]==1)
				imgout[i][j]=1;
		}
	}
	
	//膨胀
	memcpy(imginput,imgout,sizeof(imgout));
	
	for(i=1;i<59;i++)
	{
		for(j=1;j<79;j++)
		{
			imgout[i][j]=imginput[i][j];
			if(imginput[i][j+1]==0)
				imgout[i][j]=0;
			
			if(imginput[i+1][j+1]==0)
				imgout[i][j]=0;
			
			if(imginput[i+1][j]==0)
				imgout[i][j]=0;
			
			if(imginput[i+1][j-1]==0)
				imgout[i][j]=0;
			
			if(imginput[i][j-1]==0)
				imgout[i][j]=0;
			
			if(imginput[i-1][j-1]==0)
				imgout[i][j]=0;
			
			if(imginput[i-1][j]==0)
				imgout[i][j]=0;
			
			if(imginput[i-1][j+1]==0)
				imgout[i][j]=0;
			
		}
	}
	
	memcpy(imginput,imgout,sizeof(imgout));
	
}

void img_extract(void *dst, void *src, uint32_t srclen)//压缩
{
	uint8_t colour[2] = {0, 1}; //0 和 1 分别对应的颜色
	uint8_t * mdst = dst;
	uint8_t * msrc = src;
	//注：山外的摄像头 0 表示 白色，1表示 黑色
	uint8_t tmpsrc;
	while(srclen --)
	{
		tmpsrc = *msrc++;
		*mdst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
		*mdst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
		*mdst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
		*mdst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
		*mdst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
		*mdst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
		*mdst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
		*mdst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
	}
}



void img_compress(void *src, void *dst, uint32_t srclen)            //解压
{
	uint8_t * mdst = dst;
	uint8_t * msrc = src;
	uint8_t tmpsrc;
	while(srclen--)
	{
		
		tmpsrc=0;
		tmpsrc |= ((*mdst++)*255) & 0x80;
		tmpsrc |= ((*mdst++)*255) & 0x40;
		tmpsrc |= ((*mdst++)*255) & 0x20;
		tmpsrc |= ((*mdst++)*255) & 0x10;
		tmpsrc |= ((*mdst++)*255) & 0x08;
		tmpsrc |= ((*mdst++)*255) & 0x04;    
		tmpsrc |= ((*mdst++)*255) & 0x02;
		tmpsrc |= ((*mdst++)*255) & 0x01;    
		
		*msrc++=tmpsrc;
	}
	
	
}

