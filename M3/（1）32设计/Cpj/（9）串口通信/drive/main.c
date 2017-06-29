/*********************************************************************************      
* 【语    言】：C语言
* 【编译环境】：Keil4  
* 【程序功能】: 串口2 —— GSM 打电话 发短信  收电话
* 【技术支持】：				
* 【晶    振】: RCC 8*9=72MHz		
* 【芯    片】: stm32f103VE 
* 【日    期】: 2014/03/25
* 【修改日期】: 2014/03/25
* 【作    者】: Phone
* 【编程理念】: 死东西活用，活东西套用。
*********************************************************************************/

#include "stm32f10x.h"
//#include "platform_config.h"
#include "stm32f10x_usart.h"
//#include "misc.h"
#include <string.h>

#define uint unsigned int
#define uchar unsigned char

#define Send_ENGLISH_key GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)		  //发短信
#define Send_CALLING_key GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)		  //打电话

#define MAXCHAR 81
char  aa[MAXCHAR];

uchar ATE0[]="ATE0\r\n";
uchar CREG_CMD[]="AT+CREG?\r\n";
uchar SMS_send[]="AT+CMGS=18\r\n";
uchar ATCN[]="AT+CNMI=2,1\r\n";
uchar CMGF0[]="AT+CMGF=0\r\n";
uchar CMGF1[]="AT+CMGF=1\r\n";
uchar CMGR[12]="AT+CMGR=1\r\n";
uchar CMGD[12]="AT+CMGD=1\r\n";


/****************亚飞**********************/
#define CALL_YOU_TELNUMBER "ATD15238643714;\r\n"      //填入你手上的手机号码 不是板子的
#define SEND_MESSA_TO_YOUR "at+cmgs=\"15238643714\"\r\n"    //填入你手上的手机号码 不是板子的
#define SEND_MESSA_CONTENT "tpytongxin.taobao.com"
#define CALL_YOU_TELNUMBER "ATD15238643714;\r\n"    //填入你手上的手机号码 不是板子的
//#define CALL_YOU_TELNUMBER "ATD15238643714;\r\n"    //填入你手上的手机号码 不是板子的

uchar  Sms2_Pdu[]="0891683108200205F011000B815132683417F40008A704521B601D";   //亚飞
uchar tab[] = "00.0";

uchar a,j=0,flag=0;
int g_timeout=0;
	GPIO_InitTypeDef GPIO_InitStructure;

/************************************************************************/
//函数声明
/************************************************************************/
void USART_Config(u32 baud);
void Print_Str(u8* buf);
u8 Print_Char(u8 ch);
void Delay(u32 nCount);
void NVIC_Configuration(void);

/************************************************************************/
//CPU延时函数
/************************************************************************/
void delay(uint ms)// 延时子程序
{
    uchar i,h;
    while(ms--)
    {
        for(i=0;i<120;i++)
				{
					h++;
					if(h > 3)h = 0;
				}
    }
}

void clearBuff(void)
{
    for(j=0;j<MAXCHAR;j++)
    {
        aa[j]=0x00;
    }
    j=0;
}

int Hand()
{
    delay(10);
    if(strstr(aa,"OK")!=NULL)             //检测单片机和模块的连接
    {
        g_timeout=0;
        return 1;
    }
    if(g_timeout>50)
    {
        g_timeout=0;
        return -1;
    }
    g_timeout++;
    return 0;
}

void AT(void)
{
    clearBuff();
    Print_Str(ATE0);
    delay(50);
	  
    while(strstr(aa,"OK")==NULL)
    {
        delay(50);
       // led(0x01);
        //v_Lcd12864PutString_f( 1,1, "模块连接【..】") ;
        clearBuff();
        Print_Str(ATE0);
			  //GPIO_SetBits(GPIOA,GPIO_Pin_11);
    }
		
    //led(0x02);
		//GPIO_ResetBits(GPIOA,GPIO_Pin_11);
   // v_Lcd12864PutString_f( 1,1, "模块连接【OK】") ;
	
    Print_Str(ATCN);
    delay(50);


    while(1)
    {
        clearBuff();
        Print_Str(CREG_CMD);
        delay(50);
        if(((aa[9]=='0')&&(aa[11]=='1'))||((aa[9]=='0')&&(aa[11]=='5')))
        {
            clearBuff();
					  //GPIO_SetBits(GPIOA,GPIO_Pin_11);
					  //GPIO_ResetBits(GPIOA,GPIO_Pin_11);
           // v_Lcd12864PutString_f( 1,1, "模块注册【OK】") ;
          //  led(0x08);
            break;
        }
        else
        {
            clearBuff();
           // v_Lcd12864PutString_f( 1,1, "模块注册【..】") ;
           // led(0x04);
            delay(50);
        }
    }
}

