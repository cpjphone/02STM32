
/**
  ******************************************************************************
  * @file     delay.h
  * @author   Phone
  * @version  V1.0
  * @date     2014/11/11
  * @brief    
  ******************************************************************************
  * @attention
  *
  * @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#ifndef __DELAY_H
#define __DELAY_H
#include <stm32f10x.h>

void delay_init(u8 SYSCLK);		//函数声明
void delay_ms(u16 nms);
void delay_us(u32 nus);
void delay_ss(int ss);
//void wait(float i);
#define Delay_ms delay_ms    //单位ms

 #endif

