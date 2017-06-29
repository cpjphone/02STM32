/******************** 普中科技 www.prechin.com************ ********************
* 
*                          8位态数码管实验
* 
*   实验目的： 	熟悉了解用C语言在STM32平台上控制8位动态数码管。
* 
*   连接方法： 参考“连接说明”的图片演示
*
*   实验现象： 数码管显示数据累加变化
*
*******************************************************************************/


#include "stm32f10x_lib.h"

 /********************************硬件接口定义*********************************/
#define    LS138A  GPIO_Pin_5 //=P2^2;       //138译码器管脚定义
#define    LS138B  GPIO_Pin_4 //=P2^3;
#define    LS138C  GPIO_Pin_3 //=P2^4; 


/********************************宏定义*********************************/
#define    LS138a(x)  x ? GPIO_SetBits(GPIOB, LS138A): GPIO_ResetBits(GPIOB, LS138A)
#define    LS138b(x)  x ? GPIO_SetBits(GPIOB, LS138B): GPIO_ResetBits(GPIOB, LS138B)
#define    LS138c(x)  x ? GPIO_SetBits(GPIOB, LS138C): GPIO_ResetBits(GPIOB, LS138C)

/******************************** 变量定义 ------------------------------------*/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
ErrorStatus HSEStartUpStatus;

u16  PrescalerValue;
u8  Count;
unsigned long D[16], LedOut[8];

// 此表为 LED 的字模          
//此表为 LED 的字模, 共阴数码管 0-9  - 
unsigned char  Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40}; 

/*********************************声明函数 -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void SysTick_Configuration(void);
void NVIC_Configuration(void);




/*******************************************延时函数**********************************/ 
void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}

/*******************************************************************************
* 
*            主函数
* 
*******************************************************************************/
int main(void)
{  unsigned char i;
#ifdef DEBUG
  debug();
#endif

  RCC_Configuration();  //系统时钟配置

  NVIC_Configuration();	 //NVIC配置

  GPIO_Configuration();	 //通用IO端口配置

   while (1)
  {  D[1] = D[0] = Count++;
  	/********以下将数据的值送到LED数码管显示*************/
    
	  LedOut[0]=Disp_Tab[D[1]%10000/1000]; //千位
      LedOut[1]=Disp_Tab[D[1]%1000/100];   //百位
      LedOut[2]=Disp_Tab[D[1]%100/10]|0x80;	//十位
      LedOut[3]=Disp_Tab[D[1]%10];	 //个位
	  
	  LedOut[4]=Disp_Tab[D[0]%10000/1000]; ;//千位
      LedOut[5]=Disp_Tab[D[0]%1000/100]; ;//百位
      LedOut[6]=Disp_Tab[D[0]%100/10]|0x80;//十位
      LedOut[7]=Disp_Tab[D[0]%10];      //个位
  	  
      for(i=0; i<8; i++) 
  	  {		
     	//GPIOA->BSRR = LedOut[i] & 0x00FF;    //屏蔽高位操作
        //GPIOA->BRR = (~LedOut[i]) & 0x00FF;  //屏蔽高位操作
	    GPIO_Write(GPIOA , LedOut[i] )	;		 	 	  	
		switch(i)					  
         {	    	  //138译码
			case 0:	  //0 0 0 
			LS138a(0);	 LS138b(0);  LS138c(0);
			break;         
            case 1:	 //1 0 0 
			LS138a(1);	 LS138b(0);  LS138c(0);
			 break;             	
            case 2:	 //0 1 0
			LS138a(0);	 LS138b(1);  LS138c(0);
			break; 
            case 3:	 //1 1 0
			LS138a(1);	 LS138b(1);  LS138c(0);
			break;
			
			case 4:	 //0 0 1
			LS138a(0);	 LS138b(0);  LS138c(1);
			break; 
			
			case 5:	 //1 0 1
			LS138a(1);	 LS138b(0);  LS138c(1);
			break; 

			case 6:	 //0 1 1
			LS138a(0);	 LS138b(1);  LS138c(1);
			break;

			case 7:	 //1 1 1
			LS138a(1);	 LS138b(1);  LS138c(1);
			break;
		 	 
         }
		  Delay(20000); //延时函数
	  }

	
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

  /* TIM1, GPIOA and GPIOB clock enable */
 // RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
}

/*************************************************
函数: void GPIO_Config(void）
功能: GPIO配置
**************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

		//启动GPIO模块时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB1Periph_TIM2 | RCC_APB2Periph_AFIO, ENABLE);
	//把调试设置普通IO口
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);  

   /* GPIOA Configuration: */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; //所有GPIO为同一类型端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //输出的最大频率为50HZ
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA端口
	GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化GPIOB端口


}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures the nested vectored interrupt controller.
* Input          : None
* Output         : None
* Return         : None
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

   //NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick, 0, 0);
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

  while (1)
  {
  
   }
}
#endif
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
