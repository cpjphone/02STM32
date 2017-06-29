#include "iospi.h"
#include "delay.h"

void IOSPI_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_SetBits(GPIOA, GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5); 

}

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 IOSPI_ReadWriteByte(u8 x)
{		
 u8 rbyte=0;	
	
 SET_SPI_SCL_PIN(1);
 delay_us(DELAY_TIME);
 
 SET_SPI_SI_PIN((x&0x80)?1:0);
 SET_SPI_SCL_PIN(0); 
 delay_us(DELAY_TIME);
 if(GET_SPI_SO_PIN()) rbyte|=0x80;
 SET_SPI_SCL_PIN(1);
 delay_us(DELAY_TIME);

 SET_SPI_SI_PIN((x&0x40)?1:0);
 SET_SPI_SCL_PIN(0); 
 delay_us(DELAY_TIME);
 if(GET_SPI_SO_PIN()) rbyte|=0x40;
 SET_SPI_SCL_PIN(1);
 delay_us(DELAY_TIME);

 SET_SPI_SI_PIN((x&0x20)?1:0);
 SET_SPI_SCL_PIN(0); 
 delay_us(DELAY_TIME);
 if(GET_SPI_SO_PIN()) rbyte|=0x20;
 SET_SPI_SCL_PIN(1);
 delay_us(DELAY_TIME);

 SET_SPI_SI_PIN((x&0x10)?1:0);
 SET_SPI_SCL_PIN(0); 
 delay_us(DELAY_TIME);
 if(GET_SPI_SO_PIN()) rbyte|=0x10;
 SET_SPI_SCL_PIN(1);
 delay_us(DELAY_TIME);
 
 SET_SPI_SI_PIN((x&0x08)?1:0);
 SET_SPI_SCL_PIN(0); 
 delay_us(DELAY_TIME);
 if(GET_SPI_SO_PIN()) rbyte|=0x08;
 SET_SPI_SCL_PIN(1);
 delay_us(DELAY_TIME);
 
 SET_SPI_SI_PIN((x&0x04)?1:0);
 SET_SPI_SCL_PIN(0); 
 delay_us(DELAY_TIME);
 if(GET_SPI_SO_PIN()) rbyte|=0x04;
 SET_SPI_SCL_PIN(1);
 delay_us(DELAY_TIME);
 
 SET_SPI_SI_PIN((x&0x02)?1:0);
 SET_SPI_SCL_PIN(0); 
 delay_us(DELAY_TIME);
 if(GET_SPI_SO_PIN()) rbyte|=0x02;
 SET_SPI_SCL_PIN(1);
 delay_us(DELAY_TIME);
 
 SET_SPI_SI_PIN((x&0x01)?1:0);
 SET_SPI_SCL_PIN(0); 
 delay_us(DELAY_TIME);
 if(GET_SPI_SO_PIN()) rbyte|=0x01;
 SET_SPI_SCL_PIN(1);
 delay_us(DELAY_TIME);
 
 return rbyte;		
}

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 IOSPI_ReadWriteByte_R(u8 x)
{		
 u8 rbyte=0;	
	
 SET_SPI_SCL_PIN(0);
 delay_us(DELAY_TIME);
 
 SET_SPI_SI_PIN((x&0x80)?1:0);
 SET_SPI_SCL_PIN(1); 
 delay_us(DELAY_TIME);
 if(GET_SPI_SO_PIN()) rbyte|=0x80;
 SET_SPI_SCL_PIN(0);
 delay_us(DELAY_TIME);

 SET_SPI_SI_PIN((x&0x40)?1:0);
 SET_SPI_SCL_PIN(1); 
 delay_us(DELAY_TIME);
 if(GET_SPI_SO_PIN()) rbyte|=0x40;
 SET_SPI_SCL_PIN(0);
 delay_us(DELAY_TIME);
	
 SET_SPI_SI_PIN((x&0x20)?1:0);
 SET_SPI_SCL_PIN(1); 
 delay_us(DELAY_TIME);
 if(GET_SPI_SO_PIN()) rbyte|=0x20;
 SET_SPI_SCL_PIN(0);
 delay_us(DELAY_TIME);
 
 SET_SPI_SI_PIN((x&0x10)?1:0);
 SET_SPI_SCL_PIN(1); 
 delay_us(DELAY_TIME);
 if(GET_SPI_SO_PIN()) rbyte|=0x10;
 SET_SPI_SCL_PIN(0);
 delay_us(DELAY_TIME);
 
 SET_SPI_SI_PIN((x&0x08)?1:0);
 SET_SPI_SCL_PIN(1); 
 delay_us(DELAY_TIME);
 if(GET_SPI_SO_PIN()) rbyte|=0x08;
 SET_SPI_SCL_PIN(0);
 delay_us(DELAY_TIME);
 
 SET_SPI_SI_PIN((x&0x04)?1:0);
 SET_SPI_SCL_PIN(1); 
 delay_us(DELAY_TIME);
 if(GET_SPI_SO_PIN()) rbyte|=0x04;
 SET_SPI_SCL_PIN(0);
 delay_us(DELAY_TIME);
 
 SET_SPI_SI_PIN((x&0x02)?1:0);
 SET_SPI_SCL_PIN(1); 
 delay_us(DELAY_TIME);
 if(GET_SPI_SO_PIN()) rbyte|=0x02;
 SET_SPI_SCL_PIN(0);
 delay_us(DELAY_TIME);
 
 SET_SPI_SI_PIN((x&0x01)?1:0);
 SET_SPI_SCL_PIN(1); 
 delay_us(DELAY_TIME);
 if(GET_SPI_SO_PIN()) rbyte|=0x01;
 SET_SPI_SCL_PIN(0);
 delay_us(DELAY_TIME);
 
 return rbyte;		
}
