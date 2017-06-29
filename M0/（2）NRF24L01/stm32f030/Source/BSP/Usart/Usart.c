/*************************************
              ��������
���ڱ��깺����30Ԫ���´ι��򼴿ɷ���5�ǣ�
      ��50Ԫ���´ι��򼴿ɷ���1Ԫ��
     ��100Ԫ���´ι��򼴿ɷ���2Ԫ��
     ��150Ԫ���´ι��򼴿ɷ���4Ԫ��
      ��200Ԫ���´ι��򼴿�����
http://shop106001793.taobao.com/search.htm?spm=a1z10.5.w5002-3375901029.1.l1TXSl&search=y              
               V1.1
             2013.10.17
**************************************/


#include "Usart.h"
#include "includes.h"
#include <stdio.h>


/****************************************************
�������ܣ�USART1���ų�ʼ��
�����������
�����������
��    ע��PB6����USART1_TX
          PB7����USART1_RX
*****************************************************/
void Usart_IOConfig(void)
{
	  GPIO_InitTypeDef  GPIO_InitStructure;

	  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);

	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_1);//��IO��ӳ��ΪUSART�ӿ�
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_1); 
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;                 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���� 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//���� 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);   
}

/****************************************************
�������ܣ�USART1��ʼ��
���������baud : ������
�����������
��    ע��8λ���ݳ��� 1λֹͣλ ����żУ�� ���Ž����ж�         
*****************************************************/
void Usart_Init(u32 baud)
{
	USART_InitTypeDef  USART_InitStructure;

	Usart_IOConfig();//�������ų�ʼ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//����ʱ��

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��ʹ��Ӳ��������
	USART_Init(USART1, &USART_InitStructure);

//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//���ڽ����ж�����

	USART_Cmd(USART1, ENABLE);//ʹ�ܴ���
}

/****************************************************
�������ܣ�USART1����
���������byte : ����1�ֽ�����
�����������
��    ע����         
*****************************************************/
void USART_SendByte(u8 byte)
{
    while(!((USART1->ISR)&(1<<6)));//�ȴ�����Ϊ��
    USART1->TDR = byte;//��������	
}

/****************************************************
�������ܣ�USART1����
���������byte : ����һ���ַ���
�����������
��    ע����         
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
�������ܣ��ض���fputc����
���������ch : ����1�ֽ�����
�����������
��    ע����         
*****************************************************/
int fputc(int ch, FILE *f)
{      
//	while((USART1->ISR & 0X40)==0);
  while(!((USART1->ISR)&(1<<6)));//�ȴ�����Ϊ��
	USART1->TDR = (u8) ch;      
	return ch;
}

