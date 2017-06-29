#include "stm32f10x.h"
#define uint unsigned int
#define key GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)
#define key2 EXTI_GetITStatus(EXTI_Line6)


/******************************** 变量定义 ------------------------------------*/
EXTI_InitTypeDef EXTI_InitStructure;
ErrorStatus HSEStartUpStatus;


/*********************************声明函数 -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void delay(uint n);
// /*void EXTI9_5_IRQHandler(void)
// {
//   if(EXTI_GetITStatus(EXTI_Line6) != RESET) //检测制定的EXTI线路触发请求是否发生。
// 	//if (KEY==1)
// 	{
//     /* Toggle GPIO_LED pin 7*/
//     GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8))));
// 		//GPIO_Write(GPIOB,0xffff);
// 		//GPIOB->ODR|=0<<8;
// 		delay(0x7fffff);
//     /* Clear the Key Button EXTI line pending bit */
//     EXTI_ClearITPendingBit(EXTI_Line6); //清除EXTI线路挂起位
// 				//EXTI->PR=1<<6;
//   }
// 		
// }*/
void EXTI9_5_IRQHandler(void)
{
		//if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	if (key2==1)
		{

				GPIO_Write(GPIOB,0x00<<8);
				delay(0x7fffff);
				GPIO_Write(GPIOB,0xff<<8);
				delay(0x7fffff);
				EXTI_ClearITPendingBit(EXTI_Line6);
				
		}
}
void delay(uint n)
{
		for(; n>0; n--);
}


/*******************************************************************************
* 
*            主函数
* 
*******************************************************************************/
int main(void)
{
#ifdef DEBUG
  debug();
#endif
  
 
  RCC_Configuration();	 //系统时钟配置   
 
  NVIC_Configuration();	//NVIC配置
    
  GPIO_Configuration();  //配置GPIO
  
  ///*将EXTI线6连接到PB6*/
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6); 

  /* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
  //配置按钮中断线触发方式
  EXTI_InitStructure.EXTI_Line = EXTI_Line6;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE; //中断线使能
  EXTI_Init(&EXTI_InitStructure); //初始化中断

  /* Generate software interrupt: simulate a falling edge applied on Key Button EXTI line */
  EXTI_GenerateSWInterrupt(EXTI_Line6); //EXTI_Line6中断允许 到此中断配置完成,可以写中断处理函数。
        
  while (1)
  {
  }
}


/*******************************************************************************
* 
*            RCC配置
* 
*******************************************************************************/
void RCC_Configuration(void)
{
//复位RCC外部设备寄存器到默认值
  RCC_DeInit();  

  //打开外部高速晶振
  RCC_HSEConfig(RCC_HSE_ON); 

 //等待外部高速时钟准备好
  HSEStartUpStatus = RCC_WaitForHSEStartUp(); 

  //外部高速时钟已经准别好
  if(HSEStartUpStatus == SUCCESS)  
  {
    
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

   
    FLASH_SetLatency(FLASH_Latency_2);
  
   //配置AHB(HCLK)时钟=SYSCLK
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
  
    //配置APB2(PCLK2)钟=AHB时钟
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    //配置APB1(PCLK1)钟=AHB 1/2时钟
    RCC_PCLK1Config(RCC_HCLK_Div2);  

    //配置ADC时钟=PCLK2 1/4
    RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
  
    //配置PLL时钟 == 外部高速晶体时钟*9
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); 
    
	//配置ADC时钟= PCLK2/4
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);

   //使能PLL时钟
    RCC_PLLCmd(ENABLE);  

   //等待PLL时钟就绪
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)  
    {
    }

    //配置系统时钟 = PLL时钟
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 

   //检查PLL时钟是否作为系统时钟
    while(RCC_GetSYSCLKSource() != 0x08)  
    {
    }
  }
   
  /* Enable Key Button GPIO Port, GPIO_LED and AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB
                         | RCC_APB2Periph_AFIO, ENABLE);
}

/*************************************************
函数: void GPIO_Config(void）
功能: GPIO配置
**************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       //推挽输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //配置浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configure the nested vectored interrupt controller.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);   //分配中断向量表
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  //设置中断优先级
  
  /* Enable the EXTI9_5 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; //中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =  0;  //强占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//次优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //通道中断使能
  NVIC_Init(&NVIC_InitStructure);//初始化中断
}

#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
