#include "gpio.h"
#include "usart1.h"
#include "SysTick.h"

#define MAXPWM   2500						 //������PWM��������2.5ms�궨��
#define CENTER1  1500					 //��ʼ������Ƕ�ֵ�궨��
#define CENTER2  1500           //��ʱ��ÿ��1us�ۼ�һ��
#define CENTER3  1400
#define CENTER4  1500					 
#define CENTER5  1500
#define CENTER6  1600
#define CENTER7  1500
#define CENTER8  1500
#define CENTER9  1600
#define CENTER10 1500
#define CENTER11 1500
#define CENTER12 1500
#define CENTER13 1500
#define CENTER14 1500
#define CENTER15 1500
#define CENTER16 1500
#define CENTER17 1500
#define CENTER18 1500

 //���������ڱ��������վ��PWM������ֵ
int svalue[18]={CENTER1,CENTER2,CENTER3,CENTER4,CENTER5,CENTER6,	 
                CENTER7,CENTER8,CENTER9,CENTER10,CENTER11,CENTER12,
                CENTER13,CENTER14,CENTER15,CENTER16,CENTER17,CENTER18};

extern u8 date[3];
u8 num1,num2,num3;						 //ÿ������������ʱ�ۼ�
u8 count1,count2,count3;				 //ÿ����������8·����Ⱥ�ֵ����
u8 sflag,sflag1,sflag2,sflag3;			 //ÿ�����������ж�8·����Ƿ�ת��Ԥ�ڽǶ�
u8 flag,fflag,dflag,lflag,rflag,xflag,yflag;//ÿ�������������ƻ����˶���˳������
int sdate[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//���������ڱ����ֵ
int PWM[24]={1500,1500,1400,1500,1500,1500,1600,1500,	  //���������ڱ���Ԥ��PWM������ֵ
			 1500,1600,1500,1500,1500,1500,1500,1500,
			 1500,1500,1500,1500,1500,1500,1500,1500};
int CPWM[24]={1500,1500,1400,1500,1500,1500,1600,1500,	  //���������ڱ��浱ǰPWM������ֵ
			 1500,1600,1500,1500,1500,1500,1500,1500,
			 1500,1500,1500,1500,1500,1500,1500,1500};
int LPWM[24]={1500,1500,1400,1500,1500,1500,1600,1500,	  //���������ڱ����ϴ�PWM������ֵ
			 1500,1600,1500,1500,1500,1500,1500,1500,
			 1500,1500,1500,1500,1500,1500,1500,1500};

void GPIO_Config(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);//��������ʱ��

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2; //��һ���ȣ���׶˶�����м������϶˶��
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;//�ڶ����ȣ���׶˶�����м������϶˶��
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12;//�������ȣ���׶˶�����м������϶˶��
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;//����������׶˶�����м������϶˶��
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;//����������׶˶�����м������϶˶��
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;//����������׶˶�����м������϶˶��
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
//		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);

}

void alert1(void)
{
	GPIO_SetBits(GPIOD,GPIO_Pin_2);			 //�����������δ�δ𱨾��ź�
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	Delay_ms(100); 			
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
	Delay_ms(100);
	GPIO_SetBits(GPIOD,GPIO_Pin_2);			 //�����������δ�δ𱨾��ź�
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	Delay_ms(100); 
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);	
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
}

void First_Leg(int date1,int date2,int date3)
{
	PWM[0]=date1;
	PWM[1]=date2;
	PWM[2]=date3;	
}

void Second_Leg(int date1,int date2,int date3)
{
	PWM[4]=date1;
	PWM[5]=date2;
	PWM[6]=date3;	
}

void Third_Leg(int date1,int date2,int date3)
{
	PWM[8]=date1;
	PWM[9]=date2;
	PWM[10]=date3;	
}

void Fourth_Leg(int date1,int date2,int date3)
{
	PWM[12]=date1;
	PWM[13]=date2;
	PWM[14]=date3;	
}

void Fifth_Leg(int date1,int date2,int date3)
{
	PWM[16]=date1;
	PWM[17]=date2;
	PWM[18]=date3;	
} 

void Sixth_Leg(int date1,int date2,int date3)
{
	PWM[20]=date1;
	PWM[21]=date2;
	PWM[22]=date3;	
}

