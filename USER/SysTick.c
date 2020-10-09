#include "SysTick.h"

u32 a;
static __IO u32 TimingDelay;
/**************************�ж��������ú���*********************************************/
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
	if (SysTick_Config(SystemCoreClock/1000000))	// ST3.5.0��汾
	{ 
		while (1);
	}
	
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}
/***************************************************************************************/
/*************************΢����ʱ����**************************************************/
void Delay_us(u32 nTime)           
{ 
	TimingDelay = nTime;	
  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

	while(TimingDelay != 0);
	
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}
/***************************************************************************************/
/*************************������ʱ����**************************************************/
void Delay_ms(u32 nTime)           
{ 
	TimingDelay = nTime*1000;	
  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

	while(TimingDelay != 0);
	
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}
/***************************************************************************************/
/**************************��ʱ���̴���*************************************************/
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}
/***************************************************************************************/
