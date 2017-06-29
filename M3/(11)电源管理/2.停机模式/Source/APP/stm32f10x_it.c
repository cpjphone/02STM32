/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_led.h"
#include "bsp_usart1.h"
#include "bsp_exti.h"

extern void SYSCLKConfig_STOP(void);


/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
	/********************************************************************************
  * @attention
	* �������ж�
	* ����ϵͳ�����׵�������NMI�����������жϣ������յ�����ĵ�һʱ��������Ӧ��
	* ���ںܰ�ȫ -�ؼ���safety - critical����Ӧ�ã�NMI���Ǳز����ٵ�
	* �绯ѧ��Ӧ����ʧ���ǵĽ���ͣ��
	* @endverbatim
*********************************************************************************/
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
	/********************************************************************************
  * @attention
	* Ӳ�������жϴ�����
	* ����HardFault_Handler���ϵ�ԭ�����������棺
	* 1���ڴ�������߷���Խ�磬�����Ҫ�Լ�д�����ʱ��淶���룬��������Ҫ�����Ų�
	* 2����ջ��������Ӷ�ջ�Ĵ�С
	* @endverbatim
*********************************************************************************/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
	/********************************************************************************
  * @attention
	* �ڴ��쳣������̫����
	* @endverbatim
*********************************************************************************/

void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
	/********************************************************************************
  * @attention
	* �����쳣
	* @endverbatim
*********************************************************************************/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
	/********************************************************************************
  * @attention
	* �����쳣
	* @endverbatim
*********************************************************************************/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
	/********************************************************************************
  * @attention
	* �͵�Դ�����й�
	* @endverbatim
*********************************************************************************/
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
	/********************************************************************************
  * @attention
	* ����쳣
	* @endverbatim
*********************************************************************************/
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

/**
  * @brief  This function handles  usart interrupt.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE)==SET)              //���յ�����
	{
		//ch = USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}

	//���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ����������������жϵ�����[ţ��˵Ҫ����]
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
	{
		//ch = USART_ReceiveData(USART1);
		USART_ClearFlag(USART1,USART_FLAG_ORE);
	}
}

/**
  * @brief  This function handles  External interrupt.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line13) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{	
		/*  �մ�ͣ�����ѣ�����ʱ��δ������ȷ��
		��printf�������ݲ����������ͳ�ȥ */
		
		printf("\r\n �����ж� \r\n");								
		
		SYSCLKConfig_STOP();											//ͣ�����Ѻ���Ҫ����HSE	
		
		LED1( ON );	LED2( ON );	LED3( ON );				//��������LEDһ��ʱ��ָʾͣ������
		Delay(0xFFFFFF);
		LED1( OFF );	LED2( OFF );	LED3( OFF );	
		
		/*����ǰ���Ѿ�����������HSE��
		���Ա�printf������������� */
		printf("\r\n �˳��ж� \r\n");
		
		EXTI_ClearITPendingBit(EXTI_Line13);     //����жϱ�־λ
	}
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
