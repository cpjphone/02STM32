#include "stm32f10x.h"

#define TX_OK   	0x20  //TX��������ж�

#define KEY1 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13)
#define KEY2 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)

ErrorStatus HSEStartUpStatus;

u8 tmp_buf[2];

extern u8 tdata[4];

extern u8 ReadDHT11(void);
extern void TX_Mode(void);
extern void RX_Mode(void);
extern void NRF24L01_Init(void);
extern u8 NRF24L01_Check(void);
extern u8 NRF24L01_TxPacket(u8 *txbuf);
extern u8 NRF24L01_RxPacket(u8 *rxbuf);


/*******************************************************************************
* 
*            RCC����
* 
*******************************************************************************/
void RCC_Configuration(void)
{
  //��λRCC�ⲿ�豸�Ĵ�����Ĭ��ֵ
  RCC_DeInit();  

  //���ⲿ���پ���
  RCC_HSEConfig(RCC_HSE_ON); 

 //�ȴ��ⲿ����ʱ��׼����
  HSEStartUpStatus = RCC_WaitForHSEStartUp(); 

  //�ⲿ����ʱ���Ѿ�׼���
  if(HSEStartUpStatus == SUCCESS)  
  {
    
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

   
    FLASH_SetLatency(FLASH_Latency_2);
  
   //����AHB(HCLK)ʱ��=SYSCLK
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
  
    //����APB2(PCLK2)��=AHBʱ��
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    //����APB1(PCLK1)��=AHB 1/2ʱ��
    RCC_PCLK1Config(RCC_HCLK_Div2);  

    //����ADCʱ��=PCLK2 1/4
    RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
  
    //����PLLʱ�� == �ⲿ���پ���ʱ��*9
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); 
    
	//����ADCʱ��= PCLK2/4
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);

   //ʹ��PLLʱ��
    RCC_PLLCmd(ENABLE);  

   //�ȴ�PLLʱ�Ӿ���
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)  
    {
    }

    //����ϵͳʱ�� = PLLʱ��
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 

   //���PLLʱ���Ƿ���Ϊϵͳʱ��
    while(RCC_GetSYSCLKSource() != 0x08)  
    {
    }
  }

  /* TIM1, GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOE, ENABLE);
}


void GPIO_Configuraction ()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //�������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIOC ->ODR ^= GPIO_Pin_6;
	

}

void Dht11 ()
{
	u8 kk;
	kk=ReadDHT11 ();
	tmp_buf[0]=tdata[2];  //�¶�	
	tmp_buf[1]=tdata[0];	//ʪ��
}
int main ()
{
	u8 mode;
	//u8 tmp_buf[33]={10};
	RCC_Configuration ();
	GPIO_Configuraction ();
	Dht11 ();
	NRF24L01_Init();    //��ʼ��NRF24L01
	while(NRF24L01_Check())//��ⲻ��24L01
	{
		GPIO_SetBits (GPIOA,GPIO_Pin_8);
		//GPIO_ResetBits (GPIOA,GPIO_Pin_8);
	}
	while (1)
	{
		
			//if(NRF24L01_TxPacket(tmp_buf)==TX_OK)	//��������ȥ�������tmp_buf�����ȥ������
			GPIOC ->ODR ^= GPIO_Pin_6|GPIO_Pin_7;   
 		//		GPIO_SetBits (GPIOA,GPIO_Pin_8);
			
		}
// 		if (KEY1==0)
// 		{
// 			mode=0;   
// 			break;
// 		}
// 		if (KEY2==0)
// 		{
// 			mode=1;;   
// 			break;
// 		}
// 	}
// 	if(mode==0)
// 	{
// 		RX_Mode();
// 		while (1)
// 		{
// 			if(NRF24L01_RxPacket(tmp_buf)==0)		//������յ��������,tmp_buf���յ�������
// 				//GPIO_ResetBits (GPIOA,GPIO_Pin_8);
// 				GPIO_SetBits (GPIOA,GPIO_Pin_8);
// 		}
// 	}
// 	else
// 	{
// 		TX_Mode();
// 		while (1)
// 		{
// 			if(NRF24L01_TxPacket(tmp_buf)==TX_OK)	//��������ȥ�������tmp_buf�����ȥ������
// 				GPIO_SetBits (GPIOA,GPIO_Pin_8);
// 				//GPIO_ResetBits (GPIOA,GPIO_Pin_8);
// 		}
// 	}
}

