/*********************************************************************************      
* 【语    言】：C语言
* 【编译环境】：Keil4  
* 【程序功能】：通过中断请求按键控制LED灯亮灭
* 【技术支持】：				
* 【晶    振】: RCC 8*9=72MHz		
* 【芯    片】: stm32f103VET6 
* 【日    期】: 2013/6/03
* 【修改日期】: 2013/10/06-10
* 【作    者】: Phone
* 【编程理念】: 死东西活用，活东西套用。
*********************************************************************************/

#include "sys.h"								              //定义头文件

uchar flag,k,temp,count,flag_t,
	  shan1 = 0,shan = 0,setadd = 0,flagadd = 0;
char sec,min,hour;						                      
															  //定义 变量
#define keyadd  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)		  //连+按键，判断连加时用到
#define keyadd2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)		  //连-按键，判断连加时用到

NVIC_InitTypeDef NVIC_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	              //定义为对应的结构体

/************************************************************************/
//（死东西）可调用数组
/************************************************************************/
uchar XS_tab[] = {
	0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x40,0x80,0x00}
	;

/************************************************************************/
//（活东西）显示缓存
/************************************************************************/
uchar disp_buffer[8] ={0}; 

/************************************************************************/
//数码管显示函数
/************************************************************************/
void display()
{
	uchar i,temp;	
	temp = 0xfe;
	for(i=0;i<8;i++)
	{		
		GPIO_Write(GPIOA, temp+0xff00);
		GPIO_Write(GPIOB,XS_tab[disp_buffer[i]]<<8);
		wait(0.002);
		GPIO_Write(GPIOB,0x00<<8);			
		temp = (temp<<1)|0x01;	
	}
}

/************************************************************************/
//调整显示函数
/************************************************************************/
void displayset()	
{
	uchar i,temp;
	temp = 0xfe;
	for(i=0;i<8;i++)
	{
		//P2 = temp;
		GPIO_Write(GPIOA, temp+0xff00);
		if(flag_t == 2)		 //调时
		{
			if(i == 0 || i == 1)
			{
				if(shan != 0)
					//P1 = tab[12];
					GPIO_Write(GPIOB,XS_tab[12]<<8);
				else
					//P1 = tab[disp_buffer[i]];
					GPIO_Write(GPIOB,XS_tab[disp_buffer[i]]<<8);	
			}
			else
				//P1 = tab[disp_buffer[i]];
				GPIO_Write(GPIOB,XS_tab[disp_buffer[i]]<<8);
		}
		if(flag_t == 1)		   //调分
		{
			if(i == 3 || i == 4)
			{
				if(shan != 0)
					//P1 = tab[12];
					GPIO_Write(GPIOB,XS_tab[12]<<8);
				else
					//P1 = tab[disp_buffer[i]];
					GPIO_Write(GPIOB,XS_tab[disp_buffer[i]]<<8);	
			}
			else
				//P1 = tab[disp_buffer[i]];
				GPIO_Write(GPIOB,XS_tab[disp_buffer[i]]<<8);
		}
		wait(0.002);
		//P1 = 0x00;
		GPIO_Write(GPIOB,0x00<<8);
		//temp = _crol_(temp,1);
		temp = (temp<<1)|0x01;	
	}
}

/************************************************************************/
//按键输入函数
/************************************************************************/
uchar keyscan()
{
	uchar temp,ktemp;
	temp = GPIO_ReadInputData(GPIOA)>>8&0x0f;
	temp=~(temp|0xf0); 
	if(temp  == 0)
		return(0);
	delay_ms(5);
	temp = GPIO_ReadInputData(GPIOA)>>8&0x0f;
	temp=~(temp|0xf0); 
	if(temp  == 0)
		return(0);
	while(1)
	{			
		ktemp = GPIO_ReadInputData(GPIOA)>>8&0x0f;
		PBout(1) = 0;			 		
		if((ktemp | 0xf0) == 0xff)	
			break;		
	}
		PBout(1) = 1;	
	return temp;
}

