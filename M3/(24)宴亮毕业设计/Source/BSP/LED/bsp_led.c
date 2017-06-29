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

#if 0
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

#if 1
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
  * @brief   检测是否有按键按下
  * @param   具体的端口和端口位
  *		@arg GPIOx: x可以是（A...G） 
  *		@arg GPIO_PIN 可以是GPIO_PIN_x（x可以是1...16）
  * @retval  按键的状态
  *		@arg KEY_ON:按键按下
  *		@arg KEY_OFF:按键没按下
  ---------------------------------------------------------------------------------*/
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin)
{			
	/*检测是否有按键按下 */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON ) 
	{	   
		/*延时消抖*/
		Delay(10000);		
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
		{	 
			/*等待按键释放 */
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
