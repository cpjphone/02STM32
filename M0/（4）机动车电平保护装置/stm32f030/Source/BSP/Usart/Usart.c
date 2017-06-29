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

#define uint unsigned int
#define uchar unsigned char
    
char  aa[48];
unsigned char j = 0;


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

    /*
    *  USART1_TX -> PA9 , USART1_RX ->        PA10
    */                                
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;                 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);     
}

/****************************************************
函数功能：USART1初始化
输入参数：baud : 波特率
输出参数：无
备    注：8位数据长度 1位停止位 无奇偶校验 开放接收中断         
*****************************************************/
void Usart_Init(void)
{
	USART_InitTypeDef  USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;
    
	Usart_IOConfig();//串口引脚初始化

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//开启时钟

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//不使用硬件流控制
	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//串口接收中断配置

	USART_Cmd(USART1, ENABLE);//使能串口
    
    	/* USART1的NVIC中断配置 */
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x02;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
    
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
void Print_Str(unsigned char *str)//发送字符串
{
    while(*str!='\0')
    {
        USART_SendByte(*str);
        delay_ms(4);
        str++;
    }
}


/****************************************************
函数功能：重定义fputc函数
输入参数：ch : 发送1字节数据
输出参数：无
备    注：无         
*****************************************************/
//int fputc(int ch, FILE *f)
//{      
////	while((USART1->ISR & 0X40)==0);
//  while(!((USART1->ISR)&(1<<6)));//等待缓冲为空
//	USART1->TDR = (u8) ch;      
//	return ch;
//}

/****************************************************
函数功能：USART1中断函数
输入参数：无
输出参数：无
备    注：无         
*****************************************************/
void USART1_IRQHandler(void)
{
    // USART_IT_RXNE：Receive Data register not empty interrupt
    // USART_IT_RXNE：接受数据寄存器非空标志位
    if(USART_GetITStatus(USART1, USART_IT_RXNE) ==SET)
    {
        aa[j] = USART_ReceiveData(USART1);  	
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		j++;      
        if(j > 47)  j = 0;
//        USART_SendData(USART1,USART_ReceiveData(USART1));
//        while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
    }	
    
    //溢出-如果发生溢出需要先读SR,再读DR寄存器则可清除不断入中断的问题[牛人说要这样]
    //OverRun Error flag: 超出标志位
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART1,USART_FLAG_ORE);
	}
}

/****************************************************
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
*****************************************************/
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}







