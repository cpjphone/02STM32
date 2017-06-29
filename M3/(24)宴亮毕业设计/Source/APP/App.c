/**
  ******************************************************************************
  * @file    APP.c
  * @author  phone
  * @version V1.0
  * @date    2015-03-09
  * @brief   ����led
  ******************************************************************************
  * @attention
  *
  * @{ʵ��ƽ̨:
  * @{�ŵ��ȳ�������	
	* @{ USE_STM3210B_EVAL
	--cpu Cortex-M3 -g --apcs=interwork 
	-I C:\Keil\ARM\RV31\INC 
  ******************************************************************************
  */ 
	
/* Includes ------------------------------------------------------------------*/
#include <stm32f10x.h>
#include <stdio.h>
#include <string.h>
#include "BSP.h"
#include "SysTick_delay.h"
//#include "bsp_led.h"  
#include "bsp_usart1.h"
#include "dht11.h"
#include "bsp_led.h" 
unsigned char FSSJ[11]={0x41,0x54,0x2B,0x43,0x49,0x50,0x53,0x45,0x4E,0x44,0x3D};//AT+CIPSEND= ��������

/* ����APģʽ */
#define ATCWMODE "AT+CWMODE=2\r\n"    

/* ����WIFI���֣����� */
#define ATCWSAP  "AT+CWSAP=\"2015YL\",\"0123456789\",11,0\r\n"

/* ��λ */
#define ATRST "AT+RST\r\n" 

/* ���������� */
#define ATCIPMUX "AT+CIPMUX=1\r\n"   

/* �ȵ�˿ں� */
#define ATCIPSERVER "AT+CIPSERVER=1,5000\r\n"

#define ATCIPSEND "AT+CIPSEND="


extern UART_BUF buf_uart1;  

void Delay(__IO u32 nCount); 


#if 0
int main(void)
{
	/*�����ǵ���ϵͳʱ�ӣ���3.0�汾�Ŀ��У�
	��main������������ã���3.5�汾�У�
	ϵͳʱ�ӷ����������ļ�����
	�������е�ʱ�����ȴ������ຯ��ִ�е�
	�����ļ� 151��    ��
	LDR     R0, =SystemInit			  ;����ϵͳʱ��
	SetSysClock();	
	*/
	/*�������ߵ���*/
	#ifdef DEBUG
		debug();
	#endif
	char temp;
	int i;
	/*����Ӳ����ʼ��*/
	ALL_Config();

	
	LED1_OFF;
	tdata[0] = 0;
	tdata[1] = 0;
	tdata[2] = 0;
	tdata[3] = 0;
	while(1)
	{
		//DHT11_Get_Data();
		ReadDHT11();
		PCout(13) = 1;
		delay_ms(500);
	PCout(13) = 0;
		delay_ms(500);
	
			printf("#");
			printf("RH");
			printf("%d",(char)tdata[0]);
			printf(".");
			printf("%d",(char)tdata[1]);

			printf("C:");
			printf("%d",(char)tdata[2]);
			printf(".");
			printf("%d\n",(char)tdata[3]);
			printf("@");
			delay_ms(5);

		
	}
}

#endif


#if 1
/**---------------------------------------------------------------------------------
  * @brief       ������.
  * @param    		��
  *	@arg         NULL
  * @retval     null
  ---------------------------------------------------------------------------------*/
int main(void)
{
	/*�����ǵ���ϵͳʱ�ӣ���3.0�汾�Ŀ��У�
	��main������������ã���3.5�汾�У�
	ϵͳʱ�ӷ����������ļ�����
	�������е�ʱ�����ȴ������ຯ��ִ�е�
	�����ļ� 151��    ��
	LDR     R0, =SystemInit			  ;����ϵͳʱ��
	SetSysClock();	
	*/
	/*�������ߵ���*/
	#ifdef DEBUG
		debug();
	#endif
	char temp;
	int i;
	/*����Ӳ����ʼ��*/
	ALL_Config();
	clear_buf_uart1();
	/* ����APģʽ */
	printf(ATCWMODE);    
  delay_ms(300);
	/* ����WIFI���֣����� */
	printf(ATCWSAP);     
  delay_ms(300);
	
	/* ��λ */
//	printf(ATRST);     
//  delay_ms(300);
	
	/* ���������� */
	printf(ATCIPMUX);     
  delay_ms(300);
	/* �ȵ�˿ں� */
	printf(ATCIPSERVER);     
  delay_ms(300);
	
	LED1_OFF;
	tdata[0] = 1;
	tdata[1] = 2;
	tdata[2] = 3;
	tdata[3] = 4;
	while(1)
	{
		ReadDHT11();
		PCout(13) = 1;
		delay_ms(500);
	PCout(13) = 0;
		delay_ms(500);
		//DHT11_Get_Data();
		//send_data_uart1(0x0A);
		#if 1
		if(strstr(buf_uart1.buf,"aa")!=NULL)
		{
			for(i=0;i<11;i++)//AT+CIPSEND= ��������
			{
				send_data_uart1(FSSJ[i]);	delay_ms(5);
			}
			
			send_data_uart1(0x30);
			delay_ms(5);
			send_data_uart1(0x2C);
			delay_ms(5);
			send_data_uart1(0x32);
			delay_ms(5);
			send_data_uart1(0x30);
			delay_ms(5);
			send_data_uart1(0x0D);
			delay_ms(5);
			send_data_uart1(0x0A);
			delay_ms(105);
			
			printf("#");
			printf("RH");
			printf("%d",(char)tdata[0]);
			printf(".");
			printf("%d",(char)tdata[1]);

			printf("C:");
			printf("%d",(char)tdata[2]);
			printf(".");
			printf("%d\n",(char)tdata[3]);
	
			printf("Lig:");
			if(PAin(11) == 1)
			{
				printf("NO");
			}
			else
				printf("OK");
			printf("$");
			clear_buf_uart1();
		}
	#endif
		
	}
}
#endif

/**---------------------------------------------------------------------------------
  * @brief       CPU��ʱ����
  * @param    		��
  ---------------------------------------------------------------------------------*/
void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

/**---------------------------------------------------------------------------------
  * @brief       ��⴫�ݸ������Ĳ����Ƿ�����Ч�Ĳ���
  * @param    		��
	* @example		 ĳ��������ȡֵ��Χֻ����С��3������������������Ĳ�������3��
	* @example	   �����assert_param()���������г�����õ��������ʱ����			
  ---------------------------------------------------------------------------------*/
#ifdef DEBUG
void assert_failed(uint8_t* file, uint32_t line){
while (1);}
#endif
/*********************************************END OF FILE**********************/




