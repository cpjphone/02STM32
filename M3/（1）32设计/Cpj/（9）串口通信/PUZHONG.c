
/*********************************************************************************      
* ����    �ԡ���C����
* �����뻷������Keil4  
* �������ܡ�: ����2 ���� GSM ��绰 ������  �յ绰
* ������֧�֡���				
* ����    ��: RCC 8*9=72MHz		
* ��о    Ƭ��: stm32f103VE 
* ����    �ڡ�: 2014/03/25
* ���޸����ڡ�: 2014/03/26
* ����    �ߡ�: Phone
* ��������: ���������ã�������á�
*********************************************************************************/
#include "stm32f10x.h"
#include <stdio.h>
#include "stm32f10x_usart.h"
//#include "misc.h"
#include <string.h>

/*************************** �궨��*********************************************/
#define uint unsigned int
#define uchar unsigned char

#define Send_ENGLISH_key GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)		  //������
#define Send_CALLING_key GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)

#define MAXCHAR 81
char  aa[MAXCHAR];

uchar ATE0[]="ATE0\r\n";
uchar CREG_CMD[]="AT+CREG?\r\n";
uchar SMS_send[]="AT+CMGS=18\r\n";
uchar ATCN[]="AT+CNMI=2,1\r\n";
uchar CMGF0[]="AT+CMGF=0\r\n";
uchar CMGF1[]="AT+CMGF=1\r\n";
uchar CMGR[12]="AT+CMGR=1\r\n";
uchar CMGD[12]="AT+CMGD=1\r\n";
//���ѿ� �� 18439893663   8134893966F3

/****************�Ƿ�**********************/
// #define CALL_YOU_TELNUMBER "ATD15238643714;\r\n"      //���������ϵ��ֻ����� ���ǰ��ӵ�
// #define SEND_MESSA_TO_YOUR "at+cmgs=\"15238643714\"\r\n"    //���������ϵ��ֻ����� ���ǰ��ӵ�
// #define SEND_MESSA_CONTENT "tpytongxin.taobao.com"
// #define CALL_YOU_TELNUMBER "ATD15238643714;\r\n"    //���������ϵ��ֻ����� ���ǰ��ӵ�
// //#define CALL_YOU_TELNUMBER "ATD15238643714;\r\n"    //���������ϵ��ֻ����� ���ǰ��ӵ�

/****************�Ƿ�**********************/
#define CALL_YOU_TELNUMBER "ATD18439893663;\r\n"      //���������ϵ��ֻ����� ���ǰ��ӵ�
#define SEND_MESSA_TO_YOUR "at+cmgs=\"18439893663\"\r\n"    //���������ϵ��ֻ����� ���ǰ��ӵ�
#define SEND_MESSA_CONTENT "tpytongxin.taobao.com"
#define CALL_YOU_TELNUMBER "ATD18439893663;\r\n"    //���������ϵ��ֻ����� ���ǰ��ӵ�
//#define CALL_YOU_TELNUMBER "ATD15238643714;\r\n"    //���������ϵ��ֻ����� ���ǰ��ӵ�

uchar  Sms2_Pdu[]="0891683108200205F011000B818134893966F30008A704521B601D";   //�ѿ�
uchar tab[] = "00.0";
uchar a,j=0,flag=0;
int g_timeout=0;

/************************************************************************/
//CPU��ʱ����
/************************************************************************/
void delay(uint ms)// ��ʱ�ӳ���
{
    uchar i,h;
    while(ms--)
    {
        for(i=0;i<140;i++);
				{
						for(h = 0; h < 25;h--);
				}
    }
}

/************************************************************************/
//STM32�򴮿�2����1�ֽ�   
/************************************************************************/
void Print_Char(u8 ch)
{
  USART_SendData(USART2, (u8) ch);
  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

/************************************************************************/
//����һ���ַ���
/************************************************************************/
void Print_Str(u8* buf )
{   
	while(*buf!='\0')
	{ 
		Print_Char(*buf);
		delay(3);
		buf++;
	}
}

/************************************************************************/
//��λ��ʱ�ӿ��� ����
/************************************************************************/
void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;                    //�����ⲿ���پ�������״̬ö�ٱ���
  RCC_DeInit();                                    //��λRCC�ⲿ�豸�Ĵ�����Ĭ��ֵ
  RCC_HSEConfig(RCC_HSE_ON);                       //���ⲿ���پ���
  HSEStartUpStatus = RCC_WaitForHSEStartUp();      //�ȴ��ⲿ����ʱ��׼����
  if(HSEStartUpStatus == SUCCESS)                  //�ⲿ����ʱ���Ѿ�׼���
  {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //����FLASHԤ�����幦�ܣ�����FLASH�Ķ�ȡ�����г����б�����÷�.λ�ã�RCC��ʼ���Ӻ������棬ʱ������֮��
    FLASH_SetLatency(FLASH_Latency_2);                    //flash��������ʱ
      	
    RCC_HCLKConfig(RCC_SYSCLK_Div1);               //����AHB(HCLK)ʱ�ӵ���==SYSCLK
    RCC_PCLK2Config(RCC_HCLK_Div1);                //����APB2(PCLK2)��==AHBʱ��
    RCC_PCLK1Config(RCC_HCLK_Div2);                //����APB1(PCLK1)��==AHB1/2ʱ��
         
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  //����PLLʱ�� == �ⲿ���پ���ʱ�� * 9 = 72MHz
    RCC_PLLCmd(ENABLE);                                   //ʹ��PLLʱ��
   
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)    //�ȴ�PLLʱ�Ӿ���
    {
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //����ϵͳʱ�� = PLLʱ��
    while(RCC_GetSYSCLKSource() != 0x08)                  //���PLLʱ���Ƿ���Ϊϵͳʱ��
    {
    }
  }
}

