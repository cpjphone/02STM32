#include "stm32f10x.h"
//#include "delay.h"

//void delay_us(u32 n)
//{
//	u8 j;
//	while(n--)
//	for(j=0;j<10;j++);
//}
//void delay_ms(u32 n)
//{
//	while(n--)
//	delay_us(1000);
//}
void delay_us(u32 n)
{
	SysTick->LOAD= n*9;       //ʱ�����  
	SysTick->VAL= 0;        //��ռ�����      
 	SysTick->CTRL|= ENABLE;            //��ʼ����    
 	while(SysTick->VAL||!(SysTick->CTRL&(1<<16)));//�ȴ�ʱ�䵽�� 
 	SysTick->CTRL=0X00000000;       //�رռ�����
 	SysTick->VAL= 0;        //��ռ�����  
}
void delay_ms(u32 n)
{
	while(n--)
	{
		delay_us(1000);		
	}

}
