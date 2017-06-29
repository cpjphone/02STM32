/**
  ******************************************************************************
  * @file    APP.c
  * @author  phone
  * @version V1.0
  * @date    2014-12-14
  * @brief   LCD1602显示
  ******************************************************************************
  * @attention
  *
  * @{实验平台:
  * @{雅典娜程序整理	
	* @{ USE_STM3210B_EVAL
  ******************************************************************************
  */ 
	
/* Includes ------------------------------------------------------------------*/
#include <stm32f10x.h>
#include "BSP.h"
#include "LCD_1602.h"
#include "bsp_adc.h"

unsigned int ADC_ConvertedValueLocal;    
unsigned char dis1[] = {"Power voltage:"};
unsigned char dis2[8];


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
	
	LCD_1602_CONFIG();
	ADC1_Init();
	
	LCD1602_WRITE(1,0,dis1);
	
/**----------------------------------------------------------------------
* @brief 方法3，使用寄存器控制IO
-----------------------------------------------------------------------*/
	#if 1
	while(1)
	{
			ADC_ConvertedValueLocal =ADC_ConvertedValue*3300/4096; // 读取转换的AD值
			//ADC_ConvertedValueLocal = (*(__IO u16 *)ADC1_DR_Address)*3300/4096;// 读取转换的AD值
			dis2[0] = ADC_ConvertedValueLocal%10000/1000 + 0x30;
			dis2[1] = '.';
			dis2[2] = ADC_ConvertedValueLocal%1000/100 + 0x30;
			dis2[3] = ADC_ConvertedValueLocal%100/10 + 0x30;
			dis2[4] = ADC_ConvertedValueLocal%10 + 0x30;
//			
			//dis2[0] = 9 + 0x30;
			LCD1602_WRITE(2,0,dis2);
			Delay(0x2fffee);  
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




