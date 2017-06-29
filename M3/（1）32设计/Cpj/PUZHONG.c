 
 /*********************************************************************************      
* 【语		言】：C语言
* 【编译环境】：Keil4  
* 【程序功能】：
* 【技术支持】：Cortex-M3
* 【晶		振】: RCC 8*9=72MHz		
* 【芯		片】: stm32f103RBt6
* 【日		期】: 2014/09/16
* 【修改日期】: 
* 【作		者】: Phone
* 【编程理念】: 死东西活用，活东西套用。
*********************************************************************************/
/*
 * PA2  -  ce使能
 * PA3  -  cs片选
 * PA0  -  irq中断
 */
 
/* Includes ------------------------------------------------------------------*/
#include <stm32f10x.h>
//#include "BSP.h"
//#include "gprs.h"
//#include "delay.h"
//#include "usart.h"
//#include "times.h"
#include <stdio.h>
#include "stm32f10x_usart.h"
//#include "misc.h"
#include <string.h>
#define ATE0_TEXT "TEXT\r\n"           /// 禁止回显


void Delay1(unsigned long  nCount)
{
    for(; nCount != 0; nCount--);
}


/*! System Clocks Configuration */
void RCC_Configuration(void)
{
    ErrorStatus HSEStartUpStatus;
    RCC_DeInit();                               /* RCC system reset(for debug purpose) */
    RCC_HSEConfig(RCC_HSE_ON);                  /* Enable HSE */
    HSEStartUpStatus = RCC_WaitForHSEStartUp(); /* Wait till HSE is ready */

    if(HSEStartUpStatus == SUCCESS)
    {
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);   /* Enable Prefetch Buffer */
        FLASH_SetLatency(FLASH_Latency_2);                      /* Flash 2 wait state */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);                        /* HCLK = SYSCLK */
        RCC_PCLK2Config(RCC_HCLK_Div1);
        RCC_PCLK1Config(RCC_HCLK_Div1);                         /* PCLK2 = HCLK */
        //        RCC_PCLK1Config(RCC_HCLK_Div2);                         /* PCLK1 = HCLK/2 */
        //注：AHB主要负责外部存储器时钟。APB2负责AD，I/O，高级TIM，串口1
        //  APB1负责DA，USB，SPI，I2C，CAN，串口2345，普通TIM
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);    /* PLLCLK = 8MHz * 9 = 72 MHz */
        RCC_PLLCmd(ENABLE);                                     /* Enable PLL */

        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)      /* Wait till PLL is ready */
        {
        }

        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);              /* Select PLL as system clock source */

        while(RCC_GetSYSCLKSource() != 0x08)                    /* Wait till PLL is used as system clock source */
        {
        }
    }
}

/*! System NVIC Configuration */
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

#ifdef VECT_TAB_RAM
    /* Set the Vector Table base location at 0x20000000 */
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else /* VECT_TAB_FLASH */
    /* Set the Vector Table base location at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//设置优先级分组形式1，即抢占级占一位，优先级占3位

    /* Enable the USART1 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    /* Enable the USART2 Interrupt*/
    NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable the USART3 Interrupt*/
    NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 /*| RCC_APB1Periph_TIM3*/,ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/*! GPIO Configuration */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // Configure the all GPIO port pins in Analog Input Mode(Floating input
    // trigger OFF), this will reduce the power consumption and increase the
    // device immunity against EMI/EMC
    // Enables or disables the High Speed APB(APB2) peripheral clock

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC  | RCC_APB2Periph_GPIOD  | RCC_APB2Periph_GPIOE, ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);  //Enable UART4 clocks

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //LED
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

#ifdef USE_STM3210E_EVAL
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);

    GPIO_Init(GPIOF, &GPIO_InitStructure);
    GPIO_Init(GPIOG, &GPIO_InitStructure);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, DISABLE);
#endif
}

/*! USART Configuration */
void USART_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure;
    USART_InitTypeDef USART_InitStructure_gps;

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_4;        //tx
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                   //rx
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                   //tx
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                  //rx
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                   //tx
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                  //rx
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_5|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    /* Configure USART1 basic and asynchronous paramters */
    USART_Init(USART1, &USART_InitStructure);
    USART_Init(USART2, &USART_InitStructure);


    USART_InitStructure_gps.USART_BaudRate = 9600;
    USART_InitStructure_gps.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure_gps.USART_StopBits = USART_StopBits_1;
    USART_InitStructure_gps.USART_Parity = USART_Parity_No ;
    USART_InitStructure_gps.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
    USART_InitStructure_gps.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure_gps);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    /* Enable USART1 */
    USART_Cmd(USART1, ENABLE);
    USART_Cmd(USART2, ENABLE);
    USART_Cmd(USART3, ENABLE);
}



void send_data_uart3(unsigned char Data)
{
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    USART_SendData(USART3, Data);
}


void send_string_uart3(char * S)
{
    while(*S)
    {
        send_data_uart3(*S++);
    }

}


/*********************************************************************************
  * @brief       主函数.
  * @param    		无
  *	@arg         NULL
  * @retval     null
  *********************************************************************************/
int main(void)
{
	//ALL_Config();
	  RCC_Configuration();
    NVIC_Configuration();
    GPIO_Configuration();
    USART_Configuration();
	//delay_init(72);	     //滴答时钟
	
	while(1)
	{		
		//GPRS_TCPIP();
		
		send_string_uart3(ATE0_TEXT);

		GPIO_SetBits(GPIOC,GPIO_Pin_1);	 		//OFF
		Delay1(0X1FFFFF);
		GPIO_ResetBits(GPIOC,GPIO_Pin_1);	
		Delay1(0X1FFFFF);
	}
}
























          
