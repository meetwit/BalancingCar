#include "main.h"

//������ һȦ500*30�����壬�ı�Ƶ��500*30*4=60000 ���ٱ���30

//hello this is commit by vscode

rt_thread_t tid1=RT_NULL;
rt_thread_t tid2=RT_NULL;
rt_thread_t tid3=RT_NULL;
rt_thread_t tid_time=RT_NULL;

rt_timer_t timer1=RT_NULL;

int main(){
	rt_kprintf("\r\n\r\nmeetwit rtt systerm start init!\r\n\r\n");
	
	usart2_init(115200);
	
	usart3_init(115200);
	
	/*��Դ��ʼ��*/
	Encoder_Init_TIM2();
	Encoder_Init_TIM4();
	PWM_Init_TIM1(PWMMAX-1,0);	
	
	tid1=rt_thread_create("readEncode",readEncode,RT_NULL,256,6,100);
	rt_thread_startup(tid1);
	
	tid2=rt_thread_create("sendData",sendData,RT_NULL,512,5,100);//5
	rt_thread_startup(tid2);

	tid3=rt_thread_create("controlMotor",controlMotor,RT_NULL,1024,4,100);
	rt_thread_startup(tid3);

	tid_time=rt_thread_create("tid_time",time_thread,RT_NULL,256,7,100);
	rt_thread_startup(tid_time);
	
	timer1 = rt_timer_create("t1",timer1_f,RT_NULL,10,RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
	rt_timer_start(timer1);
	
	rt_kprintf("meetwit rtt systerm started!\r\n\r\n");
	
	return 0;
}







