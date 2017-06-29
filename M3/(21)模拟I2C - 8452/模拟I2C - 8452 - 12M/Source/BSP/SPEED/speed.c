
/**
  ******************************************************************************
  * @file     speed.c
  * @author   Phone
  * @version  V1.0
  * @date      2014/10/14
  * @brief    外设配置函数
  ******************************************************************************
  * @attention
  *0db增益,2Mbps,低噪声增益开启 
  * CPJ智能控制
  * @endverbatim
  ******************************************************************************
  * @attention
  * M3系列单片机SPI端口必须一个个初始化
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
//#include "usart.h"
#include "speed.h"

/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define GPIO_PORT_I2C	GPIOB			/* GPIO端口 */
#define RCC_I2C_PORT 	RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define I2C_SCL_PIN		GPIO_Pin_6			/* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN		GPIO_Pin_7			/* 连接到SDA数据线的GPIO */

/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
#if 0	/* 条件编译： 1 选择GPIO的库函数实现IO读写 */
	#define I2C_SCL_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 1 */
	#define I2C_SCL_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 0 */
	
	#define I2C_SDA_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 1 */
	#define I2C_SDA_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 0 */
	
	#define I2C_SDA_READ()  GPIO_ReadInputDataBit(GPIO_PORT_I2C, I2C_SDA_PIN)	/* 读SDA口线状态 */
#else	/* 这个分支选择直接寄存器操作实现IO读写 */
    /*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
	#define I2C_SCL_1()  GPIO_PORT_I2C->BSRR = I2C_SCL_PIN				/* SCL = 1 */
	#define I2C_SCL_0()  GPIO_PORT_I2C->BRR = I2C_SCL_PIN				/* SCL = 0 */
	
	#define I2C_SDA_1()  GPIO_PORT_I2C->BSRR = I2C_SDA_PIN				/* SDA = 1 */
	#define I2C_SDA_0()  GPIO_PORT_I2C->BRR = I2C_SDA_PIN				/* SDA = 0 */
	
	#define I2C_SDA_READ()  ((GPIO_PORT_I2C->IDR & I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
#endif

#define	SlaveAddress   0x38	//定义器件在IIC总线中的从地址,根据SA0地址引脚不同修改
#define   uchar unsigned char
#define   uint unsigned int	

char BUF2[21]; 

signed char BUF[8];   
uchar ge,shi,bai,qian,wan;           //显示变量
int  dis_data;                       //变量
int x,y,z;


/**-------------------------------------------------------------------------------
  * @brief      5ms延时
  * @param    		无
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static void Delay5ms()
{
    unsigned short n = 3360;
	
    while (n--);
}

/**-------------------------------------------------------------------------------
  * @brief      数据处理
  * @param    		无
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static void conversion(uint temp_data)  
{  
    wan=temp_data/10000+0x30 ;
    temp_data=temp_data%10000;   //取余运算
		qian=temp_data/1000+0x30 ;
    temp_data=temp_data%1000;    //取余运算
    bai=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //取余运算
    shi=temp_data/10+0x30    ;
    temp_data=temp_data%10;      //取余运算
    ge=temp_data+0x30; 	
}

/**-------------------------------------------------------------------------------
  * @brief      延时5微秒左右
  * @param    		无
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*　
	 	下面的时间是通过安富莱AX-Pro逻辑分析仪测试得到的。

		CPU主频72MHz时，在内部Flash运行, MDK工程不优化
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
        
    IAR工程编译效率高，不能设置为7
	*/
	for (i = 0; i < 20; i++);
}

/**-------------------------------------------------------------------------------
  * @brief      起始时序
  * @param    		无
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static void i2c_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	I2C_SDA_1();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}

/**-------------------------------------------------------------------------------
  * @brief      停止时序
  * @param    		无
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static void i2c_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
	i2c_Delay();
}

/**-------------------------------------------------------------------------------
  * @brief      发送应答成功
  * @param    		无
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static void i2c_Ack(void)
{
	I2C_SDA_0();	/* CPU驱动SDA = 0 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();	/* CPU释放SDA总线 */
}

/**-------------------------------------------------------------------------------
  * @brief      发送失败应答信号
  * @param    		无
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static void i2c_NAck(void)
{
	I2C_SDA_1();	/* CPU驱动SDA = 1 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();	
}

/**-------------------------------------------------------------------------------
  * @brief      等待应答信号（检测应答信号）
  * @param    		无
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	I2C_SDA_1();	/* CPU释放SDA总线 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	i2c_Delay();
	if (I2C_SDA_READ())	/* CPU读取SDA口线状态 */
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
  * @brief     主控向I2C总线发送1字节数据
  * @param    		无
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;
	/* 先发送字节的高位bit7 */
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
//			 I2C_SDA_1(); // 释放总线
//		}
		_ucByte <<= 1;	/* 左移一个bit */
		i2c_Delay();
	}
	i2c_WaitAck();
}

