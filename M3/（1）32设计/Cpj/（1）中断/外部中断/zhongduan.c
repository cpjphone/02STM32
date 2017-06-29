/*********************************************************************************      
* 【语    言】：C语言
* 【编译环境】：Keil4  
* 【程序功能】：通过中断请求按键控制LED灯亮灭
* 【技术支持】：				
* 【晶    振】: RCC 8*9=72MHz		
* 【芯    片】: stm32f103c8 
* 【日    期】: 2013/6/03
* 【修改日期】: 2013/10/06
* 【作    者】: Phone
* 【编程理念】: 死东西活用，活东西套用。
*********************************************************************************/
	
#include "sys.h"							//定义头文件

#define uint unsigned int					//宏定义 变量类型
#define uchar unsigned char

/************************************************************************/
//延时函数
/************************************************************************/

//void delay(uint n)
//{
//		for (;n>0;n--);
//}

/************************************************************************/
//GPIO_Configuration	Configuration:配置；结构；外形
//(总体死东西，自个可灵活调动)
/************************************************************************/
void GPIO_Configuration(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB 
	 						| RCC_APB2Periph_GPIOA 
							| RCC_APB2Periph_AFIO , ENABLE);		//  调用时钟

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_5;	 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;	 	 	//  浮空            输入
	 GPIO_Init(GPIOA, &GPIO_InitStructure);	                 		//  PD10 


	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
 	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
	 
}

/************************************************************************/
//中断配置函数
//(总体死东西，自个可灵活调动)
/************************************************************************/
void EXTI_Configuration(void)
{
	 EXTI_InitTypeDef EXTI_InitStructure;
				
	
	// EXTI_ClearITPendingBit(EXTI_Line0);                  		// （必有）清除EXTI线路挂起位，果不把中断挂起位清除自后呢，一但有了中断就不能产生
	
	 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    		// 设置EXTI线路为中断请求
	 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;		    // 设置为线路下降沿位中断请求
	 EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line5;		//(☆☆☆) 那个L口作为中断输入	(☆☆☆)
	 EXTI_InitStructure.EXTI_LineCmd = ENABLE;						// 中断是不是使能
	 EXTI_Init(&EXTI_InitStructure);                    		    // 开启	 初始化中断
	 EXTI_GenerateSWInterrupt(EXTI_Line0);							// (☆☆☆)产生一个软件中断
	 EXTI_GenerateSWInterrupt(EXTI_Line5);							
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); 	// 问什么用这计句话进行选择，后面（1）语句中6个端口 现在进行选择
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);	//(☆☆☆)

}

/************************************************************************/
//（活东西）中断0服务函数
/************************************************************************/
void EXTI0_IRQHandler(void)  			
{ 


		     //添加中断处理程序
		if (Exti0 == 1)
		{
			//	GPIO_Write(GPIOB,0xfffc);
			//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
				wait(2.55);
				
				EXTI_ClearITPendingBit(EXTI_Line0);
				
		}
			 
			  
			//	GPIO_Write(GPIOB,0x00);
			//	delay(0x7fffff);
			//		GPIO_Write(GPIOB,0xff);
			//		delay(0x7fffff);
			//	GPIO_ResetBits(GPIOB,  GPIO_Pin_0 );
			///	EXTI_ClearITPendingBit(EXTI_Line10);
			
}

/************************************************************************/
//（活东西）中断9_5服务函数
/************************************************************************/
void EXTI9_5_IRQHandler(void)
{	
	if (Exti5 == 1)
	{
	//	GPIO_Write(GPIOB,0xfffe);
	//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		wait(1.55);
	
		EXTI_ClearITPendingBit(EXTI_Line5);	
	
	}
}

/************************************************************************/
//（活东西）主函数
/************************************************************************/
int main(void)
{
	void NVIC_Configuration(void) ;     //对函数进行声明
	void RCC_Configuration(void);

	delay_init(72);	     	    //调用，延时初始化	,必须要有		滴答时钟
	GPIO_Configuration();				//调用
	NVIC_Configuration();
	EXTI_Configuration();

	
    while (1)
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
			delay_ms(1000);
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
			delay_ms(1000);
		}
	//	GPIO_Write(GPIOB,0xffff);
		;
	//GPIO_Write(GPIOB,0xff);
}
