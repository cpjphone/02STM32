
/*********************************************************************************      
* ����    �ԡ���C���� 
* �������ܡ�: ����2 ����
* ����    �ڡ�: 2014/03/25
* ���޸����ڡ�: 2014/03/25
* ����    �ߡ�: Phone
* ��������: ���������ã�������á�
*********************************************************************************/

#include "stm32f10x.h"
//#include "platform_config.h"
#include "stm32f10x_usart.h"
//#include "misc.h"
#include <string.h>

/*************************** �궨��***********************************************/
#define  USARTx                     USART2
#define  GPIOx                      GPIOA
#define  RCC_APB2Periph_GPIOx       RCC_APB2Periph_GPIOA
#define  GPIO_RxPin                 GPIO_Pin_3
#define  GPIO_TxPin                 GPIO_Pin_2

USART_InitTypeDef USART_InitStructure;
/************************************************************************/
//����2 ����
/************************************************************************/
void USART_Config(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    
  //����GPIOʱ��,AFIOʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC 
         | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG 
         | RCC_APB2Periph_AFIO, ENABLE);

	
  //��������2ʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//����������1ʱ��ˣ�
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_4| GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	
	
   //����USARTx_TxΪ�����������
  GPIO_InitStructure.GPIO_Pin = GPIO_TxPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOx, &GPIO_InitStructure);

  //���� USARTx_Rx Ϊ��������
  GPIO_InitStructure.GPIO_Pin = GPIO_RxPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOx, &GPIO_InitStructure);
	
	
	USART_InitStructure.USART_BaudRate = baud;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity =  USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    //��ʼ������
  USART_Init(USARTx, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	
  //��������
  USART_Cmd(USARTx, ENABLE);

}

/************************************************************************/
//����1�ж����ú���
/************************************************************************/
void NVIC_Configuration(void)
{
   NVIC_InitTypeDef NVIC_InitStructure;
  
   /* Set the Vector Table base location at 0x08000000 */
   NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
  
   /* Configure the NVIC Preemption Priority Bits */  
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
   /* Enable the USART1 Interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;       //ͨ������Ϊ����1�ж�
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	   //�ж���Ӧ���ȼ�0
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		   //���ж�
   NVIC_Init(&NVIC_InitStructure); 						   //��ʼ��
}

/************************************************************************/
//����һ���ַ�
/************************************************************************/
u8 Print_Char(u8 ch)
{
    /* Write a character to the USART */
    USART_SendData(USART2, (u8) ch);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
   {
   }
   return ch;
}

/************************************************************************/
//����һ���ַ���
/************************************************************************/
void Print_Str(u8* buf )
{   
	while(*buf!='\0')
	{ 
		Print_Char(*buf);
		//Delay(15);
		buf++;
	}
}

