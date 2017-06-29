#include <stm32f10x.h>	   
//#include "led.h"

void LED_Init()
{
		RCC->APB2ENR|=1<<3;				  // 使能GPIOB外设时钟	 
		RCC->APB2ENR|=1<<2;				  // 使能GPIOA外设时钟
		
		GPIOB->CRH&=0X00000000;
		GPIOB->CRH|=0X33333333;			  // 0011通用推挽输出，速度50MHz
		GPIOB->CRL&=0X00000000;
		GPIOB->CRL|=0X33333333;			  // 0011通用推挽输出，速度50MHz
		GPIOA->CRL&=0X00000000;
		GPIOA->CRL|=0X33333333;			  // 0011通用推挽输出，速度50MHz
		GPIOA->CRH&=0X00000000;
		GPIOA->CRH|=0X33333333;			  // 0011通用推挽输出，速度50MHz
		GPIOB->ODR|=0xffff;
		GPIOA->ODR|=0xffff;
		//GPIO_Write(GPIOB,0xffff);
}
