/*
		串口发送数据控制led灯

*/
#include "stm32f10x.h"
#define uint unsigned int 
void RCC_Configuration(void);
void GPIO_Configuration(void);
void USART_Configuration(void);
void NVIC_Configuration(void);

unsigned char i,temp=0; 

u8 Uart1_PutChar(u8 ch)
{
    USART_SendData(USART1, (u8) ch);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
   {
   }
   return ch;
}

void Uart1_PutString(u8* buf , u8 len)
{   u8 i;
    for(i=0;i<len;i++)
    {
        Uart1_PutChar(*buf++);
    }
}

void delay(uint n)
{
		for(;n>0;n--);
}

int main(void)
{
  RCC_Configuration();
  GPIO_Configuration();
  NVIC_Configuration();
  USART_Configuration();
 
  while(1)
	{
				if (temp==(1+0x30))
					GPIO_SetBits (GPIOA,GPIO_Pin_0);
				if (temp=='2')
					GPIO_ResetBits (GPIOA,GPIO_Pin_0);
				if (temp>2)
					temp=0;
	}
}
void RCC_Configuration(void)
{    
  
	
}
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void USART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  USART_InitStructure.USART_BaudRate = 1200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_Init(USART1 , &USART_InitStructure);
  USART_Cmd(USART1, ENABLE);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}
void NVIC_Configuration(void) 
{ 
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
}

void USART1_IRQHandler(void) 
{ 
    if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET) 
    {               
        i = USART_ReceiveData(USART1); 
				//j=atoi(i);			//把字符串转换为整形
				temp=i;
				Uart1_PutChar (i);
// 				if (temp==(1+0x30))
// 				GPIO_SetBits (GPIOA,GPIO_Pin_0);
// 				if (temp=='2')
// 				GPIO_ResetBits (GPIOA,GPIO_Pin_0);
// 				if (temp>2)
// 					temp=0;
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);  
				USART_ClearFlag (USART1,USART_FLAG_RXNE);
    }
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
    { 
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
