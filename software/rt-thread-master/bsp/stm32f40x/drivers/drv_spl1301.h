#ifndef SPL1301_H
#define SPL1301_H


#include "DataType.h"




#define HW_ADR 0x77
#define CONTINUOUS_PRESSURE     1
#define CONTINUOUS_TEMPERATURE  2
#define CONTINUOUS_P_AND_T      3
#define PRESSURE_SENSOR     0
#define TEMPERATURE_SENSOR  1

struct spl1301_calib_param_t {	
    int16 c0;
    int16 c1;
    int32 c00;
    int32 c10;
    int16 c01;
    int16 c11;
    int16 c20;
    int16 c21;
    int16 c30;       
};

struct spl1301_t {	
    struct spl1301_calib_param_t calib_param;/**<calibration data*/	
    uint8 chip_id; /**<chip id*/	
    int32 i32rawPressure;
    int32 i32rawTemperature;
    int32 i32kP;    
    int32 i32kT;
};


int spl1301_init(void); //初始化调用
void spl1301_rateset(uint8 iSensor, uint8 u8OverSmpl, uint8 u8SmplRate);
void spl1301_start_temperature(void);
void spl1301_start_pressure(void);
void spl1301_start_continuous(uint8 mode);
void spl1301_get_raw_temp(void);
void spl1301_get_raw_pressure(void); //调用该句转换数据
float get_spl1301_temperature(void); 
float get_spl1301_pressure(void);


#endif
