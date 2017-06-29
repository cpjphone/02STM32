/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  fire
  * @version V1.0
  * @date    2014-11-09
  * @brief   按键应用bsp（扫描模式）
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 雅典娜程序整理
  ******************************************************************************
  */ 
  
#include "bsp_key.h" 

/**---------------------------------------------------------------------------------
  * @brief       CPU延时函数,不精确
  * @param    		无
  ---------------------------------------------------------------------------------*/
void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

#if 1
/**---------------------------------------------------------------------------------
  * @brief  配置按键用到的I/O口 寄存器
  * @param  无
  * @retval 无
  ---------------------------------------------------------------------------------*/
void Key_GPIO_Config(void)
{
	RCC->APB2ENR|=1<<2;     //使能PORTA时钟
	GPIOA->CRL&=0XFFFFFFF0;//PA0设置成输入	  
	GPIOA->CRL|=0X00000008;   
	GPIOA->CRH|=0X00000000; 
	//GPIOA->CRH&=0X0F0FFFFF;//PA13,15设置成输入	  
	//GPIOA->CRH|=0X80800000; 				   
	GPIOA->ODR|=1<<0;	   //PA13上拉,PA0默认上拉
	//GPIOA->ODR|=1<<15;	   //PA15上拉
}
#endif

#if 0
/**---------------------------------------------------------------------------------
  * @brief  配置按键用到的I/O口 库
  * @param  无
  * @retval 无
  ---------------------------------------------------------------------------------*/
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启按键端口（PA）的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
#endif

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
/*********************************************END OF FILE**********************/
