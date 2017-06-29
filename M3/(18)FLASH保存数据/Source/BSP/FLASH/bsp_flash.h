
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
#ifndef __BSP_FLASH_H
#define	__BSP_FLASH_H
#include "stm32f10x.h"

typedef enum {
	FLASH_WRITE_OK = 1,
	FLASH_WRIKE_NO = 0,
	FLASH_READ_OK = 1,
	FLASH_READ_NO = 0
}FLASH_FLAG ;

extern unsigned char FLASH_WRITE(unsigned long address,unsigned short int * memory_data);
extern unsigned char FLASH_READ(unsigned long address,unsigned short int * memory_data,unsigned short int n);


#endif /* __LED_H */

/*********************************************END OF FILE**********************/

