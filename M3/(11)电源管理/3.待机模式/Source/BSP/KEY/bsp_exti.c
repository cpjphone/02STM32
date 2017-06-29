/**
  ******************************************************************************
  * @file    bsp_exti.c
  * @author  phone
  * @version V1.0
  * @date    2014-11-10
  * @brief   I/O���ж�Ӧ��bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * �ŵ��ȳ�������
  ******************************************************************************
  */
  
#include "bsp_exti.h"
#include "bsp_led.h"
//#include "bsp_usart1.h"

/**---------------------------------------------------------------------------------
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
	* @attention �ж�Դ���з��飬ȷ�����ȼ�
  ---------------------------------------------------------------------------------*/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);           //���1λ��ռ���ȼ���3����Ӧ���ȼ� 
  
  /******************�ж�Դ��0����ռ���ȼ� 3 ��Ӧ���ȼ� 0 **********************/
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;					//���ָ���ж�Դ��  �ⲿ�ж���0�ж�  ��1�������ⲿ�ж�Դ		
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //���ָ����Ӧ���ȼ���0   ���ȼ���
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//���ǿռ���ȼ�����ռ���ȼ�������Ӧ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						// ʹ���ⲿ�ж�ͨ��  ʹ�ܷ���
  NVIC_Init(&NVIC_InitStructure);
}

/**---------------------------------------------------------------------------------
  * @brief  ���� PA0 Ϊ���жϿڣ��������ж����ȼ�
  * @param  ��
  * @retval ��
  ---------------------------------------------------------------------------------*/
void EXTI_PC13_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO,ENABLE);
												
	/* config the NVIC */
	NVIC_Configuration();

	/* EXTI line gpio config*/	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // ��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* EXTI line mode config */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13); // ��ʲô����ƾ仰����ѡ�񣬺��棨1�������6���˿� ���ڽ���ѡ��
  EXTI_InitStructure.EXTI_Line = EXTI_Line13;								  //(����) �Ǹ�L����Ϊ�ж�����	(����)
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    // ����EXTI��·Ϊ�ж�����
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     //�½����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;										// �ж��ǲ���ʹ��
  EXTI_Init(&EXTI_InitStructure); 
}

static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

 /**---------------------------------------------------------------------------------
  * @brief  ���ڼ�ⰴ���Ƿ񱻳�ʱ�䰴��
  * @param  ��
  * @retval 1 ����������ʱ�䰴��  0 ������û�б���ʱ�䰴��
  ---------------------------------------------------------------------------------*/
uint8_t PWR_Check_Standby(void)
{			
	uint8_t downCnt =0;																				//��¼���µĴ���
	uint8_t upCnt =0;																					//��¼�ɿ��Ĵ���			

	while(1)																										//��ѭ������return����
	{	
		Delay(0xFFFF);																					//�ӳ�һ��ʱ���ټ��

		if(GPIO_ReadInputDataBit (GPIOC,GPIO_Pin_13) == RESET)	//��⵽���°���
		{
			LED1(ON);LED2(ON);LED3(ON);												//��������LED��

			downCnt++;																					//��¼���´���
			upCnt=0;																						//��������ͷż�¼

			if(downCnt>=100)																		//����ʱ���㹻
			{
				LED1(OFF);LED2(OFF);LED3(OFF);	  	 
				return 1; 																				//��⵽������ʱ�䳤����
			}
		}
		else 
		{
			upCnt++; 																						//��¼�ͷŴ���
			if(upCnt>5)																					//������⵽�ͷų���5��
			{
				LED1(OFF);LED2(OFF);LED3(OFF);									//�ر�����LED��	
				return 0;																				//����ʱ��̫�̣����ǰ�����������
			}
		}//	if(GPIO_ReadInputDataBit 

	}//while

}
/*********************************************END OF FILE**********************/
