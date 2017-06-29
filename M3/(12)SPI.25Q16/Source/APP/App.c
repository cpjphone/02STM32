/**
  ******************************************************************************
  * @file    APP.c
  * @author  phone
  * @version V1.0
  * @date    2014-11-18
  * @brief   SPIÖ®25Q16
  ******************************************************************************
  * @attention
  *
  * @{ÊµÑéÆ½Ì¨:Ò°»ğ iSO STM32 ¿ª·¢°å 
  * @{ÑÅµäÄÈ³ÌĞòÕûÀí	
	* @{ USE_STM3210B_EVAL
  ******************************************************************************
  */ 
	
/* Includes ------------------------------------------------------------------*/
#include <stm32f10x.h>
#include "BSP.h"
//#include "bsp_led.h"  
#include "bsp_usart1.h"
#include "bsp_spi_flash.h"

/*¶¨ÒåÒ»¸öÃ¶¾ÙÀàĞÍ£¬´ËÖĞ±äÁ¿×ö±æ±ğÕæÎ±*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;
/* »ñÈ¡»º³åÇøµÄ³¤¶È */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))  //aµÄ×Ü×Ö½ÚÊı³ıÒÔÃ¿¸ö±äÁ¿µÄ×Ö½ÚÊıµÈÓÚÓĞ¶àÉÙ¸ö±äÁ¿£¨³ÉÔ±£©
	//sizeof ²»ÊÇÒ»¸öº¯Êı£¬¶øÊÇÒ»¸ö¹Ø¼ü×Ö
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress
//#define  sFLASH_ID              0xEF3015     //W25X16
#define  sFLASH_ID              0xEF4015	 //W25Q16  (Á½ÕßÆäËûº¯ÊıÒ»Ñù£¬Ö»ÊÇÆ÷¼şµØÖ·²»Ò»Ñù)
 
/* ·¢ËÍ»º³åÇø³õÊ¼»¯ */
uint8_t Tx_Buffer[] = " ¸ĞĞ»ÄúÑ¡ÓÃÒ°»ğstm32¿ª·¢°å\r\n                http://firestm32.taobao.com";
uint8_t Rx_Buffer[BufferSize];

__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

void Delay(__IO u32 nCount); 
/*^_^ ¾­µäµÄº¯Êı±äÁ¿¶¨Òå*/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);


#if 1
/**---------------------------------------------------------------------------------
  * @brief       Ö÷º¯Êı.
  * @param    		ÎŞ
  *	@arg         NULL
  * @retval     null
  ---------------------------------------------------------------------------------*/
