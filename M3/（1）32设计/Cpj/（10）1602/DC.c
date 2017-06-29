/*************************************DH11����*************************************/


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
// sbit TRH = P2^0;// �������˿�
// sbit ST  = P2^1;//״̬��ʾ���������
#define TRH GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)		  //������
#define ST GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)

unint8 TH_data,TL_data,RH_data,RL_data,CK_data;
unint8 TH_temp,TL_temp,RH_temp,RL_temp,CK_temp;
unint8 com_data,untemp,temp;
unint8 respond;


/*********************************************************************************/
/*						�շ��źż�⣬��ȡ����									 */
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
//�¶ȶ�ȡ����
//�¶ȵ�8λ== TL_data
//�¶ȸ�8λ== TH_data
//ʪ�ȸ�8λ== RH_data
//ʪ�ȵ�8λ== RH_data
//У��8λ == CK_data
//���õĺ��� delay();, Delay_5us();,RECEIVE(); 
/***************************************************************************/


void read_TRH(void)
{				DHT11_Out = 0;
				delay_ms(18);  //������ʼ,��������Ҫ����18ms
  
				DHT11_Out = 1;	//�ͷ�����
				delay_us(25);  //wait 20-40uS �ȴ�DHT11�ĵ͵�ƽ��Ӧ�ź�
         if(!DHT11_In)   
         {   	
									GPIO_ResetBits(GPIOB,GPIO_Pin_7);
                  respond=2;
                  //�ж�dh11����80us�ź��Ƿ����
                  while((PAin(1) == 0)&& respond++);
                 respond=2;
                 //�жϴӻ��Ƿ���80us�ߵ�ƽ��������������ݽ���״̬
                while((PAin(1) == 1) && respond++);
               //���ݽ���״̬   
                RH_temp = receive(); 
                RL_temp = receive(); 
                TH_temp = receive(); 
                TL_temp = receive(); 
                CK_temp = receive();
               // TRH=1;ST=1; 
								//GPIO_SetBits(GPIOA,GPIO_Pin_1);
								//GPIO_SetBits(GPIOA,GPIO_Pin_2);
								PAout(1) = 1;PAout(2) = 1;
                 //����У��
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
         //ʪ����������
         str1[0] = (char)(0X30+RH_data/10); 
         str1[1] = (char)(0X30+RH_data%10);
         str1[2] =  0x2e; //С����
          //ʪ��С������
         str1[3] = (char)(0X30+RL_data/10); 
          str1[5] = 0X25; //"%"
         str1[6] = 0X52; //"R"
         str1[7] = 0X48; //"H"
         //�¶���������
         str2[0] = (char)(0X30+TH_data/10); 
         str2[1] = (char)(0X30+TH_data%10);
         str2[2] =  0x2e; //С����
         //�¶�С������
          str2[3] = (char)(0X30+TL_data/10); 
          str2[5] = 0X27;  //"'"
          str2[6] = 0X43;  //"C"
}

