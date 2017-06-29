/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  phone
  * @version V1.0
  * @date    2014-11-06
  * @brief   led应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 雅典娜程序整理
  ******************************************************************************
  */
  
#include "bsp_led.h"   

#if 1
/**---------------------------------------------------------------------------------
  * @brief  初始化控制LED的IO（寄存器）
  * @param  无
  * @retval 无
  --------------------------------------------------------------------------------*/
void LED_GPIO_Config(void)
{		
		RCC->APB2ENR|=1<<3;				  // 使能GPIOB外设时钟 
		RCC->APB2ENR|=1<<7;				  // 使能GPIOF外设时钟 
		
		GPIOB->CRH&=0X00000000;
		//GPIOB->CRH|=0X33333333;			// 0011通用推挽输出，速率50MHZ
		GPIOB->CRL&=0X00000000;
		GPIOB->CRL|=0X00000003;			  // 0011通用推挽输出，速率50MHZ
	
		GPIOF->CRH&=0X00000000;
		GPIOF->CRH|=0X00000003;			  // 0011通用推挽输出，速率50MHZ
		GPIOF->CRL&=0X00000000;
		GPIOF->CRL|=0X30000000;			  // 0011通用推挽输出，速率50MHZ

		GPIOB->ODR|=0xffff;
		GPIOF->ODR|=0xffff;			
}
#endif

#if 0
/**---------------------------------------------------------------------------------
  * @brief  初始化控制LED的IO（库）
  * @param  无
  * @retval 无
  --------------------------------------------------------------------------------*/
void LED_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启GPIOB和GPIOF的外设时钟*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF, ENABLE); 

		/*选择要控制的GPIOB引脚*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIOB0*/
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		
		/*选择要控制的GPIOF引脚*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;

		/*调用库函数，初始化GPIOF7*/
		GPIO_Init(GPIOF, &GPIO_InitStructure);
		
		/*选择要控制的GPIOF引脚*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;

		/*调用库函数，初始化GPIOF7*/
		GPIO_Init(GPIOF, &GPIO_InitStructure);			  

		/* 关闭所有led灯	*/
		GPIO_SetBits(GPIOB, GPIO_Pin_0);
		
		/* 关闭所有led灯	*/
		GPIO_SetBits(GPIOF, GPIO_Pin_7|GPIO_Pin_8);	 
}
#endif
/*********************************************END OF FILE**********************/
