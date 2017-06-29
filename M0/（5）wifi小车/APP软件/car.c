#include "reg51.h"       

#define uchar unsigned char
#define uint  unsigned int

uchar ch;               
bit read_flag= 0 ;     

void init_com( void ) 

{ 
  SCON = 0x50;   
  PCON = 0x00;   
  IE |= 0x90 ;    

  
  TMOD = 0x20;  
  TH1=0xE6;      
  TL1=0xE6;      
  TR1 = 1;     
 
} 

void serial () interrupt 4 using 3 

{ 
    if (RI) 

    {  
     RI = 0 ;        
     ch=SBUF;          
     read_flag= 1 ; 
    }

} 

void send_char( unsigned char ch1) 
{ 
	SBUF=ch1;       
	while (TI== 0); 
	TI= 0 ;         

} 


void delay( uint z)
{
	uint x,y;
	for(x=110;x>0;x--)
		for(y=z;y>0;y--);
}


void main(void)
{
   init_com( );
	while(1)
	{
   
	
		uint w=0;
		
		if(read_flag)      
	
	   { 
	      read_flag= 0 ; 
	      send_char(ch);  
	
		
			if(ch==0x31)
			{
				
			 		while(ch!=0x30)
					{
						P0=0x55;		//01010101
						delay(2);
						P0=0x00;
						P2=0xfe;
						delay(6);
					}
				
			}
	
			
			if(ch==0x32)
			{
				
					while(ch!=0x30)
					{
						P0=0xaa;		//10101010
						delay(2);
						P0=0x00;
						P2=0xfd;
						delay(6);
					}
				
			}
			
			if(ch==0x33)
			{	
				
					while(ch!=0x30)
					{
						P0=0x66;			//01100110
						delay(2);
						P0=0x00;
						P2=0xfb;
						delay(6);
					}
				
			}
	
			
			if(ch==0x34)
			{
				
			 		while(ch!=0x30)
					{
						P0=0x99;		//10011001
						delay(2);
						P0=0x00;
						P2=0xf7;
						delay(6);
					}
				
			}
	
			if(ch==0x30)
			{
				P0=0x00;
				P2=0xef;
			 	
			}
			
	    } 
 	} 

} 
