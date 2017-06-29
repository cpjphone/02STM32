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
#include "bsp_sdio_sdcard.h"
#include "ff.h"

void Delay(__IO u32 nCount); 

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;


FIL fnew;													/* file objects */
FATFS fs;													/* Work area (file system object) for logical drives */
FRESULT res; 
UINT br, bw;            					/* File R/W count */
BYTE buffer[4096]={0};       		  /* file copy buffer */
BYTE textFileBuffer[] = "Welcome to use Wildfire iso stm32 Development Board today is a good day";


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
	
	/* Sdio Interrupt Config */
	NVIC_Configuration();
	/*外设硬件初始化*/
	ALL_Config();
 	printf("\r\n this is a fatfs test demo \r\n");
	
	/* Register work area for each volume (Always succeeds regardless of disk status) */
	f_mount(0,&fs);

  //f_mkdir("sub");
  //f_mkdir("sub/sub1");

	/* function disk_initialize() has been called in f_open */

	/* Create new file on the drive 0 */
	res = f_open(&fnew, "0:newfile.txt", FA_CREATE_ALWAYS | FA_WRITE );
	 
	if ( res == FR_OK )
	{
		res = f_write(&fnew, textFileBuffer, sizeof(textFileBuffer), &bw);
		f_close(&fnew);      
	}

	res = f_open(&fnew, "0:newfile.txt", FA_OPEN_EXISTING | FA_READ); 	 
	res = f_read(&fnew, buffer, sizeof(buffer), &br); 

	printf("\r\n %s ", buffer);
	
	/* Close open files */
	f_close(&fnew);	                                      
	 
	/* Unregister work area prior to discard it */
	f_mount(0, NULL);
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




