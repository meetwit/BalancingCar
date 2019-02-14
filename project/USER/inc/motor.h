#ifndef __MOTOR_H
#define __MOTOR_H
#include "main.h"	 


#define PWMA1   TIM1->CCR1
#define PWMA2   TIM1->CCR2
#define PWMB1   TIM1->CCR3
#define PWMB2   TIM1->CCR4

#define left_Clockwise 1
#define left_Anti_clockwise 2
#define right_Clockwise 3
#define right_Anti_clockwise 4
#define stop 0
void PWM_Init_TIM1(u16 arr,u16 psc);
void motor_run(u8 left_right_direct, u8 Percentage);
void selfCorrecting(u8 switchOne,u8 type,s32 target);
int balance(float Angle,float Gyro);

#endif
