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
extern void Delay(__IO u32 nCount); 

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
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); 

		/*选择要控制的GPIOB引脚*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIOB0*/
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
