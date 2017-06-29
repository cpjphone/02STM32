
/**
  ******************************************************************************
  * @file     bsp.h
  * @author   Phone
  * @version  V1.0
  * @date     2014/09/16
  * @brief    
  ******************************************************************************
  * @attention
  *
  * @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#ifndef __SPEED_H
#define __SPEED_H

extern void Init_MMA8452(void);
extern void speed_open(void);
extern void i2c_CfgGpio(void);
extern char BUF2[21]; 

#endif/* __SPEED_H */



