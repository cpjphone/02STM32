#include "stm32f10x.h"

#define uint unsigned int
#define uchar unsigned char

#define key1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)
#define key2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)
#define key3 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)

uchar taba[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f,
							0xff,0xff,0xff,0xff,0xff,0xff,0XFF,0XFF};
uchar tabb[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,
							0x00,0x00,0x00,0x00,0x00,0x00};
uchar tabc[]={0x38,0x7C,0x7E,0x3F,0x7E,0x7C,0x38,0x00,
							0x38,0x44,0x5A,0x19,0x5A,0x44,0x38,0x00};
uchar	tabe[]={0xEF,0xD7,0x83,0x01,0x01,0x01,0x93,0xFF};
uchar mode[]={0x00,0x00,0x02,0x42,0xFE,0x02,0x02,0x00,
							0x00,0x21,0x43,0x85,0x89,0x91,0x61,0x01,
							0x00,0x00,0x44,0x82,0x92,0x92,0x6C,0x00};
uchar time_codege[]={0x00,0x7C,0x82,0x82,0x82,0x7C,0x00,0x00,
									 0x00,0x00,0x02,0x42,0xFE,0x02,0x02,0x00,
									 0x00,0x43,0x85,0x89,0x91,0x61,0x00,0x00,
									 0x00,0x00,0x44,0x82,0x92,0x92,0x6C,0x00,
									 0x04,0x1C,0x24,0x44,0xFE,0x04,0x04,0x00,
									 0x00,0x00,0xF2,0x91,0x91,0x91,0x9E,0x00,
									 0x00,0x3E,0x49,0x49,0x49,0x2E,0x00,0x00,
								     0x00,0x00,0x00,0x80,0x80,0x80,0xFF,0x00,
									 0x00,0x00,0x6E,0x91,0x91,0x6E,0x00,0x00,
									 0x00,0x00,0x72,0x89,0x89,0x89,0x7E,0x00
										};
uchar time_codeshi[]={0x00,0x7C,0x82,0x82,0x82,0x7C,0x00,0x00,
									 0x00,0x00,0x02,0x42,0xFE,0x02,0x02,0x00,
									 0x00,0x43,0x85,0x89,0x91,0x61,0x00,0x00,
									 0x00,0x00,0x44,0x82,0x92,0x92,0x6C,0x00,
									 0x04,0x1C,0x24,0x44,0xFE,0x04,0x04,0x00,
									 0x00,0x00,0xF2,0x91,0x91,0x91,0x9E,0x00,
									 //0x00,0x3E,0x49,0x49,0x49,0x2E,0x00,0x00,
								   //0x00,0x00,0x00,0x80,0x80,0x80,0xFF,0x00,
									 //0x00,0x00,0x6E,0x91,0x91,0x6E,0x00,0x00,
									 //0x00,0x00,0x72,0x89,0x89,0x89,0x7E,0x00
										};
//uchar aa[]={0xC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90};
//u16 bb[]={0xFFC0,0XFFF9,0XFFA4,0XFFB0,0XFF99,0XFF92,0XFF82,0XFFF8,0XFF80,0XFF90};
uint count=0,sec=0,min=0,hour=0,k1,i,j,k,h1,h=0,flag;
//u16 aa[]={0xff3f,0xff06,0xff5b,0xff4f,0xff66,0xff6d,0xff7d,0xff07,0xff7f,0xff6f,0xff40};
NVIC_InitTypeDef NVIC_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;


void Delay(vu32 nCount);
void GPIO_Configuration(void);
void keyscan(void);
void keypro(void);
void modeset(void);
void liudong(void);
void up_down(void);
void time (void);

void keyscan()
{
		if (key1==0)
		{
			Delay(10);
			if (key1==0)
				flag=1;
				while (!key1);
		}
		if (key2==0)
		{
			Delay(10);
			if (key2==0)
				//flag=2;
				h++;
				while (!key2);
		}
		if (key3==0)
		{
			Delay(10);
			if (key3==0)
				flag=3;
		}
}

