/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  phone
  * @version V1.0
  * @date    2014-11-06
  * @brief   ledӦ�ú����ӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * �ŵ��ȳ�������
  ******************************************************************************
  */
  
#include "bsp_led.h"   

#if 1
/**---------------------------------------------------------------------------------
  * @brief  ��ʼ������LED��IO���Ĵ�����
  * @param  ��
  * @retval ��
  --------------------------------------------------------------------------------*/
void LED_GPIO_Config(void)
{		
		RCC->APB2ENR|=1<<3;				  // ʹ��GPIOB����ʱ�� 
		RCC->APB2ENR|=1<<7;				  // ʹ��GPIOF����ʱ�� 
		
		GPIOB->CRH&=0X00000000;
		//GPIOB->CRH|=0X33333333;			// 0011ͨ���������������50MHZ
		GPIOB->CRL&=0X00000000;
		GPIOB->CRL|=0X00000003;			  // 0011ͨ���������������50MHZ
	
		GPIOF->CRH&=0X00000000;
		GPIOF->CRH|=0X00000003;			  // 0011ͨ���������������50MHZ
		GPIOF->CRL&=0X00000000;
		GPIOF->CRL|=0X30000000;			  // 0011ͨ���������������50MHZ

		GPIOB->ODR|=0xffff;
		GPIOF->ODR|=0xffff;			
}
#endif

#if 0
/**---------------------------------------------------------------------------------
  * @brief  ��ʼ������LED��IO���⣩
  * @param  ��
  * @retval ��
  --------------------------------------------------------------------------------*/
void LED_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����GPIOB��GPIOF������ʱ��*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF, ENABLE); 

		/*ѡ��Ҫ���Ƶ�GPIOB����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIOB0*/
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		
		/*ѡ��Ҫ���Ƶ�GPIOF����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;

		/*���ÿ⺯������ʼ��GPIOF7*/
		GPIO_Init(GPIOF, &GPIO_InitStructure);
		
		/*ѡ��Ҫ���Ƶ�GPIOF����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;

		/*���ÿ⺯������ʼ��GPIOF7*/
		GPIO_Init(GPIOF, &GPIO_InitStructure);			  

		/* �ر�����led��	*/
		GPIO_SetBits(GPIOB, GPIO_Pin_0);
		
		/* �ر�����led��	*/
		GPIO_SetBits(GPIOF, GPIO_Pin_7|GPIO_Pin_8);	 
}
#endif
/*********************************************END OF FILE**********************/
