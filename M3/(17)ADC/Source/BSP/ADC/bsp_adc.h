#ifndef __bsp_adc_H
#define	__bsp_adc_H


#include "stm32f10x.h"

void ADC1_Init(void);


#define ADC1_DR_Address    ((u32)0x40012400+0x4c) 	//模拟量转换成数字量存放到该地址处
extern __IO uint16_t ADC_ConvertedValue;

#endif /* __ADC_H */

