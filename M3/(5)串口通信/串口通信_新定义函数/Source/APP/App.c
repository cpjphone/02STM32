/**
  ******************************************************************************
  * @file    APP.c
  * @author  phone
  * @version V1.0
  * @date    2014-11-06
  * @brief   ����led
  ******************************************************************************
  * @attention
  *
  * @{ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * @{�ŵ��ȳ�������	
	* @{ USE_STM3210B_EVAL
	--cpu Cortex-M3 -g --apcs=interwork 
	-I C:\Keil\ARM\RV31\INC 
  ******************************************************************************
  */ 
	
/* Includes ------------------------------------------------------------------*/
#include <stm32f10x.h>
#include "BSP.h"
//#include "bsp_led.h"  
#include "bsp_usart1.h"
#include <stdio.h> 
#include <string.h> 
#include <stdarg.h> 

unsigned char tab[] = "A:T:00.00C/R:00.00RH/U:000V/door:000/Danger:000/";
char a,b,c;
void Delay(__IO u32 nCount); 

//void ST_printf(const char *fmt, ... )
//{
//	va_list ap;

//	va_start(ap, fmt);
//	printf( fmt, ap );
//	va_end( ap );
//}


void ST_printf(char fmt, ... )
{
	int i;
	int j;
	va_list ap;
	va_start(ap, fmt);
	for(i = 0;i < fmt;++i)
	{
			j = (int)va_arg(ap,char);
			if(9 < j)
			{
				printf("%c",j);
			}
			else
				printf("%d",j);
	}
	va_end( ap );
}


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
	
	/*����Ӳ����ʼ��*/
	ALL_Config();
	
	a = 5;
	b = '/';
	c = 7;
	
//	printf("\r\n ����һ�������жϽ��ջ���ʵ�� \r\n");	
//	printf("\r\n ���ڳ����ն˻��ߴ��ڵ������������ַ� \r\n");	
//	//printf("\r\n �¶ȣ�%d", a);
//	printf("This" "is"  "2014/12/4 13:52:49"); 

	//printf(tab);	
	//printf("%s", tab);
	ST_printf(3,a,b,c);	
	//ST_printf("\r\n ���ڳ����ն˻��ߴ��ڵ������������ַ�2 \r\n");	
	while(1)
	{
		
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




