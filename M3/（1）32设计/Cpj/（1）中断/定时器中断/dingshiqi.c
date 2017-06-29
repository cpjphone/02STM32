
#include "sys.h"

//#define TRUE  1
//#define FALSE 0
//
//enum BOOL{FALSE,TRUE} ; 
//typedef enum{error = 0,
//success =!error}Errorstatus;
//typedef enum { 译文仅供参考，与英文版冲突的，以英文版为准
//				FALSE = 0, 
//				TRUE = !FALSE 
//			  } bool;

unsigned int temp = 0xfff0;
uchar temp2 = 0,temp3 = 0;

NVIC_InitTypeDef NVIC_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	   /*定义为对应的结构体*/

void timer_init(u16 arr,u16 psc)
{
//		RCC->APB1ENR|=1<<1; //TIM3时钟使能    
//	 	TIM3->ARR=arr;  	//设定计数器自动重装值//刚好1ms    
//		TIM3->PSC=psc;  	//预分频器7200,得到10Khz的计数时钟
//							//这两个东东要同时设置才可以使用中断
//		TIM3->DIER|=1<<0;   //允许更新中断				
//	 	TIM3->DIER|=1<<6;   //允许触发中断	   
//		TIM3->CR1|=0x01;    //使能定时器3
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//		NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;
//	    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
//	    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;  
//	    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	    NVIC_Init(&NVIC_InitStructure);

		RCC->APB1ENR|=1<<2; //TIM4时钟使能    					        没有这一句中断不起作用
	 	TIM4->ARR=arr;  	//设定计数器自动重装值//刚好1ms   ,最多计时6.5536 s 
		TIM4->PSC=psc;  	//预分频器7200,得到10Khz的计数时钟
							//这两个东东要同时设置才可以使用中断
		TIM4->DIER|=1<<0;   //允许更新中断							   	没有这一句中断不起作用
	 	TIM4->DIER|=1<<6;   //允许触发中断	   
		TIM4->CR1|=0x01;    //使能定时器3								没有这一句中断不起作用
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitStructure.NVIC_IRQChannel =TIM4_IRQn;
	    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;	  //抢占优先级
	    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;  			  //响应优先级
	    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init(&NVIC_InitStructure);
}

void timer_init3(u16 arr,u16 psc)
{
		RCC->APB1ENR|=1<<1; //TIM3时钟使能    					   √
	 	TIM3->ARR=arr;  	//设定计数器自动重装值//刚好1ms    
		TIM3->PSC=psc;  	//预分频器7200,得到10Khz的计数时钟
							//这两个东东要同时设置才可以使用中断
	//	TIM3->DIER|=1<<0;   //允许更新中断					       √
							//建议关闭中断的时候用： TIM3->DIER|=0<<0;		☆
	 	TIM3->DIER|=1<<6;   //允许触发中断	   					   ×
		TIM3->CR1|=0x01;    //使能定时器3						   √
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;		//     √
	    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
	    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;  
	    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init(&NVIC_InitStructure);
}

int main()
{	
	void LED_Init();			//声明	
	LED_Init();		  		    //调用，初始化与LED连接的硬件接口
	delay_init(72);	     	    //调用，延时初始化
	timer_init(5,71);	    //1000Khz，1000000Hz的计数频率，计数到5为5us  	  计算器时钟频率（CK_CNT） = （fCK_PSC(72M) / PSC[15：0](最大为2的16次方) + 1）；此时 = 72M/（7199 + 1）= 10k
	timer_init3(20000,7199);	
	GPIOB->ODR|=0Xffff;
	temp3 = 1;
	TIM3->DIER|=1<<0;
	while(1)
	{
		//while(!temp)
		//RCC->APB1ENR|=1<<1;
		TIM3->DIER|=0<<0;
		PBout(7) = temp3;
		//	GPIO_ResetBits(GPIOB,GPIO_Pin_8); 
		//GPIOB->ODR=0Xffff;
		//GPIO_Write(GPIOA,0X00ff);
		
	//	GPIO_WriteBit(GPIOB, GPIO_Pin_0, temp2);
	//	GPIO_WriteBit(GPIOB, GPIO_Pin_1, temp3);
//		PAout(0) = temp2;
//		PAout(1) = temp3;		
		//PBout(8) = 1;	
	}
}

void TIM4_IRQHandler(void)
{
		if(TIM4->SR&0X0001)		 //溢出中断   	   时间到了！
		{
				//PBout(8) = 0;
				//	GPIO_SetBits(GPIOB,GPIO_Pin_8); 
			//	GPIO_Write(GPIOB,temp);
			temp++;
			temp = 0;
			//temp2 = ~temp2;
				//PBout(8) = 0;				
				//PBout(9) = 1;
				 // GPIOB->ODR=0x00ff;
				//PBout(9) = 1;
				//GPIOB->ODR=0Xf0<<8;
				// delay_ms(1050);
				//wait(2.100);	 //SysTick 写的可任意延迟时间函数
		}
		TIM4->SR&=~(1<<0);		//清除中断标志位 
}
void TIM3_IRQHandler(void)
{
		if(TIM3->SR&0X0001)		 //溢出中断   	   时间到了！
		{
				//PBout(8) = 0;
				//	GPIO_SetBits(GPIOB,GPIO_Pin_8); 
			//	GPIO_Write(GPIOB,temp);
			
			temp3 = ~temp3;
			
				//PBout(8) = 0;				
				//PBout(9) = 1;
				 // GPIOB->ODR=0x00ff;
				//PBout(9) = 1;
				//GPIOB->ODR=0Xf0<<8;
				// delay_ms(1050);
				//wait(2.100);	 //SysTick 写的可任意延迟时间函数
		}
		TIM3->DIER|=0<<0;
		TIM3->SR&=~(1<<0);		//清除中断标志位 
}

