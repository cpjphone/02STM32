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
extern void Delay(__IO u32 nCount); 

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
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); 

		/*ѡ��Ҫ���Ƶ�GPIOB����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIOB0*/
		GPIO_Init(GPIOA, &GPIO_InitStructure);		

}
#endif


unsigned char detection_vol(unsigned int num )
{
	if((num > 2500)|(num < 1700))
	{
		Delay(0x9fffee);
		if(((num > 2500)|(num < 1700))&(num > 1200))
			return 1;
		else
			return 0;
	}
	else
	{
		return 0;
	}
}


























/*********************************************END OF FILE**********************/
