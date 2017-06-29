#ifndef _DHT11__H_
#define _DHT11__H

#define DHT11_Out PAout(0)
#define DHT11_In  PAin(0)

#define uchar unsigned char 
#define uint  unsigned int

unsigned char StartDHT11(void);
void DHT11_PortIN(void);
void DHT11_PortOUT(void);
u8 ReadDHT11(void);
void com(void);
//**********************************

//**********************************

extern u8 tdata[4];
extern u8 sbuf;
extern u8 check;

#endif
