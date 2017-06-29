
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
#ifndef __SYSTICK_DELAY_H
#define __SYSTICK_DELAY_H
#include <stm32f10x.h>

#define SysTickBank 1
//#define SysTickREG  1

	#ifdef SysTickREG
	void delay_init(u8 SYSCLK);		//函数声明
	void delay_ms(u16 nms);
	void delay_us(u32 nus);
	void delay_ss(int ss);

	#endif

	#ifdef SysTickBank
	void SysTick_Init(void);
	void Delay_us(__IO u32 nTime);
	#define Delay_ms(x) Delay_us(100*x)	 //单位ms
	#endif


 #endif

