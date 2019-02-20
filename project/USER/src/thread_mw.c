#include "main.h"

u16 imuCorrectCount=0,imuErrorCount=0,readEncodeCount=0,sendDataCount=0,controlMotorCount=0;		//记录hz数

s32 leftEncoder=0,rightEncoder=0;

/*
线	程：readEncode
作	用：读取左右电机编码器脉冲数
频	率：100Hz
日	期：2019年1月26日
作	者：meetwit
*/
void readEncode(void* parameter){
	while(1){
		readEncodeCount++;
		leftEncoder += Read_Encoder(2);
		rightEncoder += Read_Encoder(4);
		
		rt_thread_delay(10000);
		rt_timer_check();
	}
	
}

/*
线	程：sendData
作	用：使用printf，通过串口3打印数据，也可使用rt_kprintf,通过串口1打印数据
频	率：10Hz
日	期：2019年1月26日
作	者：meetwit
*/
void sendData(void* parameter){
	while(1){
		sendDataCount++;
//		printf("leftEncoder=%d,rightEncoder=%d\r\n",leftEncoder,rightEncoder);
		rt_thread_delay(1000);		//do something
		rt_timer_check();
	}
	
}


/*
线	程：controlMotor
作	用：控制电机
频	率：1000Hz
日	期：2019年1月26日
作	者：meetwit
*/
void controlMotor(void* parameter){
	float temp1,temp2,temp3;
	temp1 = 32768/180.0;			//角度
	temp2 = 32768/2000.0;			//角速度
	temp3 = 32768/16.0;				//角加速度
	while(1){
		controlMotorCount++;
//		selfCorrecting('l',1,leftEncoder-600000);
//		selfCorrecting('r',1,rightEncoder-60000);
//		
//		motor_run(1,10);
		
//		temp[i++] = balance(stcAngle.Angle[0]/temp1,stcGyro.w[0]/temp2);
//		if(i>40){
//			i=0;
//			for(j=0;j<40;j++)
//			printf("%d\r\n",temp[j]);
//		}
	  motor_run(3,balance(stcAngle.Angle[0]/temp1,stcGyro.w[0]/temp2)); 
	  motor_run(2,balance(stcAngle.Angle[0]/temp1,stcGyro.w[0]/temp2)); 
		
//		motor_run(4,20); 
//	  motor_run(1,20); 
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
线	程：time_thread
作	用：通过串口1打印系统时间
频	率：1Hz
日	期：2019年1月26日
作	者：meetwit
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

