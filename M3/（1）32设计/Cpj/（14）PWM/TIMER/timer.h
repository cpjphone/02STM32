#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//#include "touch.h" 
//#include "lcd.h"	
#include "led.h" 
#include "key.h"		
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//通用定时器 驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

//通过改变TIM3->CCR2的值来改变占空比，从而控制LED0的亮度
//#define LED0_PWM_VAL TIM3->CCR2 
#define flage_ccr1 TIM2->CCR1
#define flage_ccr2 TIM2->CCR2
#define flage_ccr3 TIM2->CCR3
#define flage_ccr4 TIM2->CCR4
							  
#define PWM0 PAout(6)// PA8
#define PWM1 PDout(7)// PD2	

void PWM_CH1_CH2(u16 arr,u16 psc);
void PWM_nj(u16 pwm_n,u16 pwm_j);
void PWM_CH3CH4(u16 arr,u16 psc);

#endif























