/**
  ******************************************************************************
  * @file         bsp_xxx.c
  * @author   Phone
  * @version  V1.0
  * @date      2014/08/03
  * @brief      2.4g无线模块/nrf24l01+/master 应用bsp
  ******************************************************************************
  * @attention
  *STM320f0 硬件SPI连接NRF24L01
  *SPI      端 口
  *SCK     →   PA5  
  *MOSO  →  PA6  
  *MOS I   →  PA7  
  * CE、CSN、IRQ   端 口
  *CE        →  PA2
  *CSN     →  PA3
  * IRQ     →  PA0
  *0db增益,2Mbps,低噪声增益开启 
  * CPJ智能控制
  * @endverbatim
  ******************************************************************************
  * @attention
  * F0系列单片机SPI端口必须一个个初始化
  ******************************************************************************
  */ 
#include "bsp_spi_nrf.h"

 u8 RX_BUF[RX_PLOAD_WIDTH];		//接收数据缓存
 u8 TX_BUF[TX_PLOAD_WIDTH];		//发射数据缓存
 u8 TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x71};  // 定义一个静态发送地址
 u8 RX_ADDRESS[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x71};

 /*********************************************************************************
  * @brief      CPU延时函数
  * @param   无
  * @retval    无
*********************************************************************************/
void Delay(__IO u32 nCount)
{
  for(; nCount != 0; nCount--);
} 

/*********************************************************************************
  * @brief      SPI的 I/O配置
  * @param   无
  * @retval    无
*********************************************************************************/
void SPI_NRF_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /*开启相应IO端口的时钟*/
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
    /*使能SPI1时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    /*配置SPI_NRF_SPI的CE, CSN 引脚推挽输出*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*配置SPI_NRF_SPI的CE, CSN 引脚推挽输出*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*配置SPI_NRF_SPI的IRQ引脚,上拉输入*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0);

//****************************** SPI的 I/O及模式配置*****************************************//

    /*配置 SPI_NRF_SPI的 SCK,MISO,MOSI引脚，GPIOA^5,GPIOA^6,GPIOA^7 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_0);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //双线全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 					       //主模式
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	 			           //数据大小8位
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		 			   	           //时钟极性，空闲时为低
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;					           //第1个边沿有效，上升沿为采样时刻
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		   					           //NSS信号由软件产生
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  //8分频，6MHz
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  				           //高位在前
    SPI_InitStructure.SPI_CRCPolynomial = 7;										   //CRC值计算的多项式
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);          //设置接收缓存
    /* Enable SPI1  */
    SPI_Cmd(SPI1, ENABLE);
    SPI_NRF_RW(0xff);

    /* 这是自定义的宏，用于拉高csn引脚，NRF进入空闲状态 */
    NRF_CE_LOW();
    NRF_CSN_HIGH();
}


/*********************************************************************************
  * @brief       用于向NRF读/写一字节数据
  * @param    写入的数据
  *	@arg         dat 
  * @retval     读取得的数据
  *********************************************************************************/
u8 SPI_NRF_RW(u8 dat)
{  	   	
    u8 retry=0;          
    //send
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI发送缓存空标志位
    {
        retry++;
        if(retry>200)return 0;
    }              
    SPI_SendData8(SPI1, dat); //通过外设SPIx发送一个数据
    retry=0;
    //receive
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI接受缓存非空标志位
    {
        retry++;
        if(retry>200)return 0;
    }                                 
    return SPI_ReceiveData8(SPI1); //返回通过SPIx最近接收的数据
}
/*********************************************************************************
  *  @brief   用于向NRF特定的寄存器写入数据
  *  @param   
  *	 @arg       reg:NRF的命令+寄存器地址
  *	 @arg       dat:将要向寄存器写入的数据
  *  @retval   NRF的status寄存器的状态
  *********************************************************************************/
