 
 /*********************************************************************************      
* ����        �ԡ���C����
* �����뻷������Keil4  
* �������ܡ���
* ������֧�֡���Cortex-M0
* ����        ��: RCC 8*6=48MHz		
* ��о        Ƭ��: stm32f030f4
* ����        �ڡ�: 2014/07/29
* ���޸����ڡ�: 
* ����        �ߡ�: Phone
* ��������: ���������ã�������á�
*********************************************************************************/

#include <stm32f0xx.h>
#include "includes.h"
#include <ctype.h>

//�����͵��ź�
#define    foodsign1   6
#define    foodsign2   8
/*
 * PA2  -  ceʹ��
 * PA3  -  csƬѡ
 * PA0  -  irq�ж�
 */
u8 status;		 //�����жϽ���/����״̬
u8 flag;
//u8 txbuf[4];	    //���ͻ���
u8 rxbuf[8];	 //���ջ���



void dump_buffer(unsigned char *buf, int buflen);
/************************************************************************/
//������
/************************************************************************/
int main(void)
{
    u8 i,j,k ;  
    ALL_Config();
    status = NRF_Check(); 
    NRF_RX_Mode();
    LED_OFF();
    while(1)
    {  		 	
        /*�ȴ���������*/
        j = 0;
        k = 0;
        status = NRF_Rx_Dat(rxbuf);
        for(i=0;i<8;i++)
        {
            if(rxbuf[i] == foodsign1)
                j++;
            if(rxbuf[i] == foodsign2)
                k++;
            if(j&k)
            {
                flag = 2;
                break;
            }            
        }
        if(flag == 2)
        {
           LED_ON();
        }
    }
}





















          
