#include "stm32f10x.h"

#define TX_OK   	0x20  //TX发送完成中断

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
*            RCC配置
* 
*******************************************************************************/
void RCC_Configuration(void)
{
  //复位RCC外部设备寄存器到默认值
  RCC_DeInit();  

  //打开外部高速晶振
  RCC_HSEConfig(RCC_HSE_ON); 

 //等待外部高速时钟准备好
  HSEStartUpStatus = RCC_WaitForHSEStartUp(); 

  //外部高速时钟已经准别好
  if(HSEStartUpStatus == SUCCESS)  
  {
    
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

   
    FLASH_SetLatency(FLASH_Latency_2);
  
   //配置AHB(HCLK)时钟=SYSCLK
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
  
    //配置APB2(PCLK2)钟=AHB时钟
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    //配置APB1(PCLK1)钟=AHB 1/2时钟
    RCC_PCLK1Config(RCC_HCLK_Div2);  

    //配置ADC时钟=PCLK2 1/4
    RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
  
    //配置PLL时钟 == 外部高速晶体时钟*9
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); 
    
	//配置ADC时钟= PCLK2/4
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);

   //使能PLL时钟
    RCC_PLLCmd(ENABLE);  

   //等待PLL时钟就绪
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)  
    {
    }

    //配置系统时钟 = PLL时钟
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 

   //检查PLL时钟是否作为系统时钟
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
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIOC ->ODR ^= GPIO_Pin_6;
	

}

void Dht11 ()
{
	u8 kk;
	kk=ReadDHT11 ();
	tmp_buf[0]=tdata[2];  //温度	
	tmp_buf[1]=tdata[0];	//湿度
}
int main ()
{
	u8 mode;
	//u8 tmp_buf[33]={10};
	RCC_Configuration ();
	GPIO_Configuraction ();
	Dht11 ();
	NRF24L01_Init();    //初始化NRF24L01
	while(NRF24L01_Check())//检测不到24L01
	{
		GPIO_SetBits (GPIOA,GPIO_Pin_8);
		//GPIO_ResetBits (GPIOA,GPIO_Pin_8);
	}
	while (1)
	{
		
			//if(NRF24L01_TxPacket(tmp_buf)==TX_OK)	//如果发射出去，则灯灭，tmp_buf发射出去的内容
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
// 			if(NRF24L01_RxPacket(tmp_buf)==0)		//如果接收到，则灯灭,tmp_buf接收到的内容
// 				//GPIO_ResetBits (GPIOA,GPIO_Pin_8);
// 				GPIO_SetBits (GPIOA,GPIO_Pin_8);
// 		}
// 	}
// 	else
// 	{
// 		TX_Mode();
// 		while (1)
// 		{
// 			if(NRF24L01_TxPacket(tmp_buf)==TX_OK)	//如果发射出去，则灯灭，tmp_buf发射出去的内容
// 				GPIO_SetBits (GPIOA,GPIO_Pin_8);
// 				//GPIO_ResetBits (GPIOA,GPIO_Pin_8);
// 		}
// 	}
}

