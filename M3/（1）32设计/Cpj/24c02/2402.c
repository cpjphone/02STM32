#include"Function.h"

u8 ISendStr(unsigned char  sla,unsigned char  suba,unsigned char  *s,unsigned char  no)
{
   unsigned char i;

   Start_I2c();            
   SendByte(sla);            
   if(ack==0)return(0);
   SendByte(suba);            
   if(ack==0)return(0);

   for(i=0;i<no;i++)
   {   
     SendByte(*s);           
     if(ack==0)return(0);
     s++;
   } 
   Stop_I2c();              

   return(1);
}

u8 IRcvStr(unsigned char  sla,unsigned char  suba,unsigned char  *s,unsigned char  no)
{
   unsigned char i;

   Start_I2c();                 
   SendByte(sla);              
   if(ack==0)return(0);
   SendByte(suba);              
   if(ack==0)return(0);

   Start_I2c();                 
   SendByte(sla+1);
   if(ack==0)return(0);
   for(i=0;i<no-1;i++)
   {   
     *s=RcvByte();              
      Ack_I2c(0);                
     s++;
   } 
   *s=RcvByte();
   Ack_I2c(1);                   
   Stop_I2c();                 
   return(1);
}



