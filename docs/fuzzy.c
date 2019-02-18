
/************************
智能双车
2018.5.6
by seaman
********************/
#include <math.h>
#define PMAX    100
#define PMIN    -100
#define DMAX    100
#define DMIN    -100
#define FMAX    100       
/*语言值的满幅值*/



/**********************************************************/
float Fuzzy(float *P,float *D)   /*模糊*/
{
  
  
  float PFF[4]={0,25,45,65};//{0,12,24,48};
/*输入量D语言的特征点*/
float DFF[4]={0,2,4,6};//{0,16,32,64};
/*输出量U的语言值的特征点*/
float UFF[7]={0,70,80,90,100,110,120};//{0,15,30,45,60,75,90};
int rule[7][7]={
  //ec -3,-2,-1, 0, 1, 2, 3       //   e//根据需要来改
      { 6, 6, 6, 5, 5, 5, 4,},   //   -3
      { 5, 4, 4, 3, 2, 2, 1,},   //   -2
      { 4, 3, 2, 1, 0, 1, 2,},   //   -1
      { 4, 3, 1, 0, 1, 3, 4,},   //    0
      { 2, 1, 0, 1, 2, 3, 4,},   //    1
      { 1, 2, 2, 3, 4, 4, 5,},   //    2
      { 4, 5, 5, 5, 6, 6, 6}     //    3
      };   



float    U;           
/*偏差,偏差微分以及输出值的精确量*/
uint8   PF[2],DF[2],UF[4];  
/*偏差,偏差微分以及输出值的隶属度*/
int    Pn,Dn,Un[4];
long   temp1,temp2;

/*隶属度的确定*/
/*根据P/偏差/D/偏差的变化量/的指定语言获得有效隶属度*/
if(*P>-PFF[3] && *P<PFF[3])
{
    if(*P<=-PFF[2])
	{
		 Pn=-2;//交叉右区间标号
		 PF[0]=FMAX*((float)(PFF[3]-(-*P))/(PFF[3]-PFF[2]));//右区间隶属度
		//隶属度=最大值*（区间左（正）-偏差（正））/（左-右区间中值）
	}
    else if(*P<=-PFF[1])
   {
    Pn=-1;
    PF[0]=FMAX*((float)(PFF[2]-(-*P))/(PFF[2]-PFF[1]));
   }
    else if(*P<=PFF[0])
   {
    Pn=0;
    PF[0]=FMAX*((float)(PFF[1]-(-*P))/(PFF[1]-PFF[0]));
   }
    else if(*P<=PFF[1])
   {
    Pn=1; PF[0]=FMAX*((float)(PFF[0]-*P)/(PFF[0]-PFF[1]));
   }
    else if(*P<=PFF[2])
   {
    Pn=2; PF[0]=FMAX*((float)(PFF[1]-*P)/(PFF[1]-PFF[2]));
   }
    else if(*P<=PFF[3])
   {
    Pn=3; PF[0]=FMAX*((float)(PFF[2]-*P)/(PFF[2]-PFF[3]));
   }
    }
else if(*P<=-PFF[3])
   {
    Pn=-2;   PF[0]=0;
   }
else if(*P>=PFF[3])
   {
    Pn=3;   PF[0]=FMAX;
   }
   
PF[1]=FMAX-PF[0];//左区间隶属度


/*根据P/偏差/D/偏差的变化量/的指定语言获得有效隶属度*/
if(*D>-DFF[3] && *D<DFF[3])
{
    if(*D<=-DFF[2])
	{
		 Dn=-2;//交叉右区间标号
		 DF[0]=FMAX*((float)(DFF[3]-(-*D))/(DFF[3]-DFF[2]));//右区间隶属度
		//隶属度=最大值*（区间左（正）-偏差（正））/（左-右区间中值）
	}
    else if(*D<=-DFF[1])
   {
    Dn=-1;
    DF[0]=FMAX*((float)(DFF[2]-(-*D))/(DFF[2]-DFF[1]));
   }
    else if(*D<=DFF[0])
   {
    Dn=0;
    DF[0]=FMAX*((float)(DFF[1]-(-*D))/(DFF[1]-DFF[0]));
   }
    else if(*D<=DFF[1])
   {
    Dn=1; DF[0]=FMAX*((float)(DFF[0]-*D)/(DFF[0]-DFF[1]));
   }
    else if(*D<=DFF[2])
   {
    Dn=2; DF[0]=FMAX*((float)(DFF[1]-*D)/(DFF[1]-DFF[2]));
   }
    else if(*D<=DFF[3])
   {
    Dn=3; DF[0]=FMAX*((float)(DFF[2]-*D)/(DFF[2]-DFF[3]));
   }
    }
else if(*D<=-DFF[3])
   {
    Dn=-2;   DF[0]=0;
   }
else if(*D>=DFF[3])
   {
    Dn=3;   DF[0]=FMAX;
   }
   
DF[1]=FMAX-DF[0];//左区间隶属度


/*使用误差范围优化后的规则表rule[7][7]*/
/*输出值使用13个隶属函数,中心值由UFF[7]指定*/
/*取左右区间DF交叉四个规则有效*/
Un[3]=rule[Pn+3-1][Dn+3-1];//左左区间
Un[2]=rule[Pn+3][Dn+3-1];//右左区间
Un[1]=rule[Pn+3-1][Dn+3];//左右区间
Un[0]=rule[Pn+3][Dn+3];//右右区间
//3 1//Un
//2 0//
/*采用或规则，取隶属度最小值*/

UF[0]=PF[0]<=DF[0]?PF[0]:DF[0];
UF[1]=PF[1]<=DF[0]?PF[1]:DF[0];
UF[2]=PF[0]<=DF[1]?PF[0]:DF[1];
UF[3]=PF[1]<=DF[1]?PF[1]:DF[1];

/*重心法反模糊*/
/*Un[]原值为输出隶属函数的标号，转换为隶属函数值*/
/*规则表为正时，un>0*/
if(Un[0]>=0)
   Un[0]=UFF[Un[0]];
else
   Un[0]=-UFF[-Un[0]];
if(Un[1]>=0)
   Un[1]=UFF[Un[1]];
else
   Un[1]=-UFF[-Un[1]];
if(Un[2]>=0)
   Un[2]=UFF[Un[2]];
else
   Un[2]=-UFF[-Un[2]];
if(Un[3]>=0)
   Un[3]=UFF[Un[3]];
else
   Un[3]=-UFF[-Un[3]];


//隶属度UF范围100转面积比//
UF[0]=(10000-(100-UF[0])^2)/100;//0<UF<100
UF[1]=(10000-(100-UF[1])^2)/100;
UF[2]=(10000-(100-UF[2])^2)/100;
UF[3]=(10000-(100-UF[3])^2)/100;

//重心法
temp1=UF[0]*Un[0]+UF[1]*Un[1]+UF[2]*Un[2]+UF[3]*Un[3];
temp2=UF[0]+UF[1]+UF[2]+UF[3];
U=temp1/temp2;

	return U;
}

