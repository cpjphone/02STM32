/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  phone
  * @version V1.0
  * @date    2014-12-12
  * @brief   flash存储函数
  ******************************************************************************
  * @attention
  * 切记：STM32的FLASH必须以16位数据读写
  * 实验平台:野火 iSO STM32 开发板 
  * 雅典娜程序整理
  ******************************************************************************
  */
  
#include "bsp_flash.h"   

FLASH_FLAG FLASH_STATUS;
static u32 count = 0;
/**---------------------------------------------------------------------------------
  * @brief   FLASH写入必不可少步骤
  * @param  输入数据起始地址
  * @retval 函数有无执行完毕
  --------------------------------------------------------------------------------*/

unsigned char FLASH_WRITE(unsigned long address,unsigned short int * memory_data)
{	
	static int t;
	count = 0;
	/*Enables or disables the Internal High Speed oscillator (HSI).*/
	RCC_HSICmd(ENABLE);	
	/*打开FLASH可擦除控制器*/
	FLASH_Unlock();
	/*将flash三个标志位全清*/
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	//FLASH_ErasePage(0x8002000);
	FLASH_ErasePage(address);
	t = sizeof((const char *)memory_data);
	while(count <= t)
	{
		/*flash 为一个字节存储，16位数据必须地址加2*/
		FLASH_ProgramHalfWord((address +count*2),*(memory_data+count)); 
		count++;
		if(count > t)
		{
			FLASH_STATUS = FLASH_WRIKE_NO;
			return FLASH_STATUS;
		}
	}
	/* Locks the FLASH Program Erase Controller.*/
	FLASH_Lock(); 
	RCC_HSICmd(DISABLE);
	FLASH_STATUS = FLASH_WRITE_OK;
	return FLASH_STATUS;
}

/**---------------------------------------------------------------------------------
  * @brief   FLASH读出函数
  * @param  memory_data：输出存放地址，n 输出个数
  * @retval 函数有无执行完毕
  --------------------------------------------------------------------------------*/
unsigned char FLASH_READ(unsigned long address,unsigned short int * memory_data,unsigned short int n)
{	
	count = 0;
	while(count < n)
	{
	  *(memory_data+count) = *(u16 *)(address + count*2);
	//	printf("\r %d \r",*(memory_data+count));   //读取
		count++;
		if(count > n)
		{
			FLASH_STATUS = FLASH_READ_NO;	
			return FLASH_STATUS;
		}
	}
	//FLASH_ErasePage(0x8002000);
	FLASH_STATUS = FLASH_READ_OK;
	return FLASH_STATUS;
}

/*********************************************END OF FILE**********************/
