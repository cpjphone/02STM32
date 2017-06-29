#ifndef _USART_H
#define _USART_H

#include "includes.h"
#include <stdio.h>

void Usart_IOConfig(void);
void Usart_Init(u32 baud);
int fputc(int ch, FILE *f);
void USART_SendByte(u8 byte);



#endif

