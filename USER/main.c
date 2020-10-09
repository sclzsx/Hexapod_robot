#include "stm32f10x.h"	    				//包含stm32库系统头文件
//#include "adc.h"							//包含ADC库头文件
#include "gpio.h"							//包含GPIO库头文件
//#include "usart1.h"		  					//包含串口通信设置头文件
#include "SysTick.h"						//包含系统定时器库头文件
#include "timer_count.h"					//包含定时器设置头文件
//#include "ultrasonic.h"						//包含超声波设置头文件
#include "lcd.h"
#include "OLED.h"
#include "24l01.h"
#include "sys.h"

u8 date[3]={0,0,'S'};
							
int main(void)
{
	u8 tmp_buf[33];	
//	ADC1_Init();	   								   //ADC初始化
	Timer_Init();									     //定时器初始化
	SysTick_Init();									   //系统滴答定时器初始化
	GPIO_Config();									   //GPIO初始化
//	USART1_Config();								   //初始化串口1
//	USART3_Config(); 								   //初始化串口3
//	Ultrasonic_Config();							 //初始化超声波模块
	LCD_Init();
  OLED_Init();
	NRF24L01_Init();
	
	Timer_ON();										   //开启定时器
		
	Stand();	//默认站立状态
	Delay_ms(2000);	

	
	IWDG_Init(6,2000);//开门狗复位 可改复位的时间
		
 	while(NRF24L01_Check())	//检查NRF24L01是否在位.	
	OLED_6x8Str(20,0,"NRF24L01 Error  ");		
  NRF24L01_RX_Mode();		
  OLED_6x8Str(20,0,"HEXAPOD IS READY");
	
	alert1();//提示正常启动	
	
	while(1)
	{
		if(NRF24L01_RxPacket(tmp_buf)==0)//一旦接收到信息,则显示出来.
		{
			tmp_buf[32]=0;//加入字符串结束符
	
			if(tmp_buf[1]>140 && tmp_buf[1]<180)//左摇杆，上
			{date[2]='A';OLED_6x8Str(0,3,"go forward");}

			else if(tmp_buf[1]>20 && tmp_buf[1]<60)//左摇杆，下
			{date[2]='B';OLED_6x8Str(0,3,"take back ");}
			
			else if(tmp_buf[0]>20 && tmp_buf[0]<60)//左摇杆，左
			{date[2]='C';OLED_6x8Str(0,3,"turn left ");}
			
			else if(tmp_buf[0]>140 && tmp_buf[0]<180)//左摇杆，右
			{date[2]='D';OLED_6x8Str(0,3,"turn right");}

			else if(tmp_buf[2]>20 && tmp_buf[2]<80)//右摇杆，左移
			{date[2]='1';OLED_6x8Str(0,3,"left      ");}

			else if(tmp_buf[2]>120 && tmp_buf[2]<180)//右摇杆，右移
			{date[2]='3';OLED_6x8Str(0,3,"right     ");}
			
			else 
			{date[2]='S';Stand();OLED_6x8Str(0,3,"stand     ");}

		}	
		
		Control_Action();//根据接收到的数据相应对应的动作
	}

} 

