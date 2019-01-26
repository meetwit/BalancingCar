#include "main.h"

//编码器 一圈500*30个脉冲，四倍频后500*30*4=60000 减速比是30

rt_thread_t tid1=RT_NULL;
rt_thread_t tid2=RT_NULL;
rt_thread_t tid3=RT_NULL;
rt_thread_t tid_time=RT_NULL;

int main(){
	rt_kprintf("\r\n\r\nmeetwit rtt systerm start init!\r\n\r\n");
	
	usart2_init(115200);
	
	usart3_init(115200);
	
	/*资源初始化*/
	Encoder_Init_TIM2();
	Encoder_Init_TIM4();
	PWM_Init_TIM1(PWMMAX-1,0);	
	
	tid1=rt_thread_create("readEncode",readEncode,RT_NULL,256,6,100);
	rt_thread_startup(tid1);
	
	tid2=rt_thread_create("sendData",sendData,RT_NULL,256,5,100);
	rt_thread_startup(tid2);

	tid3=rt_thread_create("controlMotor",controlMotor,RT_NULL,1024,4,100);
	rt_thread_startup(tid3);

	tid_time=rt_thread_create("tid_time",time_thread,RT_NULL,256,7,100);
	rt_thread_startup(tid_time);
	
	rt_kprintf("meetwit rtt systerm started!\r\n\r\n");
	
	return 0;
}







