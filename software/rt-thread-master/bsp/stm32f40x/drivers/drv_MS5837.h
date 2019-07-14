#ifndef __MS5837_H_
#define __MS5837_H_

#include "DataType.h"
#include "sys.h"

#define MS583703BA_ADC_RD          0x00
#define	MS583703BA_PROM_RD 	       0xA0
#define MS583703BA_PROM_CRC        0xAE

#define MS583703BA_SlaveAddress    0xEC  //MS5387的地址
#define MS583703BA_RST             0x1E  //cmd 复位

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



void MS583703BA_RESET(void);

uint64 MS583703BA_getConversion(uint8_t command);
void MS583703BA_GetTemperature(void);

extern int64  dT;
extern int64  D2_Temp;
extern float  MS5837_Temperature;
extern uint32 MS5837_Pressure;
extern int32  Cal_C[7];


uint8 MS5837_Init(void);
void MS583703BA_getPressure(void);
void MS583703BA_getTemperature(void);

float get_ms5837_temperature(void);

uint32 get_ms5837_pressure(void); //获取初始压力值

#endif

