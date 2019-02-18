#ifndef KEY_H_H
#define KEY_H_H



#define KeyUp gpio_get(PTC8) //up
#define KeyDm gpio_get(PTC13) //down
#define KeyLt gpio_get(PTC10) //left
#define KeyRt gpio_get(PTC19) //right
#define KeyMd gpio_get(PTC6) //middle
#define KeyAl gpio_get(PTC11) //along

#define boma6 gpio_get(PTD1)//±£Ãü²Ëµ¥
#define boma5 gpio_get(PTD4)
#define boma4 gpio_get(PTD6)
#define boma3 gpio_get(PTD9)
#define boma2 gpio_get(PTD11)
#define boma1 gpio_get(PTD13)

uint8 key_scan(void);
void menu_define(void);
void Boma_OLED();
void key_control();
void Clear_screen();
void flash_updata();
void Flash_Key();
void Car_Run_Key();
void Mode_Screen_Key();
void Reflash();

#endif