#include "main.h"
#include "sys.h"

//u8 a,b;


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
		rightEncoder -= Read_Encoder(4);
		
		rt_thread_delay(1);
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

//	printf("d = %.2f\r\n\r\n",m[2]);
//		rt_thread_delay(1000);		//do something
//		rt_timer_check();
	
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
	float get_angle,get_gyro;
	temp1 = 32768/180.0;			//角度
	temp2 = 32768/2000.0;			//角速度
	temp3 = 32768/16.0;				//角加速度
	int finalPwm = 0,finalPwm2 = 0;
	u8 time=0;
	while(1){
		time++;
		controlMotorCount++;
			finalPwm = 0;
		get_angle = stcAngle.Angle[0]/temp1;
		get_gyro = stcGyro.w[0]/temp2;
		if(get_angle>40||get_angle<-40){									//角度过大，锁住
			motor_run(0,0);
			stop_flag = 1;
		}
		if(stop_flag){
			if(get_angle>-5&&get_angle<5){									//角度适合，解锁
				rt_thread_delay(RT_TICK_PER_SECOND*2);				//扶正后等待x秒
				stop_flag = 0;																//解锁
			}
				velocity_mw();																//执行一次清除积分
				motor_run(0,0);																
		}else{
			//10和300 进行PID控制
			finalPwm += balance_mw(get_angle,get_gyro);
			finalPwm += velocity_mw();
			finalPwm2 = finalPwm+Flag_turn;
			if(finalPwm2>=0){
					motor_run(3,finalPwm2); 
			}else{
					motor_run(4,-finalPwm2); 
			}
			if(finalPwm>=0){
					motor_run(2,finalPwm);
			}else{
					motor_run(1,-finalPwm);
			}
			if(time%20==0){
				Flag_v = 0;
				Flag_turn = 0;
				ctr_flag = 0;
			}
		}
		rt_thread_delay(5);
		rt_timer_check();
	}
	
}

void timer1_f(void* parameter){
//	Rx_Tm3--;
//	if(Rx_Tm3==1)
//	Task_Pc3();
	PcTx_Byte3(0x57);
//	a= BZ;
//	b= BZ0;
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

