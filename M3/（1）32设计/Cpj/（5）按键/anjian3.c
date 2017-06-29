/***************************普中科技 www.prechin.com****************************
	                          STM32-矩阵键盘实验
 实验目的： 熟悉了解STM32GPIO的操作和库函数的使用
 连接方法： JP8连JP3  JP10连JP4  
 实验现象：	每个按键的键值显示在景泰数码上

*******************************************************************************/

#include "stm32f10x_lib.h"

/******************************** 宏定义定义 ---------------------------------------------------------*/
#define uchar unsigned char	 //宏的定义变量类型 uchar 代替 unsigned char
#define uint  unsigned int	 //宏的定义变量类型 uint  代替 unsigned int

/******************************** 变量定义 ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
ErrorStatus HSEStartUpStatus;

/*********************************声明函数 -----------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration(void);
void Delay(vu32 nCount);
void delay0(uchar x);      //x*0.14MS
void  keyscan(void);
void  keydown(void);


// 此表为 LED 的字模          	   0	  1		2	 3	   4	 5		6	 7		8	 9	  a		  b	   c	d	  e		 f
unsigned char  LED7Code[] = {~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};
uchar  dis_buf;            //显示缓存
uchar  temp;
uchar  key;                //键顺序吗

/*******************************************************************************
                                  主函数
*******************************************************************************/
int main(void)
{ //int retval;
#ifdef DEBUG
  debug();
#endif

  RCC_Configuration(); //系统时钟配置函数    

  NVIC_Configuration();	 //NVIC配置函数

  //使能APB2总线外设时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
  //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);  //关闭调试 端口重新映射  使用仿真器调试时，不能用此语

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; // 选择所有脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //配置成推挽式输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //输出模式下 I/O输出速度 50M HZ
   GPIO_Init(GPIOA, &GPIO_InitStructure);  //初PA口始化
  
   GPIO_Write(GPIOA,0XFFFF); //初始化PA端口让 LED处于熄灭状态
  
  
    while(1)
    { 
       keydown();	 //调用按键判断检测程序
	   GPIO_Write(GPIOA,LED7Code[dis_buf%16]&0x7f);	//LED7 0x7f为小数点 共阴和共阳此处也是不一样;  %16表示输出16进制
    
    }
 	
}

/*************************************************************
*                                                           *
* 延时子程序                                                *
*                                                           *
*************************************************************/

void  delay(uchar x)
{ uchar j;
    while((x--)!=0)  //CPU执行x*12次
    { for(j=0;j<125;j++)
         {;}
    }
}

/*************************************************************
*                                                            *
* 键扫描子程序  (4*3 的矩阵) P0.4 P0.5 P0.6 P0.7为行         *
*							  P0.1 P0.2 P0.3为列             *
*                            								 *
*************************************************************/

 void  keyscan(void)
 { 	temp = 0;
    //P1=0xF0;                 //高四位输入   行为高电平  列为低电平
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 |  GPIO_Pin_9 |  GPIO_Pin_10 |  GPIO_Pin_11 ; // 选择所有脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //配置成推挽式输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //输出模式下 I/O输出速度 50M HZ
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //初PA口始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 |  GPIO_Pin_13 |  GPIO_Pin_14 |  GPIO_Pin_15 ; // 选择所有脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //配置成推挽式输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //输出模式下 I/O输出速度 50M HZ
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //初PA口始化

	GPIO_Write(GPIOB,0XF0FF); //初始化端口
    delay(1);				 //延时
	//temp=P1;               //读P1口 
    temp=GPIO_ReadInputData(GPIOB)>>8&0xF0;	 //高8位的屏蔽低四位
    temp=~((temp>>4)|0x00F0); //将高四位移到地位处处理 	  
    if(temp==1)	  // p0.4 被拉低
        key=1;	  //第一个按键值
    else if(temp==2)   // p0.5 被拉低
        key=2;	 //第2个按键值
    else if(temp==4)   // p0.6 被拉低
        key=3;	  //第3个按键值
    else if(temp==8)   // p0.7 被拉低
         key=4;	   //第4个按键值
    else
        key=16;
        
    //P1=0x0F;             //低四位输入  列为高电平 行为低电平
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 |  GPIO_Pin_9 |  GPIO_Pin_10 |  GPIO_Pin_11 ; // 选择所有脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //配置成推挽式输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //输出模式下 I/O输出速度 50M HZ
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //初PA口始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 |  GPIO_Pin_13 |  GPIO_Pin_14 |  GPIO_Pin_15 ; // 选择所有脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //配置成推挽式输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //输出模式下 I/O输出速度 50M HZ
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //初PA口始化

	GPIO_Write(GPIOB,0X0FFF);


    delay(1);			 //延时
	//temp=P1;             //读P1口
    temp=(GPIO_ReadInputData(GPIOB)>>8)&0x0F;
    temp=~(temp|0xF0);
    if(temp==2)		   //p0.1  被拉低
        key=key+0;
    else if(temp==4)   //p0.2  被拉低
        key=key+4;
    else if(temp==8)	//p0.3  被拉低
        key=key+8;
    else
        key=16;	 
    	
     dis_buf = key;	    //键值入显示缓存
     dis_buf = dis_buf & 0x0f;
 
 }

/*************************************************************
*                                                           *
*判断键是否按下                                             *
*                                                           *
*************************************************************/

 void  keydown(void)
 {   int retval;
  	//P1=0xF0;       //将高4位全部置1 低四位全部置0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 |  GPIO_Pin_9 |  GPIO_Pin_10 |  GPIO_Pin_11 ; // 选择所有脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //配置成推挽式输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //输出模式下 I/O输出速度 50M HZ
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //初PA口始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 |  GPIO_Pin_13 |  GPIO_Pin_14 |  GPIO_Pin_15 ; // 选择所有脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //配置成推挽式输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //输出模式下 I/O输出速度 50M HZ
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //初PA口始化
    
	GPIO_Write(GPIOB,0XF0FF);
	//P1!=0xF0
	retval =  GPIO_ReadInputData(GPIOB); //读PB口状态
	if(retval>>8!=0xF0)  //判断按键是否按下 如果按钮按下 会拉低P1其中的一个端口
	{
	  keyscan();  //调用按键扫描程序
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