void send_english()
{
    //P2=0XFF;
    clearBuff();

    Print_Str(CMGF1);
    delay(100);
    //led(0x01);


    Print_Str(SEND_MESSA_TO_YOUR);

    delay(300);
    //led(0x02);

    Print_Str(tab);  //发短信内容
    delay(10);
    Print_Char(0x1A);      //发送结束符号

    delay(10);
   // led(0x04);
    clearBuff();
}

void send_call()
{
    clearBuff();
    delay(100);
    Print_Str(CALL_YOU_TELNUMBER); //打电话
    clearBuff();
    delay(300);
}

void send_18B20()
{

}

void send_pdu()
{
    clearBuff();
    Print_Str(CMGF0);
    delay(100);
   // led(0x01);

    Print_Str(SMS_send);  //发送中文短信

    delay(500);
   // led(0x02);

    Print_Str(Sms2_Pdu);  //发短信内容
    Print_Char(0x1A);      //发送结束符号

    delay(500);
    //led(0x04);
    clearBuff();
}


/************************************************************************/
//（活东西）主函数
/************************************************************************/
int main(void)
{	
  USART_Config(9600);  //配置系统时钟
	NVIC_Configuration();	 //NVIC配置
	GPIO_Write(GPIOB,0xffff);
	delay(10);   
	GPIO_SetBits(GPIOA,GPIO_Pin_1);		//发英文TEXT短信按键
	GPIO_SetBits(GPIOA,GPIO_Pin_4);		//打电话按键  
	//GPIO_SetBits(GPIOA,GPIO_Pin_11);		//打电话按键 
	clearBuff();
	AT();		   //初始化模块
	Print_Str(CMGD);
	delay(50);
	while(Hand()==0);
	clearBuff();

	Print_Str(CMGF1);
	delay(30);

	Print_Str(ATCN);
	delay(30);
	send_call();
	while(1)
	{send_call();
	////////////////////////////////////////////////////////////////////////
	if(Send_ENGLISH_key==0)     //发英文TEXT短信按键
	{   
		delay(30);
		if(Send_ENGLISH_key==0)  
		{
			//v_Lcd12864PutString_f( 1,1, "英文短信【..】") ;
			send_english();
			//v_Lcd12864PutString_f( 1,1, "英文短信【OK】") ;
		}

	}
	else if(Send_CALLING_key==0)     //打电话按键
	{	
		delay(30);
		if(Send_CALLING_key==0)
		{
			//v_Lcd12864PutString_f( 1,1, "给你电话【..】") ;
			send_call();
			//v_Lcd12864PutString_f( 1,1, "给你电话【OK】") ;
		}
	}
// 	else if(Send_SMS_PDU_key==0)
// 	{

// 	}

		////////////////////////////////////////////////////////////////////////

		//        +CMTI: "SM",1


		if(strstr(aa,"RING")!=NULL)
		{
			//FENGMINGQI=0;
			delay(2);
	//		v_Lcd12864PutString_f( 1,1, "电话来了【亲】") ;
			clearBuff();
		}
		else
		{
			//FENGMINGQI=1;
		}
	}

}

/************************************************************************/
//（活东西）串口中断函数（接收）
/************************************************************************/
void USART2_IRQHandler(void)
{
	//GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	GPIO_SetBits(GPIOA,GPIO_Pin_11);	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)    //若接收数据寄存器满
  {     
    aa[j] = USART_ReceiveData(USART2);  
		j++;
   // USART_ITConfig(USART2, USART_IT_TXE, DISABLE);                                   			   
  }
	
}



