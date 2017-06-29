/**
  ******************************************************************************
  * @file    bsp_exti.c
  * @author  phone
  * @version V1.0
  * @date    2014-11-10
  * @brief   I/O线中断应用bsp
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 雅典娜程序整理
  ******************************************************************************
  */
  
#include "bsp_exti.h"

/**---------------------------------------------------------------------------------
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
	* @attention 中断源进行分组，确定优先级
  ---------------------------------------------------------------------------------*/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);           //（☆）1位抢占优先级，3无响应优先级 
  
  /******************中断源（0）抢占优先级 3 响应优先级 0 **********************/
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;					//（☆）指定中断源，  外部中断线0中断  （1）设置外部中断源		
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //（☆）指定响应优先级别0   优先级别
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//（☆）强占优先级，抢占优先级高于响应优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						// 使能外部中断通道  使能分组
  NVIC_Init(&NVIC_InitStructure);
}

/**---------------------------------------------------------------------------------
  * @brief  配置 PA0 为线中断口，并设置中断优先级
  * @param  无
  * @retval 无
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* EXTI line mode config */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); // 问什么用这计句话进行选择，后面（1）语句中6个端口 现在进行选择
  EXTI_InitStructure.EXTI_Line = EXTI_Line4;								  //(☆☆☆) 那个L口作为中断输入	(☆☆☆)
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    // 设置EXTI线路为中断请求
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;										// 中断是不是使能
  EXTI_Init(&EXTI_InitStructure); 
}
/*********************************************END OF FILE**********************/
