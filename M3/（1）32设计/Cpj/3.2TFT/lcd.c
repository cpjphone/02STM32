#include "stm32f10x.h"
#include"LCD_driver.h"
#include"chinese.h"
#include"char.h"
u16 colors[]=
{
  0xf800,0x07e0,0x001f,0xffe0,0x0000,0x07ff,0xf81f,0xffff  //颜色数据
};
void delay_ms(int count);
void LCD_Write(u8 type, u16 value)

 {
   LCD_CS(0);
   LCD_RS(type);
   LCD_WR(0);

    GPIOB->BSRR = value & 0xff00;   
    GPIOB->BRR  = (~value) & 0xff00;
   LCD_WR(1);
  Delay(100);
   LCD_WR(0);
  	GPIOB->BSRR = (value)<<8 & 0xff00;   
    GPIOB->BRR  = ((~value)<<8) & 0xff00;
   LCD_WR(1);
   Delay(100);
   LCD_CS(1);
  
 }

void LCD_Write_Data16(u16 value)
{	 
    LCD_CS(0);
	LCD_RS(1);
	LCD_WR(0);
    GPIOB->BSRR = value & 0xff00;   
    GPIOB->BRR  = (~value) & 0xff00;
    LCD_WR(1);
	Delay(5);
    LCD_WR(0);
	GPIOB->BSRR = (value)<<8 & 0xff00;   
    GPIOB->BRR  = ((~value)<<8) & 0xff00;
    LCD_WR(1);
	Delay(5);
    LCD_CS(1);

}

void Reg_Write(u16 reg,u16 value)
{
 LCD_Write(TYPE_LCD_COMMAND,reg);     //这里的TYPE_LCD_COMMAND，指的是发送命令，查阅液晶屏的资料
 LCD_Write_Data16(value);
 
}
void LCD_SetRamAddr(uint xStart, uint xEnd, uint yStart, uint yEnd) 
{

	Reg_Write(0x200, xStart); 
    Reg_Write(0x201, yStart); 

    Reg_Write(0x0210, xStart);
    Reg_Write(0x0212,yStart);
    
    Reg_Write(0x211,xEnd);
    Reg_Write(0x213,yEnd);

	LCD_Write(TYPE_LCD_COMMAND,0x0202);
}
void LCD_Init(void)
{

    LCD_RST(1);
    delay_ms(5);	
	LCD_RST(0);
	delay_ms(5);
	LCD_RST(1);
	delay_ms(5);
	LCD_CS(0);  //打开片选使能

    Reg_Write(0x000,0x0000);delay_ms(5);
	Reg_Write(0x000,0x0000);delay_ms(5);
	Reg_Write(0x000,0x0000);delay_ms(5);
	Reg_Write(0x000,0x0000);delay_ms(5);
	delay_ms(100);
    LCD_Write_Data16(0x0000);delay_ms(5);
    LCD_Write_Data16(0x0000);delay_ms(5);	
    LCD_Write_Data16(0x0000);delay_ms(5);	
    LCD_Write_Data16(0x0000);delay_ms(5);
  	delay_ms(100);

    Reg_Write(0x400,0x6200);delay_ms(5);
 	Reg_Write(0x008,0x0808);delay_ms(5);

	Reg_Write(0x300,0x0c0c);delay_ms(5);//GAMMA
	Reg_Write(0x301,0xff13);delay_ms(5);
	Reg_Write(0x302,0x0f0f);delay_ms(5);
	Reg_Write(0x303,0x150b);delay_ms(5);

	Reg_Write(0x304,0x1020);delay_ms(5);

	Reg_Write(0x305,0x0a0b);delay_ms(5);
	Reg_Write(0x306,0x0003);delay_ms(5);	
	Reg_Write(0x307,0x0d06);delay_ms(5);
	Reg_Write(0x308,0x0504);delay_ms(5); 
	Reg_Write(0x309,0x1030);delay_ms(5);

	Reg_Write(0x010,0x001b);delay_ms(5);   //60Hz
 	Reg_Write(0x011,0x0101);delay_ms(5);
	Reg_Write(0x012,0x0000);delay_ms(5);
	Reg_Write(0x013,0x0001);delay_ms(5);

	Reg_Write(0x100,0x0330);delay_ms(5);//BT,AP 0x0330
	Reg_Write(0x101,0x0247);delay_ms(5);//DC0,DC1,VC
	Reg_Write(0x103,0x1000);delay_ms(5);//VDV	//0x0f00
	Reg_Write(0x280,0xbf00);delay_ms(5);//VCM
	Reg_Write(0x102,0xd1b0);delay_ms(5);//VRH,VCMR,PSON,PON

	delay_ms(1000);

	Reg_Write(0x001,0x0100);delay_ms(5);
	Reg_Write(0x002,0x0100);delay_ms(5);
	Reg_Write(0x003,0x1030);delay_ms(5);
	Reg_Write(0x009,0x0001);delay_ms(5);
	Reg_Write(0x0C,0x0000);delay_ms(5);	//MCU interface
	Reg_Write(0x090,0x8000);delay_ms(5);
	Reg_Write(0x00f,0x0000);delay_ms(5);


   	Reg_Write(0x210,0x0000);delay_ms(5);
	Reg_Write(0x211,0x00ef);delay_ms(5);
	Reg_Write(0x212,0x0000);delay_ms(5);
	Reg_Write(0x213,0x018f);delay_ms(5);

	Reg_Write(0x500,0x0000);delay_ms(5);
	Reg_Write(0x501,0x0000);delay_ms(5);
	Reg_Write(0x502,0x005f);delay_ms(5);
	Reg_Write(0x401,0x0001);delay_ms(5);
	Reg_Write(0x404,0x0000);delay_ms(5);
	delay_ms(500);

   	Reg_Write(0x0007,0x0100);delay_ms(5);
	delay_ms(1000);

   	Reg_Write(0x200,0x0000);delay_ms(5);
	Reg_Write(0x201,0x0000);delay_ms(5);
    delay_ms(500);
   
  	LCD_Write(TYPE_LCD_COMMAND,0x0202);delay_ms(5);
  delay_ms(500);


}  

