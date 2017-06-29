#include "stm32f10x.h"
#include "LCD_driver.h"
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
unsigned char ii;
#ifdef DEBUG
  debug();
#endif

   RCC_Configuration();      //系统时钟配置函数   

   NVIC_Configuration();     //NVIC配置函数

    //启动GPIO模块时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    //把调试设置普通IO口
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);  
                            
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	 //所有GPIO为同一类型端口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //输出的最大频率为50HZ
  GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA端口
  GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化GPIOB端口

   GPIO_Write(GPIOA,0xffff);  //将GPIOA 16个端口全部置为高电平
   GPIO_Write(GPIOB,0xffff);  //将GPIOB 16个端口全部置为高电平

   LCD_Init();

    for(ii=0;ii<7;ii++)
    LCD_clear(ii);	//刷屏测试
    LCD_clear(2);  // 这句是显示背景颜色。


   //ChineseChar(60,20,2,colors[5],colors[2],1);	//普
   //ChineseChar(90,20,2,colors[5],colors[2],2);	//中
   ChineseChar(60,20,2,colors[5],colors[2],3);//科
   ChineseChar(150,20,2,colors[5],colors[2],4);	//技

    LCD_ShowString(0,90,colors[0],colors[2],"WWW.wangyunfei.com");
		LCD_ShowString(0,150,colors[7],colors[2],"www.wangyunfei.net");	   
    LCD_ShowString(12,180,colors[0],colors[2],"Joshua");
    LCD_ShowString(12,210,colors[7],colors[2],"18224525470");	
    LCD_ShowString(12,240,colors[7],colors[2],"Joshua is the best"); 
    LCD_ShowString(12,265,colors[7],colors[2],"he is number 1"); 
    LCD_ShowString(12,290,colors[7],colors[2],"hahahhahahaha"); 
		
   
   while(1);
    
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
