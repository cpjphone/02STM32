#ifndef __SYS_H_
#define __SYS_H_

#include "stm32f10x.h"
//#include "delay.h"
#include  "stdint.h"
#include <stdio.h>

#define uint  unsigned int	 //宏定义
#define uchar  unsigned char

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 


/********GPI0口控制***************/
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C 

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入


/********中断线口控制***************/													  
#define Exti0   EXTI_GetITStatus(EXTI_Line0)
#define Exti1 	EXTI_GetITStatus(EXTI_Line1)
#define Exti2 	EXTI_GetITStatus(EXTI_Line2)
#define Exti3 	EXTI_GetITStatus(EXTI_Line3)
#define Exti4 	EXTI_GetITStatus(EXTI_Line4)
#define Exti5 	EXTI_GetITStatus(EXTI_Line5)
#define Exti6 	EXTI_GetITStatus(EXTI_Line6)
#define Exti7 	EXTI_GetITStatus(EXTI_Line7)
#define Exti8 	EXTI_GetITStatus(EXTI_Line8)
#define Exti9 	EXTI_GetITStatus(EXTI_Line9)
#define Exti10  EXTI_GetITStatus(EXTI_Line10)
#define Exti11  EXTI_GetITStatus(EXTI_Line11)
#define Exti12  EXTI_GetITStatus(EXTI_Line12)
#define Exti13  EXTI_GetITStatus(EXTI_Line13)
#define Exti14  EXTI_GetITStatus(EXTI_Line14)
#define Exti15  EXTI_GetITStatus(EXTI_Line15)

#endif


