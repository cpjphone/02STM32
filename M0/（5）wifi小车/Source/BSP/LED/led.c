#include "led.h"
#include "includes.h"

/****************************************************
�������ܣ�LED��ʼ��
�����������
�����������
��    ע�����ô˺���ǰ����Ҫ��LED.h�޸ĺ궨��LED����
****************************************************/
void LED_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;

	  RCC_AHBPeriphClockCmd(LED_GPIO_CLK, ENABLE);
   
	
	
    GPIO_InitStructure.GPIO_Pin = LED_PIN | NEA_PIN | IN1_PIN | IN2_PIN | ENB_PIN | IN3_PIN | IN4_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
	

//    
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
}



/****************************************************
�������ܣ�LED��
�����������
�����������
��    ע�����ô˺���ǰ����Ҫ��LED.h�޸ĺ궨��LED����
****************************************************/
void LED_ON(void)
{
		GPIO_ResetBits(LED_PORT, LED_PIN);
}

/****************************************************
�������ܣ�LED��
�����������
�����������
��    ע�����ô˺���ǰ����Ҫ��LED.h�޸ĺ궨��LED����
****************************************************/
void LED_OFF(void)
{
		GPIO_SetBits(LED_PORT, LED_PIN);
}




