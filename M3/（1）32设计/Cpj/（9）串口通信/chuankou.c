#include "stm32f10x.h"
#define uint unsigned int
// 			发送一个字符
u8 Uart1_PutChar(u8 ch)
{
    USART_SendData(USART1, (u8) ch);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
   {
   }
   return ch;
}
void delay(uint n)
{
		for(;n>0;n--);
}
void GPIO_Configuration()
{
		  GPIO_InitTypeDef GPIO_InitStructure;

  //开启AFIO时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

   //配置USARTx_Tx为复合推挽输出
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //配置 USARTx_Rx 为浮空输入
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
//    发送一个字符串
void Uart1_PutString(u8* buf , u8 len)
{   u8 i;
    for(i=0;i<len;i++)
    {
        Uart1_PutChar(*buf++);
    }
}
int main()
{
		USART_InitTypeDef USART_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		GPIO_Configuration();
	  //串口配置： 波特率 115200 数据位 8 停止位 1  奇偶位 NONE
		  USART_InitStructure.USART_BaudRate = 115200;
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			USART_InitStructure.USART_StopBits = USART_StopBits_1;
			USART_InitStructure.USART_Parity =  USART_Parity_No ;
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
				//初始化串口
			USART_Init(USART1, &USART_InitStructure);
			//启动串口
			USART_Cmd(USART1, ENABLE);
			  while (1)
			{  
				Uart1_PutString("王云飞 出ARM/51综合板带您进入32位处理世界" ,50 );
				delay(0x00ffff);
			}
}