/************************************************************************/
//funtion:（1）按键扫描程序：带有参数返回的，返回按键值；
//	   	  （2）按下5S 制动加,减	
/************************************************************************/
uchar keyscan2()
{
	uchar temp,ktemp;
	temp = GPIO_ReadInputData(GPIOA)>>8&0x0f;
	temp=~(temp|0xf0); 
	if(temp  == 0)
		return(0);
	delay_ms(20);
	temp = GPIO_ReadInputData(GPIOA)>>8&0x0f;
	temp=~(temp|0xf0); 
	if(temp  == 0)
		return(0);
	flagadd = 0;
	shan1 = 0;
	while(1)
	{
		if((flagadd == 1)&(shan1 != 0))
		{
			if(keyadd == 0)	   //判断连加按键为‘ + ‘键 , 此判断必须在 while（1）中
			{
				if(flag_t == 1)
				{
					min++;
					if(min > 59)
						min = 0;
				}
				if(flag_t == 2)
				{
					hour++;
					if(hour > 23)
						hour = 0;
				}					
			}
			if(keyadd2 == 0)	   //判断连加按键为‘ - ‘键 , 此判断必须在 while（1）中
			{
				if(flag_t == 1)
				{
					min--;
					if(min <= 0)
						min = 59;
				}
				if(flag_t == 2)
				{
					hour--;
					if(hour <= 0)
						hour = 24;
				}					
			}					
		}
		disp_buffer[1] = hour%10;
		disp_buffer[0] = hour/10;
		disp_buffer[4] = min%10;
		disp_buffer[3] = min/10;
		disp_buffer[7] = sec%10;
		disp_buffer[6] = sec/10;
			display();		
		//ktemp = P3;
		ktemp = GPIO_ReadInputData(GPIOA)>>8&0x0f;
		//display();
		//if((ktemp | 0xc3) == 0xff)
		if((ktemp | 0xf0) == 0xff)
			break;
	}
	flagadd = 0;
	shan1 = 0;		
	return temp;
}
/************************************************************************/
//按键输入处理函数
/************************************************************************/
void kpro(uchar temp)
{
	if(temp == 1)
		flag = 1;
	if(temp == 2)
		flag = 2;
	if(temp == 4)
		flag = 3;
	if(temp == 8)
		flag = 4;
//	else
//		flag = 0;		  
}

/************************************************************************/
//按键输入处理函数
/************************************************************************/
void kpro2(uchar temp)
{
	if(temp == 1)
	//	flag = 1;
	{
		if(flag_t == 1)
		{
			min++;
			if(min > 59)
				min = 0;
		}
		if(flag_t == 2)
		{
			hour++;
			if(hour > 23)
				hour = 0;
		}
	}

	if(temp == 2)
	//	flag = 2;
	{
		if(flag_t == 1)
		{
			min--;
			if(min <= 0)
				min = 59;
		}
		if(flag_t == 2)
		{
			hour--;
			if(hour <= 0)
				hour = 23;
		}
	}

	if(temp == 4)
	//	flag = 3;
	{
		flag_t++;
		if(flag_t > 2)
		{
			flag_t = 0;
			sec = 0;
		}
	}

//	if(temp == 8)
	//	flag = 4;
//	else
//		flag = 0;		  
}

/************************************************************************/
//定时器4中断配置函数
/************************************************************************/
void timer_init(u16 arr,u16 psc)
{
		RCC->APB1ENR|=1<<2; //TIM4时钟使能    
	 	TIM4->ARR=arr;  	//设定计数器自动重装值//刚好1ms   ,最多计时6.5536 s 
		TIM4->PSC=psc;  	//预分频器7200,得到10Khz的计数时钟
							//这两个东东要同时设置才可以使用中断
		TIM4->DIER|=1<<0;   //允许更新中断				
	 	TIM4->DIER|=1<<6;   //允许触发中断	   
		TIM4->CR1|=0x01;    //使能定时器3
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitStructure.NVIC_IRQChannel =TIM4_IRQn;
	    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
	    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;  
	    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init(&NVIC_InitStructure);
}
/************************************************************************/
//定时器3中断配置函数
/************************************************************************/
void timer_init3(u16 arr,u16 psc)
{
		RCC->APB1ENR|=1<<1; //TIM3时钟使能    
	 	TIM3->ARR=arr;  	//设定计数器自动重装值//刚好1ms    
		TIM3->PSC=psc;  	//预分频器7200,得到10Khz的计数时钟
							//这两个东东要同时设置才可以使用中断
		TIM3->DIER|=1<<0;   //允许更新中断				
	 	TIM3->DIER|=1<<6;   //允许触发中断	   
		TIM3->CR1|=0x01;    //使能定时器3
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;
	    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
	    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;  
	    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init(&NVIC_InitStructure);
}
/************************************************************************/
//CPU延时函数
/************************************************************************/
void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}

