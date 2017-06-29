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
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;					//���ָ���ж�Դ��  �ⲿ�ж���0�ж�  ��1�������ⲿ�ж�Դ		
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //���ָ����Ӧ���ȼ���0   ���ȼ���
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//���ǿռ���ȼ�����ռ���ȼ�������Ӧ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						// ʹ���ⲿ�ж�ͨ��  ʹ�ܷ���
  NVIC_Init(&NVIC_InitStructure);
}

/**---------------------------------------------------------------------------------
  * @brief  ���� PA0 Ϊ���жϿڣ��������ж����ȼ�
  * @param  ��
  * @retval ��
  ---------------------------------------------------------------------------------*/
void EXTI_PA4_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
												
	/* config the NVIC */
	NVIC_Configuration();

	/* EXTI line gpio config*/	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // ��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* EXTI line mode config */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); // ��ʲô����ƾ仰����ѡ�񣬺��棨1�������6���˿� ���ڽ���ѡ��
  EXTI_InitStructure.EXTI_Line = EXTI_Line4;								  //(����) �Ǹ�L����Ϊ�ж�����	(����)
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    // ����EXTI��·Ϊ�ж�����
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     //�½����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;										// �ж��ǲ���ʹ��
  EXTI_Init(&EXTI_InitStructure); 
}
/*********************************************END OF FILE**********************/
