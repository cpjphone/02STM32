/**
  ******************************************************************************
  * @file         bsp_xxx.c
  * @author   Phone
  * @version  V1.0
  * @date      2014/08/03
  * @brief      2.4g����ģ��/nrf24l01+/master Ӧ��bsp
  ******************************************************************************
  * @attention
  *STM320f0 Ӳ��SPI����NRF24L01
  *SPI      �� ��
  *SCK     ��   PA5  
  *MOSO  ��  PA6  
  *MOS I   ��  PA7  
  * CE��CSN��IRQ   �� ��
  *CE        ��  PA2
  *CSN     ��  PA3
  * IRQ     ��  PA0
  *0db����,2Mbps,���������濪�� 
  * CPJ���ܿ���
  * @endverbatim
  ******************************************************************************
  * @attention
  * F0ϵ�е�Ƭ��SPI�˿ڱ���һ������ʼ��
  ******************************************************************************
  */ 
#include "bsp_spi_nrf.h"

 u8 RX_BUF[RX_PLOAD_WIDTH];		//�������ݻ���
 u8 TX_BUF[TX_PLOAD_WIDTH];		//�������ݻ���
 u8 TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x71};  // ����һ����̬���͵�ַ
 u8 RX_ADDRESS[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x71};

 /*********************************************************************************
  * @brief      CPU��ʱ����
  * @param   ��
  * @retval    ��
*********************************************************************************/
void Delay(__IO u32 nCount)
{
  for(; nCount != 0; nCount--);
} 

/*********************************************************************************
  * @brief      SPI�� I/O����
  * @param   ��
  * @retval    ��
*********************************************************************************/
void SPI_NRF_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /*������ӦIO�˿ڵ�ʱ��*/
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
    /*ʹ��SPI1ʱ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    /*����SPI_NRF_SPI��CE, CSN �����������*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*����SPI_NRF_SPI��CE, CSN �����������*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*����SPI_NRF_SPI��IRQ����,��������*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0);

//****************************** SPI�� I/O��ģʽ����*****************************************//

    /*���� SPI_NRF_SPI�� SCK,MISO,MOSI���ţ�GPIOA^5,GPIOA^6,GPIOA^7 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_0);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 					       //��ģʽ
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	 			           //���ݴ�С8λ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		 			   	           //ʱ�Ӽ��ԣ�����ʱΪ��
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;					           //��1��������Ч��������Ϊ����ʱ��
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		   					           //NSS�ź����������
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  //8��Ƶ��6MHz
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  				           //��λ��ǰ
    SPI_InitStructure.SPI_CRCPolynomial = 7;										   //CRCֵ����Ķ���ʽ
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);          //���ý��ջ���
    /* Enable SPI1  */
    SPI_Cmd(SPI1, ENABLE);
    SPI_NRF_RW(0xff);

    /* �����Զ���ĺ꣬��������csn���ţ�NRF�������״̬ */
    NRF_CE_LOW();
    NRF_CSN_HIGH();
}


/*********************************************************************************
  * @brief       ������NRF��/дһ�ֽ�����
  * @param    д�������
  *	@arg         dat 
  * @retval     ��ȡ�õ�����
  *********************************************************************************/
u8 SPI_NRF_RW(u8 dat)
{  	   	
    u8 retry=0;          
    //send
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI���ͻ���ձ�־λ
    {
        retry++;
        if(retry>200)return 0;
    }              
    SPI_SendData8(SPI1, dat); //ͨ������SPIx����һ������
    retry=0;
    //receive
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//���ָ����SPI���ܻ���ǿձ�־λ
    {
        retry++;
        if(retry>200)return 0;
    }                                 
    return SPI_ReceiveData8(SPI1); //����ͨ��SPIx������յ�����
}
/*********************************************************************************
  *  @brief   ������NRF�ض��ļĴ���д������
  *  @param   
  *	 @arg       reg:NRF������+�Ĵ�����ַ
  *	 @arg       dat:��Ҫ��Ĵ���д�������
  *  @retval   NRF��status�Ĵ�����״̬
  *********************************************************************************/
