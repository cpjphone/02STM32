/**
  ******************************************************************************
  * @file    APP.c
  * @author  phone
  * @version V1.0
  * @date    2014-12-10
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
//#include "bsp_led.h"  
#include "bsp_usart1.h"
#include "bsp_adc.h"

float ADC_ConvertedValueLocal;      

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
	/* enable adc1 and config adc1 to dma mode */
	ADC1_Init();

	printf("\r\n ----这是一个ADC实验(DMA传输)----\r\n");
/**--------------------------------------------------------------------------------
* @brief       ADC配置好后直接从数据地址读出
---------------------------------------------------------------------------------*/
	#if 1
	while(1)
	{
		ADC_ConvertedValueLocal =(float) (*(__IO u16 *)ADC1_DR_Address)/4096*3.3;// 读取转换的AD值
		printf("\r\n The current AD value = 0x%04X \r\n", *(__IO u16 *)ADC1_DR_Address); 	
	  printf("\r\n The current AD value = %f V \r\n",ADC_ConvertedValueLocal); 
		Delay(0x5fffee);  
	}
	#endif
/**--------------------------------------------------------------------------------
* @brief       采用DMA数据处理
---------------------------------------------------------------------------------*/
	#if 0
	while(1)
	{
		ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; // 读取转换的AD值
		printf("\r\n The current AD value = 0x%04X \r\n", ADC_ConvertedValue); 	
	  printf("\r\n The current AD value = %f V \r\n",ADC_ConvertedValueLocal); 
		Delay(0xffffee);  
	}
	#endif
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




