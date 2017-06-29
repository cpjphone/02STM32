#include "dht11.h"
#include "sys.h"
#include "math.h"
#include "SysTick_delay.h"

char tdata[4];
u8 sbuf,check;

#define DHT11_Out PAout(1)
#define DHT11_In  PAin(1)

/*******************************************延时函数**********************************/ 
void NOP()
{ vu16 i;
  for(i=0; i<100; i++); 
}

unsigned char StartDHT11(void)
{

   DHT11_Out = 0;
   delay_ms(18);  //触发开始,总线拉低要大于18ms
	//OSTimeDlyHMSM(0,0,0,18);
  
   DHT11_Out = 1;	//释放总线
   //delay_us(25);  //wait 20-40uS 等待DHT11的低电平响应信号
   NOP();NOP();NOP();NOP();NOP();   
   if(!DHT11_In)
   {
	   	 while(!DHT11_In);//低电平的响应信号，80us
		   while(DHT11_In);//紧接着是80us的高电平数据准备信号
		   return 1;
   }
	return 0;
}
void com(void)
{
	u8 i,tt;
	tt = 0;
	for(i = 0;i<8;i++)
	{
		sbuf <<= 1;
		//delay_us(20);//接收到响应后会出现50us的低电平表示发送数据的开始，所以这里小延时一下
		NOP();
		while(!DHT11_In);//等到高电平的出现，高电平的时间表示的是数据位的0和1
		//delay_us(28);//数据为0的信号时间为26-28us，1则为70us，这里超时检测
		NOP();NOP();
		tt = 100;
		while(tt++);
		if(DHT11_In)//如果还为高
		{
			 sbuf |= 0x01;
			// delay_us(30);//这里的延时足够了，40+10+30>70了
			NOP();
		}
		else //如果变低
		{
			sbuf &= 0xfe;
		}
	}
}
void ReadDHT11(void)
{
	u8 sum,tdata1[4];
	
	if(StartDHT11())	//判断是否已经响应
	{
		com();
		tdata1[0]=sbuf;
		com();
		tdata1[1]=sbuf;
		com();
		tdata1[2]=sbuf;		
		com();
		tdata1[3]=sbuf;
		com();
		check = sbuf;
		sum = (tdata1[0]+tdata1[1]+tdata1[2]+tdata1[3]);
	}
if((fabs(check - sum))<10)
	{
		tdata[2]=(char)tdata1[2];
		tdata[3]=(char)tdata1[3];
		tdata[0]=(char)tdata1[0];
		tdata[1]=(char)tdata1[1];
		
	}
	//else	
//	return 0;
}




