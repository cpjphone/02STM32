/**
  ******************************************************************************
  * @file     delay.c
  * @author   Phone
  * @version  V1.0
  * @date      2014/11/06
  * @brief    系统滴答定时器
  ******************************************************************************
  * @attention
  * 雅典娜程序
  * CPJ智能控制
  * @endverbatim
  ******************************************************************************
  * @attention
  * 无
  ******************************************************************************
  */
	
#include "SysTick_delay.h"
#include "stdint.h"

static u8  fac_us=0;//us延时倍乘数	  fac_us = 9
static u16 fac_ms=0;//ms延时倍乘数
/**---------------------------------------------------------------------------------
  * @brief       初始化延迟函数
  * @param    	 SYSTICK的时钟固定为HCLK时钟的1/8
	* @@retval		 SYSCLK:系统时钟	
  ---------------------------------------------------------------------------------*/
void delay_init(u8 SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;//bit2清空,选择外部时钟  HCLK/8
	fac_us=SYSCLK/8;		    	 //8分频 9M装9 得1us  fac_us==9	mHZ
	fac_ms=(u16)fac_us*1000;		  //*1000 的毫秒   fac_us为8位整形数值	fac_ms  16位  fac_ms==9000	mHZ
}	

/**---------------------------------------------------------------------------------
  * @brief       延时nms
  * @param    		注意nms的范围
  *	@arg         NULL
  * @retval     null
  * @attention  
	* 注意nms的范围
	* SysTick->LOAD为24位寄存器,所以,最大延时为:
  * nms<=0xffffff*8*1000/SYSCLK
	* SYSCLK单位为Hz,nms单位为ms
	* 对72M条件下,nms<=1864 
  ---------------------------------------------------------------------------------*/
	void delay_ms(u16 nms)
	{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)	  先把数  放到备用寄存器中	   小弟
	SysTick->VAL =0x00;           //清空计数器											老大哥
	SysTick->CTRL=0x01 ;          //开始倒数  
	do										  //自动从装，倒减
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   不断的查询 CTRL	第十六位 若VAL 到零，由硬件将CTRL第十六位置1，置1 说明时间到了
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  24位计数器  1864MS	    
	} 

	/**---------------------------------------------------------------------------------
  * @brief       延时nus
  * @param    		无
  ---------------------------------------------------------------------------------*/  	    								   
	void delay_us(u32 nus)
	{		
		u32 temp;	    	 
		SysTick->LOAD=nus*fac_us; //时间加载  LOAD 备用寄存器	  		 
		SysTick->VAL=0x00;        //清空计数器	 清零后 自动从装   到  VAL 中
		SysTick->CTRL=0x01 ;      //开始倒数 	 
		do
		{
			temp=SysTick->CTRL;
		}
		while(temp&0x00&&!(temp&(1<<16)));//等待时间到达   
		SysTick->CTRL=0x00;       //关闭计数器
		SysTick->VAL =0X00;       //清空计数器	 
	}
								  //最大值(2^24)/9

	/**---------------------------------------------------------------------------------
  * @brief       delay_ss
  * @param    		无
  ---------------------------------------------------------------------------------*/
	void delay_ss(int ss)
	{
		int i = 0;
		while( i< ss)
		{
			i++;
			delay_ms(1000);		
		}	
	}

//	void wait(float i)
//	{
//		uint32_t temp ,x,y;
//		temp = i * 1000000;
//		x = temp / 1000000;			
//		y = temp % 1000000;		
//		delay_ss(x);
//		delay_us(y);			
//	}
