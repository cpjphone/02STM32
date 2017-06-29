/*********************************************************************************      
* ����    �ԡ���C����
* �����뻷������Keil4  
* �������ܡ�: ����2 ���� GSM ��绰 ������  �յ绰
* ������֧�֡���				
* ����    ��: RCC 8*9=72MHz		
* ��о    Ƭ��: stm32f103VE 
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

#define uint unsigned int
#define uchar unsigned char

#define Send_ENGLISH_key GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)		  //������
#define Send_CALLING_key GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)		  //��绰

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


/****************�Ƿ�**********************/
#define CALL_YOU_TELNUMBER "ATD15238643714;\r\n"      //���������ϵ��ֻ����� ���ǰ��ӵ�
#define SEND_MESSA_TO_YOUR "at+cmgs=\"15238643714\"\r\n"    //���������ϵ��ֻ����� ���ǰ��ӵ�
#define SEND_MESSA_CONTENT "tpytongxin.taobao.com"
#define CALL_YOU_TELNUMBER "ATD15238643714;\r\n"    //���������ϵ��ֻ����� ���ǰ��ӵ�
//#define CALL_YOU_TELNUMBER "ATD15238643714;\r\n"    //���������ϵ��ֻ����� ���ǰ��ӵ�

uchar  Sms2_Pdu[]="0891683108200205F011000B815132683417F40008A704521B601D";   //�Ƿ�
uchar tab[] = "00.0";

uchar a,j=0,flag=0;
int g_timeout=0;
	GPIO_InitTypeDef GPIO_InitStructure;

/************************************************************************/
//��������
/************************************************************************/
void USART_Config(u32 baud);
void Print_Str(u8* buf);
u8 Print_Char(u8 ch);
void Delay(u32 nCount);
void NVIC_Configuration(void);

/************************************************************************/
//CPU��ʱ����
/************************************************************************/
void delay(uint ms)// ��ʱ�ӳ���
{
    uchar i,h;
    while(ms--)
    {
        for(i=0;i<120;i++)
				{
					h++;
					if(h > 3)h = 0;
				}
    }
}

void clearBuff(void)
{
    for(j=0;j<MAXCHAR;j++)
    {
        aa[j]=0x00;
    }
    j=0;
}

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

void AT(void)
{
    clearBuff();
    Print_Str(ATE0);
    delay(50);
	  
    while(strstr(aa,"OK")==NULL)
    {
        delay(50);
       // led(0x01);
        //v_Lcd12864PutString_f( 1,1, "ģ�����ӡ�..��") ;
        clearBuff();
        Print_Str(ATE0);
			  //GPIO_SetBits(GPIOA,GPIO_Pin_11);
    }
		
    //led(0x02);
		//GPIO_ResetBits(GPIOA,GPIO_Pin_11);
   // v_Lcd12864PutString_f( 1,1, "ģ�����ӡ�OK��") ;
	
    Print_Str(ATCN);
    delay(50);


    while(1)
    {
        clearBuff();
        Print_Str(CREG_CMD);
        delay(50);
        if(((aa[9]=='0')&&(aa[11]=='1'))||((aa[9]=='0')&&(aa[11]=='5')))
        {
            clearBuff();
					  //GPIO_SetBits(GPIOA,GPIO_Pin_11);
					  //GPIO_ResetBits(GPIOA,GPIO_Pin_11);
           // v_Lcd12864PutString_f( 1,1, "ģ��ע�᡾OK��") ;
          //  led(0x08);
            break;
        }
        else
        {
            clearBuff();
           // v_Lcd12864PutString_f( 1,1, "ģ��ע�᡾..��") ;
           // led(0x04);
            delay(50);
        }
    }
}

void send_english()
{
    //P2=0XFF;
    clearBuff();

    Print_Str(CMGF1);
    delay(100);
    //led(0x01);


    Print_Str(SEND_MESSA_TO_YOUR);

    delay(300);
    //led(0x02);

    Print_Str(tab);  //����������
    delay(10);
    Print_Char(0x1A);      //���ͽ�������

    delay(10);
   // led(0x04);
    clearBuff();
}

void send_call()
{
    clearBuff();
    delay(100);
    Print_Str(CALL_YOU_TELNUMBER); //��绰
    clearBuff();
    delay(300);
}

void send_18B20()
{

}

void send_pdu()
{
    clearBuff();
    Print_Str(CMGF0);
    delay(100);
   // led(0x01);

    Print_Str(SMS_send);  //�������Ķ���

    delay(500);
   // led(0x02);

    Print_Str(Sms2_Pdu);  //����������
    Print_Char(0x1A);      //���ͽ�������

    delay(500);
    //led(0x04);
    clearBuff();
}


/************************************************************************/
//�������������
/************************************************************************/
int main(void)
{	
  USART_Config(9600);  //����ϵͳʱ��
	NVIC_Configuration();	 //NVIC����
	GPIO_Write(GPIOB,0xffff);
	delay(10);   
	GPIO_SetBits(GPIOA,GPIO_Pin_1);		//��Ӣ��TEXT���Ű���
	GPIO_SetBits(GPIOA,GPIO_Pin_4);		//��绰����  
	//GPIO_SetBits(GPIOA,GPIO_Pin_11);		//��绰���� 
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
	send_call();
	while(1)
	{send_call();
	////////////////////////////////////////////////////////////////////////
	if(Send_ENGLISH_key==0)     //��Ӣ��TEXT���Ű���
	{   
		delay(30);
		if(Send_ENGLISH_key==0)  
		{
			//v_Lcd12864PutString_f( 1,1, "Ӣ�Ķ��š�..��") ;
			send_english();
			//v_Lcd12864PutString_f( 1,1, "Ӣ�Ķ��š�OK��") ;
		}

	}
	else if(Send_CALLING_key==0)     //��绰����
	{	
		delay(30);
		if(Send_CALLING_key==0)
		{
			//v_Lcd12864PutString_f( 1,1, "����绰��..��") ;
			send_call();
			//v_Lcd12864PutString_f( 1,1, "����绰��OK��") ;
		}
	}
// 	else if(Send_SMS_PDU_key==0)
// 	{

// 	}

		////////////////////////////////////////////////////////////////////////

		//        +CMTI: "SM",1


		if(strstr(aa,"RING")!=NULL)
		{
			//FENGMINGQI=0;
			delay(2);
	//		v_Lcd12864PutString_f( 1,1, "�绰���ˡ��ס�") ;
			clearBuff();
		}
		else
		{
			//FENGMINGQI=1;
		}
	}

}

/************************************************************************/
//������������жϺ��������գ�
/************************************************************************/
void USART2_IRQHandler(void)
{
	//GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	GPIO_SetBits(GPIOA,GPIO_Pin_11);	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)    //���������ݼĴ�����
  {     
    aa[j] = USART_ReceiveData(USART2);  
		j++;
   // USART_ITConfig(USART2, USART_IT_TXE, DISABLE);                                   			   
  }
	
}



