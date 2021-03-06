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
#include "bsp_led.h"
//#include "bsp_usart1.h"

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
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;					//（☆）指定中断源，  外部中断线0中断  （1）设置外部中断源		
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //（☆）指定响应优先级别0   优先级别
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//（☆）强占优先级，抢占优先级高于响应优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						// 使能外部中断通道  使能分组
  NVIC_Init(&NVIC_InitStructure);
}

/**---------------------------------------------------------------------------------
  * @brief  配置 PA0 为线中断口，并设置中断优先级
  * @param  无
  * @retval 无
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* EXTI line mode config */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13); // 问什么用这计句话进行选择，后面（1）语句中6个端口 现在进行选择
  EXTI_InitStructure.EXTI_Line = EXTI_Line13;								  //(☆☆☆) 那个L口作为中断输入	(☆☆☆)
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    // 设置EXTI线路为中断请求
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;										// 中断是不是使能
  EXTI_Init(&EXTI_InitStructure); 
}

static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

 /**---------------------------------------------------------------------------------
  * @brief  用于检测按键是否被长时间按下
  * @param  无
  * @retval 1 ：按键被长时间按下  0 ：按键没有被长时间按下
  ---------------------------------------------------------------------------------*/
uint8_t PWR_Check_Standby(void)
{			
	uint8_t downCnt =0;																				//记录按下的次数
	uint8_t upCnt =0;																					//记录松开的次数			

	while(1)																										//死循环，由return结束
	{	
		Delay(0xFFFF);																					//延迟一段时间再检测

		if(GPIO_ReadInputDataBit (GPIOC,GPIO_Pin_13) == RESET)	//检测到按下按键
		{
			LED1(ON);LED2(ON);LED3(ON);												//点亮所有LED灯

			downCnt++;																					//记录按下次数
			upCnt=0;																						//清除按键释放记录

			if(downCnt>=100)																		//按下时间足够
			{
				LED1(OFF);LED2(OFF);LED3(OFF);	  	 
				return 1; 																				//检测到按键被时间长按下
			}
		}
		else 
		{
			upCnt++; 																						//记录释放次数
			if(upCnt>5)																					//连续检测到释放超过5次
			{
				LED1(OFF);LED2(OFF);LED3(OFF);									//关闭所有LED灯	
				return 0;																				//按下时间太短，不是按键长按操作
			}
		}//	if(GPIO_ReadInputDataBit 

	}//while

}
/*********************************************END OF FILE**********************/
