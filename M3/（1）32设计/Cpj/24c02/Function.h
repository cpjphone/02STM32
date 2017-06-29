#include "stm32f10x.h"
extern u8 ack;
extern void Delay_us(unsigned n);
extern void Start_I2c(void); 
extern void Stop_I2c(void);
extern void Ack_I2c(u8 a);
extern void  SendByte(unsigned char  c);              
extern u8 ISendStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no) ;  
extern u8 ISendStrExt(unsigned char sla,unsigned char *s,unsigned char no);               
extern unsigned char RcvByte(void);               
extern u8 IRcvStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no);               
extern u8 IRcvStrExt(unsigned char sla,unsigned char *s,unsigned char no);





