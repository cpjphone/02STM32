#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>
#define BUFLEN 128

void USART1_Config(void);
void NVIC_Configuration(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
extern void USART1_receive_process_event(unsigned char ch );
extern void clear_buf_uart1(void);
extern void send_data_uart1(unsigned char Data);
extern void send_string_uart1(char * S);



/*********************************************************************************
  * @brief   定义串口输出缓存
*********************************************************************************/
typedef struct _UART_BUF
{
    char buf [BUFLEN+1];                // 多申请一个字符保证最后一个字符永远是'0'
    unsigned int index ;
}UART_BUF;



#endif /* __USART1_H */
