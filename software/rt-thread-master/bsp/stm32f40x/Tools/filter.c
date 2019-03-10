


#include "sys.h"




/* Ã°ÅÝ  */
short bubble(short *adc_value)
{
		u8 i,j;
		short res = 0;   //reserve
		short value = 0;

		for(j = 0;j < 10-1;j++){
				for(i = 0;i < 9-j;i++){
						if( adc_value[i] > adc_value[i+1] ){ //>ÉýÐò   <½µÐò
								res = adc_value[i];
								adc_value[i] = adc_value[i+1];
								adc_value[i+1] = res;
						}
					
				}
		}
		value = (*(adc_value+4)+ *(adc_value+5)+ *(adc_value+6))/3;		
		return (short)value;
} 

