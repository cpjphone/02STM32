
/**
  ******************************************************************************
  * @file     bsp.h
  * @author   Phone
  * @version  V1.0
  * @date     2014/11/06
  * @brief    
  ******************************************************************************
  * @attention
  * 无话可说，只有经典
  * @endverbatim
  ******************************************************************************
  */
#ifndef __LCD_1602_H
#define	__LCD_1602_H

#include "stm32f10x.h"

/********************************硬件接口定义*********************************/
#define RS  GPIO_Pin_1 //P2^6;             
#define RW  GPIO_Pin_2 //P2^5;
#define EN  GPIO_Pin_0 //P2^7;
#define CONPORT  GPIOB //P2^7;

typedef enum {
   WRITE_OK = 1,
   WRITE_NO = 0
}LCD1602WTITE_FLAG ;



extern void LCD_1602_CONFIG(void);
unsigned char LCD1602_WRITE(unsigned char y,unsigned char x,unsigned char *write_data);
#endif /* __LED_H */
