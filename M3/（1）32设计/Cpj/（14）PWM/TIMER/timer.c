

#include "stm32f10x.h"
//#include "stdio.h"
#include "timer.h"
#include "led.h"

//定时器3中断服务程序	 
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{
		LED1=!LED1;			    				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}
//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void Timerx_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;//TIM3时钟使能    
 	TIM3->ARR=arr;  //设定计数器自动重装值//刚好1ms    
	TIM3->PSC=psc;  //预分频器7200,得到10Khz的计数时钟
	//这两个东东要同时设置才可以使用中断
	TIM3->DIER|=1<<0;   //允许更新中断				
//	TIM3->DIER|=1<<6;   //允许触发中断	   
	TIM3->CR1|=0x01;    //使能定时器3
  MY_NVIC_Init(1,3,TIM3_IRQn,2);//抢占1，子优先级3，组2									 
}




/****************************************************************
一个定时器输出四路【频率相同】，【占空比不同】的PWM脉冲
【此函数只实现了TIM3的CH1，CH2两路输出，四路输出类似】
【一个定时器在PWM模式只能输出频率相同，占空比可调的四路PWM脉冲】
【PWM=72M/(（ARR+1）*(PSC+1))】
****************************************************************/

void PWM_CH1_CH2(u16 arr,u16 psc)	
{	  	  
	RCC->APB1ENR|=1<<1;       //TIM3时钟使能 
	
	GPIOA->CRL&=0XF0FFFFFF;//PA6输出
	GPIOA->CRL|=0X0B000000;//复用功能输出 	  
	GPIOA->ODR|=1<<6;//PA6上拉
	  	
	GPIOA->CRL&=0X0FFFFFFF;//PA7输出
	GPIOA->CRL|=0XB0000000;//复用功能输出 	  
	GPIOA->ODR|=1<<7;//PA7上拉	

   
	
	TIM3->ARR=arr;//设定计数器自动重装值 
	TIM3->PSC=psc;//预分频器不分频
	
//	TIM3->CCR1=25;
//	TIM3->CCR2=125;
	
	TIM3->CCMR1|=6<<4;  //CH1 PWM2模式	  
	TIM3->CCMR1|=1<<3; //CH1预装载使能
	
	TIM3->CCMR1|=6<<12;  //CH2 PWM2模式	  
	TIM3->CCMR1|=1<<11; //CH2预装载使能	   
	

	TIM3->CCER|=1<<0;   //OC1 输出使能
	TIM3->CCER|=1<<4;   //OC2 输出使能
	   
	
	TIM3->CR1|=0x0080;   //ARPE使能 
	TIM3->CR1|=0x0001;   //使能定时器3 	   
} 
void PWM_CH3CH4(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<0;    //TIM2时钟使能

	GPIOA->CRL&=0XFFFFFFF0;//PA0输出
	GPIOA->CRL|=0X0000000B;//复用功能输出 	  
	GPIOA->ODR|=1<<0;      //PA0上拉

	GPIOA->CRL&=0XFFFFFF0F;//PA1输出
	GPIOA->CRL|=0X000000B0;//复用功能输出 	  
	GPIOA->ODR|=1<<1;      //PA1上拉	

	GPIOA->CRL&=0XFFFFF0FF;//PA2输出
	GPIOA->CRL|=0X00000B00;//复用功能输出 	  
	GPIOA->ODR|=1<<2;      //PA2上2拉

	GPIOA->CRL&=0XFFFF0FFF;//PA3输出
	GPIOA->CRL|=0X0000B000;//复用功能输出 	  
	GPIOA->ODR|=1<<3;      //PA3上拉	

	
	TIM2->ARR=arr;//设定计数器自动重装值 
	TIM2->PSC=psc;//预分频器不分频
	
//	TIM2->CCR2=125;
//	TIM2->CCR1=125;	
//	TIM2->CCR3=125;
//	TIM2->CCR4=125;
	
//	
	TIM2->CCMR1|=6<<4;   //CH2 PWM模式	  
	TIM2->CCMR1|=1<<3;   //CH1预装载使能
							  
	TIM2->CCMR1|=6<<12;   //CH2 PWM模式	  
	TIM2->CCMR1|=1<<11;   //CH2预装载使能

	TIM2->CCMR2|=6<<4;   //CH3 PWM模式	  
	TIM2->CCMR2|=1<<3;   //CH3预装载使能

	TIM2->CCMR2|=6<<12;   //CH3 PWM模式	  
	TIM2->CCMR2|=1<<11;   //CH3预装载使能

	TIM2->CR1|=0x0080;   //ARPE使能 
	TIM2->CR1|=0x0001;    //使能定时器2 

	TIM2 ->CCER  |= 1<<0; 
	TIM2 ->CCER  |= 1<<4;
	TIM2 ->CCER  |= 1<<8; 
	TIM2 ->CCER  |= 1<<12;
//	TIM2 ->CCR1 = 200;
}

void PWM_nj(u16 pwm_n,u16 pwm_j)
{
	pwm_j=pwm_j/1.8+25;
	switch(pwm_n)
	{
		case 1:
			flage_ccr1=pwm_j;
			break;
		case 2:
			flage_ccr2=pwm_j;
			break;
		case 3:
			flage_ccr3=pwm_j;
			break;
		case 4:
			flage_ccr4=pwm_j;
			break;
	}
}







