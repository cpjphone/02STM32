
/*********************************************************************************      
* 【语    言】：C语言
* 【编译环境】：Keil4  
* 【程序功能】: 串口2 —— GSM 打电话 发短信  收电话
* 【技术支持】：				
* 【晶    振】: RCC 8*9=72MHz		
* 【芯    片】: stm32f103VE 
* 【日    期】: 2014/03/25
* 【修改日期】: 2014/03/26
* 【作    者】: Phone
* 【编程理念】: 死东西活用，活东西套用。
*********************************************************************************/
#include "stm32f10x.h"
#include <stdio.h>
#include "stm32f10x_usart.h"
//#include "misc.h"
#include <string.h>

/*************************** 宏定义*********************************************/
#define uint unsigned int
#define uchar unsigned char

#define Send_ENGLISH_key GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)		  //发短信
#define Send_CALLING_key GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)

#define MAXCHAR 81
char  aa[MAXCHAR];

uchar ATE0[]="ATE0\r\n";
uchar CREG_CMD[]="AT+CREG?\r\n";
uchar SMS_send[]="AT+CMGS=18\r\n";
uchar ATCN[]="AT+CNMI=2,1\r\n";
uchar CMGF0[]="AT+CMGF=0\r\n";
uchar CMGF1[]="AT+CMGF=1\r\n";
uchar CMGR[12]="AT+CMGR=1\r\n";
uchar CMGD[12]="AT+CMGD=1\r\n";
//康佳俊 ： 18439893663   8134893966F3

/****************亚飞**********************/
// #define CALL_YOU_TELNUMBER "ATD15238643714;\r\n"      //填入你手上的手机号码 不是板子的
// #define SEND_MESSA_TO_YOUR "at+cmgs=\"15238643714\"\r\n"    //填入你手上的手机号码 不是板子的
// #define SEND_MESSA_CONTENT "tpytongxin.taobao.com"
// #define CALL_YOU_TELNUMBER "ATD15238643714;\r\n"    //填入你手上的手机号码 不是板子的
// //#define CALL_YOU_TELNUMBER "ATD15238643714;\r\n"    //填入你手上的手机号码 不是板子的

/****************亚飞**********************/
#define CALL_YOU_TELNUMBER "ATD18439893663;\r\n"      //填入你手上的手机号码 不是板子的
#define SEND_MESSA_TO_YOUR "at+cmgs=\"18439893663\"\r\n"    //填入你手上的手机号码 不是板子的
#define SEND_MESSA_CONTENT "tpytongxin.taobao.com"
#define CALL_YOU_TELNUMBER "ATD18439893663;\r\n"    //填入你手上的手机号码 不是板子的
//#define CALL_YOU_TELNUMBER "ATD15238643714;\r\n"    //填入你手上的手机号码 不是板子的

uchar  Sms2_Pdu[]="0891683108200205F011000B818134893966F30008A704521B601D";   //佳俊
uchar tab[] = "00.0";
uchar a,j=0,flag=0;
int g_timeout=0;

/************************************************************************/
//CPU延时函数
/************************************************************************/
void delay(uint ms)// 延时子程序
{
    uchar i,h;
    while(ms--)
    {
        for(i=0;i<140;i++);
				{
						for(h = 0; h < 25;h--);
				}
    }
}

