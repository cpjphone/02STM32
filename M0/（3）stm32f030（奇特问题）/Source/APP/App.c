 
 /*********************************************************************************      
* 【语        言】：C语言
* 【编译环境】：Keil4  
* 【程序功能】：
* 【技术支持】：Cortex-M0
* 【晶        振】: RCC 8*6=48MHz		
* 【芯        片】: stm32f030f4
* 【日        期】: 2014/07/29
* 【修改日期】: 
* 【作        者】: Phone
* 【编程理念】: 死东西活用，活东西套用。
*********************************************************************************/
/*
 * PA2  -  ce使能
 * PA3  -  cs片选
 * PA0  -  irq中断
 */
 
/* Includes ------------------------------------------------------------------*/
#include <stm32f0xx.h>
#include "includes.h"
#include <ctype.h>  
#include <string.h>
/* 用于串口发送空间大小的运用 */
#define MAXCHAR 81
extern  char aa[81];

/* Private configuration  for  NRF24L01 --------------------------------*/
/* 用于NEF24L01的发送码 */
u8 status;	                                 //用于判断接收/发送状态
u8 txbuf[8]={6,6,6,6,8,8,8,8};	     //发送缓冲

/*********************************************************************************
  * @brief       主函数.
+
  * @param    无
  *	@arg         NULL
  * @retval     null
  *********************************************************************************/
int main(void)
{
    int i;
    ALL_Config();
    LED_OFF();

    send_call();
    //send_english();
    /*检测NRF模块与MCU的连接*/
    while(!NRF_Check()); 
    NRF_TX_Mode();
    //   clearBuff();  //这句 居然影响了下面
    LED_ON();
 
    while(1)
    {
        /*开始发送数据*/	
        status = NRF_Tx_Dat(txbuf);	  
        
        if((strstr(aa,"RING")!=NULL))
        {    
           LED_OFF(); 
        }
    }
 
}
























          
