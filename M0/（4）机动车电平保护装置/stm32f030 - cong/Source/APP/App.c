 
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

#include <stm32f0xx.h>
#include "includes.h"
#include <ctype.h>

//所发送的信号
#define    foodsign1   6
#define    foodsign2   8
/*
 * PA2  -  ce使能
 * PA3  -  cs片选
 * PA0  -  irq中断
 */
u8 status;		 //用于判断接收/发送状态
u8 flag;
//u8 txbuf[4];	    //发送缓冲
u8 rxbuf[8];	 //接收缓冲



void dump_buffer(unsigned char *buf, int buflen);
/************************************************************************/
//主函数
/************************************************************************/
int main(void)
{
    u8 i,j,k ;  
    ALL_Config();
    status = NRF_Check(); 
    NRF_RX_Mode();
    LED_OFF();
    while(1)
    {  		 	
        /*等待接收数据*/
        j = 0;
        k = 0;
        status = NRF_Rx_Dat(rxbuf);
        for(i=0;i<8;i++)
        {
            if(rxbuf[i] == foodsign1)
                j++;
            if(rxbuf[i] == foodsign2)
                k++;
            if(j&k)
            {
                flag = 2;
                break;
            }            
        }
        if(flag == 2)
        {
           LED_ON();
        }
    }
}





















          
