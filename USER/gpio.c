#include "gpio.h"
#include "usart1.h"
#include "SysTick.h"

#define MAXPWM   2500						 //舵机最大PWM控制脉宽2.5ms宏定义
#define CENTER1  1500					 //初始化舵机角度值宏定义
#define CENTER2  1500           //定时器每隔1us累加一次
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

 //该数组用于保存机器人站立PWM脉冲宽度值
int svalue[18]={CENTER1,CENTER2,CENTER3,CENTER4,CENTER5,CENTER6,	 
                CENTER7,CENTER8,CENTER9,CENTER10,CENTER11,CENTER12,
                CENTER13,CENTER14,CENTER15,CENTER16,CENTER17,CENTER18};

extern u8 date[3];
u8 num1,num2,num3;						 //每个变量用作延时累加
u8 count1,count2,count3;				 //每个变量用作8路舵机先后赋值控制
u8 sflag,sflag1,sflag2,sflag3;			 //每个变量用作判断8路舵机是否转动预期角度
u8 flag,fflag,dflag,lflag,rflag,xflag,yflag;//每个变量用作控制机器人动作顺序运行
int sdate[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//该数组用于保存差值
int PWM[24]={1500,1500,1400,1500,1500,1500,1600,1500,	  //该数组用于保存预期PWM脉冲宽度值
			 1500,1600,1500,1500,1500,1500,1500,1500,
			 1500,1500,1500,1500,1500,1500,1500,1500};
int CPWM[24]={1500,1500,1400,1500,1500,1500,1600,1500,	  //该数组用于保存当前PWM脉冲宽度值
			 1500,1600,1500,1500,1500,1500,1500,1500,
			 1500,1500,1500,1500,1500,1500,1500,1500};
int LPWM[24]={1500,1500,1400,1500,1500,1500,1600,1500,	  //该数组用于保存上次PWM脉冲宽度值
			 1500,1600,1500,1500,1500,1500,1500,1500,
			 1500,1500,1500,1500,1500,1500,1500,1500};

void GPIO_Config(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);//开启外设时钟

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//蜂鸣器
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2; //第一条腿，最底端舵机，中间舵机，上端舵机
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;//第二条腿，最底端舵机，中间舵机，上端舵机
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12;//第三条腿，最底端舵机，中间舵机，上端舵机
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;//第四条，最底端舵机，中间舵机，上端舵机
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;//第五条，最底端舵机，中间舵机，上端舵机
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;//第六条，最底端舵机，中间舵机，上端舵机
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
//		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);

}

