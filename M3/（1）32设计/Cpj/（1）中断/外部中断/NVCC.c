#include "stm32f10x.h"

// 中断源进行分组，确定优先级是什么样子的，通过下面的函数进行实现

void NVIC_Configuration(void)
{
		//  NVIC_InitStructure 结构体中四个变量，
		//  NVIC_InitTypeDef NVIC_InitStructure; 
	  	//  /* Configure the Priority Grouping with 1 bit */ 
	    //  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
	    //  /* Enable TIM3 global interrupt with Preemption Priority 0 and Sub 
	    //  Priority as 2 */ 
	    //  1 NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel; 
	    //  2 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		//  3 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	    //  4 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	    //  NVIC_InitStructure(&NVIC_InitStructure); 
				
	  NVIC_InitTypeDef NVIC_InitStructure; 

	  /******************中断源（0）抢占优先级 3 响应优先级 0 **********************/
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);            //（☆）4位抢占优先级，无响应优先级   	
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;           //（☆）指定中断源，  外部中断线15-10中断  （1）设置外部中断源
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //（☆）指定响应优先级别1   优先级别
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //（☆）强占优先级
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	         //使能外部中断通道  使能分组
	  NVIC_Init(&NVIC_InitStructure);

	  /******************中断源（9_5_）抢占优先级 1 响应优先级 0 **********************/
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);             //（☆）抢占式优先级别设置为无抢占优先级   	
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;          //（☆）指定中断源，  外部中断线15-10中断  （1）设置外部中断源
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //（☆）指定响应优先级别1   优先级别
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =  1;  //（☆）强占优先级
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	          //使能外部中断通道  使能分组
	  NVIC_Init(&NVIC_InitStructure);
	 					 
}	//组4 有0-15 个优先级