void  LCD_clear(u8 color)
{
  unsigned int com,seg;	
LCD_SetRamAddr(0,239, 0,399);
 // delay_ms(100);
 	
  for(com=0;com<400;com++)
  {
       for(seg=0;seg<240;seg++)
    {
      LCD_Write_Data16(colors[color]);
    }
    
  }
}




void delay_ms(int count)  // /* X1ms */
{
        int i,j;
        for(i=0;i<count;i++)
                for(j=0;j<100;j++);
}
void LCD_ShowChar(u16 x,u16 y,u16 For_color,u16 Bk_color, char ch)
{

   u8 temp;
   u8 pos,t;
   u8  CHAR_W,CHAR_H;

   CHAR_W = 8;         //8*16  这里显示的英文字符大小为8*16

   CHAR_H = 16;

   if(x>(LCD_SIZE_X-CHAR_W)||y>(LCD_SIZE_Y-CHAR_H))
   return;

LCD_SetRamAddr(x, x+CHAR_W-1, y,y+CHAR_H-1);
   ch = ch-32;                       //按照ASCII编码顺序的到相应字母的编码

   for(pos=0;pos<CHAR_H;pos++)  
   {
      temp= Font8x16[ch][pos];
      
      for(t=0;t<CHAR_W;t++)
      {                 
	    if(temp&0x80)
	    LCD_Write_Data16(For_color);  //textcolor
	    else 
	    LCD_Write_Data16(Bk_color);  //backcolor
        temp<<=1; 
      }
	  
   }
}  



void LCD_ShowString(u16  x,u16 y,u16 For_color,u16 Bk_color,char *p)
{         
   while(*p!='\0')
   {       
     LCD_ShowChar(x,y,For_color,Bk_color,*p);
     x+=8;
     p++;
   }
}



/**********************************************************

  写汉字子函数

**********************************************************/
void ChineseChar(u16 x,u16  y,u16  size,u16  For_color,u16  Bk_color ,char c)
{
   int e=0,i,j;
   int  ed;
   
   u16  ncols;	    //定义汉字的列数
   u16  nrows;		//定义汉字的行数
   u16  nbytes;	    //定义显示一个汉字所占的字节数

   u8 *pfont;
   u8 *pchar;
   u8 *fonttable[]={(u8 *)hz16,(u8 *)hz16,(u8 *)hz24};	  //将不同字模表的首地址
                                                                  //放在一个指针数组了，方便调用
   
   pfont=(u8 *)fonttable[size];									  //根据选择的字体，去调用不同大小的字模数据
                           										  // 这里可以是 hz12地址，hz16地址，hz24地址

   if(size==0)         //size为0时表示hz12表中的汉字
   {
     ncols =12-1;	   //-1 原因是下标从0开始，0~11 共12个数，以下-1原因亦如此
     nrows =12-1;
     nbytes =24;	  //取模字节数 12*12/8 一共有24个字节 24个字节只能显示单色字
   }
   else
   if(size==1)		  //size 为1时表示hz16中的地址
   {
     ncols =16-1;
     nrows =16-1;
     nbytes =32;  	  //16*16/8 =32个字节，单色字
   }
   else
   if(size==2)		   //size 为2时表示hz24中的地址
   {
     ncols =24-1;
     nrows =24-1;
     nbytes =72;   
   }
   
   pchar=pfont + (nbytes*(c-1));   //执行这句后，把相应字符的首地址送给pchar,
                                   

   LCD_SetRamAddr(x, x+nrows, y,y+ncols); //设置字的起始坐标及结束坐标
   
   for(i=0;i<nbytes;i++)		  //开始向屏送数据
   {
     ed=*pchar++;				  // 数据送给ed，变量ed中的数据准备送到屏中
	 if(size==0)
	 {
       if(i%2==0)
       e=8;
       else
       e=4;
	 }
	 else						  
	 e=8;
     
     for(j=0;j<e;j++)
     if((ed>>j)&0x01)
     {
       LCD_Write_Data16(For_color);  //textcolor
	 }
     else
     {
       LCD_Write_Data16(Bk_color);  //backcolor
	 }
  }
}

/*********************************************************/

