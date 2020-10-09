/**********************************************************************************/
#include "ultrasonic.h"
#include "SysTick.h"
#include "usart1.h"
#include "timer_count.h"

float distance_value;
/************************GPIO³õÊ¼»¯º¯Êý********************************************/
void Ultrasonic_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);  
}
/**********************************************************************************/
/********************³¬Éù²¨Ä£¿éÆô¶¯º¯Êý********************************************/
void Distance_Count(void)
{	
	distance_value=(TIM5->CNT*1.7)/100;

	if(distance_value>300)            //³¬Éù²¨Ä£¿é×î´ó¼ì²â¾àÀëÎª3m
	{
		distance_value=300;      	  
	}
	
	TIM5->CNT=0;	
}
/**********************************************************************************/
/********************³¬Éù²¨Ä£¿éÆô¶¯º¯Êý********************************************/
void Ultrasonic_ON(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	Delay_us(40);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	
	TIM_Cmd(TIM5,ENABLE);
	while(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9));
	TIM5->CNT=0;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9));
	TIM_Cmd(TIM5,DISABLE);

	Distance_Count();
					   
}
/**********************************************************************************/
