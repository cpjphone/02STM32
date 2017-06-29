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

#define uint unsigned int
#define uchar unsigned char
    
char  aa[48];
unsigned char j = 0;


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
�������ܣ�USART1��ʼ��
���������baud : ������
�����������
��    ע��8λ���ݳ��� 1λֹͣλ ����żУ�� ���Ž����ж�         
*****************************************************/
void Usart_Init(void)
{
	USART_InitTypeDef  USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;
    
	Usart_IOConfig();//�������ų�ʼ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//����ʱ��

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��ʹ��Ӳ��������
	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//���ڽ����ж�����

	USART_Cmd(USART1, ENABLE);//ʹ�ܴ���
    
    	/* USART1��NVIC�ж����� */
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x02;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
    
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
void Print_Str(unsigned char *str)//�����ַ���
{
    while(*str!='\0')
    {
        USART_SendByte(*str);
        delay_ms(4);
        str++;
    }
}


/****************************************************
�������ܣ��ض���fputc����
���������ch : ����1�ֽ�����
�����������
��    ע����         
*****************************************************/
//int fputc(int ch, FILE *f)
//{      
////	while((USART1->ISR & 0X40)==0);
//  while(!((USART1->ISR)&(1<<6)));//�ȴ�����Ϊ��
//	USART1->TDR = (u8) ch;      
//	return ch;
//}

/****************************************************
�������ܣ�USART1�жϺ���
�����������
�����������
��    ע����         
*****************************************************/
void USART1_IRQHandler(void)
{
    // USART_IT_RXNE��Receive Data register not empty interrupt
    // USART_IT_RXNE���������ݼĴ����ǿձ�־λ
    if(USART_GetITStatus(USART1, USART_IT_RXNE) ==SET)
    {
        aa[j] = USART_ReceiveData(USART1);  	
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		j++;      
        if(j > 47)  j = 0;
//        USART_SendData(USART1,USART_ReceiveData(USART1));
//        while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
    }	
    
    //���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ����������������жϵ�����[ţ��˵Ҫ����]
    //OverRun Error flag: ������־λ
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







