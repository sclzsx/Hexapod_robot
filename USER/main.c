#include "stm32f10x.h"	    				//����stm32��ϵͳͷ�ļ�
//#include "adc.h"							//����ADC��ͷ�ļ�
#include "gpio.h"							//����GPIO��ͷ�ļ�
//#include "usart1.h"		  					//��������ͨ������ͷ�ļ�
#include "SysTick.h"						//����ϵͳ��ʱ����ͷ�ļ�
#include "timer_count.h"					//������ʱ������ͷ�ļ�
//#include "ultrasonic.h"						//��������������ͷ�ļ�
#include "lcd.h"
#include "OLED.h"
#include "24l01.h"
#include "sys.h"

u8 date[3]={0,0,'S'};
							
int main(void)
{
	u8 tmp_buf[33];	
//	ADC1_Init();	   								   //ADC��ʼ��
	Timer_Init();									     //��ʱ����ʼ��
	SysTick_Init();									   //ϵͳ�δ�ʱ����ʼ��
	GPIO_Config();									   //GPIO��ʼ��
//	USART1_Config();								   //��ʼ������1
//	USART3_Config(); 								   //��ʼ������3
//	Ultrasonic_Config();							 //��ʼ��������ģ��
	LCD_Init();
  OLED_Init();
	NRF24L01_Init();
	
	Timer_ON();										   //������ʱ��
		
	Stand();	//Ĭ��վ��״̬
	Delay_ms(2000);	

	
	IWDG_Init(6,2000);//���Ź���λ �ɸĸ�λ��ʱ��
		
 	while(NRF24L01_Check())	//���NRF24L01�Ƿ���λ.	
	OLED_6x8Str(20,0,"NRF24L01 Error  ");		
  NRF24L01_RX_Mode();		
  OLED_6x8Str(20,0,"HEXAPOD IS READY");
	
	alert1();//��ʾ��������	
	
	while(1)
	{
		if(NRF24L01_RxPacket(tmp_buf)==0)//һ�����յ���Ϣ,����ʾ����.
		{
			tmp_buf[32]=0;//�����ַ���������
	
			if(tmp_buf[1]>140 && tmp_buf[1]<180)//��ҡ�ˣ���
			{date[2]='A';OLED_6x8Str(0,3,"go forward");}

			else if(tmp_buf[1]>20 && tmp_buf[1]<60)//��ҡ�ˣ���
			{date[2]='B';OLED_6x8Str(0,3,"take back ");}
			
			else if(tmp_buf[0]>20 && tmp_buf[0]<60)//��ҡ�ˣ���
			{date[2]='C';OLED_6x8Str(0,3,"turn left ");}
			
			else if(tmp_buf[0]>140 && tmp_buf[0]<180)//��ҡ�ˣ���
			{date[2]='D';OLED_6x8Str(0,3,"turn right");}

			else if(tmp_buf[2]>20 && tmp_buf[2]<80)//��ҡ�ˣ�����
			{date[2]='1';OLED_6x8Str(0,3,"left      ");}

			else if(tmp_buf[2]>120 && tmp_buf[2]<180)//��ҡ�ˣ�����
			{date[2]='3';OLED_6x8Str(0,3,"right     ");}
			
			else 
			{date[2]='S';Stand();OLED_6x8Str(0,3,"stand     ");}

		}	
		
		Control_Action();//���ݽ��յ���������Ӧ��Ӧ�Ķ���
	}

} 