void keypro()
{
		keyscan();
		if (flag==1)
			modeset();
		if (h==1|h==2)
		{	
			//h++;
			modeset();
			//flag=0;
		}
		if (h>2)
			h=0;
		if (flag==3)
		{
			if (h==0)
			{
					time();
					flag=0;
					h=0;
			}
			if (h==1)
			{
				liudong();
				flag=0;
				h=0;
			}
			if (h==2)
			{
					up_down();
					//time();
					flag=0;
					h=0;
			}
		}
}


void time ()
{
		while (1)
		{
			if (k1<6)			
			{
				for (j=0;j<8;j++)
				{
						k1=sec%10;
						for (k=0;k<200;k++)
						{
								for (i=0;i<8;i++)
								{
									GPIO_Write(GPIOB,taba[i+j]&0x00ff);
									GPIO_Write(GPIOA,time_codege[i+k1*8] & 0x00ff);
									Delay(0XFFF);
								}
						}
				}

				for (j=0;j<8;j++)
				{
						k1=sec/10;
						for (k=0;k<200;k++)
						{
								for (i=0;i<8;i++)
								{
									GPIO_Write(GPIOB,taba[i+j]&0x00ff);
									GPIO_Write(GPIOA,time_codeshi[i+k1*8] & 0x00ff);
									Delay(0XFFF);
								}
						}
				}

				
			}
		}		
}

//void timer_init(u16 arr,u16 psc);
void GPIO_Configuration()
{
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);  //关闭调试 端口重新映射  使用仿真器调试时，不能用此语
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ; //所有GPIO为同一类型端口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //输出的最大频率为50HZ
  GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化GPIOB端口
  GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOB端口
}

void modeset()
{
			for (i=0;i<8;i++)
			{
				GPIO_Write(GPIOB,taba[i]&0x00ff);
				GPIO_Write(GPIOA,mode[i+8*h] & 0x00ff);
			}
}


void up_down()
{
	for (j=0;j<8;j++)
	{
	  for(k=0;k<200;k++)
		{
			for(i=0;i<8;i++)
				{
					GPIO_Write(GPIOB,tabe[i]& 0X00FF);
					GPIO_Write(GPIOA,tabb[i+j] & 0x00ff);
					Delay(0XfFF);
				}
	  }
	}
}


void liudong ()
{
					for (h1=0;h1<2;h1++)
					{
						for(j=0;j<8;j++)
							{
								for (k=0;k<200;k++)
								{
								for(i=0;i<8;i++)
									{
											GPIO_Write(GPIOB,taba[i+j]&0x00ff);		//将数据送到P2口 并屏蔽高位，不干扰高8位IO的使用
											GPIO_Write(GPIOA,tabc[i+8*h1] & 0x00ff);
											Delay(0XFFF);
										}
										//GPIO_Write(GPIOB,tabd[j] & 0x00ff);
									}
								}
							}
}


void timer_init(u16 arr,u16 psc)
{
		RCC->APB1ENR|=1<<1;
		TIM3->ARR=arr;
		TIM3->PSC=psc;
		TIM3->DIER|=1<<0;
		TIM3->DIER|=1<<6;
		TIM3->CR1=0X01;
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	}
int main()
{
		GPIO_Configuration();
		timer_init(500,7199);
		while(1)
		{
				keypro();
// 				i=sec%10;
// 				j=sec/10;
// 				k=min%10;
// 				i1=min/10;
// 				j1=hour%10;
// 				k1=hour/10;
		}
}
void TIM3_IRQHandler(void)
{
		if(TIM3->SR&0X0001)
		{
				count++;
				if (count>19)
				{
					count=0;
					sec++;
					if (sec>59)
					{
							sec=0;
							min++;
							if (min>59)
							{
									min=0;
									hour++;
									if(hour>23)
										hour=0;
							}
					}
				}
		}
		TIM3->SR&=~(1<<0);
}
void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}


