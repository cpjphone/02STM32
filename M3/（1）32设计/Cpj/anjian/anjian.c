#include "sys.h"


//ErrorStatus HSEStartUpStatus;
uint a;
#define KEY GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)

//void RCC_Configuration(void)
//{   
// //复位RCC外部设备寄存器到默认值
//  RCC_DeInit();
//
//  //打开外部高速晶振
//  RCC_HSEConfig(RCC_HSE_ON);
//
//   //等待外部高速时钟准备好
//  HSEStartUpStatus = RCC_WaitForHSEStartUp();
//
//  if(HSEStartUpStatus == SUCCESS)   //外部高速时钟已经准别好
//  {								    
//    //开启FLASH的预取功能
//    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
//
//    //FLASH延迟2个周期
//    FLASH_SetLatency(FLASH_Latency_2);
// 	
//  //配置AHB(HCLK)时钟=SYSCLK
//    RCC_HCLKConfig(RCC_SYSCLK_Div1);  
//  
//   //配置APB2(PCLK2)钟=AHB时钟
//    RCC_PCLK2Config(RCC_HCLK_Div1); 
//
//    //配置APB1(PCLK1)钟=AHB 1/2时钟
//    RCC_PCLK1Config(RCC_HCLK_Div2);
//
//     //配置PLL时钟 == 外部高速晶体时钟*9  PLLCLK = 8MHz * 9 = 72 MHz 
//    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
//
//    //使能PLL时钟
//    RCC_PLLCmd(ENABLE);
//
//   //等待PLL时钟就绪
//    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
//    {
//    }
//
//  //配置系统时钟 = PLL时钟
//    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
//
//   //检查PLL时钟是否作为系统时钟
//    while(RCC_GetSYSCLKSource() != 0x08)
//    {
//    }
//  }
//}

//GPIO_InitTypeDef GPIO_InitStructure;

//#define KEY1 PAin(15)
//#define KEY (1<<13)
//#define KEY1 (1<<15)
//#define KEY2 (1<<8)
//#define KEY_GET() ((GPIOA->IDR&(KEY))1:0)
//#define KEY1_GET() ((GPIOA->IDR&(KEY1))1:0)
//#define KEY2_GET() ((GPIOA->IDR&(KEY2))1:0)

void ledinit()
{
		RCC->APB2ENR|=1<<3;
		RCC->APB2ENR|=1<<2;
		GPIOB->CRH&=0X00000000;
		GPIOB->CRH|=0X33333333;
			  // 0011通用推挽输出，速度50MHz

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;  // 选择所有脚
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //配置成推挽式输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //输出模式下 I/O输出速度 50M HZ
//	GPIO_Init(GPIOA, &GPIO_InitStructure); 
}
int main()
{
	//	ledinit();
	//	GPIOB->ODR=0XFE00;
	//	GPIO_Write(GPIOB,0xff00);
		RCC->APB2ENR|=1<<3;
		RCC->APB2ENR|=1<<2;
		GPIOB->CRH&=0X00000000;
		GPIOB->CRH|=0X33333333;
		while(1)
		{

			if(KEY == 0)
			{
				//wait(0.002);
				//if(KEY == 0)
				GPIO_Write(GPIOB,0xff00);

			}
		}
		
}