u8 SPI_NRF_WriteReg(u8 reg,u8 dat)
{
 	u8 status;
	 NRF_CE_LOW();
	/*置低CSN，使能SPI传输*/
    NRF_CSN_LOW();
				
	/*发送命令及寄存器号 */
	status = SPI_NRF_RW(reg);
		 
	 /*向寄存器写入数据*/
    SPI_NRF_RW(dat); 
	          
	/*CSN拉高，完成*/	   
  	NRF_CSN_HIGH();	
		
	/*返回状态寄存器的值*/
   	return(status);
}

/*********************************************************************************
  * @brief    用于从NRF特定的寄存器读出数据
  * @param   
  * @arg      reg:NRF的命令+寄存器地址
  * @retval  寄存器中的数据
  *********************************************************************************/
u8 SPI_NRF_ReadReg(u8 reg)
{
 	u8 reg_val;

	NRF_CE_LOW();
	/*置低CSN，使能SPI传输*/
 	NRF_CSN_LOW();
				
  	 /*发送寄存器号*/
	SPI_NRF_RW(reg); 

	 /*读取寄存器的值 */
	reg_val = SPI_NRF_RW(NOP);
	            
   	/*CSN拉高，完成*/
	NRF_CSN_HIGH();		
   	
	return reg_val;
}	

/*********************************************************************************
  * @brief      用于向NRF的寄存器中写入一串数据
  * @param   
  * @arg        reg : NRF的命令+寄存器地址
  * @arg        pBuf：用于存储将被读出的寄存器数据的数组，外部定义
  * @arg        bytes: pBuf的数据长度
  * @retval     NRF的status寄存器的状态
  *********************************************************************************/
u8 SPI_NRF_ReadBuf(u8 reg,u8 *pBuf,u8 bytes)
{
 	u8 status, byte_cnt;

	  NRF_CE_LOW();
	/*置低CSN，使能SPI传输*/
	NRF_CSN_LOW();
		
	/*发送寄存器号*/		
	status = SPI_NRF_RW(reg); 

 	/*读取缓冲区数据*/
	 for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)		  
	   pBuf[byte_cnt] = SPI_NRF_RW(NOP); //从NRF24L01读取数据  

	 /*CSN拉高，完成*/
	NRF_CSN_HIGH();	
		
 	return status;		//返回寄存器状态值
}

/*********************************************************************************
  * @brief     用于向NRF的寄存器中写入一串数据
  * @param   
  *	 @arg       reg : NRF的命令+寄存器地址
  *	 @arg       pBuf：存储了将要写入写寄存器数据的数组，外部定义
  *  @arg       bytes: pBuf的数据长度
  *  @retval    NRF的status寄存器的状态
  *********************************************************************************/
u8 SPI_NRF_WriteBuf(u8 reg ,u8 *pBuf,u8 bytes)
{
	 u8 status,byte_cnt;
	 NRF_CE_LOW();
   	 /*置低CSN，使能SPI传输*/
	 NRF_CSN_LOW();			

	 /*发送寄存器号*/	
  	 status = SPI_NRF_RW(reg); 
 	
  	  /*向缓冲区写入数据*/
	 for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)
		SPI_NRF_RW(*pBuf++);	//写数据到缓冲区 	 
	  	   
	/*CSN拉高，完成*/
	NRF_CSN_HIGH();			
  
  	return (status);	//返回NRF24L01的状态 		
}

/*********************************************************************************
  * @brief  配置并进入接收模式
  * @param  无
  * @retval 无
  *********************************************************************************/
void NRF_RX_Mode(void)

{
	NRF_CE_LOW();	

   SPI_NRF_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址

   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_AA,0x01);    //使能通道0的自动应答    

   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址    

   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_CH,CHANAL);      //设置RF通信频率    

   SPI_NRF_WriteReg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度      

   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0f); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   

   SPI_NRF_WriteReg(NRF_WRITE_REG+CONFIG, 0x0f);  //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 

/*CE拉高，进入接收模式*/	
  NRF_CE_HIGH();

}    

/*********************************************************************************
  * @brief  配置发送模式
  * @param  无
  * @retval 无
  *********************************************************************************/
