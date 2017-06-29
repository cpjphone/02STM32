
/**
  ******************************************************************************
  * @file     BSP.c
  * @author   Phone
  * @version  V1.0
  * @date      2014/12/14
  * @brief   LCD1602������������
  ******************************************************************************
  * @attention
  * �ŵ��ȳ���
  * CPJ���ܿ���
  * @endverbatim
  ******************************************************************************
  * @attention
  * ��
  ******************************************************************************
  */ 	
#include "LCD_1602.h"

/********************************�궨��*********************************/
#define    LCD_RS(x)  x ? GPIO_SetBits(CONPORT, RS): GPIO_ResetBits(GPIOB, RS)
#define    LCD_RW(x)  x ? GPIO_SetBits(CONPORT, RW): GPIO_ResetBits(GPIOB, RW)
#define    LCD_EN(x)  x ? GPIO_SetBits(CONPORT, EN): GPIO_ResetBits(GPIOB, EN)

/******************************** �������� -------------------------------------*/
static GPIO_InitTypeDef GPIO_InitStructure;     //GPIO

LCD1602WTITE_FLAG LCE1602_STATUS;
	
typedef unsigned char BYTE;
typedef unsigned char BOOL; 

/**---------------------------------------------------------------------------------
* @brief     ����ʱ�������ʱ����
---------------------------------------------------------------------------------*/
static void NOP()
{ 
	vu16 i;
  for(i=0; i<100; i++); 
}

/**---------------------------------------------------------------------------------
* @brief  ��ʱ����
---------------------------------------------------------------------------------*/
static void delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}

/**---------------------------------------------------------------------------------
* @brief     ��æ
* @retval     0��1
---------------------------------------------------------------------------------*/
static BOOL lcd_bz()
{                         // ����LCDæµ״̬
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
* @brief     д����
* @param    		��
*	@arg         NULL
* @retval     null
---------------------------------------------------------------------------------*/
static void lcd_wcmd(int cmd)
{                          // д��ָ�����ݵ�LCD
	while(lcd_bz());
	LCD_RS(0); // 0;
	LCD_RW(0); // 0;
	LCD_EN(0); // 0;
	NOP();
	NOP(); 
	GPIOB->BSRR = cmd<<8 & 0xff00;  //�������͵�P0�� 
	GPIOB->BRR = ((~cmd)<<8) & 0xff00;
	NOP();
	NOP();
	LCD_EN(1); // 1;
	NOP();
	NOP();
	LCD_EN(0); // 0;  
}
/**---------------------------------------------------------------------------------
* @brief      ����λ��
* @param    		��
*	@arg         NULL
* @retval     null
---------------------------------------------------------------------------------*/
static void 	lcd_pos(BYTE pos)
{                          //�趨��ʾλ��
	lcd_wcmd(pos | 0x80);
}
/**---------------------------------------------------------------------------------
* @brief      д���ݣ���ʾ���֣�
* @param    		��
*	@arg         NULL
* @retval     null
---------------------------------------------------------------------------------*/
static void lcd_wdat(int dat) 
{                          //д���ַ���ʾ���ݵ�LCD
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
* @brief      LCD��Ļ��ʼ��
* @param    		��
*	@arg         NULL
* @retval     null
---------------------------------------------------------------------------------*/
static void lcd_init()
{                        //LCD��ʼ���趨
	lcd_wcmd(0x38);          //16*2��ʾ��5*7����8λ����
	delay(20000);
	lcd_wcmd(0x0c);          //��ʾ�����ع��
	delay(20000);
	lcd_wcmd(0x06);          //�ƶ����
	delay(20000);
	lcd_wcmd(0x01);          //���LCD����ʾ����
	delay(20000);
}
/**---------------------------------------------------------------------------------
* @brief       ��Ӧ�ܽų�ʼ��
* @param    		��
*	@arg         NULL
* @retval     null
---------------------------------------------------------------------------------*/
void LCD_1602_CONFIG(void)
{
	//����GPIOģ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB 
						 | RCC_APB2Periph_AFIO, ENABLE);
	//�ѵ���������ͨIO��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; //����GPIOΪͬһ���Ͷ˿�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //��������Ƶ��Ϊ50HZ
//	GPIO_Init(GPIOA, &GPIO_InitStructure);   //��ʼ��GPIOA�˿�
	GPIO_Init(GPIOB, &GPIO_InitStructure);   //��ʼ��GPIOB�˿�

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | 
	GPIO_Pin_10 | GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; //����GPIOΪͬһ���Ͷ˿�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //��������Ƶ��Ϊ50HZ
	GPIO_Init(GPIOB, &GPIO_InitStructure);   //��ʼ��GPIOB�˿�
	
	GPIO_Write(GPIOA,0xffff);  //��GPIOA 16���˿�ȫ����Ϊ�ߵ�ƽ
	GPIO_Write(GPIOB,0xffff);  //��GPIOB 16���˿�ȫ����Ϊ�ߵ�

	lcd_init();               // ��ʼ��LCD
	delay(10);
}

/**---------------------------------------------------------------------------------
* @brief      ��Ӧλ����ʾ����
* @param    	y��1��2���ӵڼ��п�ʼд��
							x��0-15���Ӹ��еĵڼ�λ��ʼд��
*	@arg         NULL
* @retval     null
---------------------------------------------------------------------------------*/
unsigned char LCD1602_WRITE(unsigned char y,unsigned char x,unsigned char *write_data)
{
	BYTE i;
	lcd_wcmd(0x06);            //�����ƶ����
	if(y == 1)
	{	
		lcd_pos(x);                //������ʾλ��Ϊ��һ�еĵ�1���ַ�
		i = 0;
		while(*(write_data + i) != '\0')
		{                           //��ʾ�ַ�"WLCOME  TO"
			lcd_wdat(*(write_data + i));
			i++;
			delay(200);                //��������֮����ʾ�ٶ�
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
		lcd_pos(0x40 + x);                //������ʾλ��Ϊ��һ�еĵ�1���ַ�
		i = 0;
		while(*(write_data + i) != '\0')
		{                           //��ʾ�ַ�"WLCOME  TO"
			lcd_wdat(*(write_data + i));
			i++;
			delay(200);                //��������֮����ʾ�ٶ�
			if(i > 15)
			{
				LCE1602_STATUS = WRITE_NO;
				return LCE1602_STATUS;		
			}	 
		}
		LCE1602_STATUS = WRITE_OK;
	  return LCE1602_STATUS;	
	}
//	delay(80000);                //����ͣ��ʱ��
//	lcd_wcmd(0x01);            //���LCD����ʾ����
//	lcd_wcmd(0x04);            //�����ƶ����
//	lcd_pos(15);               //������ʾλ��Ϊ��һ�еĵ�16���ַ�
//	lcd_pos(0x4F);             //������ʾλ��Ϊ�ڶ��еĵ�16���ַ�
	
}
	
/*********************************************END OF FILE**********************/

