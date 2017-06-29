#ifndef _USART_H
#define _USART_H

#include "includes.h"
#include <stdio.h>

//前面的int fputc(int ch, FILE *f) 和下面的代码功能一样，可任选其一
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

void Usart_IOConfig(void);
void Usart_Init(void);
int fputc(int ch, FILE *f);
void USART_SendByte(u8 byte);
void USART1_IRQHandler(void);
void Print_Str(unsigned char *str);//发送字符串


#endif

