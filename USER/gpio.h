#ifndef __GPIO_H
#define	__GPIO_H

#include "stm32f10x.h"

void alert1(void);

void Servo1(void);
void Servo2(void);
void Servo3(void);
void Stand(void);
void Update(void);
void Forward(void);
void Draw_Back(void);
void Turn_Left(void);
void Turn_Right(void);
void Decode_One(void);
void Decode_Two(void);
void Decode_Three(void);
void GPIO_Config(void);
void Control_Action(void);
void Left_Tynanize(void);
void Right_Tynanize(void);

void First_Leg(int date1,int date2,int date3);
void Second_Leg(int date1,int date2,int date3);
void Third_Leg(int date1,int date2,int date3);
void Fourth_Leg(int date1,int date2,int date3);
void Fifth_Leg(int date1,int date2,int date3);
void Sixth_Leg(int date1,int date2,int date3);
#endif /* __GPIO_H */
