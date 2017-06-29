#ifndef __DHT11_H
#define __DHT11_H 

#include <stm32f10x.h>
#include "sys.h"
//IO方向设置
#define DHT11_IO_IN()  {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=8<<0;}	//输入	
#define DHT11_IO_OUT() {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=3<<0;}	//输出
////IO操作函数											   
#define	DHT11_DQ_OUT PAout(0) //数据端口	PA0 
#define	DHT11_DQ_IN  PAin(0)  //数据端口	PA0 
#define HIGH 1
#define LOW 0
extern u8 sbuf;
extern  char tdata[4];

//u8 start_DHT11(void);

void ReadDHT11(void);

//u32 DHT11_Get_Data(void);//获取数据
#endif

