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
#include "string.h"
#include "BSP.h"
//#include "bsp_led.h"  
#include "bsp_usart1.h"
#include "bsp_led.h"   
#include "bsp_key.h" 
#include "bsp_flash.h"  

u16 data[5]={0x0001,0x0002,0x0003,0x0004,0x0005};
u16 data2[5];

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

/**---------------------------------------------------------------------------------*/	
	
	while(1)
	{
		if( Key_Scan(GPIOA,GPIO_Pin_0) == KEY_ON  )
		{			
			/*LED1反转*/
			LED1_TOGGLE;
			FLASH_WRITE(0x8002000,data);		
		}	
		Delay(5000000);		
		
		printf("\r\n The Five Data Is : \r\n");
		//printf("个数：%d \r\n",sizeof((const char *)data2));
		
		FLASH_READ(0x8002000,data2,sizeof((const char *)data)+1);
		
		
//		while(count < 5)
//		{
//			printf("\r %d \r",*(u16 *)(0x8002000 + count*2));   //读取
//			count++;
//		}
		//FLASH_ErasePage(0x8002000);
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




