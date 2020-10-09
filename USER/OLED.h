/************************************************************************************
* Description:128*64�����OLED��ʾ�������ļ����������ڻ����Զ���(heltec.taobao.com)��SD1306����SPIͨ�ŷ�ʽ��ʾ��
* Function List:
*
* 2. void OLED_WrDat(unsigned char dat) -- ��OLEDд����
* 3. void OLED_WrCmd(unsigned char cmd) -- ��OLEDд����
* 4. void OLED_SetPos(unsigned char x, unsigned char y) -- ������ʼ������
* 5. void OLED_Fill(unsigned char bmp_dat) -- ȫ�����(0x00��������������0xff��������ȫ������)
* 6. void OLED_CLS(void) -- ����
* 7. void OLED_Init(void) -- OLED��ʾ����ʼ��
* 8. void OLED_6x8Str(unsigned char x, y,unsigned char ch[]) -- ��ʾ6x8��ASCII�ַ�
* 9. void OLED_8x16Str(unsigned char x, y,unsigned char ch[]) -- ��ʾ8x16��ASCII�ַ�
* 10.void OLED_16x16CN(unsigned char x, y, N) -- ��ʾ16x16���ĺ���,����Ҫ����ȡģ�����ȡģ
* 11.void OLED_BMP(unsigned char x0, y0,x1, y1,unsigned char BMP[]) -- ȫ����ʾ128*64��BMPͼƬ
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

/*-----------------�˿ں궨��----------------
 *******�޸ı��˿ڼ����л�OLED��ʾ�˿�*******
 -------------------------------------------*/
#define GPIOX GPIOC
#define RCC_APB2Periph_GPIOX	RCC_APB2Periph_GPIOC
#define GPIO_Pin_CLK	GPIO_Pin_12
#define GPIO_Pin_MOSI	GPIO_Pin_13
#define GPIO_Pin_CS		GPIO_Pin_14
#define GPIO_Pin_DC		GPIO_Pin_15

//-----------------OLED�˿ڶ���----------------
#define OLED_SCL_Clr() GPIO_ResetBits(GPIOX,GPIO_Pin_CLK)
#define OLED_SCL_Set() GPIO_SetBits(GPIOX,GPIO_Pin_CLK)    //��OLEDģ���CLK

#define OLED_SDA_Clr() GPIO_ResetBits(GPIOX,GPIO_Pin_MOSI)
#define OLED_SDA_Set() GPIO_SetBits(GPIOX,GPIO_Pin_MOSI)    //��OLEDģ��MOSI

#define OLED_CS_Clr() GPIO_ResetBits(GPIOX,GPIO_Pin_CS)
#define OLED_CS_Set() GPIO_SetBits(GPIOX,GPIO_Pin_CS)    //��OLEDģ��CS

#define OLED_DC_Clr() GPIO_ResetBits(GPIOX,GPIO_Pin_DC)
#define OLED_DC_Set() GPIO_SetBits(GPIOX,GPIO_Pin_DC)     //��OLEDģ��D/C

//#define OLED_CMD  0	//д����
//#define OLED_DATA 1	//д����

//OLED�����ú���
void OLED_WrDat(unsigned char dat);//д����
void OLED_WrCmd(unsigned char cmd);//д����
void OLED_SetPos(unsigned char x, unsigned char y);//������ʼ������
void OLED_Fill(unsigned char bmp_dat);//ȫ�����
void OLED_CLS(void);//����
void OLED_Init(void);//��ʼ��
void OLED_6x8Str(unsigned char x, unsigned char y, unsigned char ch[]);
void OLED_8x16Str(unsigned char x, unsigned char y, unsigned char ch[]);
void OLED_16x16CN(unsigned char x, unsigned char y, unsigned char N);
void OLED_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
void OLED_Print(unsigned char x, unsigned char y, unsigned char ch[]);
void OLED_8x16var(unsigned char x, unsigned char y, unsigned int N);
void OLED_6x8var(unsigned char x, unsigned char y, unsigned int N);
#endif