/************************************************************************/
//STM32向串口2发送1字节   
/************************************************************************/
void Print_Char(u8 ch)
{
  USART_SendData(USART2, (u8) ch);
  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

/************************************************************************/
//发送一个字符串
/************************************************************************/
void Print_Str(u8* buf )
{   
	while(*buf!='\0')
	{ 
		Print_Char(*buf);
		delay(3);
		buf++;
	}
}

/************************************************************************/
//复位和时钟控制 配置
/************************************************************************/
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

/************************************************************************/
//清除自定缓冲区
/************************************************************************/
void clearBuff(void)
{
	for(j=0;j<MAXCHAR;j++)
	{
		aa[j]=0x00;
	}
	j=0;
}

/************************************************************************/
//检测单片机和模块的连接
/************************************************************************/
int Hand()
{
	delay(10);
	if(strstr(aa,"OK")!=NULL)             //检测单片机和模块的连接
	{
		g_timeout=0;
		return 1;
	}
	if(g_timeout>50)
	{
		g_timeout=0;
		return -1;
	}
	g_timeout++;
	return 0;
}

/************************************************************************/
//GSM 初始化 
/************************************************************************/
void AT(void)
{
	clearBuff();
	Print_Str(ATE0);
	delay(50);

	while(strstr(aa,"OK")==NULL)		//模块连接【..】
	{
		delay(50);     
		clearBuff();
		Print_Str(ATE0);
	}
	/*********** 模块连接【OK】*************************/
	Print_Str(ATCN);
	delay(50);

	clearBuff();
	Print_Str(CREG_CMD);
	delay(50);
	if(((aa[10]=='0')&&(aa[12]=='1'))||((aa[10]=='0')&&(aa[12]=='5')))//模块注册【OK】
	{
		clearBuff();
	}
	else   //模块注册【..】
	{
		clearBuff();
		delay(50);
	}		
}

/************************************************************************/
//打电话
/************************************************************************/
void send_call()
{
    clearBuff();
    delay(100);
    Print_Str(CALL_YOU_TELNUMBER); //打电话
    clearBuff();
    delay(300);
}
/************************************************************************/
//发送英文短信
/************************************************************************/
void send_english()
{
	clearBuff();
	Print_Str(CMGF1);
	delay(100);
	Print_Str(SEND_MESSA_TO_YOUR);
	delay(600);
	Print_Str(tab);  //发短信内容
	delay(10);
	Print_Char(0x1A);      //发送结束符号
	delay(200);
	clearBuff();
}

/************************************************************************/
//发送中午短信
/************************************************************************/
void send_pdu()
{
	clearBuff();
	Print_Str(CMGF0);
	delay(100);
	Print_Str(SMS_send);  //发送中文短信
	delay(500);
	Print_Str(Sms2_Pdu);  //发短信内容
	Print_Char(0x1A);      //发送结束符号
	delay(500);
	//led(0x04);
	clearBuff();
}

/************************************************************************/
//发送温度
/************************************************************************/
void send_18B20()
{

}

/************************************************************************/
//串口2中断配置
/************************************************************************/
void NVIC_Configuration(void)
{
   NVIC_InitTypeDef NVIC_InitStructure;
  
   /* Set the Vector Table base location at 0x08000000 */
   NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
  
   /* Configure the NVIC Preemption Priority Bits */  
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
   /* Enable the USART1 Interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;       //通道设置为串口1中断
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	   //中断响应优先级0
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		   //打开中断
   NVIC_Init(&NVIC_InitStructure); 						   //初始化
}

/************************************************************************/
//初始化串口硬件设备，启用中断
/************************************************************************/
void USART1_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 第1步：打开GPIO和USART部件的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  // 选择所有脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //配置成推挽式输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //输出模式下 I/O输出速度 50M HZ
  GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PB口
	
	/* 第2步：将USART Tx的GPIO配置为推挽复用模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* 第3步：将USART Rx的GPIO配置为浮空输入模式
		由于CPU复位后，GPIO缺省都是浮空输入模式，因此下面这个步骤不是必须的
		但是，我还是建议加上便于阅读，并且防止其它地方修改了这个口线的设置参数
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*  第3步已经做了，因此这步可以不做
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 第4步：配置USART1参数
	    - BaudRate = 115200 baud
	    - Word Length = 8 Bits
	    - One Stop Bit
	    - No parity
	    - Hardware flow control disabled (RTS and CTS signals)
	    - Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

    /* 若接收数据寄存器满，则产生中断 */
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	/* 第5步：使能 USART1， 配置完毕 */
	USART_Cmd(USART2, ENABLE);

    /* 如下语句解决第1个字节无法正确发送出去的问题 */
  USART_ClearFlag(USART2, USART_FLAG_TC);     // 清标志
}


/************************************************************************/
//中断服务函数
/************************************************************************/
void USART2_IRQHandler(void)
{  
	if(USART_GetITStatus(USART2, USART_IT_RXNE)==SET)
	{  
		aa[j] = USART_ReceiveData(USART2);  	                                  			   
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		j++; 
	}

	//溢出-如果发生溢出需要先读SR,再读DR寄存器则可清除不断入中断的问题[牛人说要这样]
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART2,USART_FLAG_ORE);
	}
}

/************************************************************************/
//主函数
/************************************************************************/
int main(void)
{
	RCC_Configuration();
	NVIC_Configuration();
	USART1_Configuration();
	GPIO_SetBits(GPIOA,GPIO_Pin_11); 
	GPIO_SetBits(GPIOA,GPIO_Pin_4); 
	//GPIO_Write(GPIOA,0xffff);	
	clearBuff();

	AT();		   //初始化模块
	Print_Str(CMGD);
	delay(50);
	while(Hand()==0);
	clearBuff();

	Print_Str(CMGF1);
	delay(30);

	Print_Str(ATCN);
	delay(30);
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)     //打电话按键
		{	  
			delay(100);
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)
			{

				//v_Lcd12864PutString_f( 1,1, "给你电话【..】") ;
				send_call();
				//send_english();//发英文TEXT短信按键
			}
			//v_Lcd12864PutString_f( 1,1, "给你电话【OK】") ;

		}
		
		if(strstr(aa,"RING")!=NULL)
		{
			//FENGMINGQI=0;
			delay(2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			//GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			//		v_Lcd12864PutString_f( 1,1, "电话来了【亲】") ;
			clearBuff();
		}
	}
}


