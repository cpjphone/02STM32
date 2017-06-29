#include "stm32f10x.h"
#include"Function.h"
//#include "key.h"
#define uint unsigned int
#define uchar unsigned char
#define  AT24C02 0xa0
#define    Save      GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_8)//P1^0;	 //保存
#define    Read      GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_9)//P1^1;	 //读取
#define    LS138A  GPIO_Pin_5 //=P2^2;       //138译码器管脚定义
#define    LS138B  GPIO_Pin_4 //=P2^3;
#define    LS138C  GPIO_Pin_3 //=P2^4;
#define    LS138a(x)  x ? GPIO_SetBits(GPIOB, LS138A): GPIO_ResetBits(GPIOB, LS138A)
#define    LS138b(x)  x ? GPIO_SetBits(GPIOB, LS138B): GPIO_ResetBits(GPIOB, LS138B)
#define    LS138c(x)  x ? GPIO_SetBits(GPIOB, LS138C): GPIO_ResetBits(GPIOB, LS138C)
uchar aa[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uchar Disp_Tab[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
uint a=0;
uchar Count1 , Count2 ,i,save_flag = 1 , read_flag = 1,pDat[8];
unsigned long D[16], LedOut[8],LedNumVal , LedNumVal1;
NVIC_InitTypeDef NVIC_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
GPIO_InitTypeDef GPIO_InitStructure;     //GPIO
ErrorStatus HSEStartUpStatus;
//void Delay_us(uint n);
//void timer_init(u16 arr,u16 psc);
/*void ledinit()
{
		RCC->APB2ENR|=1<<3;
		RCC->APB2ENR|=1<<2;
		GPIOA->CRH&=0X00000000;
		GPIOA->CRH|=0X33333333;
}*/
void ledinit()
{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB1Periph_TIM2 | RCC_APB2Periph_AFIO, ENABLE);
			//把调试设置普通IO口
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);  
															
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; //所有GPIO为同一类型端口
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 //推挽输出
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //输出的最大频率为50HZ
			GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA端口
			GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化GPIOB端口

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入	 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //输出的最大频率为50HZ
			GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化GPIOA端口

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7  ;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  //开漏输出	 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //输出的最大频率为50HZ
			GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化GPIOA端口
			
			GPIO_Write(GPIOA,0xffff);  //将GPIOA 16个端口全部置为高电平
			GPIO_Write(GPIOB,0xffff);  //将GPIOB 16个端口全部置为高电
}
void timer_init(u16 arr,u16 psc)
{
		RCC->APB1ENR|=1<<0;
		TIM2->ARR=arr;
		TIM2->PSC=psc;
		TIM2->DIER|=1<<0;
		TIM2->DIER|=1<<6;
		TIM2->CR1=0X01;
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//????
		NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn;				//????
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;		//?????
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;  		//????
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//??????
    NVIC_Init(&NVIC_InitStructure);
	}
int main()
{
		ledinit();
		timer_init(10000,7199);
		while(1)
		{
			//GPIOB->ODR=bb[a]<<8;
			//timer_init();
				if(Read  == 0 && read_flag)	   //第二个按钮读取数据
				{ 
							IRcvStr(AT24C02, 0 , &pDat[0], 8); 
							for (i=0; i<4; i++)
							{
									D[14+i]=pDat[i*2+0]+pDat[i*2+1]*0x100;
									Count2 = D[14];
							}
								read_flag = 0;
								save_flag = 1;	     
				 }
				 	   if(Save == 0 && save_flag)    //第一个按钮保存数据
					 { 
								//D[14]= Count1;
								D[14]= a;
								for (i=0; i<4; i++)
								{
										pDat[i*2+0]=D[14+i];
										pDat[i*2+1]=D[14+i]>>8;
									}
									ISendStr(AT24C02, 0 , &pDat[0], 8); 
									read_flag = 1;
									save_flag = 0;
						 }
							LedNumVal=Count2;
							LedOut[0]=Disp_Tab[LedNumVal%10000/1000];
							LedOut[1]=Disp_Tab[LedNumVal%1000/100];
							LedOut[2]=Disp_Tab[LedNumVal%100/10]|0x80;
							LedOut[3]=Disp_Tab[LedNumVal%10];
						
							//LedNumVal1=Count1;
							LedNumVal1=a;
							LedOut[4]=Disp_Tab[LedNumVal1%10000/1000];
							LedOut[5]=Disp_Tab[LedNumVal1%1000/100];
							LedOut[6]=Disp_Tab[LedNumVal1%100/10]|0x80;
							LedOut[7]=Disp_Tab[LedNumVal1%10];
						 
						       for(i=0; i<8; i++) 
										{		
											GPIOA->BSRR = LedOut[i] & 0x00FF;   
											GPIOA->BRR = (~LedOut[i]) & 0x00FF;
																
									switch(i)					  
											 {	    	  //138译码
										case 0:	  //0 0 0 
										LS138a(0);	 LS138b(0);  LS138c(0);
										break;         
													case 1:	 //1 0 0 
										LS138a(1);	 LS138b(0);  LS138c(0);
										 break;             	
													case 2:	 //0 1 0
										LS138a(0);	 LS138b(1);  LS138c(0);
										break; 
													case 3:	 //1 1 0
										LS138a(1);	 LS138b(1);  LS138c(0);
										break;
										
										case 4:	 //0 0 1
										LS138a(0);	 LS138b(0);  LS138c(1);
										break; 
										
										case 5:	 //1 0 1
										LS138a(1);	 LS138b(0);  LS138c(1);
										break; 

										case 6:	 //0 1 1
										LS138a(0);	 LS138b(1);  LS138c(1);
										break;

										case 7:	 //1 1 1
										LS138a(1);	 LS138b(1);  LS138c(1);
										break;

										
									 
											 }
											Delay_us(700); 
									}
		}
}
void TIM2_IRQHandler(void)
{
		if(TIM2->SR&0X0001)
		{
				if (a<9)
					a++;
				else 
					a=0;
				//a<9?a++:a==0;
		}
		TIM2->SR&=~(1<<0);
}
void Delay_us(uint n)
{
		for(; n>0; n--);
}