void Decode_One(void)
{
    u8 symbol=0x00;           	//symbol1Ϊ���ת��Ԥ��λ�ñ�ǣ�CPWMΪ��ǰPWM����ֵ   
    
    if(CPWM[0]==PWM[0])			//���CPWM[0]=PWM[0],������һ�������ת��Ԥ�ڽǶȣ���ʱ��symbol1�����
    {
        symbol=symbol|0x01;
        LPWM[0]=CPWM[0];
    }
    else						//����������
    {
        symbol=symbol&0x00;
    }
    if(CPWM[1]==PWM[1])			//ͬ��
    {
        symbol=symbol|0x02;
        LPWM[1]=CPWM[1];
    }
    else
    {
        symbol=symbol&0x00;
    }
    if(CPWM[2]==PWM[2])
    {
        symbol=symbol|0x04;
        LPWM[2]=CPWM[2];
    }
    else
    {
        symbol=symbol&0x00;
    }
    if(CPWM[3]==PWM[3])
    {
        symbol=symbol|0x08;
        LPWM[3]=CPWM[3];
    }
    else
    {
        symbol=symbol&0x00;
    }
    if(CPWM[4]==PWM[4])
    {
        symbol=symbol|0x10;
        LPWM[4]=CPWM[4];
    }
    else
    {
        symbol=symbol&0x00;
    }
    if(CPWM[5]==PWM[5])
    {
        symbol=symbol|0x20;
        LPWM[5]=CPWM[5];
    }
    else
    {
        symbol=symbol&0x00;
    }
    if(CPWM[6]==PWM[6])
    {
        symbol=symbol|0x40;
        LPWM[6]=CPWM[6];
    }
    else
    {
        symbol=symbol&0x00;
    }
    if(CPWM[7]==PWM[7])
    {
        symbol=symbol|0x80;
        LPWM[7]=CPWM[7];
    }
    else
    {
        symbol=symbol&0x00;
    }
       
    if(symbol==0xff)			//���symbol1=0xff������8·����Ѿ�ȫ��ת��Ԥ��λ�ã���ʱ��sflag1�����
    {
        sflag1=0x01;
    }
    else
    {
        sflag1=0x00;
    }

	sflag=sflag1&sflag2&sflag3;	

	if(sflag)					//���sflag=1������24·�����ȫ��ת��Ԥ��λ��
	{
		Update();				//��ʱ��ʼ��������

		Control_Action();	    //���������ݿ��ƻ����˶���
	}	

}
/**********************************************************************************/
/************************������ٺ���2*********************************************/
void Decode_Two(void)
{
    u8 symbol=0x00;            	//����ͬ��
    
    if(CPWM[8]==PWM[8])
    {
        symbol=symbol|0x01;
        LPWM[8]=CPWM[8];
    }
    else
    {
        symbol=symbol&0x00;
    }
    if(CPWM[9]==PWM[9])
    {
        symbol=symbol|0x02;
        LPWM[9]=CPWM[9];
    }
    else
    {
        symbol=symbol&0x00;
    }
    if(CPWM[10]==PWM[10])
    {
        symbol=symbol|0x04;
        LPWM[10]=CPWM[10];
    }
    else
    {
        symbol=symbol&0x00;
    }
    if(CPWM[11]==PWM[11])
    {
        symbol=symbol|0x08;
        LPWM[11]=CPWM[11];
    }
    else
    {
        symbol=symbol&0x00;
    }
    if(CPWM[12]==PWM[12])
    {
        symbol=symbol|0x10;
        LPWM[12]=CPWM[12];
    }
    else
    {
        symbol=symbol&0x00;
    }
    if(CPWM[13]==PWM[13])
    {
        symbol=symbol|0x20;
        LPWM[13]=CPWM[13];
    }
    else
    {
        symbol=symbol&0x00;
    }
    if(CPWM[14]==PWM[14])
    {
        symbol=symbol|0x40;
        LPWM[14]=CPWM[14];
    }
    else
    {
        symbol=symbol&0x00;
    }
    if(CPWM[15]==PWM[15])
    {
        symbol=symbol|0x80;
        LPWM[15]=CPWM[15];
    }
    else
    {
        symbol=symbol&0x00;
    }
       
    if(symbol==0xff)
    {
        sflag2=0x01;
    }
    else
    {
        sflag2=0x00;
    }

	sflag=sflag1&sflag2&sflag3;

	if(sflag)
	{
		Update();
		
		Control_Action();	
	}	
		
}
/**********************************************************************************/
/************************������ٺ���3*********************************************/
void Decode_Three(void)
{
    u8 symbol=0x00;           	//����ͬ��  
    
    if(CPWM[16]==PWM[16])
    {
        symbol=symbol|0x01;
        LPWM[16]=CPWM[16];
    }
    else
    {
        symbol=symbol&0x00;
    }
    if(CPWM[17]==PWM[17])
    {
        symbol=symbol|0x02;
        LPWM[17]=CPWM[17];
    }
    else
    {
        symbol=symbol&0x00;
    }
    if(CPWM[18]==PWM[18])
    {
        symbol=symbol|0x04;
        LPWM[18]=CPWM[18];
    }
    else
    {
        symbol=symbol&0x00;
    }
    if(CPWM[19]==PWM[19])
    {
        symbol=symbol|0x08;
        LPWM[19]=CPWM[19];
    }
    else
    {
        symbol=symbol&0x00;
    }
    if(CPWM[20]==PWM[20])
    {
        symbol=symbol|0x10;
        LPWM[20]=CPWM[20];
    }
    else
    {
        symbol=symbol&0x00;
    }
    if(CPWM[21]==PWM[21])
    {
        symbol=symbol|0x20;
        LPWM[21]=CPWM[21];
    }
    else
    {
        symbol=symbol&0x00;
    }
    if(CPWM[22]==PWM[22])
    {
        symbol=symbol|0x40;
        LPWM[22]=CPWM[22];
    }
    else
    {
        symbol=symbol&0x00;
    }
    if(CPWM[23]==PWM[23])
    {
        symbol=symbol|0x80;
        LPWM[23]=CPWM[23];
    }
    else
    {
        symbol=symbol&0x00;
    }		 
       
    if(symbol==0xff)
    {
        sflag3=0x01;
    }
    else
    {
        sflag3=0x00;
    }
	
	sflag=sflag1&sflag2&sflag3;
	
	if(sflag)
	{
		Update();

		Control_Action();
	}
	
}
/**********************************************************************************/
/*********************����Ƕ���ֵ���㺯��1****************************************/
void Date_Calculate_One(void)
{
    sdate[0]=(PWM[0]-LPWM[0])/4;//��Ԥ�ڶ��PWM����ֵ�ֳ��Ĵ�ִ�� 
    sdate[1]=(PWM[1]-LPWM[1])/4;//�����ȿ��ö��ƽ����ת��Ҳ�û����˶�������
    sdate[2]=(PWM[2]-LPWM[2])/4;
    sdate[3]=(PWM[3]-LPWM[3])/4;
    sdate[4]=(PWM[4]-LPWM[4])/4; 
    sdate[5]=(PWM[5]-LPWM[5])/4;
    sdate[6]=(PWM[6]-LPWM[6])/4;
    sdate[7]=(PWM[7]-LPWM[7])/4; 

    CPWM[0]=sdate[0]+CPWM[0];
    CPWM[1]=sdate[1]+CPWM[1];
    CPWM[2]=sdate[2]+CPWM[2];
    CPWM[3]=sdate[3]+CPWM[3];
    CPWM[4]=sdate[4]+CPWM[4];
    CPWM[5]=sdate[5]+CPWM[5];
    CPWM[6]=sdate[6]+CPWM[6];
    CPWM[7]=sdate[7]+CPWM[7]; 	
}
/**********************************************************************************/
/*********************����Ƕ���ֵ���㺯��2****************************************/
void Date_Calculate_Two(void)
{
    sdate[8]=(PWM[8]-LPWM[8])/4;  //����ͬ��
    sdate[9]=(PWM[9]-LPWM[9])/4;
    sdate[10]=(PWM[10]-LPWM[10])/4;
    sdate[11]=(PWM[11]-LPWM[11])/4;
    sdate[12]=(PWM[12]-LPWM[12])/4; 
    sdate[13]=(PWM[13]-LPWM[13])/4;
    sdate[14]=(PWM[14]-LPWM[14])/4;
    sdate[15]=(PWM[15]-LPWM[15])/4; 

    CPWM[8]=sdate[8]+CPWM[8];
    CPWM[9]=sdate[9]+CPWM[9];
    CPWM[10]=sdate[10]+CPWM[10];
    CPWM[11]=sdate[11]+CPWM[11];
    CPWM[12]=sdate[12]+CPWM[12];
    CPWM[13]=sdate[13]+CPWM[13];
    CPWM[14]=sdate[14]+CPWM[14];
    CPWM[15]=sdate[15]+CPWM[15]; 	
}
/**********************************************************************************/
/*********************����Ƕ���ֵ���㺯��3****************************************/
void Date_Calculate_Three(void)
{
    sdate[16]=(PWM[16]-LPWM[16])/4;//����ͬ�� 
    sdate[17]=(PWM[17]-LPWM[17])/4;
    sdate[18]=(PWM[18]-LPWM[18])/4;
    sdate[19]=(PWM[19]-LPWM[19])/4;
    sdate[20]=(PWM[20]-LPWM[20])/4; 
    sdate[21]=(PWM[21]-LPWM[21])/4;
    sdate[22]=(PWM[22]-LPWM[22])/4;
    sdate[23]=(PWM[23]-LPWM[23])/4; 

    CPWM[16]=sdate[16]+CPWM[16];
    CPWM[17]=sdate[17]+CPWM[17];
    CPWM[18]=sdate[18]+CPWM[18];
    CPWM[19]=sdate[19]+CPWM[19];
    CPWM[20]=sdate[20]+CPWM[20];
    CPWM[21]=sdate[21]+CPWM[21];
    CPWM[22]=sdate[22]+CPWM[22];
    CPWM[23]=sdate[23]+CPWM[23]; 	
}
/**********************************************************************************/
/************************GPIO��ƽ��ת����1*****************************************/ 
void Flip_GPIO_One(void)
{
	switch(count1)								 //��20ms�Ķ���������ڷֳ�8�ݣ�ÿ2.5ms����һ�������ת
	{  											 //ÿ����ʱ������8·�����ת��3����ʱ������24·�����ת
		case 1: TIM2->ARR=CPWM[0];				 //����һ�����������ֵ��ֵ����ʱ��2
				GPIO_SetBits(GPIOA,GPIO_Pin_0);  //ͬʱ���߿��ƶ��1�����ŵĵ�ƽ
				break;
		
		case 2:	TIM2->ARR=MAXPWM-CPWM[0]; 		 //��2.5ms��ȥPWM����ֵ�����ݸ�ֵ��ʱ��2
				GPIO_ResetBits(GPIOA,GPIO_Pin_0);//ͬʱ���Ϳ��ƶ��1���ŵĵ�ƽ 
				break;							 //���ƶ��1��������ʣ��20ms-CPM[0]ʱ���ڽ�һֱ���ֵ͵�ƽ�����1����CPWMֵת��

		case 3:	TIM2->ARR=CPWM[1]; 
				GPIO_SetBits(GPIOA,GPIO_Pin_1); 
				break;
		
		case 4:	TIM2->ARR=MAXPWM-CPWM[1];  
				GPIO_ResetBits(GPIOA,GPIO_Pin_1); 
				break;

		case 5:	TIM2->ARR=CPWM[2];  
				GPIO_SetBits(GPIOA,GPIO_Pin_2); 
				break;
		
		case 6:	TIM2->ARR=MAXPWM-CPWM[2];  
				GPIO_ResetBits(GPIOA,GPIO_Pin_2);
				break;

		case 7:	TIM2->ARR=CPWM[3];  
				GPIO_SetBits(GPIOA,GPIO_Pin_3); 
				break;
		
		case 8:	TIM2->ARR=MAXPWM-CPWM[3];  
				GPIO_ResetBits(GPIOA,GPIO_Pin_3);
				break;

		case 9:	TIM2->ARR=CPWM[4];  
				GPIO_SetBits(GPIOA,GPIO_Pin_4); 
				break;
		
		case 10:TIM2->ARR=MAXPWM-CPWM[4];  
				GPIO_ResetBits(GPIOA,GPIO_Pin_4);
				break;

		case 11:TIM2->ARR=CPWM[5];  
				GPIO_SetBits(GPIOA,GPIO_Pin_5); 
				break;
		
		case 12:TIM2->ARR=MAXPWM-CPWM[5];  
				GPIO_ResetBits(GPIOA,GPIO_Pin_5);
				break;

		case 13:TIM2->ARR=CPWM[6];  
				GPIO_SetBits(GPIOA,GPIO_Pin_6); 
				break;
		
		case 14:TIM2->ARR=MAXPWM-CPWM[6];  
				GPIO_ResetBits(GPIOA,GPIO_Pin_6);
				break;

		case 15:TIM2->ARR=CPWM[7];  
				GPIO_SetBits(GPIOA,GPIO_Pin_7); 
				break;
		
		case 16:TIM2->ARR=MAXPWM-CPWM[7]; 
				GPIO_ResetBits(GPIOA,GPIO_Pin_7);
				count1=0; 
				break;
	}	
}
/**********************************************************************************/
/************************GPIO��ƽ��ת����2*****************************************/ 
void Flip_GPIO_Two(void)
{
	switch(count2)
	{  		
		case 1: TIM3->ARR=CPWM[8];				 //����ͬ��
				GPIO_SetBits(GPIOA,GPIO_Pin_8);  
				break;
		
		case 2:	TIM3->ARR=MAXPWM-CPWM[8]; 
				GPIO_ResetBits(GPIOA,GPIO_Pin_8); 
				break;

		case 3:	TIM3->ARR=CPWM[9]; 
				GPIO_SetBits(GPIOA,GPIO_Pin_11); 
				break;
		
		case 4:	TIM3->ARR=MAXPWM-CPWM[9];  
				GPIO_ResetBits(GPIOA,GPIO_Pin_11); 
				break;

		case 5:	TIM3->ARR=CPWM[10];  
				GPIO_SetBits(GPIOA,GPIO_Pin_12); 
				break;
		
		case 6:	TIM3->ARR=MAXPWM-CPWM[10];  
				GPIO_ResetBits(GPIOA,GPIO_Pin_12);
				break;

		case 7:	TIM3->ARR=CPWM[11];  
				GPIO_SetBits(GPIOA,GPIO_Pin_15); 
				break;
		
		case 8:	TIM3->ARR=MAXPWM-CPWM[11];  
				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
				break;

		case 9:	TIM3->ARR=CPWM[12];  
				GPIO_SetBits(GPIOC,GPIO_Pin_0); 
				break;
		
		case 10:TIM3->ARR=MAXPWM-CPWM[12];  
				GPIO_ResetBits(GPIOC,GPIO_Pin_0);
				break;

		case 11:TIM3->ARR=CPWM[13];  
				GPIO_SetBits(GPIOC,GPIO_Pin_1); 
				break;
		
		case 12:TIM3->ARR=MAXPWM-CPWM[13];  
				GPIO_ResetBits(GPIOC,GPIO_Pin_1);
				break;

		case 13:TIM3->ARR=CPWM[14];  
				GPIO_SetBits(GPIOC,GPIO_Pin_2); 
				break;
		
		case 14:TIM3->ARR=MAXPWM-CPWM[14];  
				GPIO_ResetBits(GPIOC,GPIO_Pin_2);
				break;

		case 15:TIM3->ARR=CPWM[15];  
				GPIO_SetBits(GPIOC,GPIO_Pin_3); 
				break;
		
		case 16:TIM3->ARR=MAXPWM-CPWM[15]; 
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);
				count2=0; 
				break;
	}	
}
/**********************************************************************************/
/************************GPIO��ƽ��ת����3*****************************************/ 
void Flip_GPIO_Three(void)
{
	switch(count3)
	{  		
		case 1: TIM4->ARR=CPWM[16];				 //����ͬ��
				GPIO_SetBits(GPIOC,GPIO_Pin_4);  
				break;
		
		case 2:	TIM4->ARR=MAXPWM-CPWM[16]; 
				GPIO_ResetBits(GPIOC,GPIO_Pin_4); 
				break;

		case 3:	TIM4->ARR=CPWM[17]; 
				GPIO_SetBits(GPIOC,GPIO_Pin_5); 
				break;
		
		case 4:	TIM4->ARR=MAXPWM-CPWM[17];  
				GPIO_ResetBits(GPIOC,GPIO_Pin_5); 
				break;

		case 5:	TIM4->ARR=CPWM[18];  
				GPIO_SetBits(GPIOC,GPIO_Pin_6); 
				break;
		
		case 6:	TIM4->ARR=MAXPWM-CPWM[18];  
				GPIO_ResetBits(GPIOC,GPIO_Pin_6);
				break;

		case 7:	TIM4->ARR=CPWM[19];  
				GPIO_SetBits(GPIOC,GPIO_Pin_7); 
				break;
		
		case 8:	TIM4->ARR=MAXPWM-CPWM[19];  
				GPIO_ResetBits(GPIOC,GPIO_Pin_7);
				break;

		case 9:	TIM4->ARR=CPWM[20];  
				GPIO_SetBits(GPIOC,GPIO_Pin_8); 
				break;
		
		case 10:TIM4->ARR=MAXPWM-CPWM[20];  
				GPIO_ResetBits(GPIOC,GPIO_Pin_8);
				break;

		case 11:TIM4->ARR=CPWM[21];  
				GPIO_SetBits(GPIOC,GPIO_Pin_9); 
				break;
		
		case 12:TIM4->ARR=MAXPWM-CPWM[21];  
				GPIO_ResetBits(GPIOC,GPIO_Pin_9);
				break;

		case 13:TIM4->ARR=CPWM[22];  
				GPIO_SetBits(GPIOC,GPIO_Pin_10); 
				break;
		
		case 14:TIM4->ARR=MAXPWM-CPWM[22];  
				GPIO_ResetBits(GPIOC,GPIO_Pin_10);
				break;

		case 15:TIM4->ARR=CPWM[23];  
				GPIO_SetBits(GPIOC,GPIO_Pin_11); 
				break;
		
		case 16:TIM4->ARR=MAXPWM-CPWM[23]; 
				GPIO_ResetBits(GPIOC,GPIO_Pin_11);
				count3=0; 
				break;
	}	
}
/************************������ƺ���1*********************************************/
void Servo1(void)
{		
	num1++;									 //num���������ۼӼ��������ڶ��ת��

	count1++;

	if(!(num1%50))							 //���������ֵÿ��һ��ʱ�����һ��
	{	
		num1=0;
			
		Date_Calculate_One();				 //����Ƕȼ���

		Decode_One();						 //�������
		
	}
	 
	Flip_GPIO_One();						 //��תIO��ƽ

}