/************************************************************************/
//主函数
/************************************************************************/
int main()
{	
	void LED_Init();			//声明
	LED_Init();		  		    //调用，初始化与LED连接的硬件接口
	delay_init(72);	     	    //调用，延时初始化
	timer_init(10000,7199);	    //10Khz，10000Hz的计数频率，计数到5000为500ms  	  计算器时钟频率（CK_CNT） = （fCK_PSC(72M) / PSC[15：0](最大为2的16次方) + 1）；此时 = 72M/（7199 + 1）= 10k
	timer_init3(2000,7199);
	GPIOA->ODR|=0Xffff;
	GPIOB->ODR|=0Xffff;
	while(1)
	{
//		if(shan != 0)
//	    PBout(0) = 0; 
//		if(shan == 0)
//		PBout(0) = 1; 
//		PBout(1) = ~shan;
		k = keyscan2();
		if(k != 0)	
		kpro2(k)
		;		
		disp_buffer[2] = 10;
		disp_buffer[5] = 11;
		disp_buffer[1] = hour%10;
		disp_buffer[0] = hour/10;
		disp_buffer[4] = min%10;
		disp_buffer[3] = min/10;
		disp_buffer[7] = sec%10;
		disp_buffer[6] = sec/10;
//		for(i=0;i<228;i++)
//		display();
		if(flag_t != 0)					//判断是否调整
			displayset();				//调整时 调整位置闪烁
		else
			display();
//		if(1 == flag)
//			PBout(0) = 0;
//		else 
//			PBout(0) = 1;
		
		//	GPIO_ResetBits(GPIOB,GPIO_Pin_8); 
		//GPIOB->ODR=0Xffff;
		//GPIO_Write(GPIOB,temp);
		//temp = ~temp;
		//wait(0.5);
		//			GPIO_Write(GPIOA, ~Val);// | read_val ); //写一个字数据到PB口
		//			wait(0.5); // 延时
		//			if(Val == 0x8000) Val = 1;
		//			else Val<<=1;
		//	GPIO_Write(GPIOA, 0x0000);			
	}
}

/************************************************************************/
//定时器4中断服务函数
/************************************************************************/
void TIM4_IRQHandler(void)
{
	if(TIM4->SR&0X0001)		    //溢出中断   	   时间到了！
	{	 
		setadd++;
		if(setadd > 3)
		{
			setadd = 0;
			flagadd = 1;
		}
		if(flag_t == 0)
			sec++;
		if(sec > 59)
		{
			sec = 0;
			min++;
			if(min >59)
			{
				min = 0;
				hour ++;
				if(hour > 23)
				{
					hour = 0;
				}
			}		
		}			
	}
		TIM4->SR&=~(1<<0);		//清除中断标志位 
}

/************************************************************************/
//定时器3中断服务函数
/************************************************************************/
void TIM3_IRQHandler(void)
{
	if(TIM3->SR&0X0001)		    //溢出中断   	   时间到了！
	{
		shan1 = ~shan1;
		count++;
		if(count >= 2)
		{
			count = 0;
			shan = ~shan;		
		}
				
	}
	TIM3->SR&=~(1<<0);		   //清除中断标志位 
}

/************************************************************************/
//successful
/************************************************************************/
