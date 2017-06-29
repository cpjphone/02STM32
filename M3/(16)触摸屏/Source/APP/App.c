/**
  ******************************************************************************
  * @file    APP.c
  * @author  phone
  * @version V1.0
  * @date    2014-11-06
  * @brief   ����led
  ******************************************************************************
  * @attention
  *
  * @{ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * @{�ŵ��ȳ�������	
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
  * @brief       ������.
  * @param    		��
  *	@arg         NULL
  * @retval     null
  ---------------------------------------------------------------------------------*/
int main(void)
{
	/*�����ǵ���ϵͳʱ�ӣ���3.0�汾�Ŀ��У�
	��main������������ã���3.5�汾�У�
	ϵͳʱ�ӷ����������ļ�����
	�������е�ʱ�����ȴ������ຯ��ִ�е�
	�����ļ� 151��    ��
	LDR     R0, =SystemInit			  ;����ϵͳʱ��
	SetSysClock();	
	*/
	/*�������ߵ���*/
	#ifdef DEBUG
		debug();
	#endif
	
	uint8_t k;
	/*����Ӳ����ʼ��*/
	ALL_Config();
	
	LCD_Init();	
	/* GRAMɨ�跽��Ϊ���½�->���Ͻ� */
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
	
	 /* ������IO���жϳ�ʼ�� */
  Touch_Init();
	/*-------------------------------------------------------------------------------------------------------*/  
  #if 1
  SPI_FLASH_BufferRead(&cal_flag, 0, 1);
  if( cal_flag == 0x55 )	//���½��� ���޸ĳ�0x11�����Ϊ11�Ļ�����Ҫ����
  {
    SPI_FLASH_BufferRead((void*)cal_p, 1, sizeof(cal_p));
    SPI_FLASH_CS_HIGH();
    for( k=0; k<6; k++ )
          printf("\r\n rx = %LF \r\n",cal_p[k]);
  }
  else
  {
    /* �ȴ�������У����� */
    while(Touch_Calibrate() !=0);
  }
  #elif 0
  /* �ȴ�������У����� */
  while(Touch_Calibrate() !=0);
  #endif
/*------------------------------------------------------------------------------------------------------*/  
  //Touch_Init();
  /* ����ȡɫ���ʼ�� */
  Palette_Init();
  
	while( 1 )
  {
    if(touch_flag == 1)			/*������ʰ�����*/
    {
      /*��ȡ�������*/
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
  * @brief       CPU��ʱ����
  * @param    		��
  ---------------------------------------------------------------------------------*/
void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

/**---------------------------------------------------------------------------------
  * @brief       ��⴫�ݸ������Ĳ����Ƿ�����Ч�Ĳ���
  * @param    		��
	* @example		 ĳ��������ȡֵ��Χֻ����С��3������������������Ĳ�������3��
	* @example	   �����assert_param()���������г�����õ��������ʱ����			
  ---------------------------------------------------------------------------------*/
#ifdef DEBUG
void assert_failed(uint8_t* file, uint32_t line){
while (1);}
#endif
/*********************************************END OF FILE**********************/