/************************������ƺ���2*********************************************/
void Servo2(void)
{		
	num2++;									 //����ͬ��

	count2++;

	if(!(num2%50))
	{	
		num2=0;
			
		Date_Calculate_Two();		

		Decode_Two();
	
	}
	
	Flip_GPIO_Two();
}

/************************������ƺ���4*********************************************/
void Servo3(void)
{		
	num3++;									 //����ͬ��

	count3++;

	if(!(num3%50))		 
	{	
		num3=0;
			
		Date_Calculate_Three();	

		Decode_Three();
		
	}
	
	Flip_GPIO_Three();

}

/*******************�����Ƕ�������*************************************************/
void Stand(void)
{

	fflag=dflag=lflag=0;					  //������ж�������
	xflag=yflag=rflag=0;

	First_Leg(svalue[0],svalue[1],svalue[2]); 
	Second_Leg(svalue[3],svalue[4],svalue[5]);
	Third_Leg(svalue[6],svalue[7],svalue[8]);	
	Fourth_Leg(svalue[9],svalue[10],svalue[11]);
	Fifth_Leg(svalue[12],svalue[13],svalue[14]);
	Sixth_Leg(svalue[15],svalue[16],svalue[17]);
		
}


/*ע����ע�����д� ˼·���� ��
�������ȷ�Ϊ���飬�������ǲ�̬
����ǰ��:��һ�� ̧��Ȼ����ǰת��һ���Ƕ� ������ء�
�����Щ����ǰ���ڶ��鲻����������֧�Ż����˵����á�
��һ����ɺ󣬵ڶ�����ִ�����Ƶ�һ��Ķ�����

�ȱ�� 1st 3rd 5th Ϊ��һ�� �� 2ed 4th 6th Ϊ�ڶ���

1��PA0 PA1 PA2
�������ȵĶ�Ӧ��ϵ��GPIO���ã�


ÿ������������ʱ��������ģ��PWM���ƣ�ÿ����ʱ�������˰�·PWM����ʵ��ÿ��ֻ������·��
ÿ���ȶ���һ����Ӧ�ĺ��� ����һ��������Ӧ�ڲ��� ���ڶ���������Ӧ������� ������������Ӧ�ϲ����

TIM2��Ӧ �ײ��ڲ�Ķ��  �ö������ǰ��
TIM3��Ӧ �ײ����Ķ��  �ö������̧��
TIM4��Ӧ �ϲ��Ķ��      �ö��������� Ҳ�и���̧��

��Ҫ�Ż���������ÿ���ȵĺ����svalue[x]+/-һ�������� 
�磺���õ�һ�������Ķ��Ӧ����ת��һ��ĽǶȣ������� svalue[1]+200 ��Ϊ svalue[1]+250
������ÿ�������װ��������һ�£�����svalue[1]+150 ������ת��һ��ĽǶȣ�
�������װʱ���ǹ�������⣬�о�Ŀǰ����õķ�ʽ�ˣ�

*/
void Forward(void)
{
	switch(fflag)
	{
		case 0:	
//��һ������һ����̧��	
First_Leg(svalue[0],svalue[1]+200,svalue[2]-200);	
Third_Leg(svalue[6],svalue[7]-200,svalue[8]+200);
Fifth_Leg(svalue[12],svalue[13]+200,svalue[14]-200);
				break;

		case 1:
//�ڶ�������һ����ǰ��	
First_Leg(svalue[0]-200,svalue[1]+200,svalue[2]-200);	
Third_Leg(svalue[6]+200,svalue[7]-200,svalue[8]+200);
Fifth_Leg(svalue[12]-200,svalue[13]+200,svalue[14]-200);

//�ڶ������ڶ����ȹ�λվ��
Second_Leg(svalue[3],svalue[4],svalue[5]);	
Fourth_Leg(svalue[9],svalue[10],svalue[11]);
Sixth_Leg(svalue[15],svalue[16],svalue[17]);
				break;

		case 2:	
//����������һ�������	
First_Leg(svalue[0]-200,svalue[1],svalue[2]);	
Third_Leg(svalue[6]+200,svalue[7],svalue[8]);
Fifth_Leg(svalue[12]-200,svalue[13],svalue[14]);
				break; 

		case 3: 
//���Ĳ����ڶ�����̧��	
Second_Leg(svalue[3],svalue[4]-200,svalue[5]+200);	
Fourth_Leg(svalue[9],svalue[10]-200,svalue[11]+200);
Sixth_Leg(svalue[15],svalue[16]+200,svalue[17]-200);
				break;

		case 4:	
//���岽���ڶ�����ǰ��
Second_Leg(svalue[3]+200,svalue[4]-200,svalue[5]+200);	
Fourth_Leg(svalue[9]+200,svalue[10]-200,svalue[11]+200);
Sixth_Leg(svalue[15]-200,svalue[16]+200,svalue[17]-200);

//���岽��ͬʱ��һ���ȹ�λ
First_Leg(svalue[0],svalue[1],svalue[2]);	
Third_Leg(svalue[6],svalue[7],svalue[8]);
Fifth_Leg(svalue[12],svalue[13],svalue[14]);			
				break;

		case 5:
//���������ڶ��������
Second_Leg(svalue[3]+200,svalue[4],svalue[5]);	
Fourth_Leg(svalue[9]+200,svalue[10],svalue[11]);
Sixth_Leg(svalue[15]-200,svalue[16],svalue[17]);	
				break;

		default: break;
	}	
}