/************************************************************************/
//����Զ�������
/************************************************************************/
void clearBuff(void)
{
	for(j=0;j<MAXCHAR;j++)
	{
		aa[j]=0x00;
	}
	j=0;
}

/************************************************************************/
//��ⵥƬ����ģ�������
/************************************************************************/
int Hand()
{
	delay(10);
	if(strstr(aa,"OK")!=NULL)             //��ⵥƬ����ģ�������
	{
		g_timeout=0;
		return 1;
	}
	if(g_timeout>50)
	{
		g_timeout=0;
		return -1;
	}
	g_timeout++;
	return 0;
}

/************************************************************************/
//GSM ��ʼ�� 
/************************************************************************/
void AT(void)
{
	clearBuff();
	Print_Str(ATE0);
	delay(50);

	while(strstr(aa,"OK")==NULL)		//ģ�����ӡ�..��
	{
		delay(50);     
		clearBuff();
		Print_Str(ATE0);
	}
	/*********** ģ�����ӡ�OK��*************************/
	Print_Str(ATCN);
	delay(50);

	clearBuff();
	Print_Str(CREG_CMD);
	delay(50);
	if(((aa[10]=='0')&&(aa[12]=='1'))||((aa[10]=='0')&&(aa[12]=='5')))//ģ��ע�᡾OK��
	{
		clearBuff();
	}
	else   //ģ��ע�᡾..��
	{
		clearBuff();
		delay(50);
	}		
}

/************************************************************************/
//��绰
/************************************************************************/
void send_call()
{
    clearBuff();
    delay(100);
    Print_Str(CALL_YOU_TELNUMBER); //��绰
    clearBuff();
    delay(300);
}
/************************************************************************/
//����Ӣ�Ķ���
/************************************************************************/
void send_english()
{
	clearBuff();
	Print_Str(CMGF1);
	delay(100);
	Print_Str(SEND_MESSA_TO_YOUR);
	delay(600);
	Print_Str(tab);  //����������
	delay(10);
	Print_Char(0x1A);      //���ͽ�������
	delay(200);
	clearBuff();
}

/************************************************************************/
//�����������
/************************************************************************/
void send_pdu()
{
	clearBuff();
	Print_Str(CMGF0);
	delay(100);
	Print_Str(SMS_send);  //�������Ķ���
	delay(500);
	Print_Str(Sms2_Pdu);  //����������
	Print_Char(0x1A);      //���ͽ�������
	delay(500);
	//led(0x04);
	clearBuff();
}

/************************************************************************/
//�����¶�
/************************************************************************/
void send_18B20()
{

}

/************************************************************************/
//����2�ж�����
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
//��ʼ������Ӳ���豸�������ж�
/************************************************************************/
void USART1_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* ��1������GPIO��USART������ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  // ѡ�����н�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //���ó�����ʽ���
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //���ģʽ�� I/O����ٶ� 50M HZ
  GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PB��
	
	/* ��2������USART Tx��GPIO����Ϊ���츴��ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* ��3������USART Rx��GPIO����Ϊ��������ģʽ
		����CPU��λ��GPIOȱʡ���Ǹ�������ģʽ���������������費�Ǳ����
		���ǣ��һ��ǽ�����ϱ����Ķ������ҷ�ֹ�����ط��޸���������ߵ����ò���
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*  ��3���Ѿ����ˣ�����ⲽ���Բ���
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ��4��������USART1����
	    - BaudRate = 115200 baud
	    - Word Length = 8 Bits
	    - One Stop Bit
	    - No parity
	    - Hardware flow control disabled (RTS and CTS signals)
	    - Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

    /* ���������ݼĴ�������������ж� */
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	/* ��5����ʹ�� USART1�� ������� */
	USART_Cmd(USART2, ENABLE);

    /* �����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
  USART_ClearFlag(USART2, USART_FLAG_TC);     // ���־
}


/************************************************************************/
//�жϷ�����
/************************************************************************/
void USART2_IRQHandler(void)
{  
	if(USART_GetITStatus(USART2, USART_IT_RXNE)==SET)
	{  
		aa[j] = USART_ReceiveData(USART2);  	                                  			   
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		j++; 
	}

	//���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ����������������жϵ�����[ţ��˵Ҫ����]
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART2,USART_FLAG_ORE);
	}
}

/************************************************************************/
//������
/************************************************************************/
int main(void)
{
	RCC_Configuration();
	NVIC_Configuration();
	USART1_Configuration();
	GPIO_SetBits(GPIOA,GPIO_Pin_11); 
	GPIO_SetBits(GPIOA,GPIO_Pin_4); 
	//GPIO_Write(GPIOA,0xffff);	
	clearBuff();

	AT();		   //��ʼ��ģ��
	Print_Str(CMGD);
	delay(50);
	while(Hand()==0);
	clearBuff();

	Print_Str(CMGF1);
	delay(30);

	Print_Str(ATCN);
	delay(30);
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)     //��绰����
		{	  
			delay(100);
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)
			{

				//v_Lcd12864PutString_f( 1,1, "����绰��..��") ;
				send_call();
				//send_english();//��Ӣ��TEXT���Ű���
			}
			//v_Lcd12864PutString_f( 1,1, "����绰��OK��") ;

		}
		
		if(strstr(aa,"RING")!=NULL)
		{
			//FENGMINGQI=0;
			delay(2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			//GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			//		v_Lcd12864PutString_f( 1,1, "�绰���ˡ��ס�") ;
			clearBuff();
		}
	}
}


