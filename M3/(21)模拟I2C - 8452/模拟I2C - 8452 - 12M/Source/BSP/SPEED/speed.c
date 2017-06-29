
/**
  ******************************************************************************
  * @file     speed.c
  * @author   Phone
  * @version  V1.0
  * @date      2014/10/14
  * @brief    �������ú���
  ******************************************************************************
  * @attention
  *0db����,2Mbps,���������濪�� 
  * CPJ���ܿ���
  * @endverbatim
  ******************************************************************************
  * @attention
  * M3ϵ�е�Ƭ��SPI�˿ڱ���һ������ʼ��
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
//#include "usart.h"
#include "speed.h"

/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define GPIO_PORT_I2C	GPIOB			/* GPIO�˿� */
#define RCC_I2C_PORT 	RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define I2C_SCL_PIN		GPIO_Pin_6			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define I2C_SDA_PIN		GPIO_Pin_7			/* ���ӵ�SDA�����ߵ�GPIO */

/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 0	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */
	#define I2C_SCL_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 1 */
	#define I2C_SCL_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 0 */
	
	#define I2C_SDA_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 1 */
	#define I2C_SDA_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 0 */
	
	#define I2C_SDA_READ()  GPIO_ReadInputDataBit(GPIO_PORT_I2C, I2C_SDA_PIN)	/* ��SDA����״̬ */
#else	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
    /*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
	#define I2C_SCL_1()  GPIO_PORT_I2C->BSRR = I2C_SCL_PIN				/* SCL = 1 */
	#define I2C_SCL_0()  GPIO_PORT_I2C->BRR = I2C_SCL_PIN				/* SCL = 0 */
	
	#define I2C_SDA_1()  GPIO_PORT_I2C->BSRR = I2C_SDA_PIN				/* SDA = 1 */
	#define I2C_SDA_0()  GPIO_PORT_I2C->BRR = I2C_SDA_PIN				/* SDA = 0 */
	
	#define I2C_SDA_READ()  ((GPIO_PORT_I2C->IDR & I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */
#endif

#define	SlaveAddress   0x38	//����������IIC�����еĴӵ�ַ,����SA0��ַ���Ų�ͬ�޸�
#define   uchar unsigned char
#define   uint unsigned int	

char BUF2[21]; 

signed char BUF[8];   
uchar ge,shi,bai,qian,wan;           //��ʾ����
int  dis_data;                       //����
int x,y,z;


/**-------------------------------------------------------------------------------
  * @brief      5ms��ʱ
  * @param    		��
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static void Delay5ms()
{
    unsigned short n = 3360;
	
    while (n--);
}

/**-------------------------------------------------------------------------------
  * @brief      ���ݴ���
  * @param    		��
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static void conversion(uint temp_data)  
{  
    wan=temp_data/10000+0x30 ;
    temp_data=temp_data%10000;   //ȡ������
		qian=temp_data/1000+0x30 ;
    temp_data=temp_data%1000;    //ȡ������
    bai=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //ȡ������
    shi=temp_data/10+0x30    ;
    temp_data=temp_data%10;      //ȡ������
    ge=temp_data+0x30; 	
}

/**-------------------------------------------------------------------------------
  * @brief      ��ʱ5΢������
  * @param    		��
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*��
	 	�����ʱ����ͨ��������AX-Pro�߼������ǲ��Եõ��ġ�

		CPU��Ƶ72MHzʱ�����ڲ�Flash����, MDK���̲��Ż�
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
        
    IAR���̱���Ч�ʸߣ���������Ϊ7
	*/
	for (i = 0; i < 20; i++);
}

/**-------------------------------------------------------------------------------
  * @brief      ��ʼʱ��
  * @param    		��
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static void i2c_Start(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
	I2C_SDA_1();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}

/**-------------------------------------------------------------------------------
  * @brief      ֹͣʱ��
  * @param    		��
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static void i2c_Stop(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
	i2c_Delay();
}

/**-------------------------------------------------------------------------------
  * @brief      ����Ӧ��ɹ�
  * @param    		��
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static void i2c_Ack(void)
{
	I2C_SDA_0();	/* CPU����SDA = 0 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
}

/**-------------------------------------------------------------------------------
  * @brief      ����ʧ��Ӧ���ź�
  * @param    		��
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static void i2c_NAck(void)
{
	I2C_SDA_1();	/* CPU����SDA = 1 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();	
}

/**-------------------------------------------------------------------------------
  * @brief      �ȴ�Ӧ���źţ����Ӧ���źţ�
  * @param    		��
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	i2c_Delay();
	if (I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_0();
	i2c_Delay();
	return re;
}

/**-------------------------------------------------------------------------------
  * @brief     ������I2C���߷���1�ֽ�����
  * @param    		��
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;
	/* �ȷ����ֽڵĸ�λbit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		i2c_Delay();
		I2C_SCL_1();
		i2c_Delay();	
		I2C_SCL_0();
//		if (i == 7)
//		{
//			 I2C_SDA_1(); // �ͷ�����
//		}
		_ucByte <<= 1;	/* ����һ��bit */
		i2c_Delay();
	}
	i2c_WaitAck();
}

