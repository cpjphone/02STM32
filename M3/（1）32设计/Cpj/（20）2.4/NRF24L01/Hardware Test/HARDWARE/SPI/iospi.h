#ifndef _IOSPI_H_
#define _IOSPI_H_

#include "sys.h"

#define SET_SPI_SCL_PIN(val) PAout(5)=val //操作SPI的时钟
#define SET_SPI_SI_PIN(val)  PAout(7)=val //操作SPI的主出从入
#define GET_SPI_SO_PIN()     PAin(6)      //获取SPI主入从出

#define DELAY_TIME 100 //SPI的延时参数，此值越大，SPI速度越慢

void IOSPI_GPIO_Init(void);			 //初始化SPI口 
u8 IOSPI_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
u8 IOSPI_ReadWriteByte_R(u8 x);
		 
#endif
