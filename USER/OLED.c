/************************************************************************************
* Description:128*64点阵的OLED显示屏驱动文件，仅适用于惠特自动化(heltec.taobao.com)的SD1306驱动SPI通信方式显示屏
* Function List:
*
* 2. void OLED_WrDat(unsigned char dat) -- 向OLED写数据
* 3. void OLED_WrCmd(unsigned char cmd) -- 向OLED写命令
* 4. void OLED_SetPos(unsigned char x, unsigned char y) -- 设置起始点坐标
* 5. void OLED_Fill(unsigned char bmp_dat) -- 全屏填充(0x00可以用于清屏，0xff可以用于全屏点亮)
* 6. void OLED_CLS(void) -- 清屏
* 7. void OLED_Init(void) -- OLED显示屏初始化
* 8. void OLED_6x8Str(unsigned char x, y,unsigned char ch[]) -- 显示6x8的ASCII字符
* 9. void OLED_8x16Str(unsigned char x, y,unsigned char ch[]) -- 显示8x16的ASCII字符
* 10.void OLED_16x16CN(unsigned char x, y, N) -- 显示16x16中文汉字,汉字要先在取模软件中取模
* 11.void OLED_BMP(unsigned char x0, y0,x1, y1,unsigned char BMP[]) -- 全屏显示128*64的BMP图片
*
*************************************************************************************/

#include "OLED.h"
#include "SysTick.h"						//包含系统定时器库头文件
#include "codetab.h"

void OLED_WrDat(unsigned char dat)//写数据
{
	unsigned char i;
	OLED_DC_Set();
	for(i=0;i<8;i++)
	{
		if((dat << i) & 0x80)
		{
			OLED_SDA_Set();
		}
		else
			OLED_SDA_Clr();
		OLED_SCL_Clr();
		OLED_SCL_Set();
	}
}

void OLED_WrCmd(unsigned char cmd)//写命令
{
	unsigned char i;
	OLED_DC_Clr();
	for(i=0;i<8;i++) //发送一个八位数据
	{
		if((cmd << i) & 0x80)
		{
			OLED_SDA_Set();
		}
		else
		{
			OLED_SDA_Clr();
		}
		OLED_SCL_Clr();
		OLED_SCL_Set();
	}
}

void OLED_SetPos(unsigned char x, unsigned char y)//设置起始点坐标
{
	OLED_WrCmd(0xb0 + y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x01);
}

void OLED_Fill(unsigned char bmp_dat)//全屏填充
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		{
			OLED_WrDat(bmp_dat);
		}
	}
}

void OLED_CLS(void)//清屏
{
	OLED_Fill(0x00);
}

void OLED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOX, ENABLE);	 //使能端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_CLK|GPIO_Pin_MOSI|GPIO_Pin_CS|GPIO_Pin_DC;	 //IO推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOX, &GPIO_InitStructure);	  //初始化
 	GPIO_SetBits(GPIOX,GPIO_Pin_CLK|GPIO_Pin_MOSI|GPIO_Pin_CS|GPIO_Pin_DC);	//输出高

	Delay_ms(500);	//延时初始化
	OLED_CS_Clr();
	
	OLED_WrCmd(0xae);
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(0xcf); // Set SEG Output Current Brightness
	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0,0xa1
	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0,0xc8
	OLED_WrCmd(0xa6);//--set normal display
	OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WrCmd(0x3f);//--1/64 duty
	OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd(0x00);//-not offset
	OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);//--set pre-charge period
	OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrCmd(0xda);//--set com pins hardware configuration
	OLED_WrCmd(0x12);
	OLED_WrCmd(0xdb);//--set vcomh
	OLED_WrCmd(0x40);//Set VCOM Deselect Level
	OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrCmd(0x02);//
	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);//--set(0x10) disable
	OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
	OLED_WrCmd(0xaf);//--turn on oled panel
	OLED_Fill(0x00);
	OLED_SetPos(0,0);
}

void OLED_6x8Str(unsigned char x, unsigned char y, unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c = ch[j]-32;
		if(x>126)
		{
			x=0;y++;
		}
		OLED_SetPos(x,y);
		for(i=0;i<6;i++)
		{
			OLED_WrDat(F6x8[c][i]);
		}
		x+=6;
		j++;
	}
}

void OLED_8x16Str(unsigned char x, unsigned char y, unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>120)
		{
			x=0;y++;
		}
		OLED_SetPos(x,y);
		for(i=0;i<8;i++)
		{
			OLED_WrDat(F8X16[c*16+i]);
		}
		OLED_SetPos(x,y+1);
		for(i=0;i<8;i++)
		{
			OLED_WrDat(F8X16[c*16+i+8]);
		}
		x+=8;
		j++;
	}
}