/**-------------------------------------------------------------------------------
  * @brief      主控从I2C接受1字节数据
  * @param    		无
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
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
  * @brief      向MMA8452对应地址写数据
  * @param    		无
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static void Single_Write_MMA8452(uchar REG_Address,uchar REG_data)
{
		i2c_Start();                  //起始信号
		i2c_SendByte(SlaveAddress);   //发送设备地址+写信号
		i2c_SendByte(REG_Address);    //内部寄存器地址
		i2c_SendByte(REG_data);       //内部寄存器数据
		i2c_Stop();                   //发送停止信号
}

/**-------------------------------------------------------------------------------
  * @brief      读取MMA8452数据
  * @param    		无
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
static void Multiple_read_MMA8452(void)
{   
		uchar i;
    i2c_Start();                          //起始信号
    i2c_SendByte(SlaveAddress);           //发送设备地址+写信号
    i2c_SendByte(0x01);                   //发送存储单元地址，从0x01开始	
    i2c_Start();                          //起始信号
    i2c_SendByte(SlaveAddress+1);         //发送设备地址+读信号
	 for (i=0; i<6; i++)                      //连续读取6个地址数据，存储中BUF
    {
        BUF[i] = i2c_ReadByte();          //BUF[0]存储0x32地址中的数据
        if (i == 5)
        {
          i2c_NAck();                //最后一个数据需要回NOACK
        }
        else
        {
          i2c_Ack();                //回应ACK
        }
   }
    i2c_Stop();                          //停止信号
    Delay5ms();
}

/**-------------------------------------------------------------------------------
  * @brief      显示x轴
  * @retval     null
--------------------------------------------------------------------------------*/
static void display_x()
{   float temp;
    //SeriPushSend('X');
		BUF2[0] = 'X';
    dis_data=(BUF[0]<<8)|BUF[1];  //合成数据   
	dis_data>>=4;
	x=dis_data;
	if(dis_data<0){
  	     dis_data=-dis_data;
	     //SeriPushSend('-');
			BUF2[1] = '-';
	}
	else //SeriPushSend('+');
		BUF2[1] = '+';

    temp=(float)dis_data;      //计算数据和显示
    conversion(temp);          //转换出显示需要的数据

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
  * @brief      显示y轴
  * @retval     null
--------------------------------------------------------------------------------*/
static void display_y()
{   float temp;
    //SeriPushSend('Y');
		BUF2[7] = 'Y';
    dis_data=(BUF[2]<<8)|BUF[3];  //合成数据   
	dis_data>>=4;
	y=dis_data;
	if(dis_data<0){
	dis_data=-dis_data;
    //SeriPushSend('-');
		BUF2[8] = '-';
	}
	else //SeriPushSend('+');
	BUF2[8] = '+';

    temp=(float)dis_data;      //计算数据和显示
    conversion(temp);          //转换出显示需要的数据

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
  * @brief      显示z轴
  * @retval     null
--------------------------------------------------------------------------------*/
static void display_z()
{   float temp;
    //SeriPushSend('Z');
	BUF2[14] = 'Z';
    dis_data=(BUF[4]<<8)|BUF[5];    //合成数据  
    dis_data>>=4;
	z=dis_data;
	if(dis_data<0){
	dis_data=-dis_data;
    //SeriPushSend('-');
		BUF2[15] = '-';
	}
	else //SeriPushSend('+');
	BUF2[15] = '+';

    temp=(float)dis_data;      //计算数据和显示
    conversion(temp);          //转换出显示需要的数据
    
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
  * @brief      初始化MMA8452，根据需要请参考pdf进行修改************************
  * @param    		对外应用层
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
void Init_MMA8452()
{
   Single_Write_MMA8452(0x2A,0x01);   //
   Single_Write_MMA8452(0x2B,0x02);   //
}


/*********************************************************************************
  * @brief   I2C端口配置
  * @param   无
  * @retval    无
*********************************************************************************/
void i2c_CfgGpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	/* 打开GPIO时钟 */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* 开漏输出 */
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	//i2c_Stop();
}

/**-------------------------------------------------------------------------------
  * @brief      对外函数
  * @param    		无
  *	@arg         NULL
  * @retval     null
--------------------------------------------------------------------------------*/
void speed_open(void)
{
	Multiple_read_MMA8452();       //连续读出数据，存储在BUF中 每一次循环，只能读取X.Y.Z其中的一个

//	SeriPushSend(0x0d);
//	SeriPushSend(0x0a);

	display_x();                   //---------显示X轴
	display_y();                   //---------显示Y轴
	display_z();                   //---------显示Z轴

}




/*********************************************END OF FILE**********************/





















