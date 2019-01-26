#ifndef __THREAD_MW_H
#define __THREAD_MW_H
#include <rtthread.h>
#include "stdio.h"
#include "stm32f10x.h"
#include "string.h"

extern u16 imuCorrectCount,imuErrorCount;		//¼ÇÂ¼hzÊý


void readEncode(void* parameter);
void sendData(void* parameter);
void controlMotor(void* parameter);

void time_thread(void* parameter);

#endif
