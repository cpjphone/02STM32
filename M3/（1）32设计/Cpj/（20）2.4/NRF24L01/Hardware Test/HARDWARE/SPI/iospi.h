#ifndef _IOSPI_H_
#define _IOSPI_H_

#include "sys.h"

#define SET_SPI_SCL_PIN(val) PAout(5)=val //����SPI��ʱ��
#define SET_SPI_SI_PIN(val)  PAout(7)=val //����SPI����������
#define GET_SPI_SO_PIN()     PAin(6)      //��ȡSPI����ӳ�

#define DELAY_TIME 100 //SPI����ʱ��������ֵԽ��SPI�ٶ�Խ��

void IOSPI_GPIO_Init(void);			 //��ʼ��SPI�� 
u8 IOSPI_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
u8 IOSPI_ReadWriteByte_R(u8 x);
		 
#endif