u8 SPI_NRF_WriteReg(u8 reg,u8 dat)
{
 	u8 status;
	 NRF_CE_LOW();
	/*�õ�CSN��ʹ��SPI����*/
    NRF_CSN_LOW();
				
	/*��������Ĵ����� */
	status = SPI_NRF_RW(reg);
		 
	 /*��Ĵ���д������*/
    SPI_NRF_RW(dat); 
	          
	/*CSN���ߣ����*/	   
  	NRF_CSN_HIGH();	
		
	/*����״̬�Ĵ�����ֵ*/
   	return(status);
}

/*********************************************************************************
  * @brief    ���ڴ�NRF�ض��ļĴ�����������
  * @param   
  * @arg      reg:NRF������+�Ĵ�����ַ
  * @retval  �Ĵ����е�����
  *********************************************************************************/
u8 SPI_NRF_ReadReg(u8 reg)
{
 	u8 reg_val;

	NRF_CE_LOW();
	/*�õ�CSN��ʹ��SPI����*/
 	NRF_CSN_LOW();
				
  	 /*���ͼĴ�����*/
	SPI_NRF_RW(reg); 

	 /*��ȡ�Ĵ�����ֵ */
	reg_val = SPI_NRF_RW(NOP);
	            
   	/*CSN���ߣ����*/
	NRF_CSN_HIGH();		
   	
	return reg_val;
}	

/*********************************************************************************
  * @brief      ������NRF�ļĴ�����д��һ������
  * @param   
  * @arg        reg : NRF������+�Ĵ�����ַ
  * @arg        pBuf�����ڴ洢���������ļĴ������ݵ����飬�ⲿ����
  * @arg        bytes: pBuf�����ݳ���
  * @retval     NRF��status�Ĵ�����״̬
  *********************************************************************************/
u8 SPI_NRF_ReadBuf(u8 reg,u8 *pBuf,u8 bytes)
{
 	u8 status, byte_cnt;

	  NRF_CE_LOW();
	/*�õ�CSN��ʹ��SPI����*/
	NRF_CSN_LOW();
		
	/*���ͼĴ�����*/		
	status = SPI_NRF_RW(reg); 

 	/*��ȡ����������*/
	 for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)		  
	   pBuf[byte_cnt] = SPI_NRF_RW(NOP); //��NRF24L01��ȡ����  

	 /*CSN���ߣ����*/
	NRF_CSN_HIGH();	
		
 	return status;		//���ؼĴ���״ֵ̬
}

/*********************************************************************************
  * @brief     ������NRF�ļĴ�����д��һ������
  * @param   
  *	 @arg       reg : NRF������+�Ĵ�����ַ
  *	 @arg       pBuf���洢�˽�Ҫд��д�Ĵ������ݵ����飬�ⲿ����
  *  @arg       bytes: pBuf�����ݳ���
  *  @retval    NRF��status�Ĵ�����״̬
  *********************************************************************************/
u8 SPI_NRF_WriteBuf(u8 reg ,u8 *pBuf,u8 bytes)
{
	 u8 status,byte_cnt;
	 NRF_CE_LOW();
   	 /*�õ�CSN��ʹ��SPI����*/
	 NRF_CSN_LOW();			

	 /*���ͼĴ�����*/	
  	 status = SPI_NRF_RW(reg); 
 	
  	  /*�򻺳���д������*/
	 for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)
		SPI_NRF_RW(*pBuf++);	//д���ݵ������� 	 
	  	   
	/*CSN���ߣ����*/
	NRF_CSN_HIGH();			
  
  	return (status);	//����NRF24L01��״̬ 		
}

/*********************************************************************************
  * @brief  ���ò��������ģʽ
  * @param  ��
  * @retval ��
  *********************************************************************************/
void NRF_RX_Mode(void)

{
	NRF_CE_LOW();	

   SPI_NRF_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ

   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_AA,0x01);    //ʹ��ͨ��0���Զ�Ӧ��    

   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ    

   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_CH,CHANAL);      //����RFͨ��Ƶ��    

   SPI_NRF_WriteReg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��      

   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0f); //����TX�������,0db����,2Mbps,���������濪��   

   SPI_NRF_WriteReg(NRF_WRITE_REG+CONFIG, 0x0f);  //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 

/*CE���ߣ��������ģʽ*/	
  NRF_CE_HIGH();

}    

/*********************************************************************************
  * @brief  ���÷���ģʽ
  * @param  ��
  * @retval ��
  *********************************************************************************/
