/***************************普中科技 www.prechin.com****************************
	                          STM32-独立按键实验
 实验目的： 熟悉了解STM32GPIO的操作和库函数的使用
 连接方法： 参考接线图片
 实验现象：	没前面3个按钮 按下LED灯亮 松开LED灭

*******************************************************************************/

#include "stm32f10x_lib.h"

/******************************** 变量定义 ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
ErrorStatus HSEStartUpStatus;

/*********************************声明函数 -----------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration(void);
void Delay(vu32 nCount);


/*******************************************************************************
                                  主函数
*******************************************************************************/
int main(void)
{
  RCC_Configuration(); //系统时钟配置函数    

  NVIC_Configuration();	 //NVIC配置函数

  //使能APB2总线外设时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
  //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);  //关闭调试 端口重新映射  使用仿真器调试时，不能用此语

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; // 选择所有脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //配置成推挽式输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //输出模式下 I/O输出速度 50M HZ
   GPIO_Init(GPIOA, &GPIO_InitStructure);  //初PA口始化
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;  // 选择所有脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //配置成推挽式输入
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //输出模式下 I/O输出速度 50M HZ
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PB口


  GPIO_Write(GPIOA,0XFFFF); //初始化PA端口让 LED处于熄灭状态
 
  while (1)
  { 
  	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8))  //检测PB口的第9个管脚的电平
	GPIO_ResetBits(GPIOA,GPIO_Pin_0); //使第1个管脚为 0 
	else
	GPIO_SetBits(GPIOA,GPIO_Pin_0);	  //使第1个管脚为 1

	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))  //检测PB口的第10个管脚的电平
	GPIO_ResetBits(GPIOA,GPIO_Pin_1); //使第2个管脚为 0 
	else
	GPIO_SetBits(GPIOA,GPIO_Pin_1);	 //使第1个管脚为 1

	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10)) //检测PB口的第11个管脚的电平
	GPIO_ResetBits(GPIOA,GPIO_Pin_2); //使第3个管脚为 0 
	else
	GPIO_SetBits(GPIOA,GPIO_Pin_2);	  //使第1个管脚为 1

	
  }
}
/*******************************************************************************
*                           配置RCC
*******************************************************************************/
void RCC_Configuration(void)
{   
 //复位RCC外部设备寄存器到默认值
  RCC_DeInit();

  //打开外部高速晶振
  RCC_HSEConfig(RCC_HSE_ON);

   //等待外部高速时钟准备好
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)   //外部高速时钟已经准别好
  {								    
    //开启FLASH的预取功能
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    //FLASH延迟2个周期
    FLASH_SetLatency(FLASH_Latency_2);
 	
  //配置AHB(HCLK)时钟=SYSCLK
    RCC_HCLKConfig(RCC_SYSCLK_Div1);  
  
   //配置APB2(PCLK2)钟=AHB时钟
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    //配置APB1(PCLK1)钟=AHB 1/2时钟
    RCC_PCLK1Config(RCC_HCLK_Div2);

     //配置PLL时钟 == 外部高速晶体时钟*9  PLLCLK = 8MHz * 9 = 72 MHz 
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

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
}

/*******************************************************************************
*                             NVIC配置函数
*******************************************************************************/
void NVIC_Configuration(void)
{
#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif
}

/*******************************************************************************
*                              延时函数
*******************************************************************************/
void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
