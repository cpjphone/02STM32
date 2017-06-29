
/************************
函数功能：LED头文件
输入参数：无
输出参数：无
备    注：无
************************/
#ifndef __led_H
#define __led_H 

#include "includes.h"

#define LED_GPIO_CLK   RCC_AHBPeriph_GPIOA 
#define LED_PORT   	   GPIOA
#define LED_PIN        GPIO_Pin_1


void LED_Init(void);
void LED_ON(void);
void LED_OFF(void);
void GSM_start(void);

#endif



