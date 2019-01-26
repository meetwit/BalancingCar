#include "main.h"

/*
��������PWM_Init_TIM1
��	�Σ�u16 arr,u16 psc Ƶ��
����ֵ����
��	�ã���ʼ����ʱ��1����·PWM���
��	�ڣ�2019��1��26��
��	�ߣ�meetwit
*/
void PWM_Init_TIM1(u16 arr,u16 psc)
{		 					 
	RCC->APB2ENR|=1<<11;       //ʹ��TIM1ʱ��    
	RCC->APB2ENR|=1<<2;        //PORTAʱ��ʹ��   
	RCC->APB2ENR|=1<<3;        //PORTBʱ��ʹ��     
	GPIOA->CRH&=0XFFFF0FF0;    //PORTA8 11�������
	GPIOA->CRH|=0X0000B00B;    //PORTA8 11�������  
	GPIOB->CRH&=0X00FFFFFF;    //PORTB14,B15�������
	GPIOB->CRH|=0XBB000000;    //PORT�������
	
	TIM1->ARR=arr;             //�趨�������Զ���װֵ 
	TIM1->PSC=psc;             //Ԥ��Ƶ������Ƶ
	TIM1->CCMR1|=6<<4;         //CH1 PWM1ģʽ	
	TIM1->CCMR1|=6<<12;        //CH2 PWM1ģʽ	
	TIM1->CCMR2|=6<<4;         //CH3 PWM1ģʽ	
	TIM1->CCMR2|=6<<12;        //CH4 PWM1ģʽ	
	TIM1->CCMR1|=1<<3;         //CH1Ԥװ��ʹ��	  
	TIM1->CCMR1|=1<<11;        //CH2Ԥװ��ʹ��	 
	TIM1->CCMR2|=1<<3;         //CH3Ԥװ��ʹ��	  
	TIM1->CCMR2|=1<<11;        //CH4Ԥװ��ʹ��	   
	TIM1->CCER|=1<<0;          //CH1���ʹ��	
	TIM1->CCER|=1<<6; //TIM1->CCER|=1<<4;         //CH2���ʹ��	 
	TIM1->CCER|=1<<10;//TIM1->CCER|=1<<8;          //CH3���ʹ��	
	TIM1->CCER|=1<<12;         //CH4���ʹ��	 
	TIM1->BDTR |= 1<<15;       //TIM1����Ҫ��仰�������PWM
	TIM1->CR1 |=1<<7;          //ARPEʹ�� 
	TIM1->CR1|=0x01;          //ʹ�ܶ�ʱ��1 											  
} 

/*
��������motor_run
��	�Σ�u8 left_right_direct ����, u8 Percentage ���ת������
����ֵ����
��	�ã������1��2�ǵ��1���������򣬷������3��4�ǵ��2���������򣬵���0ֹͣ
��	�ڣ�2019��1��26��
��	�ߣ�meetwit
*/
void motor_run(u8 left_right_direct, u8 Percentage){
	u16 temp,temp_N;
	
	if(Percentage>100){
		Percentage=100;
	}else if(Percentage>=10&&Percentage<=100){
		;
	}else if(Percentage>3&&Percentage<10){
		Percentage = 10;
	}else{
		Percentage=0;
	}
	
	temp=Percentage*PWMMAX/100;
	temp_N=PWMMAX-temp;
	
	if(left_right_direct==1){
		PWMA1=temp_N;
		PWMA2=PWMMAX;
	}else if(left_right_direct==2){
		PWMA1=PWMMAX;
		PWMA2=temp_N;
	}else if(left_right_direct==3){
		PWMB1=temp_N;
		PWMB2=PWMMAX;
	}else if(left_right_direct==4){
		PWMB1=PWMMAX;
		PWMB2=temp_N;
	}else{//stop
		PWMA1=0;
		PWMA2=0;
		PWMB1=0;
		PWMB2=0;	
	}
	
	
}

/*
��������selfCorrecting
��	�Σ�u8 switchOne ѡ��һ�����,u8 type ���Ʒ�ʽ,s32 target Ŀ��ֵ
����ֵ����
��	�ã�switchOne���ڡ�l����ߵ�������ڡ�r���ұߵ����
				type����1��λ�ÿ��ƣ�
				target��Ŀ��ֵ��λ�ÿ���ʱ�Ǳ���������������6�������һȦ
��	�ڣ�2019��1��26��
��	�ߣ�meetwit
*/
void selfCorrecting(u8 switchOne,u8 type,s32 target){
	u8 temp=1;
	if(type==1){		//λ��
		if(target<0){
			target = -target;
			temp=2;
		}
		if(switchOne=='r'){
		temp=temp+2;
		}
		motor_run(temp,target/600);
	}
}
