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
#include <stdio.h>
#include "bsp_led.h"
#include "bsp_irda.h"
#include "SysTick_delay.h"

extern uint32_t frame_data;
extern uint8_t  frame_cnt;
extern uint8_t  frame_flag;

uint8_t isr_cnt;  /* 用于计算进了多少次中断 */ 

extern void TimingDelay_Decrement(void);

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
	TimingDelay_Decrement();
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

//void USART1_IRQHandler(void)
//{
//	uint8_t ch;
//	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//	{ 	
//	    //ch = USART1->DR;
//			ch = USART_ReceiveData(USART1);
//	  	printf( "%c", ch );    //将接受到的数据直接返回打印
//	} 
//	 
//}

// IO 线中断，中断口为PB11 接红外接收头的数据管脚
void EXTI15_10_IRQHandler(void)
{
	uint8_t pulse_time = 0;
  uint8_t leader_code_flag = 0; /* 引导码标志位，当引导码出现时，表示一帧数据开始 */
  uint8_t irda_data = 0;        /* 数据暂存位 */
  
  if(EXTI_GetITStatus(EXTI_Line11) != RESET) /* 确保是否产生了EXTI Line中断 */
	{   
    while(1)
    {
      if( IrDa_DATA_IN()== SET )        /* 只测量高电平的时间 */
      {       
        pulse_time = Get_Pulse_Time();
        
        /* >=5ms 不是有用信号 当出现干扰或者连发码时，也会break跳出while(1)循环 */
        if( pulse_time >= 250 )                
        {
          break; /* 跳出while(1)循环 */
        }
        
        if(pulse_time>=200 && pulse_time<250)         /* 获得前导位 4ms~4.5ms */
        {
          leader_code_flag = 1;
        }
        else if(pulse_time>=10 && pulse_time<50)      /* 0.56ms: 0.2ms~1ms */
        {
          irda_data = 0;
        }
        else if(pulse_time>=50 && pulse_time<100)     /* 1.68ms：1ms~2ms */
        {
          irda_data =1 ; 
        }        
        else if( pulse_time>=100 && pulse_time<=200 ) /* 2.1ms：2ms~4ms */
        {//LED2_TOGGLE;
					/* 连发码，在第二次中断出现 */
          frame_flag = 1;               /* 一帧数据接收完成 */
          frame_cnt++;                  /* 按键次数加1 */
          isr_cnt ++;                   /* 进中断一次加1 */
          break;                        /* 跳出while(1)循环 */
        }
        
        if( leader_code_flag == 1 )
        {/* 在第一次中断中完成 */
          frame_data <<= 1;
          frame_data += irda_data;
          frame_cnt = 0;
          isr_cnt = 1;
        }
      }      
    }// while(1)   
		EXTI_ClearITPendingBit(EXTI_Line11);     //清除中断标志位
    
	}  
}
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
