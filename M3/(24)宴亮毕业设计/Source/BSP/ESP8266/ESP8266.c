/**
  ******************************************************************************
  * @file     gprs.c
  * @author   Phone
  * @version  V1.0
  * @date     2014/09/16
  * @brief    gprs��������
  ******************************************************************************
  * @attention
	*
	* �ŵ��ȳ����д
  * CPJ���ܿ���
  * @endverbatim
  ******************************************************************************
  * @attention
  * F0ϵ�е�Ƭ��SPI�˿ڱ���һ������ʼ��
  ******************************************************************************
  */
	
#include "usart.h"
#include <stm32f10x.h>

/*********************************************************************************
  * @brief  Э�����ӳɹ�����"AT+CIPSEND\x00D\x00A" ���淢������
  * @param  "AT+CIPSEND\x00D\x00A"																			@**��
  * @retval    ��
*********************************************************************************/
void send_gprs_data(char * buf , unsigned int count)
{
	unsigned int i ;
//	if (current_status != TCP_IP_OK)        // ���Э��û�����ӳɹ�ֱ�ӷ���
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
