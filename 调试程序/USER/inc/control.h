#ifndef __CONTROL_H
#define __CONTROL_H
#include "main.h"	 

extern u8 xunji_flag;
void remoteControl(u8 data);
void track_zhixian2(u8 Temp);
void track_zhixian(u8 Temp);

void up_down(s16 num);
#endif
