/**
  ******************************************************************************
  * @file    APP.c
  * @author  phone
  * @version V1.0
  * @date    2014-11-06
  * @brief   测试led
  ******************************************************************************
  * @attention
  *
  * @{实验平台:野火 iSO STM32 开发板 
  * @{雅典娜程序整理	
	* @{ USE_STM3210B_EVAL
	--cpu Cortex-M3 -g --apcs=interwork 
	-I C:\Keil\ARM\RV31\INC 
  ******************************************************************************
  */ 
	
/* Includes ------------------------------------------------------------------*/
#include <stm32f10x.h>
#include "BSP.h"
//#include "bsp_led.h"  
#include "bsp_usart1.h"
#include "bsP_i2c_ee.h"
#include <string.h>

#define  EEP_Firstpage      0x00
u8 I2c_Buf_Write[256];
u8 I2c_Buf_Read[256];
void I2C_Test(void);

void Delay(__IO u32 nCount); 

#if 1
/**---------------------------------------------------------------------------------
  * @brief       主函数.
  * @param    		无
  *	@arg         NULL
  * @retval     null
  ---------------------------------------------------------------------------------*/
int main(void)
{
	/*首先是调用系统时钟，在3.0版本的库中，
	在main函数中仍需调用，在3.5版本中，
	系统时钟放在了启动文件这里
	函数运行的时候是先从这个汇编函数执行的
	启动文件 151行    ：
	LDR     R0, =SystemInit			  ;设置系统时钟
	SetSysClock();	
	*/
	/*便于在线调试*/
	#ifdef DEBUG
		debug();
	#endif
	
	/*外设硬件初始化*/
	ALL_Config();

	printf("\r\n 这是一个I2C外设(AT24C02)读写测试例程 \r\n");
	//LED_GPIO_Config();

	/* I2C 外设初(AT24C02)始化 */
	I2C_EE_Init();

	printf("\r\n 这是一个I2C外设(AT24C02)读写测试例程 \r\n");	
	 	 
	I2C_Test();
  
  while (1)
  {      
  }
}
#endif


/**---------------------------------------------------------------------------------
  * @brief  I2C(AT24C02)读写测试
  * @param  无
  * @retval 无
  ---------------------------------------------------------------------------------*/
void I2C_Test(void)
{
	u16 i;

	printf("写入的数据\n\r");
    
	for ( i=0; i<=255; i++ ) //填充缓冲
  {   
    I2c_Buf_Write[i] = i;

    printf("0x%02X ", I2c_Buf_Write[i]);
    if(i%16 == 15)    
        printf("\n\r");    
   }

  //将I2c_Buf_Write中顺序递增的数据写入EERPOM中 
   //LED1(ON);
	I2C_EE_BufferWrite( I2c_Buf_Write, EEP_Firstpage, 256);
	//LED1(OFF);   
  
  printf("\n\r写成功\n\r");
   
   printf("\n\r读出的数据\n\r");
  //将EEPROM读出数据顺序保持到I2c_Buf_Read中
    //LED2(ON);   
	I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage, 256); 
   //LED2(OFF);
   
  //将I2c_Buf_Read中的数据通过串口打印
	for (i=0; i<256; i++)
	{	
		if(I2c_Buf_Read[i] != I2c_Buf_Write[i])
		{
			printf("0x%02X ", I2c_Buf_Read[i]);
			printf("错误:I2C EEPROM写入与读出的数据不一致\n\r");
			return;
		}
    printf("0x%02X ", I2c_Buf_Read[i]);
    if(i%16 == 15)    
        printf("\n\r");
    
	}
  printf("I2C(AT24C02)读写测试成功\n\r");
}

/**---------------------------------------------------------------------------------
  * @brief       CPU延时函数
  * @param    		无
  ---------------------------------------------------------------------------------*/
void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

/**---------------------------------------------------------------------------------
  * @brief       检测传递给函数的参数是否是有效的参数
  * @param    		无
	* @example		 某个参数的取值范围只能是小于3的正整数，如果给出的参数大于3，
	* @example	   则这个assert_param()可以再运行程序调用到这个函数时报错			
  ---------------------------------------------------------------------------------*/
#ifdef DEBUG
void assert_failed(uint8_t* file, uint32_t line){
while (1);}
#endif
/*********************************************END OF FILE**********************/