/**-------------------------------------------------------------------------------
  * @brief      ���ش�I2C����1�ֽ�����
  * @param    		��
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
	value = 0;
	I2C_SDA_1();
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_1();
		i2c_Delay();
//		if (I2C_SDA_READ())
//		{
//			value++;
//		}
		value |=I2C_SDA_READ();
		I2C_SCL_0();
		i2c_Delay();
	}
	return value;
}

/**-------------------------------------------------------------------------------
  * @brief      ��MMA8452��Ӧ��ַд����
  * @param    		��
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static void Single_Write_MMA8452(uchar REG_Address,uchar REG_data)
{
		i2c_Start();                  //��ʼ�ź�
		i2c_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
		i2c_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ
		i2c_SendByte(REG_data);       //�ڲ��Ĵ�������
		i2c_Stop();                   //����ֹͣ�ź�
}

/**-------------------------------------------------------------------------------
  * @brief      ��ȡMMA8452����
  * @param    		��
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static void Multiple_read_MMA8452(void)
{   
		uchar i;
    i2c_Start();                          //��ʼ�ź�
    i2c_SendByte(SlaveAddress);           //�����豸��ַ+д�ź�
    i2c_SendByte(0x01);                   //���ʹ洢��Ԫ��ַ����0x01��ʼ	
    i2c_Start();                          //��ʼ�ź�
    i2c_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
	 for (i=0; i<6; i++)                      //������ȡ6����ַ���ݣ��洢��BUF
    {
        BUF[i] = i2c_ReadByte();          //BUF[0]�洢0x32��ַ�е�����
        if (i == 5)
        {
          i2c_NAck();                //���һ��������Ҫ��NOACK
        }
        else
        {
          i2c_Ack();                //��ӦACK
        }
   }
    i2c_Stop();                          //ֹͣ�ź�
    Delay5ms();
}

/**-------------------------------------------------------------------------------
  * @brief      ��ʾx��
  * @retval     null
--------------------------------------------------------------------------------*/
static void display_x()
{   float temp;
    //SeriPushSend('X');
		BUF2[0] = 'X';
    dis_data=(BUF[0]<<8)|BUF[1];  //�ϳ�����   
	dis_data>>=4;
	x=dis_data;
	if(dis_data<0){
  	     dis_data=-dis_data;
	     //SeriPushSend('-');
			BUF2[1] = '-';
	}
	else //SeriPushSend('+');
		BUF2[1] = '+';

    temp=(float)dis_data;      //�������ݺ���ʾ
    conversion(temp);          //ת������ʾ��Ҫ������

    //SeriPushSend(':');
	BUF2[2] = ':';
   // SeriPushSend(qian);
	BUF2[3] = qian;
	//SeriPushSend('.');
	BUF2[4] = '.';
	//SeriPushSend(bai);
	BUF2[5] = bai;
	//SeriPushSend(shi);
	BUF2[6] = shi;
}

/**-------------------------------------------------------------------------------
  * @brief      ��ʾy��
  * @retval     null
--------------------------------------------------------------------------------*/
static void display_y()
{   float temp;
    //SeriPushSend('Y');
		BUF2[7] = 'Y';
    dis_data=(BUF[2]<<8)|BUF[3];  //�ϳ�����   
	dis_data>>=4;
	y=dis_data;
	if(dis_data<0){
	dis_data=-dis_data;
    //SeriPushSend('-');
		BUF2[8] = '-';
	}
	else //SeriPushSend('+');
	BUF2[8] = '+';

    temp=(float)dis_data;      //�������ݺ���ʾ
    conversion(temp);          //ת������ʾ��Ҫ������

	//SeriPushSend(':');
	BUF2[9] = ':';
	//SeriPushSend(qian);
	BUF2[10] = qian;
	//SeriPushSend('.');
	BUF2[11] = '.';
	//SeriPushSend(bai);
	BUF2[12] = bai;
	//SeriPushSend(shi);
	BUF2[13] = shi;
}

/**-------------------------------------------------------------------------------
  * @brief      ��ʾz��
  * @retval     null
--------------------------------------------------------------------------------*/
static void display_z()
{   float temp;
    //SeriPushSend('Z');
	BUF2[14] = 'Z';
    dis_data=(BUF[4]<<8)|BUF[5];    //�ϳ�����  
    dis_data>>=4;
	z=dis_data;
	if(dis_data<0){
	dis_data=-dis_data;
    //SeriPushSend('-');
		BUF2[15] = '-';
	}
	else //SeriPushSend('+');
	BUF2[15] = '+';

    temp=(float)dis_data;      //�������ݺ���ʾ
    conversion(temp);          //ת������ʾ��Ҫ������
    
	//SeriPushSend(':');
	BUF2[16] = ':';
	//SeriPushSend(qian);
	BUF2[17] = qian;
	//SeriPushSend('.');
	BUF2[18] = '.';
	//SeriPushSend(bai);
	BUF2[19] = bai;
	//SeriPushSend(shi);
	BUF2[20] = shi;
}

/**-------------------------------------------------------------------------------
  * @brief      ��ʼ��MMA8452��������Ҫ��ο�pdf�����޸�************************
  * @param    		����Ӧ�ò�
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
void Init_MMA8452()
{
   Single_Write_MMA8452(0x2A,0x01);   //
   Single_Write_MMA8452(0x2B,0x02);   //
}


/*********************************************************************************
  * @brief   I2C�˿�����
  * @param   ��
  * @retval    ��
*********************************************************************************/
void i2c_CfgGpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	/* ��GPIOʱ�� */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* ��©��� */
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
	//i2c_Stop();
}

/**-------------------------------------------------------------------------------
  * @brief      ���⺯��
  * @param    		��
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
void speed_open(void)
{
	Multiple_read_MMA8452();       //�����������ݣ��洢��BUF�� ÿһ��ѭ����ֻ�ܶ�ȡX.Y.Z���е�һ��

//	SeriPushSend(0x0d);
//	SeriPushSend(0x0a);

	display_x();                   //---------��ʾX��
	display_y();                   //---------��ʾY��
	display_z();                   //---------��ʾZ��

}




/*********************************************END OF FILE**********************/





















