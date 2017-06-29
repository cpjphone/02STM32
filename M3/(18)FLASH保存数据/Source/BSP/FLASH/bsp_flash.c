/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  phone
  * @version V1.0
  * @date    2014-12-12
  * @brief   flash�洢����
  ******************************************************************************
  * @attention
  * �мǣ�STM32��FLASH������16λ���ݶ�д
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * �ŵ��ȳ�������
  ******************************************************************************
  */
  
#include "bsp_flash.h"   

FLASH_FLAG FLASH_STATUS;
static u32 count = 0;
/**---------------------------------------------------------------------------------
  * @brief   FLASHд��ز����ٲ���
  * @param  ����������ʼ��ַ
  * @retval ��������ִ�����
  --------------------------------------------------------------------------------*/

unsigned char FLASH_WRITE(unsigned long address,unsigned short int * memory_data)
{	
	static int t;
	count = 0;
	/*Enables or disables the Internal High Speed oscillator (HSI).*/
	RCC_HSICmd(ENABLE);	
	/*��FLASH�ɲ���������*/
	FLASH_Unlock();
	/*��flash������־λȫ��*/
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	//FLASH_ErasePage(0x8002000);
	FLASH_ErasePage(address);
	t = sizeof((const char *)memory_data);
	while(count <= t)
	{
		/*flash Ϊһ���ֽڴ洢��16λ���ݱ����ַ��2*/
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
  * @brief   FLASH��������
  * @param  memory_data�������ŵ�ַ��n �������
  * @retval ��������ִ�����
  --------------------------------------------------------------------------------*/
unsigned char FLASH_READ(unsigned long address,unsigned short int * memory_data,unsigned short int n)
{	
	count = 0;
	while(count < n)
	{
	  *(memory_data+count) = *(u16 *)(address + count*2);
	//	printf("\r %d \r",*(memory_data+count));   //��ȡ
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
