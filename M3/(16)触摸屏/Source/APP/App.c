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
#include "bsp_ili9341_lcd.h"
#include "bsp_touch.h"
#include "bsp_spi_flash.h"
#include "bsp_led.h" 

uint32_t n;


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
	
	uint8_t k;
	/*外设硬件初始化*/
	ALL_Config();
	
	LCD_Init();	
	/* GRAM扫描方向为左下脚->右上角 */
  Lcd_GramScan(2);
	LCD_Clear(0, 0, 320, 240, BACKGROUND);
	SPI_FLASH_Init();
	
#if 0
	LCD_Clear(0, 0, 240, 320, BACKGROUND);	
  
	LCD_DispChar(60, 60, 'A', RED);

  
	LCD_DispStr(10, 10, (uint8_t *)"This is a lcd demo to display ascii", RED);	
	
	LCD_DispStr(40, 100, (uint8_t *)"count:", RED);
	
	for( n=0; n<500000; n++ )
	{
		LCD_DisNum(100, 100, n, RED);	
		Lcd_Delay(0xAFFf>>4);
	}
#endif
	
	 /* 触摸屏IO和中断初始化 */
  Touch_Init();
	/*-------------------------------------------------------------------------------------------------------*/  
  #if 1
  SPI_FLASH_BufferRead(&cal_flag, 0, 1);
  if( cal_flag == 0x55 )	//重新矫正 可修改成0x11，如果为11的话则需要矫正
  {
    SPI_FLASH_BufferRead((void*)cal_p, 1, sizeof(cal_p));
    SPI_FLASH_CS_HIGH();
    for( k=0; k<6; k++ )
          printf("\r\n rx = %LF \r\n",cal_p[k]);
  }
  else
  {
    /* 等待触摸屏校正完毕 */
    while(Touch_Calibrate() !=0);
  }
  #elif 0
  /* 等待触摸屏校正完毕 */
  while(Touch_Calibrate() !=0);
  #endif
/*------------------------------------------------------------------------------------------------------*/  
  //Touch_Init();
  /* 触摸取色板初始化 */
  Palette_Init();
  
	while( 1 )
  {
    if(touch_flag == 1)			/*如果触笔按下了*/
    {
      /*获取点的坐标*/
      if(Get_touch_point(&display, Read_2046_2(), &touch_para ) !=DISABLE)      
      {					
        Palette_draw_point(display.x,display.y);
        LED1_TOGGLE;       
      }
    }		
  }
}
#endif

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




