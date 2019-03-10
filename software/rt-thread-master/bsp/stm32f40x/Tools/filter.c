#include "sys.h"
#include <math.h>
#include "filter.h"

float  KalmanGain;//  卡尔曼增益
float  EstimateCovariance = 0.1f;//估计协方差
float  MeasureCovariance = 0.2f;//测量协方差
float  EstimateValue = 0.0f;//估计值






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

float  KalmanGain;//  卡尔曼增益
float  EstimateCovariance = 0.1f;//估计协方差
float  MeasureCovariance = 0.2f;//测量协方差
float  EstimateValue = 0.0f;//估计值


*/

 
float KalmanFilter(float *Measure)
{
		//计算卡尔曼增益
		KalmanGain=EstimateCovariance*sqrt(1/(EstimateCovariance*EstimateCovariance+MeasureCovariance*MeasureCovariance));
		//计算本次滤波估计值
		EstimateValue=EstimateValue+KalmanGain*(*Measure-EstimateValue);
		//更新估计协方差
		EstimateCovariance=sqrt(1-KalmanGain)*EstimateCovariance;
		//更新测量方差
		MeasureCovariance=sqrt(1-KalmanGain)*MeasureCovariance;
		//返回估计值
		return EstimateValue;
}


float kalmanFilter(float *Original_Data) 
{
  static float prevData=0; 
  static float p=10, q=0.0001, r=0.005, kGain=0;
	p = p+q; 
	kGain = p/(p+r);

	*Original_Data = prevData+(kGain*(*Original_Data-prevData)); 
	p = (1-kGain)*p;

	prevData = *Original_Data;

	return *Original_Data; 
}
