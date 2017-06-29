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
	SysTick->LOAD= n*9;       //时间加载  
	SysTick->VAL= 0;        //清空计数器      
 	SysTick->CTRL|= ENABLE;            //开始倒数    
 	while(SysTick->VAL||!(SysTick->CTRL&(1<<16)));//等待时间到达 
 	SysTick->CTRL=0X00000000;       //关闭计数器
 	SysTick->VAL= 0;        //清空计数器  
}
void delay_ms(u32 n)
{
	while(n--)
	{
		delay_us(1000);		
	}

}
