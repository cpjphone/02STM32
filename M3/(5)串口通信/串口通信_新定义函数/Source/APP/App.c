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
	
	a = 5;
	b = '/';
	c = 7;
	
//	printf("\r\n 这是一个串口中断接收回显实验 \r\n");	
//	printf("\r\n 请在超级终端或者串口调试助手输入字符 \r\n");	
//	//printf("\r\n 温度：%d", a);
//	printf("This" "is"  "2014/12/4 13:52:49"); 

	//printf(tab);	
	//printf("%s", tab);
	ST_printf(3,a,b,c);	
	//ST_printf("\r\n 请在超级终端或者串口调试助手输入字符2 \r\n");	
	while(1)
	{
		
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




