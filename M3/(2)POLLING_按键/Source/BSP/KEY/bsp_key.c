/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  fire
  * @version V1.0
  * @date    2014-11-09
  * @brief   ����Ӧ��bsp��ɨ��ģʽ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * �ŵ��ȳ�������
  ******************************************************************************
  */ 
  
#include "bsp_key.h" 

/**---------------------------------------------------------------------------------
  * @brief       CPU��ʱ����,����ȷ
  * @param    		��
  ---------------------------------------------------------------------------------*/
void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

#if 1
/**---------------------------------------------------------------------------------
  * @brief  ���ð����õ���I/O�� �Ĵ���
  * @param  ��
  * @retval ��
  ---------------------------------------------------------------------------------*/
void Key_GPIO_Config(void)
{
	RCC->APB2ENR|=1<<2;     //ʹ��PORTAʱ��
	GPIOA->CRL&=0XFFFFFFF0;//PA0���ó�����	  
	GPIOA->CRL|=0X00000008;   
	GPIOA->CRH|=0X00000000; 
	//GPIOA->CRH&=0X0F0FFFFF;//PA13,15���ó�����	  
	//GPIOA->CRH|=0X80800000; 				   
	GPIOA->ODR|=1<<0;	   //PA13����,PA0Ĭ������
	//GPIOA->ODR|=1<<15;	   //PA15����
}
#endif

#if 0
/**---------------------------------------------------------------------------------
  * @brief  ���ð����õ���I/O�� ��
  * @param  ��
  * @retval ��
  ---------------------------------------------------------------------------------*/
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*���������˿ڣ�PA����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
#endif

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
/*********************************************END OF FILE**********************/
