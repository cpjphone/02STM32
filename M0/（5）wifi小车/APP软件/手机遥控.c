#include<AT89x51.H>
#define uchar unsigned char
#define uint unsigned int
	#define Left_moto_pwm     P1_6	   //接驱动模块ENA	使能端，输入PWM信号调节速度
	#define Right_moto_pwm    P1_7	   //接驱动模块ENB

    #define you       P1_0	   //四路寻迹模块接口第一路
	#define qian      P1_1	   //四路寻迹模块接口第二路
    #define zuo       P1_2	   //四路寻迹模块接口第三路
	#define hou       P1_3	   //四路寻迹模块接口第四路


	#define Left_moto_go      {P0_0=0,P0_1=1;} //P3_4 P3_5 接IN1  IN2    当 P3_4=0,P3_5=1; 时左电机前进
	#define Left_moto_back    {P0_0=1,P0_1=0;} //P3_4 P3_5 接IN1  IN2    当 P3_4=1,P3_5=0; 时左电机后退    
	#define Left_moto_stp     {P0_0=1,P0_1=1;} //P3_4 P3_5 接IN1  IN2    当 P3_4=1,P3_5=1; 时左电机停转               
	#define Right_moto_go     {P0_2=0,P0_3=1;} //P3_6 P3_7 接IN1  IN2	 当 P3_6=0,P3_7=1; 时右电机前转
	#define Right_moto_back   {P0_2=1,P0_3=0;} //P3_6 P3_7 接IN1  IN2	 当 P3_6=1,P3_7=0; 时右电机后退
    #define Right_moto_stp    {P0_2=1,P0_3=1;} //P3_6 P3_7 接IN1  IN2	 当 P3_6=1,P3_7=0; 时右电机停转



	uchar pwm_val_left  =0;//变量定义
	uchar push_val_left =4;// 左电机占空比N/10
	uchar pwm_val_right =0;
	uchar push_val_right=4;// 右电机占空比N/10
	bit Right_moto_stop=1;
	bit Left_moto_stop =1;
	uchar  time=0;

			void delay1(unsigned int k)	   //延时函数
{    
     uint x,y;
	 for(x=0;x<k;x++) 
	   for(y=0;y<2000;y++);
}
	    void  run(void)	//前进函数
{
//     push_val_left  =5;  
//	 push_val_right =5;	 
	 Left_moto_go ;		 //左电机前进
	 Right_moto_go ;	 //右电机前进
}

/************************************************************************/
     void  back(void)	//HOU函数
{
//     push_val_left  =5;  
//	 push_val_right =5;	 
	 Left_moto_back;		 //左电机前进
	 Right_moto_back;	 //右电机前进
}

/************************************************************************/
     void  left(void)	//左转函数
{
//     push_val_left  =5;  
//	 push_val_right =5;	 
	 Right_moto_go;		//右电机继续
	 Left_moto_back;	 	//左电机停走
}

 /************************************************************************/
     void  right(void)	//右转函数
{
//     push_val_left  =5;  
//	 push_val_right =5;	 
	 Right_moto_back;		//右电机停走
	 Left_moto_go;		//左电机继续
}
 
/************************************************************************/
 	    void  ting(void)	//前进函数
{
//     push_val_left  =5;  
//	 push_val_right =5;	 
	 Left_moto_stp ;		 //左电机前进
	 Right_moto_stp ;	 //右电机前进
}                                                    
/************************************************************************/
/*                    左电机调速                                        */
/*调节push_val_left的值改变电机转速,占空比            */
void pwm_out_left_moto(void)
{  
   if(Left_moto_stop)
   {
    if(pwm_val_left<=push_val_left)
	       {Left_moto_pwm = 1;} 
	else 
	       {Left_moto_pwm = 0;}	
   }
    else  {Left_moto_stp;}
}
/******************************************************************/
/*                    右电机调速                                  */  
   void pwm_out_right_moto(void)
{ 
  if(Right_moto_stop)
   { 
    if(pwm_val_right<=push_val_right)
	       {Right_moto_pwm = 1;} 
	else 
	       {Right_moto_pwm = 0;}	
   }
    else    {Right_moto_stp;}
}
/***************************************************/
///*TIMER0中断服务子函数产生PWM信号*/
 	void timer0()interrupt 1   using 2
{
     TH0=0Xfc;	  //1Ms定时11111000
	 TL0=0X18;	   //00110000
	 time++;
	 pwm_val_left++;
	 pwm_val_right++;
	 if(pwm_val_left>=10)
	 pwm_val_left=0;
	 if(pwm_val_right>=10)
	 pwm_val_right=0;

	 pwm_out_left_moto();
	 pwm_out_right_moto();
 	
 }	
 /***************************************************/
   uchar flag,a,b,c;
void delay(uint z)
{
  	uint d,e;
	for(d=z;d>0;d--)
	    for(e=110;e>0;e--);
}
void init()
{
	TMOD=0X21;
	TH1=0Xfd;
	TL1=0xfd;
	TR1=1;
	SM0=0;
	SM1=1;
	REN=1;
	TH0= 0Xfc;		  //1ms定时
 	TL0= 0X18;		  //00110000+
	TR0= 1;
	ET0= 1;
	EA=1;
	ES=1;
}
	void main(void)
{
	int x;


	
    
	init();
	while(1)
	{
	if(~qian)
		{
			for(x=0;x<30;x++) 
			back();

		}
		if(~hou)
		{
			for(x=0;x<30;x++) 
			run();

		}
		if(~zuo)
		{
			for(x=0;x<30;x++)
			right();
		}
		if(~you)
		{		 
			for(x=0;x<30;x++)
			left();
		}
		else
		if(flag==1)
		{

		ES=0;
		SBUF=a;
		while(!TI);
		TI=0;
		  c=a;
//		   b=a;
//		   x=(b&0xf0);					                                                                                                                                                                                                                                                                                                                                          
		       if(c==0x01)
			   	     
			       P0=0xaa;
			   if(c==0x02)	    
			       P0=0x55;	
			   if(c==0x03)	     
			       P0=0x06;	
			   if(c==0x04)	      
			       P0=0x09;	
 			   if(c==0x05)	      
			       P0=0xff;	
				    delay(500);

			   if(c==0x06)
			       {
				   P0=0xaa;
				   delay(500);
				   P0=0xff;
				   }
			   if(c==0x07)	    
			       {
				   P0=0x55;
				   	delay(500);
				   P0=0xff;
				   }	
			   if(c==0x08)	     
			       {
				   P0=0x06;
				   	delay(500);
				   P0=0xff;
				   }
			   if(c==0x09)	      
			       {
				   P0=0x09;
				   delay(500);	 
				   P0=0xff;
				   }
			
			  ES=1;
			  	flag=0;
				}
				else
		   ting();
			
	}
}
void ser() interrupt 4
{
	RI=0;
	a=SBUF;
	flag=1;
}