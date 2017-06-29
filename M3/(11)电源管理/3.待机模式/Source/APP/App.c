/**
  ******************************************************************************
  * @file    APP.c
  * @author  phone
  * @version V1.0
  * @date    2014-11-09
  * @brief   ��Դ����֮˯��
  ******************************************************************************
  * @attention
  *
  * @{ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * @{�ŵ��ȳ�������	
	* @{ http://www.cnblogs.com/Ph-one/
  ******************************************************************************
  */ 
	
/* Includes ------------------------------------------------------------------*/
#include <stm32f10x.h>
#include "BSP.h"
#include "bsp_led.h"  
#include "bsp_usart1.h"
#include "bsp_exti.h" 
#include "delay.h"

#define CLI()      __set_PRIMASK(1)		/* �ر����ж� */  
#define SEI() __set_PRIMASK(0)				/* �������ж� */

void SYSCLKConfig_STOP(void);

/**---------------------------------------------------------------------------------
  * @brief       CPU��ʱ
  * @param    		��	
  ---------------------------------------------------------------------------------*/
#if 1
static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
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
	
	/*����Ӳ����ʼ��*/
	ALL_Config();
	EXTI_PC13_Config();
	//	CLI();  	//��
	//	SEI();		//��
	//	EXTI_PA0_Config();
	
	if(PWR_GetFlagStatus(PWR_FLAG_WU) == SET)
	{
		printf("\r\n ʹ�ܵ�Դ����ʱ�ӵ�Ԫǰ�ļ�⣬�������Ѹ�λ \r\n");

	}
	else
		printf("\r\n ʹ�ܵ�Դ����ʱ�ӵ�Ԫǰ�ļ�⣬�ϵ縴λ \r\n");
	
	
	/* ʹ�ܵ�Դ����Ԫ��ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);
	
	
	if(PWR_GetFlagStatus(PWR_FLAG_WU) == SET)
	{
		printf("\r\n ʹ�ܺ��⣬�������Ѹ�λ \r\n");

	}
	else
		printf("\r\n ʹ�ܺ��⣬�ϵ縴λ \r\n");
	
	
	while(1)
	{
		/* wait interrupt */
		LED1( ON );			  // ��
		Delay(0xFFFFF);
		LED1( OFF );		  // ��

		LED2( ON );			  // ��
		Delay(0xFFFFF);
		LED2( OFF );		  // ��

		LED3( ON );			  // ��
		Delay(0xFFFFF);
		LED3( OFF );		  // ��		
	
		//__WFI();					//����˯��ģʽ���ȴ��жϻ���
	}
}
#endif

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