void Draw_Back(void)
{
	switch(dflag)
	{
//��һ������һ����̧��	
First_Leg(svalue[0],svalue[1]+200,svalue[2]-200);	
Third_Leg(svalue[6],svalue[7]-200,svalue[8]+200);
Fifth_Leg(svalue[12],svalue[13]+200,svalue[14]-200);
				break;

		case 1:
//�ڶ�������һ����ǰ��	
First_Leg(svalue[0]+200,svalue[1]+200,svalue[2]-200);	
Third_Leg(svalue[6]-200,svalue[7]-200,svalue[8]+200);
Fifth_Leg(svalue[12]+200,svalue[13]+200,svalue[14]-200);

//�ڶ������ڶ����ȹ�λվ��
Second_Leg(svalue[3],svalue[4],svalue[5]);	
Fourth_Leg(svalue[9],svalue[10],svalue[11]);
Sixth_Leg(svalue[15],svalue[16],svalue[17]);
				break;

		case 2:	
//����������һ�������	
First_Leg(svalue[0]+200,svalue[1],svalue[2]);	
Third_Leg(svalue[6]-200,svalue[7],svalue[8]);
Fifth_Leg(svalue[12]+200,svalue[13],svalue[14]);
				break; 

		case 3: 
//���Ĳ����ڶ�����̧��	
Second_Leg(svalue[3],svalue[4]-200,svalue[5]+200);	
Fourth_Leg(svalue[9],svalue[10]-200,svalue[11]+200);
Sixth_Leg(svalue[15],svalue[16]+200,svalue[17]-200);
				break;

		case 4:	
//���岽���ڶ�����ǰ��
Second_Leg(svalue[3]-200,svalue[4]-200,svalue[5]+200);	
Fourth_Leg(svalue[9]-200,svalue[10]-200,svalue[11]+200);
Sixth_Leg(svalue[15]+200,svalue[16]+200,svalue[17]-200);

//���岽��ͬʱ��һ���ȹ�λ
First_Leg(svalue[0],svalue[1],svalue[2]);	
Third_Leg(svalue[6],svalue[7],svalue[8]);
Fifth_Leg(svalue[12],svalue[13],svalue[14]);			
				break;

		case 5:
//���������ڶ��������
Second_Leg(svalue[3]-200,svalue[4],svalue[5]);	
Fourth_Leg(svalue[9]-200,svalue[10],svalue[11]);
Sixth_Leg(svalue[15]+200,svalue[16],svalue[17]);	
				break;

		default: break;
	}
}

