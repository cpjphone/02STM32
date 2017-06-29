#include "nrf24l01.h"
#include "iospi.h"

const unsigned char TX_ADDRESS[TX_ADR_WIDTH]={0xE7,0xE7,0xE7,0xE7,0xE7}; //发送地址
const unsigned char RX_ADDRESS[RX_ADR_WIDTH]={0xE7,0xE7,0xE7,0xE7,0xE7}; //发送地址

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
	
	CLR_NRF_CE;  //使能24L01
	SET_NRF_CSN; //数据片选关闭
}

//上电检测NRF24L01是否存在
//写5个数据然后再读回来进行比较，相同时返回值:0，表示存在;否则返回1，表示不存在
unsigned char NRF_Check(void)
{
	unsigned char buf[5]={0X11,0X11,0X11,0X11,0X11};
	unsigned char buf1[5];
	unsigned char i;   

	NRF_Write_Buf(SPI_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.	
	NRF_Read_Buf(TX_ADDR,buf1,5);              //读出写入的地址  	

	for(i=0;i<5;i++) if(buf1[i]!=0X11) break;					   
	if(i!=5) return 1;                          //NRF不存在	
	return 0;		                                //NRF存在
}

//通过SPI写寄存器
unsigned char NRF_Write_Reg(unsigned char regaddr,unsigned char data)
{
	unsigned char status;	
	
  CLR_NRF_CSN;                    //使能SPI传输
  status =IOSPI_ReadWriteByte_R(regaddr); //发送寄存器号 
  IOSPI_ReadWriteByte_R(data);            //写入寄存器的值
  SET_NRF_CSN;                    //禁止SPI传输	 
  
  return(status);       		         //返回状态值
}

//读取SPI寄存器值 ，regaddr:要读的寄存器
unsigned char NRF_Read_Reg(unsigned char regaddr)
{
	unsigned char reg_val;	
	
 	CLR_NRF_CSN;                //使能SPI传输		
  IOSPI_ReadWriteByte_R(regaddr);     //发送寄存器号
  reg_val=IOSPI_ReadWriteByte_R(0X00);//读取寄存器内容
  SET_NRF_CSN;                //禁止SPI传输	
	
  return(reg_val);                 //返回状态值
}	

//在指定位置读出指定长度的数据
//*pBuf:数据指针
//返回值,此次读到的状态寄存器值 
unsigned char NRF_Read_Buf(unsigned char regaddr,unsigned char *pBuf,unsigned char datalen)
{
	unsigned char status,u8_ctr;	       
  	
	CLR_NRF_CSN;                     //使能SPI传输
  status=IOSPI_ReadWriteByte_R(regaddr);   //发送寄存器值(位置),并读取状态值   	   
 	for(u8_ctr=0;u8_ctr<datalen;u8_ctr++) pBuf[u8_ctr]=IOSPI_ReadWriteByte_R(0X00);//读出数据
  SET_NRF_CSN;                     //关闭SPI传输
	
  return status;                        //返回读到的状态值
}

//在指定位置写指定长度的数据
//*pBuf:数据指针
//返回值,此次读到的状态寄存器值
unsigned char NRF_Write_Buf(unsigned char regaddr, unsigned char *pBuf, unsigned char datalen)
{
	unsigned char status,u8_ctr;	 
	
 	CLR_NRF_CSN;                                    //使能SPI传输
  status = IOSPI_ReadWriteByte_R(regaddr);                //发送寄存器值(位置),并读取状态值
  for(u8_ctr=0; u8_ctr<datalen; u8_ctr++) IOSPI_ReadWriteByte_R(*pBuf++); //写入数据	 
  SET_NRF_CSN;                                    //关闭SPI传输
	
  return status;                                       //返回读到的状态值
}		
/*
unsigned char NRF_RxPacket(unsigned char *rxbuf)
{
	unsigned char state;		    							      
	state=NRF_Read_Reg(STATUS);                //读取状态寄存器的值    	 
	NRF_Write_Reg(SPI_WRITE_REG+STATUS,state); //清除TX_DS或MAX_RT中断标志
	if(state&RX_OK)                                 //接收到数据
	{
		NRF_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
		NRF_Write_Reg(FLUSH_RX,0xff);          //清除RX FIFO寄存器 
		return 0; 
	}	   
	return 1;                                      //没收到任何数据
}

//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了		   
void RX_Mode(void)
{
	CLR_NRF_CE;	  
  NRF_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(unsigned char*)RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址
	 
  NRF_Write_Reg(SPI_WRITE_REG+EN_AA,0);//0x01);    //使能通道0的自动应答    
  NRF_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址  	 
  NRF_Write_Reg(SPI_WRITE_REG+RF_CH,40);	     //设置RF通信频率		  
  NRF_Write_Reg(SPI_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 	    
  NRF_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
	
  NRF_Write_Reg(SPI_WRITE_REG+CONFIG, 0x03);  //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
  SET_NRF_CE;                                //CE为高,进入接收模式 
}						 
//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了		   
//CE为高大于10us,则启动发送.	 
void TX_Mode(void)
{														 
	CLR_NRF_CE;	    
  NRF_Write_Buf(SPI_WRITE_REG+TX_ADDR,(unsigned char*)TX_ADDRESS,TX_ADR_WIDTH);    //写TX节点地址 
  NRF_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(unsigned char*)RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  

  NRF_Write_Reg(SPI_WRITE_REG+EN_AA,0);//0x01);    //使能通道0的自动应答    
  NRF_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01);  //使能通道0的接收地址  
  NRF_Write_Reg(SPI_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  NRF_Write_Reg(SPI_WRITE_REG+RF_CH,40);       //设置RF通道为40
  NRF_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	
	NRF_Write_Reg(SPI_WRITE_REG+CONFIG,0x02);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	SET_NRF_CE;                                  //CE为高,10us后启动发送
}

//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
void NRF_TxPacket(unsigned char *txbuf)
{
  CLR_NRF_CE;
  NRF_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(unsigned char*)RX_ADDRESS,RX_ADR_WIDTH); 
  NRF_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  20个字节
  NRF_Write_Reg(SPI_WRITE_REG+CONFIG,0x0e);
  SET_NRF_CE;
}
*/

