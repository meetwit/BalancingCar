#include "main.h"

u16 imuCorrectCount=0,imuErrorCount=0,readEncodeCount=0,sendDataCount=0,controlMotorCount=0;		//¼ÇÂ¼hzÊý

s32 leftEncoder=0,rightEncoder=0;

void readEncode(void* parameter){
	while(1){
		readEncodeCount++;
		leftEncoder += Read_Encoder(2);
		rightEncoder += Read_Encoder(4);
		
		rt_thread_delay(10);
		rt_timer_check();
	}
	
}

void sendData(void* parameter){
	while(1){
		sendDataCount++;
		printf("leftEncoder=%d,rightEncoder=%d\r\n",leftEncoder,rightEncoder);
		printf("%d\r\n",stcAngle.Angle[0]);
		rt_thread_delay(1000);		//do something
		rt_timer_check();
	}
	
}

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

