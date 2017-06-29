/********************************************************************************
*  描述:                LCD1602 滚动显示                                       *
* 显示方式：                                                                   *
* 1、从左到右逐字显示，闪动二次，清屏。                                        *
* 2、再从右到左逐字显示，闪动二次，清屏。                                      *
* 3、周期性地重复上述显示方式。                                                *                            
********************************************************************************/
	
#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"

extern unsigned char str1[];
extern unsigned char str2[];
extern unsigned char  dis1[];
extern unsigned char  dis2[];
extern unsigned char  aa[];

//extern char receive();
extern unsigned char read_TRH(void);
#define DHT11_Out PAout(1)
#define DHT11_In  PAin(1)


/********************************硬件接口定义*********************************/
#define RS  GPIO_Pin_1 //P2^6;             
#define RW  GPIO_Pin_2 //P2^5;
#define EN  GPIO_Pin_0 //P2^7;

/********************************宏定义*********************************/
#define    LCD_RS(x)  x ? GPIO_SetBits(GPIOB, RS): GPIO_ResetBits(GPIOB, RS)
#define    LCD_RW(x)  x ? GPIO_SetBits(GPIOB, RW): GPIO_ResetBits(GPIOB, RW)
#define    LCD_EN(x)  x ? GPIO_SetBits(GPIOB, EN): GPIO_ResetBits(GPIOB, EN)

/******************************** 变量定义 ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;     //GPIO
ErrorStatus HSEStartUpStatus;

	
typedef unsigned char BYTE;
typedef unsigned char BOOL; 


/*********************************声明函数 -----------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration(void);
void NOP(void);
 

	
/*******************************************延时函数**********************************/ 
void NOP()
{ vu16 i;
  for(i=0; i<100; i++); 
}

/*******************************************延时函数**********************************/ 
void delay1(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}

/******************************测忙*************************************************/	
	BOOL lcd_bz()
	{                         // 测试LCD忙碌状态
	BOOL result;
	LCD_RS(0);  //0;
	LCD_RW(1);  //1;
	LCD_EN(1);  //1;
	NOP();
	NOP();
	result = (GPIO_ReadInputData(GPIOB) & 0x8000)>>8;
	LCD_EN(0); // 0;
	return result; 
	}
	
/*************************************************************************************
写指令
*************************************************************************************/
	lcd_wcmd(int cmd)
	{                          // 写入指令数据到LCD
	while(lcd_bz());
	LCD_RS(0); // 0;
	LCD_RW(0); // 0;
	LCD_EN(0); // 0;
	NOP();
	NOP(); 
	GPIOB->BSRR = cmd<<8 & 0xff00;  //将数据送到P0口 
  GPIOB->BRR = ((~cmd)<<8) & 0xff00;
	NOP();
	NOP();
	LCD_EN(1); // 1;
	NOP();
	NOP();
	LCD_EN(0); // 0;  
	}
/******************************设置位置*************************************************/		
	lcd_pos(BYTE pos)
	{                          //设定显示位置
	lcd_wcmd(pos | 0x80);
	}

/******************************设置位置*************************************************/	
	lcd_wdat(int dat) 
	{                          //写入字符显示数据到LCD
	while(lcd_bz());
	LCD_RS(1); //= 1;
	LCD_RW(0); //= 0;
	LCD_EN(0); //= 0;
	
	GPIOB->BSRR = dat<<8 & 0xff00;    ////P0 = dat
    GPIOB->BRR = ((~dat)<<8) & 0xff00;
	NOP();
	NOP();
	LCD_EN(1);// = 1;
	NOP();
	NOP();
	LCD_EN(0);// = 0; 
	}
	
	lcd_init()
	{                        //LCD初始化设定
	lcd_wcmd(0x38);          //16*2显示，5*7点阵，8位数据
	lcd_wcmd(0x0c);          //显示开，关光标
	lcd_wcmd(0x06);          //移动光标
	lcd_wcmd(0x01);          //清除LCD的显示内容
	}

	
main()
{
	unsigned char i,n=0x40,m;
	RCC_Configuration();      //系统时钟配置函数 

	NVIC_Configuration();     //NVIC配置函数 
	//启动GPIO模块时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB1Periph_TIM2 | RCC_APB2Periph_AFIO, ENABLE);
	//把调试设置普通IO口
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; //所有GPIO为同一类型端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //输出的最大频率为50HZ
	//GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA端口
	GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化GPIOB端口

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; //所有GPIO为同一类型端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //输出的最大频率为50HZ
	GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化GPIOB端口
// 	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //所有GPIO为同一类型端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //输出的最大频率为50HZ
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOB端口

	GPIO_Write(GPIOA,0xffff);  //将GPIOA 16个端口全部置为高电平
	GPIO_Write(GPIOB,0xffff);  //将GPIOB 16个端口全部置为高电
  delay_init(72);	
	lcd_init();               // 初始化LCD
	delay1(10);
	while(1)              
	{ 
		read_TRH();		//读取数据
		for(i=0;i<=7;i++)			//字符显示
		{
			lcd_pos(i);    //第一行显示
			lcd_wdat(dis1[i]);
			lcd_pos(n+i);    //第二行显示
			lcd_wdat(dis2[i]);
		}
		 m=0x08;
		for(i=0;i<=7;i++)    		//湿度显示  
		{
			lcd_pos(m); 
			lcd_wdat(str1[i]);
			m++;
		}
		m=0x48;
		for(i=0;i<=7;i++)      		//温度显示
		{
			lcd_pos(m); 
			lcd_wdat(str2[i]);
			m++;
		}
		delay_ms(1000) ;		
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
 NVIC_InitTypeDef NVIC_InitStructure;
#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif

  
       /* 开启定时器2 */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
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
	 
	 
