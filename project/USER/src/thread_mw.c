#include "main.h"

u16 imuCorrectCount=0,imuErrorCount=0,readEncodeCount=0,sendDataCount=0,controlMotorCount=0;		//��¼hz��

s32 leftEncoder=0,rightEncoder=0;

/*
��	�̣�readEncode
��	�ã���ȡ���ҵ��������������
Ƶ	�ʣ�100Hz
��	�ڣ�2019��1��26��
��	�ߣ�meetwit
*/
void readEncode(void* parameter){
	while(1){
		readEncodeCount++;
		leftEncoder += Read_Encoder(2);
		rightEncoder -= Read_Encoder(4);
		
		rt_thread_delay(1);
		rt_timer_check();
	}
	
}

/*
��	�̣�sendData
��	�ã�ʹ��printf��ͨ������3��ӡ���ݣ�Ҳ��ʹ��rt_kprintf,ͨ������1��ӡ����
Ƶ	�ʣ�10Hz
��	�ڣ�2019��1��26��
��	�ߣ�meetwit
*/
void sendData(void* parameter){

//	printf("d = %.2f\r\n\r\n",m[2]);
//		rt_thread_delay(1000);		//do something
//		rt_timer_check();
	
}


/*
��	�̣�controlMotor
��	�ã����Ƶ��
Ƶ	�ʣ�1000Hz
��	�ڣ�2019��1��26��
��	�ߣ�meetwit
*/
void controlMotor(void* parameter){
	float temp1,temp2,temp3;
	temp1 = 32768/180.0;			//�Ƕ�
	temp2 = 32768/2000.0;			//���ٶ�
	temp3 = 32768/16.0;				//�Ǽ��ٶ�
	int finalPwm = 0;
	while(1){
		controlMotorCount++;
			finalPwm = 0;
			finalPwm += balance_mw(stcAngle.Angle[0]/temp1,stcGyro.w[0]/temp2);
			finalPwm += velocity_mw();
		if(finalPwm>=0){
				motor_run(3,finalPwm); 
				motor_run(2,finalPwm);
		}else{
				motor_run(4,-finalPwm); 
				motor_run(1,-finalPwm);
		}
//		motor_run(1,20); 
//		motor_run(3,20); 
				
		/*
		2019��3��6�� 
		4.3 0 0.8	= 2.58 0 0.48
		4.6 0  0.31=2.76 0 0.186
		           = 5.4 0 0.2
		*/
	
		rt_thread_delay(5);
		rt_timer_check();
	}
	
}

void timer1_f(void* parameter){
	Rx_Tm3--;
	if(Rx_Tm3==1)
	Task_Pc3();
}

/*
��	�̣�time_thread
��	�ã�ͨ������1��ӡϵͳʱ��
Ƶ	�ʣ�1Hz
��	�ڣ�2019��1��26��
��	�ߣ�meetwit
*/
void time_thread(void* parameter){
	rt_tick_t tick_temp;
  rt_uint8_t h=0,m=0,s=0;
	
	while(1){
		tick_temp = rt_tick_get();
		s=tick_temp/RT_TICK_PER_SECOND%60;
		m=tick_temp/RT_TICK_PER_SECOND/60%60;
		h=tick_temp/RT_TICK_PER_SECOND/60/60%24;
		rt_kprintf("\r\nThe system runtime is %d:%d:%d.%d\r\n",h,m,s,tick_temp%RT_TICK_PER_SECOND);
		rt_kprintf("imuCorrectCount=%d,imuErrorCount=%d,readEncodeCount=%d,sendDataCount=%d,controlMotorCount=%d\r\n",imuCorrectCount/3,imuErrorCount,readEncodeCount,sendDataCount,controlMotorCount);
		
		
//		rt_kprintf("leftEncoder=%d;rightEncoder=%d\r\n",leftEncoder,rightEncoder);	
//		leftEncoder = 0;
//		rightEncoder = 0;
		
		imuCorrectCount=0;
		imuErrorCount=0;
		readEncodeCount=0;
		sendDataCount=0;
		controlMotorCount=0;
		rt_thread_delay(RT_TICK_PER_SECOND);
	}
	
}

