
/**
  ******************************************************************************
  * @file     BSP.c
  * @author   Phone
  * @version  V1.0
  * @date      2014/12/14
  * @brief   LCD1602配置驱动函数
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
#include "LCD_1602.h"

/********************************宏定义*********************************/
#define    LCD_RS(x)  x ? GPIO_SetBits(CONPORT, RS): GPIO_ResetBits(GPIOB, RS)
#define    LCD_RW(x)  x ? GPIO_SetBits(CONPORT, RW): GPIO_ResetBits(GPIOB, RW)
#define    LCD_EN(x)  x ? GPIO_SetBits(CONPORT, EN): GPIO_ResetBits(GPIOB, EN)

/******************************** 变量定义 -------------------------------------*/
static GPIO_InitTypeDef GPIO_InitStructure;     //GPIO

LCD1602WTITE_FLAG LCE1602_STATUS;
	
typedef unsigned char BYTE;
typedef unsigned char BOOL; 

/**---------------------------------------------------------------------------------
* @brief     用于时序控制延时函数
---------------------------------------------------------------------------------*/
static void NOP()
{ 
	vu16 i;
  for(i=0; i<100; i++); 
}

/**---------------------------------------------------------------------------------
* @brief  延时函数
---------------------------------------------------------------------------------*/
static void delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}

/**---------------------------------------------------------------------------------
* @brief     测忙
* @retval     0或1
---------------------------------------------------------------------------------*/
static BOOL lcd_bz()
{                         // 测试LCD忙碌状态
	BOOL result;
	LCD_RS(0);  //0;
	LCD_RW(1);  //1;
	LCD_EN(1);  //1;
	NOP();
	NOP();
	result = (GPIO_ReadInputData(GPIOB) & 0x8000)>>8;
	LCD_EN(0); // 0;
	return result; 
}	
/**---------------------------------------------------------------------------------
* @brief     写命令
* @param    		无
*	@arg         NULL
* @retval     null
---------------------------------------------------------------------------------*/
static void lcd_wcmd(int cmd)
{                          // 写入指令数据到LCD
	while(lcd_bz());
	LCD_RS(0); // 0;
	LCD_RW(0); // 0;
	LCD_EN(0); // 0;
	NOP();
	NOP(); 
	GPIOB->BSRR = cmd<<8 & 0xff00;  //将数据送到P0口 
	GPIOB->BRR = ((~cmd)<<8) & 0xff00;
	NOP();
	NOP();
	LCD_EN(1); // 1;
	NOP();
	NOP();
	LCD_EN(0); // 0;  
}
/**---------------------------------------------------------------------------------
* @brief      设置位置
* @param    		无
*	@arg         NULL
* @retval     null
---------------------------------------------------------------------------------*/
static void 	lcd_pos(BYTE pos)
{                          //设定显示位置
	lcd_wcmd(pos | 0x80);
}
/**---------------------------------------------------------------------------------
* @brief      写数据（显示部分）
* @param    		无
*	@arg         NULL
* @retval     null
---------------------------------------------------------------------------------*/
static void lcd_wdat(int dat) 
{                          //写入字符显示数据到LCD
	while(lcd_bz());
	LCD_RS(1); //= 1;
	LCD_RW(0); //= 0;
	LCD_EN(0); //= 0;

	GPIOB->BSRR = dat<<8 & 0xff00;    ////P0 = dat
	GPIOB->BRR = ((~dat)<<8) & 0xff00;
	NOP();
	NOP();
	LCD_EN(1);// = 1;
	NOP();
	NOP();
	LCD_EN(0);// = 0; 
}
/**---------------------------------------------------------------------------------
* @brief      LCD屏幕初始化
* @param    		无
*	@arg         NULL
* @retval     null
---------------------------------------------------------------------------------*/
static void lcd_init()
{                        //LCD初始化设定
	lcd_wcmd(0x38);          //16*2显示，5*7点阵，8位数据
	delay(20000);
	lcd_wcmd(0x0c);          //显示开，关光标
	delay(20000);
	lcd_wcmd(0x06);          //移动光标
	delay(20000);
	lcd_wcmd(0x01);          //清除LCD的显示内容
	delay(20000);
}
/**---------------------------------------------------------------------------------
* @brief       对应管脚初始化
* @param    		无
*	@arg         NULL
* @retval     null
---------------------------------------------------------------------------------*/
void LCD_1602_CONFIG(void)
{
	//启动GPIO模块时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB 
						 | RCC_APB2Periph_AFIO, ENABLE);
	//把调试设置普通IO口
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; //所有GPIO为同一类型端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //输出的最大频率为50HZ
//	GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA端口
	GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化GPIOB端口

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | 
	GPIO_Pin_10 | GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; //所有GPIO为同一类型端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //输出的最大频率为50HZ
	GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化GPIOB端口
	
	GPIO_Write(GPIOA,0xffff);  //将GPIOA 16个端口全部置为高电平
	GPIO_Write(GPIOB,0xffff);  //将GPIOB 16个端口全部置为高电

	lcd_init();               // 初始化LCD
	delay(10);
}

/**---------------------------------------------------------------------------------
* @brief      对应位置显示函数
* @param    	y：1，2（从第几行开始写）
							x：0-15（从改行的第几位开始写）
*	@arg         NULL
* @retval     null
---------------------------------------------------------------------------------*/
unsigned char LCD1602_WRITE(unsigned char y,unsigned char x,unsigned char *write_data)
{
	BYTE i;
	lcd_wcmd(0x06);            //向右移动光标
	if(y == 1)
	{	
		lcd_pos(x);                //设置显示位置为第一行的第1个字符
		i = 0;
		while(*(write_data + i) != '\0')
		{                           //显示字符"WLCOME  TO"
			lcd_wdat(*(write_data + i));
			i++;
			delay(200);                //控制两字之间显示速度
			if(i > 15)
			{
				LCE1602_STATUS = WRITE_NO;
				return LCE1602_STATUS;		
			}	 
		}
		LCE1602_STATUS = WRITE_OK;
	  return LCE1602_STATUS;	
	}
	else
	{
		lcd_pos(0x40 + x);                //设置显示位置为第一行的第1个字符
		i = 0;
		while(*(write_data + i) != '\0')
		{                           //显示字符"WLCOME  TO"
			lcd_wdat(*(write_data + i));
			i++;
			delay(200);                //控制两字之间显示速度
			if(i > 15)
			{
				LCE1602_STATUS = WRITE_NO;
				return LCE1602_STATUS;		
			}	 
		}
		LCE1602_STATUS = WRITE_OK;
	  return LCE1602_STATUS;	
	}
//	delay(80000);                //控制停留时间
//	lcd_wcmd(0x01);            //清除LCD的显示内容
//	lcd_wcmd(0x04);            //向左移动光标
//	lcd_pos(15);               //设置显示位置为第一行的第16个字符
//	lcd_pos(0x4F);             //设置显示位置为第二行的第16个字符
	
}
	
/*********************************************END OF FILE**********************/

