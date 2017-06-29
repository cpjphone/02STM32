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

static void Delay(__IO u32 nCount);
void SYSCLKConfig_STOP(void);

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
	
	printf("\r\n Ұ��ISO�����壬˯��ʵ�� \r\n");

	/* ʹ�ܵ�Դ����Ԫ��ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	printf("\r\n ����ֹͣģʽ \r\n");

	/* ����ֹͣģʽ�����õ�ѹ������Ϊ�͹���ģʽ���ȴ��жϻ���*/
	PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);	

	while(1)
	{
		/* wait interrupt */
		LED1( ON );			  // ��
		delay_ms(200);
		LED1( OFF );		  // ��

		LED2( ON );			  // ��
		delay_ms(200);
		LED2( OFF );		  // ��

		LED3( ON );			  // ��
		delay_ms(200);
		LED3( OFF );		  // ��			
	
		//__WFI();					//����˯��ģʽ���ȴ��жϻ���
	}
}
#endif


/**
  * @brief  ͣ�����Ѻ�����ϵͳʱ��: ʹ�� HSE, PLL
  *         ����ѡ��PLL��Ϊϵͳʱ��.
  * @param  None
  * @retval None
  */
void SYSCLKConfig_STOP(void)
{
	ErrorStatus HSEStartUpStatus;
  /* ʹ�� HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* �ȴ� HSE ׼������ */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {

    /* ʹ�� PLL */ 
    RCC_PLLCmd(ENABLE);

    /* �ȴ� PLL ׼������ */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* ѡ��PLL��Ϊϵͳʱ��Դ */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* �ȴ�PLL��ѡ��Ϊϵͳʱ��Դ */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
}

/**---------------------------------------------------------------------------------
  * @brief       CPU��ʱ
  * @param    		��	
  ---------------------------------------------------------------------------------*/
static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
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




