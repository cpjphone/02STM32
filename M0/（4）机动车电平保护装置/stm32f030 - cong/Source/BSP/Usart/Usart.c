/*************************************
              今明电子
凡在本店购买满30元，下次购买即可返还5角；
      满50元，下次购买即可返还1元；
     满100元，下次购买即可返还2元；
     满150元，下次购买即可返还4元；
      满200元，下次购买即可免邮
http://shop106001793.taobao.com/search.htm?spm=a1z10.5.w5002-3375901029.1.l1TXSl&search=y              
               V1.1
             2013.10.17
**************************************/


#include "Usart.h"
#include "includes.h"
#include <stdio.h>


/****************************************************
函数功能：USART1引脚初始化
输入参数：无
输出参数：无
备    注：PB6——USART1_TX
          PB7——USART1_RX
*****************************************************/
void Usart_IOConfig(void)
{
	  GPIO_InitTypeDef  GPIO_InitStructure;

	  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);

	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_1);//将IO口映射为USART接口
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_1); 
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;                 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);   
}

/****************************************************
函数功能：USART1初始化
输入参数：baud : 波特率
输出参数：无
备    注：8位数据长度 1位停止位 无奇偶校验 开放接收中断         
*****************************************************/
void Usart_Init(u32 baud)
{
	USART_InitTypeDef  USART_InitStructure;

	Usart_IOConfig();//串口引脚初始化

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//开启时钟

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//不使用硬件流控制
	USART_Init(USART1, &USART_InitStructure);

//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//串口接收中断配置

	USART_Cmd(USART1, ENABLE);//使能串口
}

/****************************************************
函数功能：USART1发送
输入参数：byte : 发送1字节数据
输出参数：无
备    注：无         
*****************************************************/
void USART_SendByte(u8 byte)
{
    while(!((USART1->ISR)&(1<<6)));//等待缓冲为空
    USART1->TDR = byte;//发送数据	
}

/****************************************************
函数功能：USART1发送
输入参数：byte : 发送一个字符串
输出参数：无
备    注：无         
*****************************************************/
//void Print_Str(u8* buf )
//{   
//	while(*buf!='\0')
//	{ 
//		USART_SendByte(*buf);
//		delay_ms(2);
//		buf++;
//	}
//}


/****************************************************
函数功能：重定义fputc函数
输入参数：ch : 发送1字节数据
输出参数：无
备    注：无         
*****************************************************/
int fputc(int ch, FILE *f)
{      
//	while((USART1->ISR & 0X40)==0);
  while(!((USART1->ISR)&(1<<6)));//等待缓冲为空
	USART1->TDR = (u8) ch;      
	return ch;
}

