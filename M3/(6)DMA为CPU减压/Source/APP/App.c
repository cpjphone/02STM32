/**
  ******************************************************************************
  * @file    APP.c
  * @author  phone
  * @version V1.0
  * @date    2014-11-06
  * @brief   测试led
  ******************************************************************************
  * @attention
  *
  * @{实验平台:野火 iSO STM32 开发板 
  * @{雅典娜程序整理	
	* @{ USE_STM3210B_EVAL
	--cpu Cortex-M3 -g --apcs=interwork 
	-I C:\Keil\ARM\RV31\INC 
  ******************************************************************************
  */ 
	
/* Includes ------------------------------------------------------------------*/
#include <stm32f10x.h>
#include "BSP.h"
#include "bsp_led.h"  
#include "bsp_usart1.h"

extern uint8_t SendBuff[SENDBUFF_SIZE];
void Delay(__IO u32 nCount); 

#if 1
/**---------------------------------------------------------------------------------
  * @brief       主函数.
  * @param    		无
  *	@arg         NULL
  * @retval     null
  ---------------------------------------------------------------------------------*/
int main(void)
{
	/*首先是调用系统时钟，在3.0版本的库中，
	在main函数中仍需调用，在3.5版本中，
	系统时钟放在了启动文件这里
	函数运行的时候是先从这个汇编函数执行的
	启动文件 151行    ：
	LDR     R0, =SystemInit			  ;设置系统时钟
	SetSysClock();	
	*/
	/*便于在线调试*/
	#ifdef DEBUG
		debug();
	#endif
	
	/*外设硬件初始化*/
	ALL_Config();

	printf("\r\n usart1 DMA TX 测试 \r\n");
	{
		uint16_t i;	
		/*填充将要发送的数据*/
		for(i=0;i<SENDBUFF_SIZE;i++)
		{
			SendBuff[i]	 = 'A';
		}
	}
	/* USART1 向 DMA发出TX请求 */
	/**@{显然，此函数在while(1)大循环之外，但在上电后，数据始终传递 */
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	
	
	/* 此时CPU是空闲的，可以干其他的事情 */
	//例如同时控制LED
	while(1)
	{
		LED1(ON);
		Delay(0xFFFFF);
		LED1(OFF);
		Delay(0xFFFFF);
	}
		
		
		
}
#endif

/**---------------------------------------------------------------------------------
  * @brief       CPU延时函数
  * @param    		无
  ---------------------------------------------------------------------------------*/
void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

/**---------------------------------------------------------------------------------
  * @brief       检测传递给函数的参数是否是有效的参数
  * @param    		无
	* @example		 某个参数的取值范围只能是小于3的正整数，如果给出的参数大于3，
	* @example	   则这个assert_param()可以再运行程序调用到这个函数时报错			
  ---------------------------------------------------------------------------------*/
#ifdef DEBUG
void assert_failed(uint8_t* file, uint32_t line){
while (1);}
#endif
/*********************************************END OF FILE**********************/




