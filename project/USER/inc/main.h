#ifndef __MAIN_MW_H
#define __MAIN_MW_H	 

#include "stdio.h"
#include "stm32f10x.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"

#include <rtthread.h>
#include "thread_mw.h"

#include "motor.h"
#include "control.h"
#include "encoder.h"
#include "JY901.h"

#include "xnumx.h"

	#define PWMMAX 7200	

	extern double m[10];
	extern char s[];
	extern s32 leftEncoder,rightEncoder;
	
	extern float stop_flag,ctr_flag,Flag_v,Flag_turn;

#endif

















