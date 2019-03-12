#include "sys.h"
#include <math.h>
#include "filter.h"





/* 冒泡  */
short bubble(short *adc_value)
{
		u8 i,j;
		short res = 0;   //reserve
		short value = 0;

		for(j = 0;j < 10-1;j++){
				for(i = 0;i < 9-j;i++){
						if( adc_value[i] > adc_value[i+1] ){ //>升序   <降序
								res = adc_value[i];
								adc_value[i] = adc_value[i+1];
								adc_value[i+1] = res;
						}
					
				}
		}
		value = (*(adc_value+4)+ *(adc_value+5)+ *(adc_value+6))/3;		
		return (short)value;
} 

/*
其中p的初值可以随便取，但是不能为0（为0的话卡尔曼滤波器就认为已经是最优滤波器了） 

q,r的值需要我们试出来，讲白了就是(买的破温度计有多破，以及你的超人力有多强)

q参数调整滤波后的曲线与实测曲线的相近程度，q越大越接近。

r参数调滤波后的曲线平滑程度，r越大越平滑。 

*/
float KalmanFilter(float *Original_Data) 
{
  static float prevData=0; 
  static float p=10, q=0.0001, r=0.05, kGain=0;
	
	p = p+q; 
	kGain = p/(p+r);

	*Original_Data = prevData+(kGain*(*Original_Data-prevData)); 
	p = (1-kGain)*p;

	prevData = *Original_Data;

	return *Original_Data; 
}