/************************��������ת����********************************************/
void Turn_Left(void)
{
	switch(lflag)
	{
		case 0:	
			  First_Leg(svalue[0],svalue[1]+200,svalue[2]-200);	
				Third_Leg(svalue[6],svalue[7]-200,svalue[8]+200);
				Fifth_Leg(svalue[12],svalue[13]+200,svalue[14]-200);
				break;

		case 1:	
				First_Leg(svalue[0]+200,svalue[1]+200,svalue[2]-200);	
				Third_Leg(svalue[6]-200,svalue[7]-200,svalue[8]+200);
				Fifth_Leg(svalue[12]-200,svalue[13]+200,svalue[14]-200);
				break;


				Second_Leg(svalue[3],svalue[4],svalue[5]);	
				Fourth_Leg(svalue[9],svalue[10],svalue[11]);
				Sixth_Leg(svalue[15],svalue[16],svalue[17]);
				break;


		case 2:	
				First_Leg(svalue[0]+200,svalue[1],svalue[2]);	
				Third_Leg(svalue[6]-200,svalue[7],svalue[8]);
				Fifth_Leg(svalue[12]-200,svalue[13],svalue[14]);
				break; 

		case 3:	 
				Second_Leg(svalue[3],svalue[4]-200,svalue[5]+200);	
				Fourth_Leg(svalue[9],svalue[10]-200,svalue[11]+200);
				Sixth_Leg(svalue[15],svalue[16]+200,svalue[17]-200);

				break;

		case 4:	
				Second_Leg(svalue[3]-200,svalue[4]-200,svalue[5]+200);	
				Fourth_Leg(svalue[9]+200,svalue[10]-200,svalue[11]+200);
				Sixth_Leg(svalue[15]-200,svalue[16]+200,svalue[17]-200);


				First_Leg(svalue[0],svalue[1],svalue[2]);					
				Third_Leg(svalue[6],svalue[7],svalue[8]);
				Fifth_Leg(svalue[12],svalue[13],svalue[14]);
				break;

		case 5: 
				Second_Leg(svalue[3]-200,svalue[4],svalue[5]);					
				Fourth_Leg(svalue[9]+200,svalue[10],svalue[11]);
				Sixth_Leg(svalue[15]-200,svalue[16],svalue[17]);
				break;

		default: break;
	}
}

