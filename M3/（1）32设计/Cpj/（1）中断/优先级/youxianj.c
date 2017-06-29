/*************************************/
//  GPIOB_PIN_8显示
//	GPIOB_PIN_7按键输入
/**************************************/

#include "stm32f10x.h"
#define uint unsigned int
#define key1 EXTI_GetITStatus(EXTI_Line1)
#define key2 EXTI_GetITStatus(EXTI_Line5)

ErrorStatus HSEStartUpStatus;

void delay(uint n)
{
		for (;n>0;n--);
}
void GPIO_Configuration()
{
			GPIO_InitTypeDef GPIO_InitStructure;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB
                         | RCC_APB2Periph_AFIO, ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       //推挽输入
			GPIO_Init(GPIOB, &GPIO_InitStructure);

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_5;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //配置浮空输入
			GPIO_Init(GPIOB, &GPIO_InitStructure);
	
// 			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
// 			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //配置浮空输入
// 			GPIO_Init(GPIOB, &GPIO_InitStructure);
			
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

void NVIC_Configuration()	  //嵌套向量中断控制器
{
			EXTI_InitTypeDef EXTI_InitStructure;
			NVIC_InitTypeDef NVIC_InitStructure;
	
				EXTI_InitStructure.EXTI_Line = EXTI_Line1|EXTI_Line5;
				EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
				EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
				EXTI_InitStructure.EXTI_LineCmd = ENABLE; //中断线使能
				EXTI_Init(&EXTI_InitStructure); //初始化中断
				EXTI_GenerateSWInterrupt(EXTI_Line1);
				EXTI_GenerateSWInterrupt(EXTI_Line5);
	
// 				EXTI_InitStructure.EXTI_Line = EXTI_Line5;
// 				EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
// 				EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
// 				EXTI_InitStructure.EXTI_LineCmd = ENABLE; //中断线使能
// 				EXTI_Init(&EXTI_InitStructure); //初始化中断
// 				EXTI_GenerateSWInterrupt(EXTI_Line5);
	
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
			NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn; //中断通道
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =  0;  //强占优先级
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//次优先级
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //通道中断使能
			NVIC_Init(&NVIC_InitStructure);//初始化中断
			
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
			NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; //中断通道
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =  1;  //强占优先级
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//次优先级
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //通道中断使能
			NVIC_Init(&NVIC_InitStructure);//初始化中断
	
}
void EXTI1_IRQHandler(void)
{
			if (key1==1)
		{
				GPIO_Write(GPIOB,0xfE<<8);
				delay(0x7fffff);
				EXTI_ClearITPendingBit(EXTI_Line1);
				
		}			
}
void EXTI9_5_IRQHandler(void)
{
			if (key2==1)
		{
				GPIO_Write(GPIOB,0xFF<<8);
				delay(0x7fffff);
				EXTI_ClearITPendingBit(EXTI_Line5);
				
		}			
}
int main()
{			
				RCC_Configuration();
				NVIC_Configuration();	//NVIC配置
				GPIO_Configuration(); 
				GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
				GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);

				while (1)
				{
						GPIO_Write(GPIOB,0xf0<<8);
				}
}


