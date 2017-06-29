#include "stm32f10x.h"
//#include "key.h"
#define uint unsigned int
#define uchar unsigned char
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
		timer_init(50000,7199);
		while(1)
		{
			GPIOB->ODR=0X76<<8;
			//timer_init();
		}
}
void TIM3_IRQHandler(void)
{
		if(TIM3->SR&0X0001)
		{
				GPIOB->ODR=0Xc0<<8;
				delay(0xffffff);
		}
		TIM3->SR&=~(1<<0);
}
void delay(uint n)
{
		for(; n>0; n--);
}
