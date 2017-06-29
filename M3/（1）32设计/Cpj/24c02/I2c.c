#include"Function.h"   

#define    SData  GPIO_Pin_7   //P2^0;     //I2C  
#define    SCLK  GPIO_Pin_6    //P2^1;     //I2C 
#define SCL(x)	x ? GPIO_SetBits(GPIOB , SCLK)  : GPIO_ResetBits(GPIOB , SCLK)
#define	SDA(x)  x ? GPIO_SetBits(GPIOB , SData) : GPIO_ResetBits(GPIOB , SData)
u8 ack;
void Start_I2c()
{
  
  SDA(1);  //SDA=1;
  Delay_us(1);
  SCL(1);	  //SCL=1;
  Delay_us(5);     
  
  SDA(0);  //SDA=0;       
  Delay_us(5);      
  SCL(0);	  //SCL=0;      
  Delay_us(2);
}

void Stop_I2c()
{
  
  SDA(0);  //SDA=0;  
  Delay_us(1);    
  SCL(1)	; //SCL=1; 
  Delay_us(5);
  SDA(0);  //SDA=1; 
  Delay_us(4);
}

void  SendByte(unsigned char  c)
{
 unsigned char  BitCnt;
 
 for(BitCnt=0;BitCnt<8;BitCnt++) 
    {
     if((c<<BitCnt)&0x80) SDA(1);  //SDA=1;
     else SDA(0); // SDA=0                
     Delay_us(1);
     SCL(1);      //SCL=1 
	 Delay_us(5); 
     SCL(0);	     //SCL = 0
    }
    
    Delay_us(2);
    SDA(1);   //SDA=1 
	Delay_us(2);   
   	SCL(1);   //SCL=1
    Delay_us(3);
    if(GPIO_ReadInputDataBit(GPIOB ,SData)==1)ack=0;     
    else ack=1;     
  	SCL(0);   //SCL=0;
    Delay_us(3);
}
   
unsigned char   RcvByte()
{
  unsigned char  retc;
  unsigned char  BitCnt;
  
  retc=0; 
  SDA(1); //SDA=1 
  for(BitCnt=0;BitCnt<8;BitCnt++)
      {
        Delay_us(1);           
        SCL(0);    //SCL=0 
		Delay_us(5); 
     	
		SCL(1);	 //SCL=1
        Delay_us(2);
        retc=retc<<1;
        if(GPIO_ReadInputDataBit(GPIOB , SData)== 1)retc=retc+1; // SDA == 1
        Delay_us(2);
      }
  SCL(0); // SCL=0;
  Delay_us(2);
  return(retc);
}

void Ack_I2c(u8 a)
{
 
  if(a==0)
   
   SDA(0);  //SDA=0
  else 
   SDA(1);  //SDA=1;
  Delay_us(3);    
 
  SCL(1);   //SCL=1;	 
  Delay_us(4);	
 
  SCL(0)	;   //SCL=0;
  Delay_us(2); 
 
}