/************************��������ת����********************************************/
void Turn_Right(void)
{
	switch(rflag)
	{
		case 0:	
			  First_Leg(svalue[0],svalue[1]+200,svalue[2]-200);	
				Third_Leg(svalue[6],svalue[7]-200,svalue[8]+200);
				Fifth_Leg(svalue[12],svalue[13]+200,svalue[14]-200);				
		break;

		case 1:	
			  First_Leg(svalue[0]-200,svalue[1]+200,svalue[2]-200);
				Third_Leg(svalue[6]+200,svalue[7]-200,svalue[8]+200);
				Fifth_Leg(svalue[12]+200,svalue[13]+200,svalue[14]-200);
				break;


				Second_Leg(svalue[3],svalue[4],svalue[5]);
				Fourth_Leg(svalue[9],svalue[10],svalue[11]);
				Sixth_Leg(svalue[15],svalue[16],svalue[17]);
				break;

		case 2:	
			  First_Leg(svalue[0]-200,svalue[1],svalue[2]);		 
			     Third_Leg(svalue[6]+200,svalue[7],svalue[8]);
				Fifth_Leg(svalue[12]+200,svalue[13],svalue[14]);
				break; 

		case 3: 
			  Second_Leg(svalue[3],svalue[4]-200,svalue[5]+200);	
				Fourth_Leg(svalue[9],svalue[10]-200,svalue[11]+200);
				Sixth_Leg(svalue[15],svalue[16]+200,svalue[17]-200);

				break;

		case 4:	 
			  Second_Leg(svalue[3]+200,svalue[4]-200,svalue[5]+200);	
				Fourth_Leg(svalue[9]-200,svalue[10]-200,svalue[11]+200);
				Sixth_Leg(svalue[15]+200,svalue[16]+200,svalue[17]-200);

				First_Leg(svalue[0],svalue[1],svalue[2]);	
				Third_Leg(svalue[6],svalue[7],svalue[8]);
				Fifth_Leg(svalue[12],svalue[13],svalue[14]);
				break;

		case 5: 
			  Second_Leg(svalue[3]+200,svalue[4],svalue[5]);
				Fourth_Leg(svalue[9]-200,svalue[10],svalue[11]);
				Sixth_Leg(svalue[15]+200,svalue[16],svalue[17]);
				break;

		default: break;
	}
}

