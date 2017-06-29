#include "nrf24l01.h"
#include "iospi.h"

const unsigned char TX_ADDRESS[TX_ADR_WIDTH]={0xE7,0xE7,0xE7,0xE7,0xE7}; //���͵�ַ
const unsigned char RX_ADDRESS[RX_ADR_WIDTH]={0xE7,0xE7,0xE7,0xE7,0xE7}; //���͵�ַ

void NRF_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);				
	GPIO_SetBits(GPIOC, GPIO_Pin_11 | GPIO_Pin_12);			

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);				
	GPIO_SetBits(GPIOB, GPIO_Pin_2);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);	
	
	IOSPI_GPIO_Init(); 
	SET_SPI_SCL_PIN(0);
	
	CLR_NRF_CE;  //ʹ��24L01
	SET_NRF_CSN; //����Ƭѡ�ر�
}

//�ϵ���NRF24L01�Ƿ����
//д5������Ȼ���ٶ��������бȽϣ���ͬʱ����ֵ:0����ʾ����;���򷵻�1����ʾ������
unsigned char NRF_Check(void)
{
	unsigned char buf[5]={0X11,0X11,0X11,0X11,0X11};
	unsigned char buf1[5];
	unsigned char i;   

	NRF_Write_Buf(SPI_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF_Read_Buf(TX_ADDR,buf1,5);              //����д��ĵ�ַ  	

	for(i=0;i<5;i++) if(buf1[i]!=0X11) break;					   
	if(i!=5) return 1;                          //NRF������	
	return 0;		                                //NRF����
}

//ͨ��SPIд�Ĵ���
unsigned char NRF_Write_Reg(unsigned char regaddr,unsigned char data)
{
	unsigned char status;	
	
  CLR_NRF_CSN;                    //ʹ��SPI����
  status =IOSPI_ReadWriteByte_R(regaddr); //���ͼĴ����� 
  IOSPI_ReadWriteByte_R(data);            //д��Ĵ�����ֵ
  SET_NRF_CSN;                    //��ֹSPI����	 
  
  return(status);       		         //����״ֵ̬
}

//��ȡSPI�Ĵ���ֵ ��regaddr:Ҫ���ļĴ���
unsigned char NRF_Read_Reg(unsigned char regaddr)
{
	unsigned char reg_val;	
	
 	CLR_NRF_CSN;                //ʹ��SPI����		
  IOSPI_ReadWriteByte_R(regaddr);     //���ͼĴ�����
  reg_val=IOSPI_ReadWriteByte_R(0X00);//��ȡ�Ĵ�������
  SET_NRF_CSN;                //��ֹSPI����	
	
  return(reg_val);                 //����״ֵ̬
}	

//��ָ��λ�ö���ָ�����ȵ�����
//*pBuf:����ָ��
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
unsigned char NRF_Read_Buf(unsigned char regaddr,unsigned char *pBuf,unsigned char datalen)
{
	unsigned char status,u8_ctr;	       
  	
	CLR_NRF_CSN;                     //ʹ��SPI����
  status=IOSPI_ReadWriteByte_R(regaddr);   //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
 	for(u8_ctr=0;u8_ctr<datalen;u8_ctr++) pBuf[u8_ctr]=IOSPI_ReadWriteByte_R(0X00);//��������
  SET_NRF_CSN;                     //�ر�SPI����
	
  return status;                        //���ض�����״ֵ̬
}

//��ָ��λ��дָ�����ȵ�����
//*pBuf:����ָ��
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
unsigned char NRF_Write_Buf(unsigned char regaddr, unsigned char *pBuf, unsigned char datalen)
{
	unsigned char status,u8_ctr;	 
	
 	CLR_NRF_CSN;                                    //ʹ��SPI����
  status = IOSPI_ReadWriteByte_R(regaddr);                //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  for(u8_ctr=0; u8_ctr<datalen; u8_ctr++) IOSPI_ReadWriteByte_R(*pBuf++); //д������	 
  SET_NRF_CSN;                                    //�ر�SPI����
	
  return status;                                       //���ض�����״ֵ̬
}		
/*
unsigned char NRF_RxPacket(unsigned char *rxbuf)
{
	unsigned char state;		    							      
	state=NRF_Read_Reg(STATUS);                //��ȡ״̬�Ĵ�����ֵ    	 
	NRF_Write_Reg(SPI_WRITE_REG+STATUS,state); //���TX_DS��MAX_RT�жϱ�־
	if(state&RX_OK)                                 //���յ�����
	{
		NRF_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF_Write_Reg(FLUSH_RX,0xff);          //���RX FIFO�Ĵ��� 
		return 0; 
	}	   
	return 1;                                      //û�յ��κ�����
}

//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
void RX_Mode(void)
{
	CLR_NRF_CE;	  
  NRF_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(unsigned char*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
	 
  NRF_Write_Reg(SPI_WRITE_REG+EN_AA,0);//0x01);    //ʹ��ͨ��0���Զ�Ӧ��    
  NRF_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ  	 
  NRF_Write_Reg(SPI_WRITE_REG+RF_CH,40);	     //����RFͨ��Ƶ��		  
  NRF_Write_Reg(SPI_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
  NRF_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f); //����TX�������,0db����,2Mbps,���������濪��   
	
  NRF_Write_Reg(SPI_WRITE_REG+CONFIG, 0x03);  //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
  SET_NRF_CE;                                //CEΪ��,�������ģʽ 
}						 
//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
void TX_Mode(void)
{														 
	CLR_NRF_CE;	    
  NRF_Write_Buf(SPI_WRITE_REG+TX_ADDR,(unsigned char*)TX_ADDRESS,TX_ADR_WIDTH);    //дTX�ڵ��ַ 
  NRF_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(unsigned char*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

  NRF_Write_Reg(SPI_WRITE_REG+EN_AA,0);//0x01);    //ʹ��ͨ��0���Զ�Ӧ��    
  NRF_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01);  //ʹ��ͨ��0�Ľ��յ�ַ  
  NRF_Write_Reg(SPI_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  NRF_Write_Reg(SPI_WRITE_REG+RF_CH,40);       //����RFͨ��Ϊ40
  NRF_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
  	
	NRF_Write_Reg(SPI_WRITE_REG+CONFIG,0x02);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	SET_NRF_CE;                                  //CEΪ��,10us����������
}

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
void NRF_TxPacket(unsigned char *txbuf)
{
  CLR_NRF_CE;
  NRF_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(unsigned char*)RX_ADDRESS,RX_ADR_WIDTH); 
  NRF_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  20���ֽ�
  NRF_Write_Reg(SPI_WRITE_REG+CONFIG,0x0e);
  SET_NRF_CE;
}
*/