void alert1(void)
{
	GPIO_SetBits(GPIOD,GPIO_Pin_2);			 //蜂鸣器发出滴答滴答报警信号
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	Delay_ms(100); 			
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
	Delay_ms(100);
	GPIO_SetBits(GPIOD,GPIO_Pin_2);			 //蜂鸣器发出滴答滴答报警信号
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
    u8 symbol=0x00;           	//symbol1为舵机转到预定位置标记，CPWM为当前PWM脉冲值   
    
    if(CPWM[0]==PWM[0])			//如果CPWM[0]=PWM[0],表明第一个舵机已转到预期角度，此时用symbol1做标记
    {
        symbol=symbol|0x01;
        LPWM[0]=CPWM[0];
    }
    else						//否则，清除标记
    {
        symbol=symbol&0x00;
    }
    if(CPWM[1]==PWM[1])			//同上
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
       
    if(symbol==0xff)			//如果symbol1=0xff，表明8路舵机已经全部转到预期位置，此时用sflag1做标记
    {
        sflag1=0x01;
    }
    else
    {
        sflag1=0x00;
    }

	sflag=sflag1&sflag2&sflag3;	

	if(sflag)					//如果sflag=1，表明24路舵机已全部转到预期位置
	{
		Update();				//此时开始更新数据

		Control_Action();	    //并根据数据控制机器人动作
	}	

}
/**********************************************************************************/
/************************舵机调速函数2*********************************************/
void Decode_Two(void)
{
    u8 symbol=0x00;            	//功能同上
    
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
/************************舵机调速函数3*********************************************/
void Decode_Three(void)
{
    u8 symbol=0x00;           	//功能同上  
    
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
/*********************舵机角度数值计算函数1****************************************/
void Date_Calculate_One(void)
{
    sdate[0]=(PWM[0]-LPWM[0])/4;//将预期舵机PWM脉宽值分成四次执行 
    sdate[1]=(PWM[1]-LPWM[1])/4;//这样既可让舵机平滑运转，也让机器人动作流畅
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
/*********************舵机角度数值计算函数2****************************************/
void Date_Calculate_Two(void)
{
    sdate[8]=(PWM[8]-LPWM[8])/4;  //功能同上
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
/*********************舵机角度数值计算函数3****************************************/
void Date_Calculate_Three(void)
{
    sdate[16]=(PWM[16]-LPWM[16])/4;//功能同上 
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
/************************GPIO电平反转函数1*****************************************/ 
void Flip_GPIO_One(void)
{
	switch(count1)								 //将20ms的舵机控制周期分成8份，每2.5ms控制一个舵机运转
	{  											 //每个定时器控制8路舵机运转，3个定时器控制24路舵机运转
		case 1: TIM2->ARR=CPWM[0];				 //将第一个舵机脉冲宽度值赋值给定时器2
				GPIO_SetBits(GPIOA,GPIO_Pin_0);  //同时拉高控制舵机1的引脚的电平
				break;
		
		case 2:	TIM2->ARR=MAXPWM-CPWM[0]; 		 //将2.5ms减去PWM脉宽值的数据赋值定时器2
				GPIO_ResetBits(GPIOA,GPIO_Pin_0);//同时拉低控制舵机1引脚的电平 
				break;							 //控制舵机1的引脚在剩下20ms-CPM[0]时间内将一直保持低电平，舵机1按照CPWM值转动

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
/************************GPIO电平反转函数2*****************************************/ 
void Flip_GPIO_Two(void)
{
	switch(count2)
	{  		
		case 1: TIM3->ARR=CPWM[8];				 //功能同上
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
/************************GPIO电平反转函数3*****************************************/ 
void Flip_GPIO_Three(void)
{
	switch(count3)
	{  		
		case 1: TIM4->ARR=CPWM[16];				 //功能同上
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
/************************舵机控制函数1*********************************************/
void Servo1(void)
{		
	num1++;									 //num变量用来累加计数，调节舵机转速

	count1++;

	if(!(num1%50))							 //舵机脉冲宽度值每隔一段时间更新一次
	{	
		num1=0;
			
		Date_Calculate_One();				 //舵机角度计算

		Decode_One();						 //舵机调速
		
	}
	 
	Flip_GPIO_One();						 //反转IO电平

}

/************************舵机控制函数2*********************************************/
void Servo2(void)
{		
	num2++;									 //功能同上

	count2++;

	if(!(num2%50))
	{	
		num2=0;
			
		Date_Calculate_Two();		

		Decode_Two();
	
	}
	
	Flip_GPIO_Two();
}

/************************舵机控制函数4*********************************************/
void Servo3(void)
{		
	num3++;									 //功能同上

	count3++;

	if(!(num3%50))		 
	{	
		num3=0;
			
		Date_Calculate_Three();	

		Decode_Three();
		
	}
	
	Flip_GPIO_Three();

}

/*******************以下是动作函数*************************************************/
void Stand(void)
{

	fflag=dflag=lflag=0;					  //清空所有动作计数
	xflag=yflag=rflag=0;

	First_Leg(svalue[0],svalue[1],svalue[2]); 
	Second_Leg(svalue[3],svalue[4],svalue[5]);
	Third_Leg(svalue[6],svalue[7],svalue[8]);	
	Fourth_Leg(svalue[9],svalue[10],svalue[11]);
	Fifth_Leg(svalue[12],svalue[13],svalue[14]);
	Sixth_Leg(svalue[15],svalue[16],svalue[17]);
		
}


/*注：备注可能有错 思路就是 ：
把六条腿分为两组，采用三角步态
对于前进:第一组 抬起，然后向前转动一个角度 ，再落地。
完成这些动作前，第二组不动，并起着支撑机器人的作用。
第一组完成后，第二组腿执行类似第一组的动作。

腿编号 1st 3rd 5th 为第一组 ； 2ed 4th 6th 为第二组

1：PA0 PA1 PA2
（其他腿的对应关系见GPIO配置）


每条腿由三个定时器产生的模拟PWM控制，每个定时器产生了八路PWM波（实际每个只用了六路）
每条腿都有一个对应的函数 ，第一个变量对应内侧舵机 ，第二个变量对应外侧舵机， 第三个变量对应上部舵机

TIM2对应 底部内侧的舵机  该舵机决定前进
TIM3对应 底部外侧的舵机  该舵机决定抬脚
TIM4对应 上部的舵机      该舵机决定落脚 也有辅助抬脚

若要优化动作，在每条腿的函数里，svalue[x]+/-一个数即可 
如：觉得第一条腿外侧的舵机应该旋转大一点的角度，可试着 svalue[1]+200 改为 svalue[1]+250
（由于每个舵机安装的正反不一致，可能svalue[1]+150 才是旋转大一点的角度）
（舵机安装时考虑过这个问题，感觉目前是最好的方式了）

*/
void Forward(void)
{
	switch(fflag)
	{
		case 0:	
//第一步：第一组腿抬脚	
First_Leg(svalue[0],svalue[1]+200,svalue[2]-200);	
Third_Leg(svalue[6],svalue[7]-200,svalue[8]+200);
Fifth_Leg(svalue[12],svalue[13]+200,svalue[14]-200);
				break;

		case 1:
//第二步：第一组腿前移	
First_Leg(svalue[0]-200,svalue[1]+200,svalue[2]-200);	
Third_Leg(svalue[6]+200,svalue[7]-200,svalue[8]+200);
Fifth_Leg(svalue[12]-200,svalue[13]+200,svalue[14]-200);

//第二步：第二组腿归位站立
Second_Leg(svalue[3],svalue[4],svalue[5]);	
Fourth_Leg(svalue[9],svalue[10],svalue[11]);
Sixth_Leg(svalue[15],svalue[16],svalue[17]);
				break;

		case 2:	
//第三步：第一组腿落脚	
First_Leg(svalue[0]-200,svalue[1],svalue[2]);	
Third_Leg(svalue[6]+200,svalue[7],svalue[8]);
Fifth_Leg(svalue[12]-200,svalue[13],svalue[14]);
				break; 

		case 3: 
//第四步：第二组腿抬脚	
Second_Leg(svalue[3],svalue[4]-200,svalue[5]+200);	
Fourth_Leg(svalue[9],svalue[10]-200,svalue[11]+200);
Sixth_Leg(svalue[15],svalue[16]+200,svalue[17]-200);
				break;

		case 4:	
//第五步：第二组腿前移
Second_Leg(svalue[3]+200,svalue[4]-200,svalue[5]+200);	
Fourth_Leg(svalue[9]+200,svalue[10]-200,svalue[11]+200);
Sixth_Leg(svalue[15]-200,svalue[16]+200,svalue[17]-200);

//第五步：同时第一组腿归位
First_Leg(svalue[0],svalue[1],svalue[2]);	
Third_Leg(svalue[6],svalue[7],svalue[8]);
Fifth_Leg(svalue[12],svalue[13],svalue[14]);			
				break;

		case 5:
//第六步：第二组腿落脚
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
//第一步：第一组腿抬脚	
First_Leg(svalue[0],svalue[1]+200,svalue[2]-200);	
Third_Leg(svalue[6],svalue[7]-200,svalue[8]+200);
Fifth_Leg(svalue[12],svalue[13]+200,svalue[14]-200);
				break;

		case 1:
//第二步：第一组腿前移	
First_Leg(svalue[0]+200,svalue[1]+200,svalue[2]-200);	
Third_Leg(svalue[6]-200,svalue[7]-200,svalue[8]+200);
Fifth_Leg(svalue[12]+200,svalue[13]+200,svalue[14]-200);

//第二步：第二组腿归位站立
Second_Leg(svalue[3],svalue[4],svalue[5]);	
Fourth_Leg(svalue[9],svalue[10],svalue[11]);
Sixth_Leg(svalue[15],svalue[16],svalue[17]);
				break;

		case 2:	
//第三步：第一组腿落脚	
First_Leg(svalue[0]+200,svalue[1],svalue[2]);	
Third_Leg(svalue[6]-200,svalue[7],svalue[8]);
Fifth_Leg(svalue[12]+200,svalue[13],svalue[14]);
				break; 

		case 3: 
//第四步：第二组腿抬脚	
Second_Leg(svalue[3],svalue[4]-200,svalue[5]+200);	
Fourth_Leg(svalue[9],svalue[10]-200,svalue[11]+200);
Sixth_Leg(svalue[15],svalue[16]+200,svalue[17]-200);
				break;

		case 4:	
//第五步：第二组腿前移
Second_Leg(svalue[3]-200,svalue[4]-200,svalue[5]+200);	
Fourth_Leg(svalue[9]-200,svalue[10]-200,svalue[11]+200);
Sixth_Leg(svalue[15]+200,svalue[16]+200,svalue[17]-200);

//第五步：同时第一组腿归位
First_Leg(svalue[0],svalue[1],svalue[2]);	
Third_Leg(svalue[6],svalue[7],svalue[8]);
Fifth_Leg(svalue[12],svalue[13],svalue[14]);			
				break;

		case 5:
//第六步：第二组腿落脚
Second_Leg(svalue[3]-200,svalue[4],svalue[5]);	
Fourth_Leg(svalue[9]-200,svalue[10],svalue[11]);
Sixth_Leg(svalue[15]+200,svalue[16],svalue[17]);	
				break;

		default: break;
	}
}

/************************机器人左转函数********************************************/
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

/************************机器人右转函数********************************************/
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

/************************机器人左移函数********************************************/
void Left_Tynanize(void)
{
	switch(xflag)											//重复二至五步完成行走
	{
		
		//修改日期：12.28
		
		
	case 0:		//站立踮脚
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
/************************机器人右移函数********************************************/
void Right_Tynanize(void)
{
	switch(yflag)
	{
				//修改日期：12.28
		
		
	case 0:		//站立踮脚
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


/************************控制运动函数******************************************/
void Control_Action(void)
{
	switch(date[2])
	{
		case 'A':	Forward();							
					break;									//机器人前进
		
		case 'B': 	Draw_Back();						
					break;									//机器人后退
				
		case 'C': 	flag=0;								
					Turn_Left();							//机器人左转
					break;
					
		case 'D': 	Turn_Right();					
					break;									//机器人右转

		case '1': 	flag=0;								
					Left_Tynanize();						//机器人左走
					break;				

		case '2':	flag=1;							
					break;									//机器人进入超声波避障模式

		case '3': 	flag=0;							
					Right_Tynanize();					   //机器人右走
					break;

		default:	Stand();							   //其它情况
					break;								   //机器人保持站立待命状态
	}
}


/************************控制参数更新函数******************************************/ 
void Update(void)
{
	switch(date[2])
	{
		case 'A': 	fflag++;
						
					if(fflag>5)							   //如果步数大于5
					{
						fflag=0;						   //清零步数计数
					}
					break;
					
		case 'B': 	dflag++;							   //功能同上
						
					if(dflag>5)
					{
						dflag=0;
					}
					break;
						   
		case 'C': 	lflag++;							   //功能同上
						
					if(lflag>5)
					{
						lflag=0;
					}
					break;			

		case 'D': 	rflag++;							   //功能同上
						
					if(rflag>5)
					{
						rflag=0;
					}
					break;
		
		case '1': 	xflag++;							   //功能同上
											
					if(xflag>9)
					{
						xflag=1;
					}
					break;

		case '2':	flag=1;								   //功能同上
					break;

		case '3': 	yflag++;							   //功能同上
						
					if(yflag>9)
					{
						yflag=1;
					}
					break;
						
		default:	break;								  //其它情形退出
	}	
}