void OLED_P14x16Str(u8 x,u8 y,u8 ch[])
{
	u8 wm=0,ii = 0;
	u16 adder=1; 
	
	while(ch[ii] != '\0')
	{
  	wm = 0;
  	adder = 1;
  	while(F14x16_Idx[wm] > 127)
  	{
  		if(F14x16_Idx[wm] == ch[ii])
  		{
  			if(F14x16_Idx[wm + 1] == ch[ii + 1])
  			{
  				adder = wm * 14;
  				break;
  			}
  		}
  		wm += 2;			
  	}
  	if(x>118){x=0;y++;}
  	OLED_SetPos(x , y); 
  	if(adder != 1)// ????					
  	{
  		OLED_SetPos(x , y);
  		for(wm = 0;wm < 14;wm++)               
  		{
  			OLED_WrDat(F14x16[adder]);	
  			adder += 1;
  		}      
  		OLED_SetPos(x,y + 1); 
  		for(wm = 0;wm < 14;wm++)          
  		{
  			OLED_WrDat(F14x16[adder]);
  			adder += 1;
  		}   		
  	}
  	else			  //??????			
  	{
  		ii += 1;
      OLED_SetPos(x,y);
  		for(wm = 0;wm < 16;wm++)
  		{
  				OLED_WrDat(0);
  		}
  		OLED_SetPos(x,y + 1);
  		for(wm = 0;wm < 16;wm++)
  		{   		
  				OLED_WrDat(0);	
  		}
  	}
  	x += 14;
  	ii += 2;
	}
} 

void OLED_16x16CN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	}
}

void OLED_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;
	
  if(y1%8==0)
	{
		y=y1/8;
	}
  else
		y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			OLED_WrDat(BMP[j++]);
		}
	}
}

void OLED_Print(u8 x, u8 y, u8 ch[])
{
	u8 ch2[3];
	u8 ii=0;        
	while(ch[ii] != '\0')
	{
		if(ch[ii] > 127)
		{
			ch2[0] = ch[ii];
	 		ch2[1] = ch[ii + 1];
			ch2[2] = '\0';			//???????
			OLED_P14x16Str(x , y , ch2);	//????
			x += 14;
			ii += 2;
		}
		else
		{
			ch2[0] = ch[ii];	
			ch2[1] = '\0';			//???????
			OLED_8x16Str(x , y , ch2);	//????
			x += 8;
			ii+= 1;
		}
	}
} 

void OLED_var(u8 x,u8 y,unsigned int p)
{
	switch(p)
	{
		case 0:OLED_8x16Str(x,y,"0");break;
		case 1:OLED_8x16Str(x,y,"1");break;
		case 2:OLED_8x16Str(x,y,"2");break;
		case 3:OLED_8x16Str(x,y,"3");break;
		case 4:OLED_8x16Str(x,y,"4");break;
		case 5:OLED_8x16Str(x,y,"5");break;
		case 6:OLED_8x16Str(x,y,"6");break;
		case 7:OLED_8x16Str(x,y,"7");break;
		case 8:OLED_8x16Str(x,y,"8");break;
		case 9:OLED_8x16Str(x,y,"9");break;
	}
}	

void OLED_8x16var(unsigned char x, unsigned char y, u32 N)
{
	unsigned int ge = 0;
	unsigned int shi = 0;
	unsigned int bai = 0;
	unsigned int qian = 0;
	unsigned int wan = 0;
	ge = N%10;
	shi = N/10%10;
	bai = (N/100)%10;
	qian = (N/1000)%10;
	wan = (N/10000)%10;
	OLED_var(x+32,y,ge);
	OLED_var(x+24,y,shi);
	OLED_var(x+16,y,bai);
	OLED_var(x+8,y,qian);
	OLED_var(x,y,wan);
}

void OLED_var1(u8 x,u8 y,unsigned int p)
{
	switch(p)
	{
		case 0:OLED_6x8Str(x,y,"0");break;
		case 1:OLED_6x8Str(x,y,"1");break;
		case 2:OLED_6x8Str(x,y,"2");break;
		case 3:OLED_6x8Str(x,y,"3");break;
		case 4:OLED_6x8Str(x,y,"4");break;
		case 5:OLED_6x8Str(x,y,"5");break;
		case 6:OLED_6x8Str(x,y,"6");break;
		case 7:OLED_6x8Str(x,y,"7");break;
		case 8:OLED_6x8Str(x,y,"8");break;
		case 9:OLED_6x8Str(x,y,"9");break;
	}
}	

void OLED_6x8var(unsigned char x, unsigned char y, u32 N)
{
	unsigned int ge = 0;
	unsigned int shi = 0;
	unsigned int bai = 0;
	unsigned int qian = 0;
	ge = N%10;
	shi = N/10%10;
	bai = (N/100)%10;
	qian = (N/1000)%10;
	OLED_var1(x+18,y,ge);
	OLED_var1(x+12,y,shi);
	OLED_var1(x+6,y,bai);
	OLED_var1(x,y,qian);
}



