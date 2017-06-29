

#include "stm32f10x.h"
#include "stdio.h"

#define uint unsigned int
#define uchar unsigned char
#define MAXCHAR 81
char  aa[MAXCHAR];
uchar j=0;

uchar ATE0[]="ATE0\r\n";
u8 temp[13]="#SERVERSDATA#";
u8 temp_1[2]="C#";
u8 temp_2[2]="%#";


// 			发送一个字符
u8 Uart1_PutChar(u8 ch)
{
    USART_SendData(USART1, (u8) ch);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
   {
   }
   return ch;
}
void delay(uint n)
{
		for(;n>0;n--);
}
void GPIO_Configuration()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC 
         | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG 
         | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  

  //开启AFIO时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

   //配置USARTx_Tx为复合推挽输出
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //配置 USARTx_Rx 为浮空输入
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void GPIO_Config_LED(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
	  //GPIOC ->ODR ^= GPIO_Pin_6;
}
//    发送一个字符串
void Uart1_PutString(u8* buf , u8 len)
{   
	u8 i;
    for(i=0;i<len;i++)
    {
        Uart1_PutChar(*buf++);
    }
}

void Print_Str(u8* buf )
{   
	while(*buf!='\0')
	{ 
		Uart1_PutChar(*buf);
		delay(15);
		buf++;
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

void usart_init ()
{
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//串口配置： 波特率 115200 数据位 8 停止位 1  奇偶位 NONE
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity =  USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	/* 若接收数据寄存器满，则产生中断 */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    /* 如下语句解决第1个字节无法正确发送出去的问题 */
    USART_ClearFlag(USART1, USART_FLAG_TC);     // 清标志
	//初始化串口
	USART_Init(USART1, &USART_InitStructure);
	//启动串口
	USART_Cmd(USART1, ENABLE);
	
	
}

void NVIC_Configuration(void)
{
   NVIC_InitTypeDef NVIC_InitStructure;
  
   /* Set the Vector Table base location at 0x08000000 */
   NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
  
   /* Configure the NVIC Preemption Priority Bits */  
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
   /* Enable the USART1 Interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;       //通道设置为串口1中断
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	   //中断响应优先级0
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		   //打开中断
   NVIC_Init(&NVIC_InitStructure); 						   //初始化
}
int main()
{
		unsigned char k[8];
		clearBuff();
		GPIO_Configuration();
	GPIO_Config_LED();
		usart_init ();
	  NVIC_Configuration();
		k[0]=0+0x30;
		k[1]=0+0x30;
		k[2]=0+0x30;
		k[3]=0+0x30;
	
	
// 	GPIO_SetBits(GPIOC,GPIO_Pin_6);	
// 	GPIO_SetBits(GPIOC,GPIO_Pin_7);	
// 	GPIO_ResetBits(GPIOC,GPIO_Pin_6);	
		while (1)
		{
			//Print_Str(ATE0);
			
			Uart1_PutString (temp,13);
			Uart1_PutString (&k[0],1);
			Uart1_PutString (&k[1],1);
			Uart1_PutString (temp_1,2);
			
			Uart1_PutString (&k[2],1);
			Uart1_PutString (&k[3],1);
			Uart1_PutString (temp_2,2);
			k[0]++;
			k[1]++;
			k[2]++;
			k[3]++;
			//Uart1_PutString ("#SERVERSDATA#12C#35%#",21);
			delay (0x7fffff);
			if ((k[0]>9+0x30) || (k[1]>9+0x30) || (k[2]>9+0x30) || (k[3]>9+0x30))
			{
				k[0]=0+0x30;
				k[1]=0+0x30;
				k[2]=0+0x30;
				k[3]=0+0x30;
			}
			if(strstr(aa,"A")!=NULL)
		{
		//	GPIO_SetBits(GPIOC,GPIO_Pin_6);	
			GPIO_SetBits(GPIOC,GPIO_Pin_7);	
			clearBuff();
	  }	
			if(strstr(aa,"B")!=NULL)
		{
			GPIO_SetBits(GPIOC,GPIO_Pin_6);	
		//	GPIO_SetBits(GPIOC,GPIO_Pin_7);	
			clearBuff();
	  }	
		}
}

/*******************************************************************/
/*                                                                 */
/* STM32在串口1接收1字节                                           */
/* 说明：串口1接收中断                                             */
/*                                                                 */
/*******************************************************************/
void USART1_IRQHandler(void)            //在中断服务程序中，由于主机响应中断时并不知道是哪个中断源发出中断请求，因此必须在中断服务程序中对中断源进行判别，然后分别进行处理。当然，如果只涉及到一个中断请求，是不用做上述判别的。但是无论什么情况，做上述判别是个好习惯
{  
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)    //若接收数据寄存器满
  {     
    aa[j] = USART_ReceiveData(USART1);          
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		j++;		
  }
	
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART1,USART_FLAG_ORE);
	}
}

