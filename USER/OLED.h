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

#ifndef __OLED_H
#define __OLED_H
#include "stm32f10x.h"

#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel	    ((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xCF 
#define X_WIDTH 	128
#define Y_WIDTH 	64

/*-----------------端口宏定义----------------
 *******修改本端口即可切换OLED显示端口*******
 -------------------------------------------*/
#define GPIOX GPIOC
#define RCC_APB2Periph_GPIOX	RCC_APB2Periph_GPIOC
#define GPIO_Pin_CLK	GPIO_Pin_12
#define GPIO_Pin_MOSI	GPIO_Pin_13
#define GPIO_Pin_CS		GPIO_Pin_14
#define GPIO_Pin_DC		GPIO_Pin_15

//-----------------OLED端口定义----------------
#define OLED_SCL_Clr() GPIO_ResetBits(GPIOX,GPIO_Pin_CLK)
#define OLED_SCL_Set() GPIO_SetBits(GPIOX,GPIO_Pin_CLK)    //接OLED模块的CLK

#define OLED_SDA_Clr() GPIO_ResetBits(GPIOX,GPIO_Pin_MOSI)
#define OLED_SDA_Set() GPIO_SetBits(GPIOX,GPIO_Pin_MOSI)    //接OLED模块MOSI

#define OLED_CS_Clr() GPIO_ResetBits(GPIOX,GPIO_Pin_CS)
#define OLED_CS_Set() GPIO_SetBits(GPIOX,GPIO_Pin_CS)    //接OLED模块CS

#define OLED_DC_Clr() GPIO_ResetBits(GPIOX,GPIO_Pin_DC)
#define OLED_DC_Set() GPIO_SetBits(GPIOX,GPIO_Pin_DC)     //接OLED模块D/C

//#define OLED_CMD  0	//写命令
//#define OLED_DATA 1	//写数据

//OLED控制用函数
void OLED_WrDat(unsigned char dat);//写数据
void OLED_WrCmd(unsigned char cmd);//写命令
void OLED_SetPos(unsigned char x, unsigned char y);//设置起始点坐标
void OLED_Fill(unsigned char bmp_dat);//全屏填充
void OLED_CLS(void);//清屏
void OLED_Init(void);//初始化
void OLED_6x8Str(unsigned char x, unsigned char y, unsigned char ch[]);
void OLED_8x16Str(unsigned char x, unsigned char y, unsigned char ch[]);
void OLED_16x16CN(unsigned char x, unsigned char y, unsigned char N);
void OLED_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
void OLED_Print(unsigned char x, unsigned char y, unsigned char ch[]);
void OLED_8x16var(unsigned char x, unsigned char y, unsigned int N);
void OLED_6x8var(unsigned char x, unsigned char y, unsigned int N);
#endif
