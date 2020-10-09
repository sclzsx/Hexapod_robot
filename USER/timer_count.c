/**********************************************************************************/
#include "timer_count.h"
/**********************��ʱ��2�ж����ú���*********************************************/
void TIM2_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**************************************************************************************/
/**********************��ʱ��3�ж����ú���*********************************************/
void TIM3_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**************************************************************************************/
/**********************��ʱ��4�ж����ú���*********************************************/
void TIM4_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**************************************************************************************/
/**********************��ʱ��5�ж����ú���*********************************************/
void TIM5_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**************************************************************************************/
/************************��ʱ��2���ú���***********************************************/
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    
	TIM_DeInit(TIM2);											/* ����������ʱ�� */

    TIM_TimeBaseStructure.TIM_Period=2000;		 				/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    TIM_TimeBaseStructure.TIM_Prescaler=71;						/* ʱ��Ԥ��Ƶ�� 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* �ⲿʱ�Ӳ�����Ƶ */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	/* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM2,TIM_FLAG_Update);						/* �������жϱ�־ */

    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);					/* �����жϴ���*/   
	
	TIM_Cmd(TIM2,DISABLE);	
}
/**************************************************************************************/
/************************��ʱ��3���ú���***********************************************/
void TIM3_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    
	  TIM_DeInit(TIM3);											/* ����������ʱ�� */

    TIM_TimeBaseStructure.TIM_Period=2000;		 				/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    TIM_TimeBaseStructure.TIM_Prescaler=71;						/* ʱ��Ԥ��Ƶ�� 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* �ⲿʱ�Ӳ�����Ƶ */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	/* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM3,TIM_FLAG_Update);						/* �������жϱ�־ */

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);					/* �����жϴ���*/   
	
	TIM_Cmd(TIM3,DISABLE);	
}
/**************************************************************************************/
/************************��ʱ��4���ú���***********************************************/
void TIM4_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    
	TIM_DeInit(TIM4);											/* ����������ʱ�� */

    TIM_TimeBaseStructure.TIM_Period=2000;		 				/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    TIM_TimeBaseStructure.TIM_Prescaler=71;						/* ʱ��Ԥ��Ƶ�� 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* �ⲿʱ�Ӳ�����Ƶ */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	/* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM4,TIM_FLAG_Update);						/* �������жϱ�־ */

    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);					/* �����жϴ���*/   
	
	TIM_Cmd(TIM4,DISABLE);	
}
/**************************************************************************************/
/************************��ʱ�����ú���************************************************/
void TIM5_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);
    
	TIM_DeInit(TIM5);											/* ����������ʱ�� */

    TIM_TimeBaseStructure.TIM_Period=7199;		 				/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    TIM_TimeBaseStructure.TIM_Prescaler=999;						/* ʱ��Ԥ��Ƶ�� 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* �ⲿʱ�Ӳ�����Ƶ */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	/* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM5, TIM_FLAG_Update);						/* �������жϱ�־ */

    TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);					/* �����жϴ���*/    
	
	TIM_Cmd(TIM5,DISABLE);									    /* �رն�ʱ�� */ 
}
/**************************************************************************************/
/************************��ʱ����ʼ������*********************************************/
void Timer_Init(void)
{
	TIM2_NVIC_Configuration();
	TIM2_Configuration();

	TIM3_NVIC_Configuration();
	TIM3_Configuration();

	TIM4_NVIC_Configuration();
	TIM4_Configuration();

	TIM5_NVIC_Configuration();
	TIM5_Configuration();
}
/**************************************************************************************/
/************************������ʱ������************************************************/
void Timer_ON(void)
{	
	TIM_Cmd(TIM2,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
	TIM_Cmd(TIM4,ENABLE);	
//	TIM_Cmd(TIM5,ENABLE);
}
/**************************************************************************************/
/************************�رն�ʱ������************************************************/
void Timer_OFF(void)
{	
	TIM_Cmd(TIM2,DISABLE);
	TIM_Cmd(TIM3,DISABLE);
	TIM_Cmd(TIM4,DISABLE);	
//	TIM_Cmd(TIM5,DISABLE);
}
/**************************************************************************************/

