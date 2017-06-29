 
 /*********************************************************************************      
* ����        �ԡ���C����
* �����뻷������Keil4  
* �������ܡ���
* ������֧�֡���Cortex-M0
* ����        ��: RCC 8*6=48MHz		
* ��о        Ƭ��: stm32f030f4
* ����        �ڡ�: 2015/02/12
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
#include <math.h> 

/* ����APģʽ */
#define ATCWMODE "AT+CWMODE=2\r\n"    

/* ����WIFI���֣����� */
#define ATCWSAP  "AT+CWSAP=\"FXY\",\"0123456789\",11,0\r\n"

/* ���������� */
#define ATCIPMUX "AT+CIPMUX=1\r\n"   

/* �ȵ�˿ں� */
#define ATCIPSERVER "AT+CIPSERVER=1,8000\r\n"   


extern char CON_sign;

/************************************************************************/
//�������� ������ǰ;��������
/************************************************************************/

void car_freedom(signed char led0pwmvalone,signed char led0pwmvaltwo)
{
				/*****�ҵ��********/
	if(led0pwmvaltwo > 0)
	{
		GPIO_SetBits(LED_PORT,IN2_PIN);	
	  GPIO_ResetBits(LED_PORT,IN1_PIN);
		
//		GPIO_SetBits(LED_PORT,NEA_PIN);
//		delay_us(led0pwmvaltwo);
//		GPIO_ResetBits(LED_PORT,NEA_PIN);
//		delay_us(100 - led0pwmvaltwo);
	}
	else if(led0pwmvaltwo < 0)
	{
		GPIO_SetBits(LED_PORT,IN1_PIN);	
	  GPIO_ResetBits(LED_PORT,IN2_PIN);
		
//		led0pwmvaltwo = fabs(led0pwmvaltwo);
//		GPIO_SetBits(LED_PORT,NEA_PIN);
//		delay_us(led0pwmvaltwo);
//		GPIO_ResetBits(LED_PORT,NEA_PIN);
//		delay_us(100 - led0pwmvaltwo);
	}
	else if(led0pwmvaltwo == 0)
	{
		GPIO_ResetBits(LED_PORT,IN1_PIN);	
		GPIO_ResetBits(LED_PORT,IN2_PIN);	
		//GPIO_ResetBits(LED_PORT,NEA_PIN);
	}
	
	/*****����********/
	if(led0pwmvalone > 0)
	{
		GPIO_SetBits(LED_PORT,IN4_PIN);	
	  GPIO_ResetBits(LED_PORT,IN3_PIN);
		
//		GPIO_SetBits(LED_PORT,ENB_PIN);
//		delay_us(led0pwmvalone);
//		GPIO_ResetBits(LED_PORT,ENB_PIN);
//		delay_us(100 - led0pwmvalone);
	}
	else if(led0pwmvalone < 0)
	{
		GPIO_SetBits(LED_PORT,IN3_PIN);	
	  GPIO_ResetBits(LED_PORT,IN4_PIN);
		
//		led0pwmvalone = fabs(led0pwmvalone);
//		GPIO_SetBits(LED_PORT,ENB_PIN);
//		delay_us(led0pwmvalone);
//		GPIO_ResetBits(LED_PORT,ENB_PIN);
//		delay_us(100 - led0pwmvalone);
	}
	else if(led0pwmvalone == 0)
	{
		GPIO_ResetBits(LED_PORT,IN3_PIN);	
		GPIO_ResetBits(LED_PORT,IN4_PIN);	
		//GPIO_ResetBits(LED_PORT,ENB_PIN);
	}
}

/*********************************************************************************
  * @brief       ������.
  * @param    ��
  *	@arg         NULL
  * @retval     null
  *********************************************************************************/


#if 1

int main(void)
{
	ALL_Config();
	clearBuff();
	printf(ATCWMODE);    
  delay_ms(200);
	
	printf(ATCWSAP);     
  delay_ms(200);
	
	printf(ATCIPMUX);     
  delay_ms(100);
	
	printf(ATCIPSERVER);     
  delay_ms(100);

	LED_OFF();
	delay_ms(800);
	LED_ON();
	delay_ms(800);
	while(1)
	{
		if(0x30 < CON_sign < 0x35)
		{
			if(CON_sign==0x31)
			{
				while(CON_sign!=0x30)
				{
					car_freedom(1,1);
					LED_ON();
				}
				car_freedom(0,0);
				LED_OFF();
				
			}

			if(CON_sign==0x32)
			{
				while(CON_sign!=0x30)
				{
					car_freedom(-1,-1);
					LED_ON();
				}
				car_freedom(0,0);
				LED_OFF();
			}

			if(CON_sign==0x33)
			{	
				while(CON_sign!=0x30)
				{
					car_freedom(-1,1);
					LED_ON();
				}
				car_freedom(0,0);
				LED_OFF();
			}


			if(CON_sign==0x34)
			{
				while(CON_sign!=0x30)
				{
					car_freedom(1,-1);
					LED_ON();
				}
				car_freedom(0,0);
				LED_OFF();
			}
		}
		else
		car_freedom(0,0);

	}

}

#endif





#if 0
int main(void)
{
    ALL_Config();
    LED_OFF();
    GSM_start();
    clearBuff();
    //send_call();
    //send_english();
    /*���NRFģ����MCU������*/
    // while(!NRF_CON_signeck()); 
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

#endif
