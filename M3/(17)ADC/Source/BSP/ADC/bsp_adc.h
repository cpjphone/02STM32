#ifndef __bsp_adc_H
#define	__bsp_adc_H


#include "stm32f10x.h"

void ADC1_Init(void);


#define ADC1_DR_Address    ((u32)0x40012400+0x4c) 	//ģ����ת������������ŵ��õ�ַ��
extern __IO uint16_t ADC_ConvertedValue;

#endif /* __ADC_H */

