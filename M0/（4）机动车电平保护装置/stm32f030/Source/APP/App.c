 
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

#define CALL_NUM "ATD13700865872;\r\n"  
/* Private configuration  for  NRF24L01 --------------------------------*/
/* 用于NEF24L01的发送码 */
u8 status;	                                 //用于判断接收/发送状态
u8 txbuf[8]={6,6,6,6,8,8,8,8};	     //发送缓冲
extern int rep_flag;                                //重复发送标志
extern  char aa[48];

/*********************************************************************************
  * @brief       主函数.
  * @param    无
  *	@arg         NULL
  * @retval     null
  *********************************************************************************/
int main(void)
{
    ALL_Config();
    LED_OFF();
    GSM_start();
    clearBuff();
    //send_call();
    //send_english();
    /*检测NRF模块与MCU的连接*/
    // while(!NRF_Check()); 
    NRF_TX_Mode();
    while(1)
    {
        /*开始发送数据*/	
        NRF_Tx_Dat(txbuf);	  
        if(rep_flag) 
        {
            clearBuff();
            delay_ms(10);
            while(strstr(aa,"OK") ==NULL)
            {
                delay_ms(10);
                printf(CALL_NUM); //打电话     
                delay_ms(30);
                NRF_Tx_Dat(txbuf);	      
            }
            clearBuff();            
            LED_ON();     
            NRF_Tx_Dat(txbuf);	                  
        }      
        NRF_Tx_Dat(txbuf);	        
    }
}
























          
