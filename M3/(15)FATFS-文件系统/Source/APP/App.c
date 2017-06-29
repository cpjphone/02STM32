/**
  ******************************************************************************
  * @file    APP.c
  * @author  phone
  * @version V1.0
  * @date    2014-11-06
  * @brief   ����led
  ******************************************************************************
  * @attention
  *
  * @{ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * @{�ŵ��ȳ�������	
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
	
	/* Sdio Interrupt Config */
	NVIC_Configuration();
	/*����Ӳ����ʼ��*/
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
  * @brief       CPU��ʱ����
  * @param    		��
  ---------------------------------------------------------------------------------*/
void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
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




