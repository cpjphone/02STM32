#include "sys.h"
unsigned char temp,count = 0;

NVIC_InitTypeDef NVIC_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	   /*定义为对应的结构体*/

int main()
{
	
		void LED_Init();			//声明
		LED_Init();		  		    //调用，初始化与LED连接的硬件接口
		delay_init(72);	     	    //调用，延时初始化
		//timer_init(500,7199);	    //10Khz，10000Hz的计数频率，计数到5000为500ms  	  计算器时钟频率（CK_CNT） = （fCK_PSC(72M) / PSC[15：0](最大为2的16次方) + 1）；此时 = 72M/（7199 + 1）= 10k
		RCC->APB1ENR|=1<<2; //TIM4时钟使能    
	 	TIM4->ARR=1;  	//设定计数器自动重装值//刚好1ms   ,最多计时6.5536 s 
		TIM4->PSC=3599;  	//预分频器7200,得到10Khz的计数时钟
							//这两个东东要同时设置才可以使用中断
		TIM4->DIER|=1<<0;   //允许更新中断				
	 	TIM4->DIER|=1<<6;   //允许触发中断	   
		//TIM4->CR1|=0x01;    //使能定时器3
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitStructure.NVIC_IRQChannel =TIM4_IRQn;
	    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
	    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;  
	    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init(&NVIC_InitStructure);
		GPIOB->ODR|=0Xffff;
		while(1)
		{	
			GPIO_Write(GPIOB,0xff<<8);
			TIM4->CR1|=0x01;    //使能定时器3
			if(count > 0)
			GPIO_Write(GPIOB,0xfe<<8);
			if(count >25)
			GPIO_Write(GPIOB,0xfc<<8);
			if(count >50)
			GPIO_Write(GPIOB,0xf8<<8);
			if(count > 75)
			GPIO_Write(GPIOB,0xf0<<8);
			if(count > 100)
			GPIO_Write(GPIOB,0xe0<<8);
			if(count >125)
			GPIO_Write(GPIOB,0xc0<<8);
			if(count > 150)
			GPIO_Write(GPIOB,0x80<<8);
			if(count > 175)
			GPIO_Write(GPIOB,0x00<<8);
				
		}
}

void TIM4_IRQHandler(void)
{
		if(TIM4->SR&0X0001)		 //溢出中断   	   时间到了！
		{
			count++;
			if(count >200)
			{
				count = 0;
			}	
	  	 }
		TIM4->SR&=~(1<<0);		//清除中断标志位 
}

