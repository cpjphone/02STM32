#include "stm32f10x.h"
//#include "key.h"
#define uint unsigned int
#define uchar unsigned char
//#define TIM2 TIM3
uchar aa[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uchar bb[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
uint a=0;
NVIC_InitTypeDef NVIC_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
void delay(uint n);
//void timer_init(u16 arr,u16 psc);
void ledinit()
{
		RCC->APB2ENR|=1<<3;
		RCC->APB2ENR|=1<<2;
		GPIOB->CRH&=0X00000000;
		GPIOB->CRH|=0X33333333;
}
void timer_init(u16 arr,u16 psc)
{
		RCC->APB1ENR|=1<<0;
		TIM2->ARR=arr;
		TIM2->PSC=psc;
		TIM2->DIER|=1<<0;
		TIM2->DIER|=1<<6;
		TIM2->CR1=0X01;
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//中断分组
		NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn;				//中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;		//抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;  		//次优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//中断通道使能
    NVIC_Init(&NVIC_InitStructure);
	}
int main()
{
		ledinit();
		timer_init(10000,7199);
		while(1)
		{
			GPIOB->ODR=bb[a]<<8;
			//timer_init();
		}
}
void TIM2_IRQHandler(void)
{
		if(TIM2->SR&0X0001)
		{
				//GPIOB->ODR=0Xc0<<8;
				//delay(0xffffff);
				if (a<9)
					a++;
				else 
					a=0;
				//a<9?a++:a==0;
		}
		TIM2->SR&=~(1<<0);
}
void delay(uint n)
{
		for(; n>0; n--);
}
