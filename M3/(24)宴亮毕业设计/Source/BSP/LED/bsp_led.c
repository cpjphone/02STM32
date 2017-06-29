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

#if 0
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

#if 1
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
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE); 

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		
		GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
			
		GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
		GPIO_Init(GPIOC, &GPIO_InitStructure); 
}
#endif

#if 0
/**---------------------------------------------------------------------------------
  * @brief   ����Ƿ��а�������
  * @param   ����Ķ˿ںͶ˿�λ
  *		@arg GPIOx: x�����ǣ�A...G�� 
  *		@arg GPIO_PIN ������GPIO_PIN_x��x������1...16��
  * @retval  ������״̬
  *		@arg KEY_ON:��������
  *		@arg KEY_OFF:����û����
  ---------------------------------------------------------------------------------*/
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin)
{			
	/*����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON ) 
	{	   
		/*��ʱ����*/
		Delay(10000);		
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
		{	 
			/*�ȴ������ͷ� */
			while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
			return 	KEY_ON;	 
		}
		else
			return KEY_OFF;
	}
	else
		return KEY_OFF;
}
#endif
/*********************************************END OF FILE**********************/
