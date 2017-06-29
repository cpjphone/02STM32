�мǣ�ʹ��printf����ʱ��ע��򿪿�
/*��flash������־λȫ��*/
Ϊһ���ֽڴ洢��16λ���ݱ����ַ��2


#include "bsp_flash.h"   

FLASH_FLAG FLASH_STATUS;
static u32 count = 0;
/**---------------------------------------------------------------------------------
  * @brief   FLASHд��ز����ٲ���
  * @param  ����������ʼ��ַ
  * @retval ��������ִ�����
  --------------------------------------------------------------------------------*/

unsigned char FLASH_WRITE(unsigned short int * memory_data)
{	
	static int t;
	count = 0;
	/*Enables or disables the Internal High Speed oscillator (HSI).*/
	RCC_HSICmd(ENABLE);	
	/*��FLASH�ɲ���������*/
	FLASH_Unlock();
	/*��flash������־λȫ��*/
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(0x8002000);
	t = sizeof((const char *)memory_data);
	while(count <= t)
	{
		/*flash Ϊһ���ֽڴ洢��16λ���ݱ����ַ��2*/
		FLASH_ProgramHalfWord((0x8002000 +count*2),*(memory_data+count)); 
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
unsigned char FLASH_READ(unsigned short int * memory_data,unsigned short int n)
{	
	count = 0;
	while(count < n)
	{
	  *(memory_data+count) = *(u16 *)(0x8002000 + count*2);
		printf("\r %d \r",*(memory_data+count));   //��ȡ
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



#ifndef __BSP_FLASH_H
#define	__BSP_FLASH_H
#include "stm32f10x.h"

typedef enum {
	FLASH_WRITE_OK = 1,
	FLASH_WRIKE_NO = 0,
	FLASH_READ_OK = 1,
	FLASH_READ_NO = 0
}FLASH_FLAG ;

extern unsigned char FLASH_WRITE(unsigned short int * memory_data);
extern unsigned char FLASH_READ(unsigned short int * memory_data,unsigned short int n);


#endif /* __LED_H */






















