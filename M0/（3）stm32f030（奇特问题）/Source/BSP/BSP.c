
/************************
函数功能：总初始化
输入参数：无
输出参数：无
备    注：无
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
