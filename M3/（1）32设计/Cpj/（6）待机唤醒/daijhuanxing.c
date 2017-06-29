#include "sys.h"
#define uint unsigned int
#define key GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
//u8 Check_WKUP2(void);

ErrorStatus HSEStartUpStatus;


void ledinit(void);
void delay(uint n)
{
		for (;n>0;n--);
}

void WKUP_Init(void)
{
				
					EXTI_InitTypeDef EXTI_InitStructure;
					NVIC_InitTypeDef NVIC_InitStructure;
					GPIO_InitTypeDef GPIO_InitStructure;	    //结构体声明

//					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA
//                         | RCC_APB2Periph_AFIO, ENABLE);	    //库函数使能GPIOA,GPIOB 和复用功能时钟

					RCC->APB2ENR|=1<<2;     					//寄存器使能GPIOA，GPIOB 和复用功能时钟   
					RCC->APB2ENR|=1<<0;    						//开启辅助时钟
					RCC->APB2ENR|=1<<3;
					GPIOB->CRH&=0XFFFFFFF0;
					GPIOB->CRH|=0X00000003;

					GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7); 
															   //选择GPIO管脚用作外部中断线路
					
	
					GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
					GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
															  //浮空输入
					GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
					EXTI_InitStructure.EXTI_Line = EXTI_Line7;
					EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
					EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
					EXTI_InitStructure.EXTI_LineCmd = ENABLE; 
					EXTI_Init(&EXTI_InitStructure); 
					//EXTI_GenerateSWInterrupt(EXTI_Line7);			//产生一个软件中断

					NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
					NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; 
					NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =  0; 
					NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
					NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
					NVIC_Init(&NVIC_InitStructure);
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
/*****************************************
funtion: 待机函数；电流2uA左右
*****************************************/
void Sys_Standby(void)
{
	//SCB->SCR|=1<<2;//使能SLEEPDEEP位 (SYS->CTRL)
	NVIC_SystemLPConfig(NVIC_LP_SLEEPDEEP,ENABLE);	   	//这句话的功能就是上面那句SCB->SCR 的第二（SLEEPDEEP）位至1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//使能PWR外设时钟   电源时钟打开 

	PWR_WakeUpPinCmd(ENABLE);  //使能唤醒管脚功能	   使能某端口  让它作为唤醒引脚

	/* Request to enter STANDBY mode (Wake Up flag is cleared in PWR_EnterSTANDBYMode function) */
	PWR_EnterSTANDBYMode();	  //进入待命（STANDBY）模式  STM32 进入待命模式		 
}

int main()
{
			RCC_Configuration();
			WKUP_Init();
			while(1)
			{
						GPIO_WriteBit(GPIOA,GPIO_Pin_7,1);
							//ledinit();
						GPIO_WriteBit(GPIOB,GPIO_Pin_8,Bit_RESET);
						delay(0x7fffff);
					//	delay_ms(800);
						GPIO_WriteBit(GPIOB,GPIO_Pin_8,Bit_SET);
						delay(0x7fffff);
						GPIO_WriteBit(GPIOB,GPIO_Pin_8,Bit_RESET);
						delay(0x7fffff);
						GPIO_WriteBit(GPIOB,GPIO_Pin_8,Bit_SET);
						delay(0x7fffff);
						//PWR_WakeUpPinCmd(DISABLE);	  //使能或者失能唤醒管脚功能   不管用

						//PWR_EnterSTANDBYMode ();	  //进入待命（STANDBY）模式

						PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);
						 //PWR_Regulator_ON  //停止模式下电压转换器 ON 
						 //PWR_Regulator_LowPower ;// 停止模式下电压转换器进入低功耗模式   
						 //PWR_STOPEntry_WFI  ;//(设置成任意中断唤醒)	PWR_STOPEntry_WFI  使用指令 WFI来进入停止模式
						  
						 //	Sys_Standby(); 		// ☆ 待机
						RCC_Configuration();
						GPIO_WriteBit(GPIOB,GPIO_Pin_8,Bit_RESET);
						delay(0x7fffff);
						GPIO_WriteBit(GPIOB,GPIO_Pin_8,Bit_SET);
						delay(0x7fffff);
		 	}
}

void EXTI9_5_IRQHandler(void)
{ 		    		    				     		    
	EXTI_ClearITPendingBit(EXTI_Line7);		  
} 
