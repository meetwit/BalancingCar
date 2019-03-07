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
	
	switch (left_right_direct){
		case 0:
			PWMA1=0;
			PWMA2=0;
			PWMB1=0;
			PWMB2=0;
			break;
		case 1:
			PWMA1=temp_N;
			PWMA2=PWMMAX;
			break;
		case 2:
			PWMA1=PWMMAX;
			PWMA2=temp_N;
			break;
		case 3:
			PWMB1=temp_N;
			PWMB2=PWMMAX;
			break;
		case 4:
			PWMB1=PWMMAX;
			PWMB2=temp_N;
			break;
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

/**************************************************************************
�������ܣ�ֱ��PD����
��ڲ������Ƕȡ����ٶ�
����  ֵ��ֱ������PWM
**************************************************************************/
int balance_mw(float Angle,float Gyro)
{  
   float Bias,kp,kd;
	 int pwm;
   kp = m[0];
   kd = m[2];
	 Bias=Angle-0;       //===���ƽ��ĽǶ���ֵ �ͻ�е���
	 pwm=kp*Bias+Gyro*kd;   //===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	 return pwm;
}

/**************************************************************************
�������ܣ��ٶ�PI���� �޸�ǰ�������ٶȣ�����Target_Velocity�����磬�ĳ�60�ͱȽ�����
��ڲ��������ֱ����������ֱ�����
����  ֵ���ٶȿ���PWM
��    �ߣ�ƽ��С��֮��
**************************************************************************/
int velocity_mw(void)
{  
  static float Velocity,Encoder_Least,Encoder,Movement = 0;
	static float Encoder_Integral = 0,Target_Velocity = 0;
	float Velocity_Kp = 0,Velocity_Ki = 0;
	Velocity_Kp = m[3];
	Velocity_Ki = m[4];
	
	  //=============ң��ǰ�����˲���=======================// 
//	  if(Bi_zhang==1&&Flag_sudu==1)  Target_Velocity=45;                 //����������ģʽ,�Զ��������ģʽ
//    else 	                         Target_Velocity=110;                 
//		if(1==Flag_Qian)    	Movement=Target_Velocity/Flag_sudu;	         //===ǰ����־λ��1 
//		else if(1==Flag_Hou)	Movement=-Target_Velocity/Flag_sudu;         //===���˱�־λ��1
//	  else  Movement=0;	
//	  if(Bi_zhang==1&&Distance<500&&Flag_Left!=1&&Flag_Right!=1)        //���ϱ�־λ��1�ҷ�ң��ת���ʱ�򣬽������ģʽ
//	  Movement=-Target_Velocity/Flag_sudu;
   //=============�ٶ�PI������=======================//	
		Encoder_Least =(leftEncoder+rightEncoder)-0;                    //===��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩 
		Encoder *= 0.7;		                                                //===һ�׵�ͨ�˲���       
		Encoder += Encoder_Least*0.3;	                                    //===һ�׵�ͨ�˲���    
		Encoder_Integral +=Encoder;                                       //===���ֳ�λ�� ����ʱ�䣺10ms
		Encoder_Integral=Encoder_Integral-Movement;                       //===����ң�������ݣ�����ǰ������
		if(Encoder_Integral>10000)  	Encoder_Integral=10000;             //===�����޷�
		if(Encoder_Integral<-10000)	Encoder_Integral=-10000;              //===�����޷�	
		Velocity=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki;                          //===�ٶȿ���	
//		printf("Encoder=%f,Encoder_Integral=%f\r\n",Encoder,Encoder_Integral);
		leftEncoder = 0;
		rightEncoder = 0;
		if(m[8]>10)   Encoder_Integral=0;      //===����رպ��������
	  return Velocity;
}

