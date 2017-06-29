#ifndef fei
#define fei

#define uint unsigned int 
#define uchar unsigned char
//#define KEY1 PAin(15)
//#define KEY (1<<13)
//#define KEY1 (1<<15)
//#define KEY2 (1<<8)
//#define KEY_GET() ((GPIOA->IDR&(KEY))1:0)
//#define KEY1_GET() ((GPIOA->IDR&(KEY1))1:0)
//#define KEY2_GET() ((GPIOA->IDR&(KEY2))1:0)
#define KEY GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)

#define WKUP_KD PAin(0) 	 
u8 Check_WKUP(void); 
void WKUP_Init(void); 
void Sys_Enter_Standby(void);

#endif
