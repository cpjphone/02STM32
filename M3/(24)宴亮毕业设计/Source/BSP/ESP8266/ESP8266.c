/**
  ******************************************************************************
  * @file     gprs.c
  * @author   Phone
  * @version  V1.0
  * @date     2014/09/16
  * @brief    gprs驱动函数
  ******************************************************************************
  * @attention
	*
	* 雅典娜程序编写
  * CPJ智能控制
  * @endverbatim
  ******************************************************************************
  * @attention
  * F0系列单片机SPI端口必须一个个初始化
  ******************************************************************************
  */
	
#include "usart.h"
#include <stm32f10x.h>

/*********************************************************************************
  * @brief  协议连接成功后发送"AT+CIPSEND\x00D\x00A" 下面发送内容
  * @param  "AT+CIPSEND\x00D\x00A"																			@**☆
  * @retval    无
*********************************************************************************/
void send_gprs_data(char * buf , unsigned int count)
{
	unsigned int i ;
//	if (current_status != TCP_IP_OK)        // 如果协议没有连接成功直接返回
//	return ;

	send_data_uart1(0x1A);  //LF
	send_data_uart1(0x0D);  //LF
	send_data_uart1(0x0A);  //LF
	
	#if 1
		_GPRS_WRITE_STRING(GPRS_SEND_DATA);
		//delay_GSM(1000);
		OSTimeDlyHMSM(0,0,1,800);
		for( i = 0 ;i < count ; i ++)
		{
			for ( i = 0 ; i < strlen((const char*)buf) ; i ++)
			{
				//            if(buf[i] != 0x0d  && buf[i] != 0x0a)							 
                send_data_uart1(buf[i]);       		
			}
		}
		send_data_uart1(0x1A);  //LF
		send_data_uart1(0x0D);  //LF
		send_data_uart1(0x0A);  //LF
	#endif
	//delay_GSM(3000);
}