/************************���������ƺ���********************************************/
void Left_Tynanize(void)
{
	switch(xflag)											//�ظ������岽�������
	{
		
		//�޸����ڣ�12.28
		
		
	case 0:		//վ���ڽ�
	First_Leg(svalue[0]-200,svalue[1]-500,svalue[2]-400); 
	Second_Leg(svalue[3]+200,svalue[4]+500,svalue[5]+400);
	Third_Leg(svalue[6],svalue[7]+500,svalue[8]+400);	
	Fourth_Leg(svalue[9],svalue[10]+500,svalue[11]+400);
	Fifth_Leg(svalue[12],svalue[13]-500,svalue[14]-400);
	Sixth_Leg(svalue[15],svalue[16]-500,svalue[17]-400);

		break;
		
			case 1:		
	First_Leg(svalue[0]-200,svalue[1]-300,svalue[2]-400); 
	Third_Leg(svalue[6],svalue[7]+300,svalue[8]+400);			
	Fifth_Leg(svalue[12],svalue[13]-300,svalue[14]-400);
	Second_Leg(svalue[3]+200,svalue[4]+500,svalue[5]+400);
	Fourth_Leg(svalue[9],svalue[10]+500,svalue[11]+400);
	Sixth_Leg(svalue[15],svalue[16]-500,svalue[17]-400);
		break;
		
		
			case 2:		
		First_Leg(svalue[0]-200,svalue[1]-300,svalue[2]+100); 
		Third_Leg(svalue[6],svalue[7]+400,svalue[8]+600);	
		Fifth_Leg(svalue[12],svalue[13]-300,svalue[14]);
		Second_Leg(svalue[3]+200,svalue[4]+500,svalue[5]+400);
		Fourth_Leg(svalue[9],svalue[10]+500,svalue[11]+400);
		Sixth_Leg(svalue[15],svalue[16]-500,svalue[17]-400);
		break;
					
			case 3:	
	First_Leg(svalue[0]-200,svalue[1]-600,svalue[2]+100); 
	Third_Leg(svalue[6],svalue[7]+700,svalue[8]+600);
	Fifth_Leg(svalue[12],svalue[13]-600,svalue[14]);			
	Second_Leg(svalue[3]+200,svalue[4]+500,svalue[5]+400);
	Fourth_Leg(svalue[9],svalue[10]+500,svalue[11]+400);
	Sixth_Leg(svalue[15],svalue[16]-500,svalue[17]-400);
		break;
					
					
			case 4:		
	First_Leg(svalue[0]-200,svalue[1]-500,svalue[2]-400); 
	Third_Leg(svalue[6],svalue[7]+600,svalue[8]+200);	
	Fifth_Leg(svalue[12],svalue[13]-500,svalue[14]-400);
	Second_Leg(svalue[3]+200,svalue[4]+500,svalue[5]+400);
	Fourth_Leg(svalue[9],svalue[10]+500,svalue[11]+400);
	Sixth_Leg(svalue[15],svalue[16]-500,svalue[17]-400);	
		break;
			
				case 5:		
	First_Leg(svalue[0]-200,svalue[1]-500,svalue[2]-400);
  Third_Leg(svalue[6],svalue[7]+500,svalue[8]+400);
	Fifth_Leg(svalue[12],svalue[13]-500,svalue[14]-400);				
	Second_Leg(svalue[3]+200,svalue[4]+300,svalue[5]+400);
	Fourth_Leg(svalue[9],svalue[10]+300,svalue[11]+400);
	Sixth_Leg(svalue[15],svalue[16]-300,svalue[17]-400);
		break;
			
		case 6:		
	First_Leg(svalue[0]-200,svalue[1]-500,svalue[2]-400);
  Third_Leg(svalue[6],svalue[7]+500,svalue[8]+400);
	Fifth_Leg(svalue[12],svalue[13]-500,svalue[14]-400);				
	Second_Leg(svalue[3]+200,svalue[4]+300,svalue[5]+600);
	Fourth_Leg(svalue[9],svalue[10]+300,svalue[11]+600);
	Sixth_Leg(svalue[15],svalue[16]-300,svalue[17]-100);
		break;
		
				case 7:		
	First_Leg(svalue[0]-200,svalue[1]-500,svalue[2]-400);
  Third_Leg(svalue[6],svalue[7]+500,svalue[8]+400);
	Fifth_Leg(svalue[12],svalue[13]-500,svalue[14]-400);				
	Second_Leg(svalue[3]+200,svalue[4]+600,svalue[5]+600);
	Fourth_Leg(svalue[9],svalue[10]+600,svalue[11]+600);
	Sixth_Leg(svalue[15],svalue[16]-600,svalue[17]-100);
		break;
		
				case 8:		
	First_Leg(svalue[0]-200,svalue[1]-500,svalue[2]-400);
  Third_Leg(svalue[6],svalue[7]+500,svalue[8]+400);
	Fifth_Leg(svalue[12],svalue[13]-500,svalue[14]-400);				
	Second_Leg(svalue[3]+200,svalue[4]+600,svalue[5]+400);
	Fourth_Leg(svalue[9],svalue[10]+600,svalue[11]+400);
	Sixth_Leg(svalue[15],svalue[16]-600,svalue[17]-500);
		break;
				
		case 9:		
	First_Leg(svalue[0]-200,svalue[1]-500,svalue[2]-400); 
	Second_Leg(svalue[3]+200,svalue[4]+500,svalue[5]+400);
	Third_Leg(svalue[6],svalue[7]+500,svalue[8]+400);	
	Fourth_Leg(svalue[9],svalue[10]+500,svalue[11]+400);
	Fifth_Leg(svalue[12],svalue[13]-500,svalue[14]-400);
	Sixth_Leg(svalue[15],svalue[16]-500,svalue[17]-400);
		break;

		default: break;
	}
}
/**********************************************************************************/
/************************���������ƺ���********************************************/
void Right_Tynanize(void)
{
	switch(yflag)
	{
				//�޸����ڣ�12.28
		
		
	case 0:		//վ���ڽ�
	First_Leg(svalue[0]-200,svalue[1]-500,svalue[2]-400); 
	Second_Leg(svalue[3]+200,svalue[4]+500,svalue[5]+400);
	Third_Leg(svalue[6],svalue[7]+500,svalue[8]+400);	
	Fourth_Leg(svalue[9],svalue[10]+500,svalue[11]+400);
	Fifth_Leg(svalue[12],svalue[13]-500,svalue[14]-400);
	Sixth_Leg(svalue[15],svalue[16]-500,svalue[17]-400);

		break;
		
			case 1:		
	First_Leg(svalue[0]-200,svalue[1]-300,svalue[2]-400); 
	Third_Leg(svalue[6],svalue[7]+300,svalue[8]+400);			
	Fifth_Leg(svalue[12],svalue[13]-300,svalue[14]-400);
	Second_Leg(svalue[3]+200,svalue[4]+500,svalue[5]+400);
	Fourth_Leg(svalue[9],svalue[10]+500,svalue[11]+400);
	Sixth_Leg(svalue[15],svalue[16]-500,svalue[17]-400);
		break;
		
		
			case 2:		
		First_Leg(svalue[0]-200,svalue[1]-300,svalue[2]-800); 
		Third_Leg(svalue[6],svalue[7]+400,svalue[8]+200);	
		Fifth_Leg(svalue[12],svalue[13]-300,svalue[14]-800);
		Second_Leg(svalue[3]+200,svalue[4]+500,svalue[5]+400);
		Fourth_Leg(svalue[9],svalue[10]+500,svalue[11]+400);
		Sixth_Leg(svalue[15],svalue[16]-500,svalue[17]-400);
		break;
					
			case 3:	
	First_Leg(svalue[0]-200,svalue[1]-600,svalue[2]-800); 
	Third_Leg(svalue[6],svalue[7]+700,svalue[8]+200);
	Fifth_Leg(svalue[12],svalue[13]-600,svalue[14]-800);			
	Second_Leg(svalue[3]+200,svalue[4]+500,svalue[5]+400);
	Fourth_Leg(svalue[9],svalue[10]+500,svalue[11]+400);
	Sixth_Leg(svalue[15],svalue[16]-500,svalue[17]-400);
		break;
					
					
			case 4:		
	First_Leg(svalue[0]-200,svalue[1]-500,svalue[2]-400); 
	Third_Leg(svalue[6],svalue[7]+600,svalue[8]+400);	
	Fifth_Leg(svalue[12],svalue[13]-500,svalue[14]-400);
	Second_Leg(svalue[3]+200,svalue[4]+500,svalue[5]+400);
	Fourth_Leg(svalue[9],svalue[10]+500,svalue[11]+400);
	Sixth_Leg(svalue[15],svalue[16]-500,svalue[17]-400);	
		break;
			
				case 5:		
	First_Leg(svalue[0]-200,svalue[1]-500,svalue[2]-400);
  Third_Leg(svalue[6],svalue[7]+500,svalue[8]+400);
	Fifth_Leg(svalue[12],svalue[13]-500,svalue[14]-400);				
	Second_Leg(svalue[3]+200,svalue[4]+300,svalue[5]+400);
	Fourth_Leg(svalue[9],svalue[10]+300,svalue[11]+400);
	Sixth_Leg(svalue[15],svalue[16]-300,svalue[17]-400);
		break;
			
		case 6:		
	First_Leg(svalue[0]-200,svalue[1]-500,svalue[2]-400);
  Third_Leg(svalue[6],svalue[7]+500,svalue[8]+400);
	Fifth_Leg(svalue[12],svalue[13]-500,svalue[14]-400);				
	Second_Leg(svalue[3]+200,svalue[4]+300,svalue[5]+200);
	Fourth_Leg(svalue[9],svalue[10]+300,svalue[11]+200);
	Sixth_Leg(svalue[15],svalue[16]-300,svalue[17]-600);
		break;
		
				case 7:		
	First_Leg(svalue[0]-200,svalue[1]-500,svalue[2]-400);
  Third_Leg(svalue[6],svalue[7]+500,svalue[8]+400);
	Fifth_Leg(svalue[12],svalue[13]-500,svalue[14]-400);				
	Second_Leg(svalue[3]+200,svalue[4]+600,svalue[5]+200);
	Fourth_Leg(svalue[9],svalue[10]+600,svalue[11]+200);
	Sixth_Leg(svalue[15],svalue[16]-600,svalue[17]-600);
		break;
		
				case 8:		
	First_Leg(svalue[0]-200,svalue[1]-500,svalue[2]-400);
  Third_Leg(svalue[6],svalue[7]+500,svalue[8]+400);
	Fifth_Leg(svalue[12],svalue[13]-500,svalue[14]-400);				
	Second_Leg(svalue[3]+200,svalue[4]+600,svalue[5]+400);
	Fourth_Leg(svalue[9],svalue[10]+600,svalue[11]+400);
	Sixth_Leg(svalue[15],svalue[16]-600,svalue[17]-500);
		break;
				
		case 9:		
	First_Leg(svalue[0]-200,svalue[1]-500,svalue[2]-400); 
	Second_Leg(svalue[3]+200,svalue[4]+500,svalue[5]+400);
	Third_Leg(svalue[6],svalue[7]+500,svalue[8]+400);	
	Fourth_Leg(svalue[9],svalue[10]+500,svalue[11]+400);
	Fifth_Leg(svalue[12],svalue[13]-500,svalue[14]-400);
	Sixth_Leg(svalue[15],svalue[16]-500,svalue[17]-400);
		break;

		default: break;
	}
}


