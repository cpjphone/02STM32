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
	* 不屏蔽中断
	* 除非系统被彻底地锁定，NMI（不可屏蔽中断）会在收到请求的第一时间予以响应，
	* 对于很安全 -关键（safety - critical）的应用，NMI都是必不可少的
	* 如化学反应即将失控是的紧急停机
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
	* 硬件故障中断处理函数
	* 出现HardFault_Handler故障的原因有两个方面：
	* 1，内存溢出或者访问越界，这个需要自己写程序的时候规范代码，遇到了需要慢慢排查
	* 2，堆栈溢出，增加堆栈的大小
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
	* 内存异常（不是太懂）
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
	* 总线异常
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
	* 故障异常
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
	* 和电源管理有关
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
	* 监控异常
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

static void Delay(__IO uint32_t nCount)	 //简单的延时函数
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
	if(USART_GetITStatus(USART1, USART_IT_RXNE)==SET)              //接收到数据
	{
		//ch = USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}

	//溢出-如果发生溢出需要先读SR,再读DR寄存器则可清除不断入中断的问题[牛人说要这样]
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
	if(EXTI_GetITStatus(EXTI_Line13) != RESET) //确保是否产生了EXTI Line中断
	{	
		/*  刚从停机唤醒，由于时钟未配置正确，
		此printf语句的内容不能正常发送出去 */
		
		printf("\r\n 进入中断 \r\n");								
		
		SYSCLKConfig_STOP();											//停机唤醒后需要启动HSE	
		
		LED1( ON );	LED2( ON );	LED3( ON );				//点亮所有LED一段时间指示停机唤醒
		Delay(0xFFFFFF);
		LED1( OFF );	LED2( OFF );	LED3( OFF );	
		
		/*由于前面已经重新启动了HSE，
		所以本printf语句能正常发出 */
		printf("\r\n 退出中断 \r\n");
		
		EXTI_ClearITPendingBit(EXTI_Line13);     //清除中断标志位
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
