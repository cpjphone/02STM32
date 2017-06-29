/****************************/
//		GPIOB<<8		ฮปัก
//		GPIOA		ตอ8	ถฮัก
/****************************/
#include "stm32f10x.h"

#define uint unsigned int
#define uchar unsigned char

#define key1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
#define key2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)
#define key3 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)
#define key4 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)

//uchar aa[]={0xC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90};
//u16 bb[]={0xFFC0,0XFFF9,0XFFA4,0XFFB0,0XFF99,0XFF92,0XFF82,0XFFF8,0XFF80,0XFF90};
uint count=0,sec=0,min=0,hour=0,i,j,k,i1,j1,k1,flag;
u16 aa[]={0xff3f,0xff06,0xff5b,0xff4f,0xff66,0xff6d,0xff7d,0xff07,0xff7f,0xff6f,0xff40};
NVIC_InitTypeDef NVIC_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;


void delay(uint n);
void keyscan()
{
		if (key1==0)
		{
			delay(10);
			if (key1==0)
			{
			flag=1;
			while (!key1);
			}
		}
}
//void timer_init(u16 arr,u16 psc);
void ledinit()
{
		RCC->APB2ENR|=1<<3;
		RCC->APB2ENR|=1<<2;
		GPIOB->CRH&=0X00000000;
		GPIOB->CRH|=0X33333333;
		GPIOA->CRL&=0X00000000;
		GPIOA->CRL|=0X33333333;
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
		ledinit();
		timer_init(500,7199);
		//GPIO_Write(GPIOB,0x00ff);
		while(1)
		{
				keyscan();
				if (flag==1)
				{
						min++;
						flag=0;
						sec=0;
						if (min>59)
							min=0;
				}
				i=sec%10;
				j=sec/10;
				k=min%10;
				i1=min/10;
				j1=hour%10;
				k1=hour/10;
				GPIO_Write(GPIOB,0x7fff);
				GPIO_Write(GPIOA,aa[i]);
				delay(0x7ff);
				GPIO_Write(GPIOB,0xbfff);
				GPIO_Write(GPIOA,aa[j]);
				delay(0x7ff);
				GPIO_Write(GPIOB,0xdfff);
				GPIO_Write(GPIOA,aa[10]);
				delay(0x7ff);
				GPIO_Write(GPIOB,0xefff);
				GPIO_Write(GPIOA,aa[k]);
				delay(0x7ff);
				GPIO_Write(GPIOB,0xf7ff);
				GPIO_Write(GPIOA,aa[i1]);
				delay(0x7ff);
				GPIO_Write(GPIOB,0xfbff);
				GPIO_Write(GPIOA,aa[10]);
				delay(0x7ff);
				GPIO_Write(GPIOB,0xfdff);
				GPIO_Write(GPIOA,aa[j1]);
				delay(0x7ff);
				GPIO_Write(GPIOB,0xfeff);
				GPIO_Write(GPIOA,aa[k1]);
				delay(0x7ff);
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
void delay(uint n)
{
		for(; n>0; n--);
}


