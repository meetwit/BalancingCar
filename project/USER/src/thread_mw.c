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
		rightEncoder += Read_Encoder(4);
		
		rt_thread_delay(10);
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
	while(1){
		sendDataCount++;
		printf("leftEncoder=%d,rightEncoder=%d\r\n",leftEncoder,rightEncoder);
		printf("%d\r\n",stcAngle.Angle[0]);
		rt_thread_delay(100);		//do something
		rt_timer_check();
	}
	
}

/*
��	�̣�controlMotor
��	�ã����Ƶ��
Ƶ	�ʣ�1000Hz
��	�ڣ�2019��1��26��
��	�ߣ�meetwit
*/
void controlMotor(void* parameter){
	while(1){
		controlMotorCount++;
//		selfCorrecting('l',1,leftEncoder-600000);
		selfCorrecting('r',1,rightEncoder-60000);
//		
//		motor_run(1,10);
		rt_thread_delay(1);
		rt_timer_check();
	}
	
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
		imuCorrectCount=0;
		imuErrorCount=0;
		readEncodeCount=0;
		sendDataCount=0;
		controlMotorCount=0;
		rt_thread_delay(RT_TICK_PER_SECOND);
	}
	
}

