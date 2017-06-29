
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
#define LED_PIN        GPIO_Pin_4

#define NEA_PIN        GPIO_Pin_7
#define IN1_PIN        GPIO_Pin_6
#define IN2_PIN        GPIO_Pin_5

#define ENB_PIN        GPIO_Pin_3
#define IN3_PIN        GPIO_Pin_2
#define IN4_PIN        GPIO_Pin_1



void LED_Init(void);
void LED_ON(void);
void LED_OFF(void);
void GSM_start(void);

#endif



