
/************************
�������ܣ��ܳ�ʼ��
�����������
�����������
��    ע����
************************/
#include "includes.h"

void ALL_Config(void)
{
    Delay_Init(48);
    LED_Init();
    Usart_IOConfig();
    Usart_Init();
    SPI_NRF_Init();
}
