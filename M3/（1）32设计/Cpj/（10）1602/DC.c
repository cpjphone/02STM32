/*************************************DH11部分*************************************/


#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#define DHT11_Out PAout(1)
#define DHT11_In  PAin(1)

#define DHT11_Out PAout(1)
#define DHT11_In  PAin(1)

typedef unsigned char unint8;
typedef unsigned char unint16;

unsigned char str1[]={"        "};
unsigned char str2[]={"        "};
unsigned char  dis1[] = {"TRH RH: "};
unsigned char  dis2[] = {"TRH  T: "};
unsigned char  aa[] = {"0123456789"};
// sbit TRH = P2^0;// 传感器端口
// sbit ST  = P2^1;//状态显示，方便调试
#define TRH GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)		  //发短信
#define ST GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)

unint8 TH_data,TL_data,RH_data,RL_data,CK_data;
unint8 TH_temp,TL_temp,RH_temp,RL_temp,CK_temp;
unint8 com_data,untemp,temp;
unint8 respond;


/*********************************************************************************/
/*						收发信号检测，读取数据									 */
/*********************************************************************************/
char receive()
{    
          unint8 i;
          //ST=0;
					PAout(2) == 0 ;
          com_data=0;
          for(i=0;i<=7;i++)    
         {
                    respond=2;
                    while((PAin(1) == 0)&&respond++);
                    delay_us(3);	
										//GPIO_SetBits(GPIOA,GPIO_Pin_1);
							
                    if(PAin(1) == 1)
                    {					
                              temp=1;
                              respond=2;
                              while((PAin(1) == 1)&&respond++); 
                    } 
                    else
                    temp=0;
                    com_data<<=1;
                    com_data|=temp;    
          }
          return(com_data);  
}
/****************************************************************************/

/****************************************************************************/
//温度读取函数
//温度低8位== TL_data
//温度高8位== TH_data
//湿度高8位== RH_data
//湿度低8位== RH_data
//校验8位 == CK_data
//调用的函数 delay();, Delay_5us();,RECEIVE(); 
/***************************************************************************/


void read_TRH(void)
{				DHT11_Out = 0;
				delay_ms(18);  //触发开始,总线拉低要大于18ms
  
				DHT11_Out = 1;	//释放总线
				delay_us(25);  //wait 20-40uS 等待DHT11的低电平响应信号
         if(!DHT11_In)   
         {   	
									GPIO_ResetBits(GPIOB,GPIO_Pin_7);
                  respond=2;
                  //判断dh11发送80us信号是否结束
                  while((PAin(1) == 0)&& respond++);
                 respond=2;
                 //判断从机是否发生80us高电平，发出则进入数据接收状态
                while((PAin(1) == 1) && respond++);
               //数据接收状态   
                RH_temp = receive(); 
                RL_temp = receive(); 
                TH_temp = receive(); 
                TL_temp = receive(); 
                CK_temp = receive();
               // TRH=1;ST=1; 
								//GPIO_SetBits(GPIOA,GPIO_Pin_1);
								//GPIO_SetBits(GPIOA,GPIO_Pin_2);
								PAout(1) = 1;PAout(2) = 1;
                 //数据校验
                 untemp=(RH_temp+RL_temp+TH_temp+TL_temp);
                 if(untemp==CK_temp)
                 {
                          RH_data = RH_temp;
                          RL_data = RL_temp;
                          TH_data = TH_temp; 
                          TL_data = TL_temp;
                          CK_data = CK_temp;
                }
        }
         //湿度整数部分
         str1[0] = (char)(0X30+RH_data/10); 
         str1[1] = (char)(0X30+RH_data%10);
         str1[2] =  0x2e; //小数点
          //湿度小数部分
         str1[3] = (char)(0X30+RL_data/10); 
          str1[5] = 0X25; //"%"
         str1[6] = 0X52; //"R"
         str1[7] = 0X48; //"H"
         //温度整数部分
         str2[0] = (char)(0X30+TH_data/10); 
         str2[1] = (char)(0X30+TH_data%10);
         str2[2] =  0x2e; //小数点
         //温度小数部分
          str2[3] = (char)(0X30+TL_data/10); 
          str2[5] = 0X27;  //"'"
          str2[6] = 0X43;  //"C"
}

