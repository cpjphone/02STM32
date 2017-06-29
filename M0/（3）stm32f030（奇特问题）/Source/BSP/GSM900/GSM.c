/**
  ******************************************************************************
  * @file         bsp_xxx.c
  * @author   Phone
  * @version  V1.0
  * @date      2014/08/03
  * @brief      GSM900 拨打电话接通号码
  ******************************************************************************
  * @attention
  *
  *
  *
  *
  * CPJ智能控制
  * @endverbatim
  ******************************************************************************
  * @attention
  * 改换所要拨打的电话号码只需将18239932500改换
  ******************************************************************************
  **/
  
  
/* Includes ------------------------------------------------------------------*/
#include <stm32f0xx.h>
#include "includes.h"
#include <ctype.h>  
#include <string.h>

/* Private configuration  for  GSM --------------------------------------*/

/* @KEY  所拨打号码*/
#define CALL_NUM "ATD18239932500;\r\n"    

/* 用于表明发送字符短信TEXT模式 */
#define CMGF1 "AT+CMGF=1\r\n"

/*短信内容*/
unsigned char MESSA_CONTENT[] ="Your Car is in danger, PLease Ask for help.";
unsigned char MASSA_NUM[] = "at+cmgs=\"18239932500\"\r\n"   ;

/* 用于串口发送空间大小的运用 */
#define MAXCHAR 81
extern  char aa[81];

/*********************************************************************************
  * @brief       清除自定义缓存
  * @param    无
  *********************************************************************************/
void clearBuff(void)
{
    unsigned int    j ;
    for(j=0;j<MAXCHAR;j++)
    {
        aa[j]=0x00;
    }
    j=0;
}


/*********************************************************************************
  * @brief       向单一号码拨打电话
  * @param    无
  *	@arg         NULL
  * @retval     null
  *********************************************************************************/
void send_call(void)
{
    clearBuff();
    while(strstr(aa,"OK") ==NULL)
    {
      
        delay_ms(10);
        printf(CALL_NUM); //打电话     
        delay_ms(50);
    }
    //clearBuff();  
}

/*********************************************************************************
  * @brief       发送字符短信
  * @param    无
  *********************************************************************************/
void send_english(void)
{
    clearBuff();
	while((strstr(aa,"OK") ==NULL)&(strstr(aa,"C") ==NULL))
    {
        printf(CMGF1);
        delay_ms(60);
        clearBuff();
        printf("%s",MASSA_NUM);//短信号码
        
        delay_ms(80);
        
        printf("%s",MESSA_CONTENT);//打电话 //发短信内容
        
        delay_ms(200);
      
        USART_SendByte(0x1A);      //发送结束符号
        delay_ms(500);
        if((strstr(aa,"OK") !=NULL)|(strstr(aa,"C") !=NULL)) break;
     }
    clearBuff();
}

/*********************************************************************************
  * @brief       接到电话
  * @param    
  * @retval     1:收到电话；0：没有电话打来
  *********************************************************************************/
u8 RING_COM(void)
{
    if((strstr(aa,"RING")!=NULL))
    {    
        LED_ON();
        return 1;
    }
    else
    {
       return 0;
    }
}

/*********************************************END OF FILE**********************/

