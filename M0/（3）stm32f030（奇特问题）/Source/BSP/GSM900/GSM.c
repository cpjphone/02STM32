/**
  ******************************************************************************
  * @file         bsp_xxx.c
  * @author   Phone
  * @version  V1.0
  * @date      2014/08/03
  * @brief      GSM900 ����绰��ͨ����
  ******************************************************************************
  * @attention
  *
  *
  *
  *
  * CPJ���ܿ���
  * @endverbatim
  ******************************************************************************
  * @attention
  * �Ļ���Ҫ����ĵ绰����ֻ�轫18239932500�Ļ�
  ******************************************************************************
  **/
  
  
/* Includes ------------------------------------------------------------------*/
#include <stm32f0xx.h>
#include "includes.h"
#include <ctype.h>  
#include <string.h>

/* Private configuration  for  GSM --------------------------------------*/

/* @KEY  ���������*/
#define CALL_NUM "ATD18239932500;\r\n"    

/* ���ڱ��������ַ�����TEXTģʽ */
#define CMGF1 "AT+CMGF=1\r\n"

/*��������*/
unsigned char MESSA_CONTENT[] ="Your Car is in danger, PLease Ask for help.";
unsigned char MASSA_NUM[] = "at+cmgs=\"18239932500\"\r\n"   ;

/* ���ڴ��ڷ��Ϳռ��С������ */
#define MAXCHAR 81
extern  char aa[81];

/*********************************************************************************
  * @brief       ����Զ��建��
  * @param    ��
  *********************************************************************************/
void clearBuff(void)
{
    unsigned int    j ;
    for(j=0;j<MAXCHAR;j++)
    {
        aa[j]=0x00;
    }
    j=0;
}


/*********************************************************************************
  * @brief       ��һ���벦��绰
  * @param    ��
  *	@arg         NULL
  * @retval     null
  *********************************************************************************/
void send_call(void)
{
    clearBuff();
    while(strstr(aa,"OK") ==NULL)
    {
      
        delay_ms(10);
        printf(CALL_NUM); //��绰     
        delay_ms(50);
    }
    //clearBuff();  
}

/*********************************************************************************
  * @brief       �����ַ�����
  * @param    ��
  *********************************************************************************/
void send_english(void)
{
    clearBuff();
	while((strstr(aa,"OK") ==NULL)&(strstr(aa,"C") ==NULL))
    {
        printf(CMGF1);
        delay_ms(60);
        clearBuff();
        printf("%s",MASSA_NUM);//���ź���
        
        delay_ms(80);
        
        printf("%s",MESSA_CONTENT);//��绰 //����������
        
        delay_ms(200);
      
        USART_SendByte(0x1A);      //���ͽ�������
        delay_ms(500);
        if((strstr(aa,"OK") !=NULL)|(strstr(aa,"C") !=NULL)) break;
     }
    clearBuff();
}

/*********************************************************************************
  * @brief       �ӵ��绰
  * @param    
  * @retval     1:�յ��绰��0��û�е绰����
  *********************************************************************************/
u8 RING_COM(void)
{
    if((strstr(aa,"RING")!=NULL))
    {    
        LED_ON();
        return 1;
    }
    else
    {
       return 0;
    }
}

/*********************************************END OF FILE**********************/

