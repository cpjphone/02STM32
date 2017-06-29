/**
  ******************************************************************************
  * @file    APP.c
  * @author  phone
  * @version V1.0
  * @date    2015-03-09
  * @brief   测试led
  ******************************************************************************
  * @attention
  *
  * @{实验平台:
  * @{雅典娜程序整理	
	* @{ USE_STM3210B_EVAL
	--cpu Cortex-M3 -g --apcs=interwork 
	-I C:\Keil\ARM\RV31\INC 
  ******************************************************************************
  */ 
	
/* Includes ------------------------------------------------------------------*/
#include <stm32f10x.h>
#include <stdio.h>
#include <string.h>
#include "BSP.h"
#include "SysTick_delay.h"
//#include "bsp_led.h"  
#include "bsp_usart1.h"
#include "dht11.h"
#include "bsp_led.h" 
unsigned char FSSJ[11]={0x41,0x54,0x2B,0x43,0x49,0x50,0x53,0x45,0x4E,0x44,0x3D};//AT+CIPSEND= 发送数据

/* 设置AP模式 */
#define ATCWMODE "AT+CWMODE=2\r\n"    

/* 设置WIFI名字，密码 */
#define ATCWSAP  "AT+CWSAP=\"2015YL\",\"0123456789\",11,0\r\n"

/* 复位 */
#define ATRST "AT+RST\r\n" 

/* 启动多连接 */
#define ATCIPMUX "AT+CIPMUX=1\r\n"   

/* 热点端口号 */
#define ATCIPSERVER "AT+CIPSERVER=1,5000\r\n"

#define ATCIPSEND "AT+CIPSEND="


extern UART_BUF buf_uart1;  

void Delay(__IO u32 nCount); 


#if 0
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
	char temp;
	int i;
	/*外设硬件初始化*/
	ALL_Config();

	
	LED1_OFF;
	tdata[0] = 0;
	tdata[1] = 0;
	tdata[2] = 0;
	tdata[3] = 0;
	while(1)
	{
		//DHT11_Get_Data();
		ReadDHT11();
		PCout(13) = 1;
		delay_ms(500);
	PCout(13) = 0;
		delay_ms(500);
	
			printf("#");
			printf("RH");
			printf("%d",(char)tdata[0]);
			printf(".");
			printf("%d",(char)tdata[1]);

			printf("C:");
			printf("%d",(char)tdata[2]);
			printf(".");
			printf("%d\n",(char)tdata[3]);
			printf("@");
			delay_ms(5);

		
	}
}

#endif


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
	char temp;
	int i;
	/*外设硬件初始化*/
	ALL_Config();
	clear_buf_uart1();
	/* 设置AP模式 */
	printf(ATCWMODE);    
  delay_ms(300);
	/* 设置WIFI名字，密码 */
	printf(ATCWSAP);     
  delay_ms(300);
	
	/* 复位 */
//	printf(ATRST);     
//  delay_ms(300);
	
	/* 启动多连接 */
	printf(ATCIPMUX);     
  delay_ms(300);
	/* 热点端口号 */
	printf(ATCIPSERVER);     
  delay_ms(300);
	
	LED1_OFF;
	tdata[0] = 1;
	tdata[1] = 2;
	tdata[2] = 3;
	tdata[3] = 4;
	while(1)
	{
		ReadDHT11();
		PCout(13) = 1;
		delay_ms(500);
	PCout(13) = 0;
		delay_ms(500);
		//DHT11_Get_Data();
		//send_data_uart1(0x0A);
		#if 1
		if(strstr(buf_uart1.buf,"aa")!=NULL)
		{
			for(i=0;i<11;i++)//AT+CIPSEND= 发送数据
			{
				send_data_uart1(FSSJ[i]);	delay_ms(5);
			}
			
			send_data_uart1(0x30);
			delay_ms(5);
			send_data_uart1(0x2C);
			delay_ms(5);
			send_data_uart1(0x32);
			delay_ms(5);
			send_data_uart1(0x30);
			delay_ms(5);
			send_data_uart1(0x0D);
			delay_ms(5);
			send_data_uart1(0x0A);
			delay_ms(105);
			
			printf("#");
			printf("RH");
			printf("%d",(char)tdata[0]);
			printf(".");
			printf("%d",(char)tdata[1]);

			printf("C:");
			printf("%d",(char)tdata[2]);
			printf(".");
			printf("%d\n",(char)tdata[3]);
	
			printf("Lig:");
			if(PAin(11) == 1)
			{
				printf("NO");
			}
			else
				printf("OK");
			printf("$");
			clear_buf_uart1();
		}
	#endif
		
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




