#include "main.h"


void PWM_Init_TIM1(u16 arr,u16 psc)
{		 					 
	RCC->APB2ENR|=1<<11;       //使能TIM1时钟    
	RCC->APB2ENR|=1<<2;        //PORTA时钟使能   
	RCC->APB2ENR|=1<<3;        //PORTB时钟使能     
	GPIOA->CRH&=0XFFFF0FF0;    //PORTA8 11复用输出
	GPIOA->CRH|=0X0000B00B;    //PORTA8 11复用输出  
	GPIOB->CRH&=0X00FFFFFF;    //PORTB14,B15复用输出
	GPIOB->CRH|=0XBB000000;    //PORT复用输出
	
	TIM1->ARR=arr;             //设定计数器自动重装值 
	TIM1->PSC=psc;             //预分频器不分频
	TIM1->CCMR1|=6<<4;         //CH1 PWM1模式	
	TIM1->CCMR1|=6<<12;        //CH2 PWM1模式	
	TIM1->CCMR2|=6<<4;         //CH3 PWM1模式	
	TIM1->CCMR2|=6<<12;        //CH4 PWM1模式	
	TIM1->CCMR1|=1<<3;         //CH1预装载使能	  
	TIM1->CCMR1|=1<<11;        //CH2预装载使能	 
	TIM1->CCMR2|=1<<3;         //CH3预装载使能	  
	TIM1->CCMR2|=1<<11;        //CH4预装载使能	   
	TIM1->CCER|=1<<0;          //CH1输出使能	
	TIM1->CCER|=1<<6; //TIM1->CCER|=1<<4;         //CH2输出使能	 
	TIM1->CCER|=1<<10;//TIM1->CCER|=1<<8;          //CH3输出使能	
	TIM1->CCER|=1<<12;         //CH4输出使能	 
	TIM1->BDTR |= 1<<15;       //TIM1必须要这句话才能输出PWM
	TIM1->CR1 |=1<<7;          //ARPE使能 
	TIM1->CR1|=0x01;          //使能定时器1 											  
} 

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
	//printf("PWMMAX=%d,temp=%d\r\n",PWMMAX,temp);
	/**
#define left_Clockwise 1
#define left_Anti_clockwise 2
#define right_Clockwise 3
#define right_Anti_clockwise 4
	**/
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

void selfCorrecting(u8 switchOne,u8 type,s32 target){
	u8 temp=1;
	if(type==1){		//位置
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
