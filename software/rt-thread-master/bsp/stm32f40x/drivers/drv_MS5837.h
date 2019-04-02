#ifndef __MS5837_H_
#define __MS5837_H_


#include "sys.h"
#include "drv_i2c.h"



#define MS583703BA_ADC_RD          0x00
#define	MS583703BA_PROM_RD 	       0xA0
#define MS583703BA_PROM_CRC        0xAE

#define MS583703BA_SlaveAddress    0xEC  //MS5387µÄµØÖ·
#define MS583703BA_RST             0x1E  //cmd ¸´Î»

#define MS583703BA_D1_OSR_256					 		 0x40
#define MS583703BA_D1_OSR_512					 		 0x42
#define MS583703BA_D1_OSR_1024					   0x44
#define MS583703BA_D1_OSR_2048					   0x46
#define MS583703BA_D1_OSR_4096					   0x48
#define	MS583703BA_D1_OSR_8192   	         0x4A

#define MS583703BA_D2_OSR_256					 		 0x50
#define MS583703BA_D2_OSR_512					 		 0x52
#define MS583703BA_D2_OSR_1024					   0x54
#define MS583703BA_D2_OSR_2048					   0x56
#define MS583703BA_D2_OSR_4096					   0x58
#define	MS583703BA_D2_OSR_8192   	         0x5A


#define FILTER_num 20

u8 MS5837_Get_PROM(void);
u8 MS5837_Init(void);
u32 MS583703BA_getPressure(void);
void MS583703BA_getTemperature(void);
void MS583703BA_RESET(void);
unsigned long MS583703BA_getConversion(uint8_t command);


float Get_High(void);
void Filter_Hight(unsigned int set_hight);
double MS583703BA_GetDeep(void);




extern int64_t dT;
extern int64_t D2_Temp;
extern float MS_TEMP;
extern uint32_t MS5837_Pressure;
extern uint32_t  Cal_C[7];

#endif

