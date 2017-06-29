/**
  ******************************************************************************
  * @file    APP.c
  * @author  phone
  * @version V1.0
  * @date    2014-12-14
  * @brief   LCD1602��ʾ
  ******************************************************************************
  * @attention
  *
  * @{ʵ��ƽ̨:
  * @{�ŵ��ȳ�������	
	* @{ USE_STM3210B_EVAL
  ******************************************************************************
  */ 
	
/* Includes ------------------------------------------------------------------*/
#include <stm32f10x.h>
#include "BSP.h"
#include "LCD_1602.h"
#include "bsp_adc.h"
#include "bsp_led.h"
#include "delay.h"

#define time_open 180


unsigned int ADC_ConvertedValueLocal;    
unsigned char dis1[] = {"Power voltage:"};
unsigned char dis2[8];


void Delay(__IO u32 nCount); 


void display()
{

	if(ADC_ConvertedValue > 1.9)
		ADC_ConvertedValueLocal =ADC_ConvertedValue*3000/4096; // ��ȡת����ADֵ
	else
		ADC_ConvertedValueLocal =ADC_ConvertedValue*2500/4096; // ��ȡת����ADֵ	
	//ADC_ConvertedValueLocal = (*(__IO u16 *)ADC1_DR_Address)*3300/4096;// ��ȡת����ADֵ
	dis2[0] = ADC_ConvertedValueLocal%10000/1000 + 0x30;
	//	dis2[1] = '.';
	dis2[1] = ADC_ConvertedValueLocal%1000/100 + 0x30;
	dis2[2] = ADC_ConvertedValueLocal%100/10 + 0x30;
	dis2[3] = '.';
	dis2[4] = ADC_ConvertedValueLocal%10 + 0x30;
	//			
	//dis2[0] = 9 + 0x30;
	if(ADC_ConvertedValueLocal > 10)
	LCD1602_WRITE(2,0,dis2);

}


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
	
	int i,j,flag;
	/*����Ӳ����ʼ��*/
	ALL_Config();
	delay_init(72);
	LCD_1602_CONFIG();
	ADC1_Init();

	LCD1602_WRITE(1,0,dis1);

	//�̵���
	RELAY_OFF;
	
	//������
	BUZZER_OFF;	 
/**----------------------------------------------------------------------
* @brief ����3��ʹ�üĴ�������IO
-----------------------------------------------------------------------*/
	#if 1
	while(1)
	{
			
			display();
			if(detection_vol(ADC_ConvertedValueLocal))
			{
				RELAY_ON;
				
				for(i = 0;i < 3;i++)
				{
					BUZZER_ON;
					delay_ms(500);	
					BUZZER_OFF;	
					delay_ss(2);
					display();
				}
				flag = 1;
			}
			
			if(1700 <= ADC_ConvertedValueLocal <= 2500)
			{
					if(flag)
					{
						for(j = 0; j < time_open;j++)
						{
							delay_ss(1);
							display();
						}	
						if(1700 <= ADC_ConvertedValueLocal <= 2500)
						{
							BUZZER_OFF;	
							RELAY_OFF;
							flag = 0;				
						}
					}
					else
					{
							BUZZER_OFF;	
							RELAY_OFF;									
					}
			}

			Delay(0x4fffee);  
	}
	#endif	
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


//void INT0_SERVE()
//{
//    water_data1.mL_data++;
//    if(water_data1.mL_data >= 450)    
//    {
//        water_data1.mL_data = 0;
//        water_data1.L_data ++;
//        if(water_data1.L_data >= 1000)
//        {
//            water_data1.L_data = 0;
//            water_data1.M3_data ++ ;
//            if(water_data1.M3_data >= 1000)
//            {
//                water_data1.M3_data = 0;         
//            }
//        }        
//    }		
//}