void NRF_TX_Mode(void)
{  
	NRF_CE_LOW();		

   SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);    //写TX节点地址 

   SPI_NRF_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK   

   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    

   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  

   SPI_NRF_WriteReg(NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次

   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_CH,CHANAL);       //设置RF通道为CHANAL

   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
	
   SPI_NRF_WriteReg(NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发射模式,开启所有中断

/*CE拉高，进入发送模式*/	
  NRF_CE_HIGH();
    Delay(0xffff); //CE要拉高一段时间才进入发送模式
}

/*********************************************************************************
  * @brief  主要用于NRF与MCU是否正常连接
  * @param  无
  * @retval SUCCESS/ERROR 连接正常/连接失败
  *********************************************************************************/
u8 NRF_Check(void)
{
	u8 buf[5]={0xC2,0xC2,0xC2,0xC2,0xC2};
	u8 buf1[5];
	u8 i; 
	 
	/*写入5个字节的地址.  */  
	SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR,buf,5);

	/*读出写入的地址 */
	SPI_NRF_ReadBuf(TX_ADDR,buf1,5); 
	 
	/*比较*/               
	for(i=0;i<5;i++)
	{
		if(buf1[i]!=0xC2)
		break;
	} 
	       
	if(i==5)
		return SUCCESS ;        //MCU与NRF成功连接 
	else
		return ERROR ;        //MCU与NRF不正常连接
}

/*********************************************************************************
  * @brief   用于向NRF的发送缓冲区中写入数据
  * @param   
  *	 @arg     txBuf：存储了将要发送的数据的数组，外部定义	
  * @retval  发送结果，成功返回TXDS,失败返回MAXRT或ERROR
  *********************************************************************************/
u8 NRF_Tx_Dat(u8 *txbuf)
{
	u8 state;  

	 /*ce为低，进入待机模式1*/
	NRF_CE_LOW();

	/*写数据到TX BUF 最大 32个字节*/						
   SPI_NRF_WriteBuf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);

      /*CE为高，txbuf非空，发送数据包 */   
 	 NRF_CE_HIGH();
	  	
	  /*等待发送完成中断 */                            
	while(NRF_Read_IRQ()!=0); 	
	
	/*读取状态寄存器的值 */                              
	state = SPI_NRF_ReadReg(STATUS);

	 /*清除TX_DS或MAX_RT中断标志*/                  
	SPI_NRF_WriteReg(NRF_WRITE_REG+STATUS,state); 	

	SPI_NRF_WriteReg(FLUSH_TX,NOP);    //清除TX FIFO寄存器 

	 /*判断中断类型*/    
	if(state&MAX_RT)                     //达到最大重发次数
			 return MAX_RT; 

	else if(state&TX_DS)                  //发送完成
		 	return TX_DS;
	 else						  
			return ERROR;                 //其他原因发送失败
} 

/*********************************************************************************
  * @brief   用于从NRF的接收缓冲区中读出数据
  * @param   
  *	 @arg     rxBuf ：用于接收该数据的数组，外部定义	
  * @retval 
  *	 @arg    接收结果
  *********************************************************************************/
u8 NRF_Rx_Dat(u8 *rxbuf)
{
	u8 state; 
	NRF_CE_HIGH();	 //进入接收状态
	 /*等待接收中断*/
	while(NRF_Read_IRQ()!=0); 
	
	NRF_CE_LOW();  	 //进入待机状态
	/*读取status寄存器的值  */               
	state=SPI_NRF_ReadReg(STATUS);
	 
	/* 清除中断标志*/      
	SPI_NRF_WriteReg(NRF_WRITE_REG+STATUS,state);

	/*判断是否接收到数据*/
	if(state&RX_DR)                                 //接收到数据
	{
	  SPI_NRF_ReadBuf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
	     SPI_NRF_WriteReg(FLUSH_RX,NOP);          //清除RX FIFO寄存器
	  return RX_DR; 
	}
	else    
		return ERROR;                    //没收到任何数据
}
/*********************************************END OF FILE**********************/