int main(void)
{
	/*Ê×ÏÈÊÇµ÷ÓÃÏµÍ³Ê±ÖÓ£¬ÔÚ3.0°æ±¾µÄ¿âÖĞ£¬
	ÔÚmainº¯ÊıÖĞÈÔĞèµ÷ÓÃ£¬ÔÚ3.5°æ±¾ÖĞ£¬
	ÏµÍ³Ê±ÖÓ·ÅÔÚÁËÆô¶¯ÎÄ¼şÕâÀï
	º¯ÊıÔËĞĞµÄÊ±ºòÊÇÏÈ´ÓÕâ¸ö»ã±àº¯ÊıÖ´ĞĞµÄ
	Æô¶¯ÎÄ¼ş 151ĞĞ    £º
	LDR     R0, =SystemInit			  ;ÉèÖÃÏµÍ³Ê±ÖÓ
	SetSysClock();	
	*/
	/*±ãÓÚÔÚÏßµ÷ÊÔ*/
	#ifdef DEBUG
		debug();
	#endif
	
	/*ÍâÉèÓ²¼ş³õÊ¼»¯*/
	ALL_Config();
	/* 2M´®ĞĞflash W25Q16³õÊ¼»¯ */
	SPI_FLASH_Init();
	
	/* Get SPI Flash Device ID */
	DeviceID = SPI_FLASH_ReadDeviceID();
	
	Delay( 200 );
	
	/* Get SPI Flash ID */
	FlashID = SPI_FLASH_ReadID();
	
	printf("\r\n FlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	/* Check the SPI Flash ID */
	if (FlashID == sFLASH_ID)  /* #define  sFLASH_ID  0xEF3015 */
	{	
		printf("\r\n ¼ì²âµ½»ª°î´®ĞĞflash W25X16 !\r\n");
		
		/* Erase SPI FLASH Sector to write on */
		SPI_FLASH_SectorErase(FLASH_SectorToErase);	 	 
		
		/* ½«·¢ËÍ»º³åÇøµÄÊı¾İĞ´µ½flashÖĞ */
		SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
		printf("\r\n Ğ´ÈëµÄÊı¾İÎª£º%s \r\t", Tx_Buffer);
		
		/* ½«¸Õ¸ÕĞ´ÈëµÄÊı¾İ¶Á³öÀ´·Åµ½½ÓÊÕ»º³åÇøÖĞ */
		SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
		printf("\r\n ¶Á³öµÄÊı¾İÎª£º%s \r\n", Tx_Buffer);
		
		/* ¼ì²éĞ´ÈëµÄÊı¾İÓë¶Á³öµÄÊı¾İÊÇ·ñÏàµÈ */
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		
		if( PASSED == TransferStatus1 )
		{    
			printf("\r\n 2M´®ĞĞflash(W25X16)²âÊÔ³É¹¦!\n\r");
		}
		else
		{        
			printf("\r\n 2M´®ĞĞflash(W25X16)²âÊÔÊ§°Ü!\n\r");
		}
	}// if (FlashID == sFLASH_ID)
	else
	{    
		printf("\r\n »ñÈ¡²»µ½ W25X16 ID!\n\r");
	}
	
	SPI_Flash_PowerDown(); 
	while(1)
	{
		
	}
}
#endif


/**---------------------------------------------------------------------------------
 * @brief Buffercmp
 * @param º±È½ÏÁ½¸ö»º³åÇøÖĞµÄÊı¾İÊÇ·ñÏàµÈ
 * @arg    -pBuffer1     src»º³åÇøÖ¸Õë
 *         -pBuffer2     dst»º³åÇøÖ¸Õë
 *         -BufferLength »º³åÇø³¤¶È
 * @retval  ÎŞ
 * @retval -PASSED pBuffer1 µÈÓÚ   pBuffer2
 *         -FAILED pBuffer1 ²»Í¬ÓÚ pBuffer2
 ---------------------------------------------------------------------------------*/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}

/**---------------------------------------------------------------------------------
  * @brief       CPUÑÓÊ±º¯Êı
  * @param    		ÎŞ
  ---------------------------------------------------------------------------------*/
void Delay(__IO uint32_t nCount)	 //¼òµ¥µÄÑÓÊ±º¯Êı
{
	for(; nCount != 0; nCount--);
}

/**---------------------------------------------------------------------------------
  * @brief       ¼ì²â´«µİ¸øº¯ÊıµÄ²ÎÊıÊÇ·ñÊÇÓĞĞ§µÄ²ÎÊı
  * @param    		ÎŞ
	* @example		 Ä³¸ö²ÎÊıµÄÈ¡Öµ·¶Î§Ö»ÄÜÊÇĞ¡ÓÚ3µÄÕıÕûÊı£¬Èç¹û¸ø³öµÄ²ÎÊı´óÓÚ3£¬
	* @example	   ÔòÕâ¸öassert_param()¿ÉÒÔÔÙÔËĞĞ³ÌĞòµ÷ÓÃµ½Õâ¸öº¯ÊıÊ±±¨´í			
  ---------------------------------------------------------------------------------*/
#ifdef DEBUG
void assert_failed(uint8_t* file, uint32_t line){
while (1);}
#endif
/*********************************************END OF FILE**********************/




