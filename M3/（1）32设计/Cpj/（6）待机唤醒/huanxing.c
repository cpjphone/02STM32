

#include "stm32f10x.h"
	  

/**********************************************************************
* 名    称：Delay()
* 功    能：延时
* 入口参数：cnt
* 出口参数：
-----------------------------------------------------------------------
* 说明：
***********************************************************************/
void Delay(vu16 cnt) 
{
  u16 i,j;

  for (i=0;i<cnt;i++)
  { 
    for (j=0;j<1000;j++)
    { 
	}
  }
}

/*************************************************
函数: void RCC_Configuration(void)
功能: 配置系统时钟
参数: 无
返回: 无
**************************************************/
void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;                    //定义外部高速晶体启动状态枚举变量
  RCC_DeInit();                                    //复位RCC外部设备寄存器到默认值
  RCC_HSEConfig(RCC_HSE_ON);                       //打开外部高速晶振
  HSEStartUpStatus = RCC_WaitForHSEStartUp();      //等待外部高速时钟准备好
  if(HSEStartUpStatus == SUCCESS)                  //外部高速时钟已经准别好
  {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //开启FLASH预读缓冲功能，加速FLASH的读取。所有程序中必须的用法.位置：RCC初始化子函数里面，时钟起振之后
    FLASH_SetLatency(FLASH_Latency_2);                    //flash操作的延时
      	
    RCC_HCLKConfig(RCC_SYSCLK_Div1);               //配置AHB(HCLK)时钟等于==SYSCLK
    RCC_PCLK2Config(RCC_HCLK_Div1);                //配置APB2(PCLK2)钟==AHB时钟
    RCC_PCLK1Config(RCC_HCLK_Div2);                //配置APB1(PCLK1)钟==AHB1/2时钟
         
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  //配置PLL时钟 == 外部高速晶体时钟 * 9 = 72MHz
    RCC_PLLCmd(ENABLE);                                   //使能PLL时钟
   
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)    //等待PLL时钟就绪
    {
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //配置系统时钟 = PLL时钟
    while(RCC_GetSYSCLKSource() != 0x08)                  //检查PLL时钟是否作为系统时钟
    {
    }
  }

}




/**********************************************************************
* 名    称：GPIO_Configuration()
* 功    能：配置输入输出
* 入口参数： 
* 出口参数：
-----------------------------------------------------------------------
* 说明：
***********************************************************************/
void GPIO_Configuration(void)
{

  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);	//给GPIOA、GPIOB提供时钟

  /*设置PB0为输出*/
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  
}


void Sys_Standby(void)
{
	SCB->SCR|=1<<2;//使能SLEEPDEEP位 (SYS->CTRL)
	NVIC_SystemLPConfig(NVIC_LP_SLEEPDEEP,ENABLE);	   	//这句话的功能就是上面那句SCB->SCR 的第二（SLEEPDEEP）位至1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//使能PWR外设时钟   电源时钟打开 

	PWR_WakeUpPinCmd(ENABLE);  //使能唤醒管脚功能	   使能某端口  让它作为唤醒引脚

	/* Request to enter STANDBY mode (Wake Up flag is cleared in PWR_EnterSTANDBYMode function) */
	PWR_EnterSTANDBYMode();	  //进入待命（STANDBY）模式  STM32 进入待命模式		 
}

/*************************************************
函数: int main(void)
功能: main主函数
参数: 无
返回: 无
**************************************************/
int main (void) 
{

  RCC_Configuration();                      
  GPIO_Configuration(); 

  while(1)
  {
    GPIO_WriteBit(GPIOB, GPIO_Pin_0,(BitAction)1);//置1
    Delay(10000);
    GPIO_WriteBit(GPIOB, GPIO_Pin_0,(BitAction)0);//置0
    Delay(10000);
    GPIO_WriteBit(GPIOB, GPIO_Pin_0,(BitAction)1);//置1
    Delay(10000);
    GPIO_WriteBit(GPIOB, GPIO_Pin_0,(BitAction)0);//置0
    Delay(10000);
    GPIO_WriteBit(GPIOB, GPIO_Pin_0,(BitAction)1);//置1
    Delay(10000);
    GPIO_WriteBit(GPIOB, GPIO_Pin_0,(BitAction)0);//置0
    Delay(10000);
   	GPIO_WriteBit(GPIOB, GPIO_Pin_0,(BitAction)1);//置1
    Delay(10000);
    GPIO_WriteBit(GPIOB, GPIO_Pin_0,(BitAction)0);//置0
    Delay(10000);
	Sys_Standby();  // 到这句小灯就不闪烁了 ，若给PA0 及WK_UP引脚一个上升沿触发	  则STM32又进入	 正常模式	
  }
}


