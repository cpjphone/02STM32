#include "board_init.h"
#include "sys.h"
#include "delay.h"

void ZNX_Board_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void ZNX_Board_Init()  
{
 int i=0;
	
 ZNX_Board_GPIO_Init();
	
 PCout(0)=PCout(1)=PCout(2)=PCout(3)=1;

 //==================================================================
 PAout(7)=PAout(4)=0; //SMG-DIN,DZ-DINH,DZ-DINL=0
 for(i=0;i<16;i++) //向数码管、点阵的HC595中写入全0，以防它们乱显
 {
  PAout(5)=0; //SCK=0
	delay_us(10);
  PAout(5)=1; //SCK=1
	delay_us(10);
 }
 
 PBout(11)=0; //SMG-LC=0
 PBout(11)=1; //SMG-LC=1
 PBout(10)=0; //DZ-LC=0
 PBout(10)=1; //DZ-LC=1 
 
 //=============以上代码为ZN-X板上数码管、点阵显示清0===================
}