/************************�����˶�����******************************************/
void Control_Action(void)
{
	switch(date[2])
	{
		case 'A':	Forward();							
					break;									//������ǰ��
		
		case 'B': 	Draw_Back();						
					break;									//�����˺���
				
		case 'C': 	flag=0;								
					Turn_Left();							//��������ת
					break;
					
		case 'D': 	Turn_Right();					
					break;									//��������ת

		case '1': 	flag=0;								
					Left_Tynanize();						//����������
					break;				

		case '2':	flag=1;							
					break;									//�����˽��볬��������ģʽ

		case '3': 	flag=0;							
					Right_Tynanize();					   //����������
					break;

		default:	Stand();							   //�������
					break;								   //�����˱���վ������״̬
	}
}


/************************���Ʋ������º���******************************************/ 
void Update(void)
{
	switch(date[2])
	{
		case 'A': 	fflag++;
						
					if(fflag>5)							   //�����������5
					{
						fflag=0;						   //���㲽������
					}
					break;
					
		case 'B': 	dflag++;							   //����ͬ��
						
					if(dflag>5)
					{
						dflag=0;
					}
					break;
						   
		case 'C': 	lflag++;							   //����ͬ��
						
					if(lflag>5)
					{
						lflag=0;
					}
					break;			

		case 'D': 	rflag++;							   //����ͬ��
						
					if(rflag>5)
					{
						rflag=0;
					}
					break;
		
		case '1': 	xflag++;							   //����ͬ��
											
					if(xflag>9)
					{
						xflag=1;
					}
					break;

		case '2':	flag=1;								   //����ͬ��
					break;

		case '3': 	yflag++;							   //����ͬ��
						
					if(yflag>9)
					{
						yflag=1;
					}
					break;
						
		default:	break;								  //���������˳�
	}	
}