void NRF_TX_Mode(void)
{  
	NRF_CE_LOW();		

   SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);    //дTX�ڵ��ַ 

   SPI_NRF_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK   

   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    

   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  

   SPI_NRF_WriteReg(NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��

   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_CH,CHANAL);       //����RFͨ��ΪCHANAL

   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
	
   SPI_NRF_WriteReg(NRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�

/*CE���ߣ����뷢��ģʽ*/	
  NRF_CE_HIGH();
    Delay(0xffff); //CEҪ����һ��ʱ��Ž��뷢��ģʽ
}

/*********************************************************************************
  * @brief  ��Ҫ����NRF��MCU�Ƿ���������
  * @param  ��
  * @retval SUCCESS/ERROR ��������/����ʧ��
  *********************************************************************************/
u8 NRF_Check(void)
{
	u8 buf[5]={0xC2,0xC2,0xC2,0xC2,0xC2};
	u8 buf1[5];
	u8 i; 
	 
	/*д��5���ֽڵĵ�ַ.  */  
	SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR,buf,5);

	/*����д��ĵ�ַ */
	SPI_NRF_ReadBuf(TX_ADDR,buf1,5); 
	 
	/*�Ƚ�*/               
	for(i=0;i<5;i++)
	{
		if(buf1[i]!=0xC2)
		break;
	} 
	       
	if(i==5)
		return SUCCESS ;        //MCU��NRF�ɹ����� 
	else
		return ERROR ;        //MCU��NRF����������
}

/*********************************************************************************
  * @brief   ������NRF�ķ��ͻ�������д������
  * @param   
  *	 @arg     txBuf���洢�˽�Ҫ���͵����ݵ����飬�ⲿ����	
  * @retval  ���ͽ�����ɹ�����TXDS,ʧ�ܷ���MAXRT��ERROR
  *********************************************************************************/
u8 NRF_Tx_Dat(u8 *txbuf)
{
	u8 state;  

	 /*ceΪ�ͣ��������ģʽ1*/
	NRF_CE_LOW();

	/*д���ݵ�TX BUF ��� 32���ֽ�*/						
   SPI_NRF_WriteBuf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);

      /*CEΪ�ߣ�txbuf�ǿգ��������ݰ� */   
 	 NRF_CE_HIGH();
	  	
	  /*�ȴ���������ж� */                            
	while(NRF_Read_IRQ()!=0); 	
	
	/*��ȡ״̬�Ĵ�����ֵ */                              
	state = SPI_NRF_ReadReg(STATUS);

	 /*���TX_DS��MAX_RT�жϱ�־*/                  
	SPI_NRF_WriteReg(NRF_WRITE_REG+STATUS,state); 	

	SPI_NRF_WriteReg(FLUSH_TX,NOP);    //���TX FIFO�Ĵ��� 

	 /*�ж��ж�����*/    
	if(state&MAX_RT)                     //�ﵽ����ط�����
			 return MAX_RT; 

	else if(state&TX_DS)                  //�������
		 	return TX_DS;
	 else						  
			return ERROR;                 //����ԭ����ʧ��
} 

/*********************************************************************************
  * @brief   ���ڴ�NRF�Ľ��ջ������ж�������
  * @param   
  *	 @arg     rxBuf �����ڽ��ո����ݵ����飬�ⲿ����	
  * @retval 
  *	 @arg    ���ս��
  *********************************************************************************/
u8 NRF_Rx_Dat(u8 *rxbuf)
{
	u8 state; 
	NRF_CE_HIGH();	 //�������״̬
	 /*�ȴ������ж�*/
	while(NRF_Read_IRQ()!=0); 
	
	NRF_CE_LOW();  	 //�������״̬
	/*��ȡstatus�Ĵ�����ֵ  */               
	state=SPI_NRF_ReadReg(STATUS);
	 
	/* ����жϱ�־*/      
	SPI_NRF_WriteReg(NRF_WRITE_REG+STATUS,state);

	/*�ж��Ƿ���յ�����*/
	if(state&RX_DR)                                 //���յ�����
	{
	  SPI_NRF_ReadBuf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
	     SPI_NRF_WriteReg(FLUSH_RX,NOP);          //���RX FIFO�Ĵ���
	  return RX_DR; 
	}
	else    
		return ERROR;                    //û�յ��κ�����
}
/*********************************************END OF FILE**********************/
