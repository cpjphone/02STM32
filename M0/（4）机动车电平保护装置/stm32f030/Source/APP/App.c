 
 /*********************************************************************************      
* ����        �ԡ���C����
* �����뻷������Keil4  
* �������ܡ���
* ������֧�֡���Cortex-M0
* ����        ��: RCC 8*6=48MHz		
* ��о        Ƭ��: stm32f030f4
* ����        �ڡ�: 2014/07/29
* ���޸����ڡ�: 
* ����        �ߡ�: Phone
* ��������: ���������ã�������á�
*********************************************************************************/
/*
 * PA2  -  ceʹ��
 * PA3  -  csƬѡ
 * PA0  -  irq�ж�
 */
 
/* Includes ------------------------------------------------------------------*/
#include <stm32f0xx.h>
#include "includes.h"
#include <ctype.h>  
#include <string.h>

#define CALL_NUM "ATD13700865872;\r\n"  
/* Private configuration  for  NRF24L01 --------------------------------*/
/* ����NEF24L01�ķ����� */
u8 status;	                                 //�����жϽ���/����״̬
u8 txbuf[8]={6,6,6,6,8,8,8,8};	     //���ͻ���
extern int rep_flag;                                //�ظ����ͱ�־
extern  char aa[48];

/*********************************************************************************
  * @brief       ������.
  * @param    ��
  *	@arg         NULL
  * @retval     null
  *********************************************************************************/
int main(void)
{
    ALL_Config();
    LED_OFF();
    GSM_start();
    clearBuff();
    //send_call();
    //send_english();
    /*���NRFģ����MCU������*/
    // while(!NRF_Check()); 
    NRF_TX_Mode();
    while(1)
    {
        /*��ʼ��������*/	
        NRF_Tx_Dat(txbuf);	  
        if(rep_flag) 
        {
            clearBuff();
            delay_ms(10);
            while(strstr(aa,"OK") ==NULL)
            {
                delay_ms(10);
                printf(CALL_NUM); //��绰     
                delay_ms(30);
                NRF_Tx_Dat(txbuf);	      
            }
            clearBuff();            
            LED_ON();     
            NRF_Tx_Dat(txbuf);	                  
        }      
        NRF_Tx_Dat(txbuf);	        
    }
}
























          
