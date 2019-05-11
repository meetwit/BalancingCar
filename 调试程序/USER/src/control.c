#include "main.h"
#include "sys.h"

u8 cal2[]={20,10,5,2};

u8 xunji_flag = 0;

void left_right(s8 num){
	Flag_turn = num;
	ctr_flag = 1;
}

void up_down(s16 num){
	Flag_v = num;
	ctr_flag = 1;
}

void stop_func(void){
	Flag_v = 0;
	Flag_turn = 0;
	ctr_flag = 0;
}
//turn
void remoteControl(u8 data){
	switch(data){
		case 'a':
			left_right(-20);		//-20
			break;
		
		case 'd':
			left_right(20);			//20
			break;
		
		case 'w':
			up_down(500);			//-500
			break;
		
		case 's':
			up_down(-500);				//500
			break;
		
		case 'p':
			stop_func();				//0
		xunji_flag = 0;
			break;
		
		case 'q':
			xunji_flag = 1;				//0
			break;
		
		case 'x':
			
		if(BZ0==0){
				up_down(-500);
			}
			break;
	}
	PcTx_Byte(data);
}

void motor(u8 a,u8 b){
	left_right(a-b);	//wd
	up_down(500);
}

/*************************************
*函数名称：track_zhixian2()
*函数功能：直线循迹
*参数：
*说明：
*			
**************************************/
void track_zhixian2(u8 Temp)
{
	
	switch(Temp)
	{                                         //8765 4321
		case 0xFE:		motor(0,40);	break;      //1111 1110
		case 0xFC:		motor(10,40);	break;      //1111 1100
		case 0xFD:		motor(15,40);	break;      //1111 1101
		case 0xF9:		motor(20,40);	break;      //1111 1001
		case 0xFB:		motor(25,40);	break;      //1111 1011
		case 0xF3:		motor(25,40);	break;      //1111 0011
		case 0xF7:		motor(38,40);	break;      //1111 0111
		case 0xE7:		motor(40,40);	break;      //1110 0111		//正中间位置
		case 0xEF:		motor(40,38);	break;      //1110 1111			
		case 0xCF:		motor(40,25);	break;      //1100 1111
		case 0xDF:		motor(40,25);	break;      //1101 1111
		case 0x9F:		motor(40,20);	break;      //1001 1111
		case 0xBF:		motor(40,15);	break;      //1011 1111
		case 0x3F:		motor(40,10);	break;      //0011 1111
		case 0x7F:		motor(40,0);	break;      //0111 1111
		case 0x00:		break;    
		case 0xFF:		break;     
		
		default :			motor(20,20);	break;
	}
	PcTx_Byte(Temp);
	PcTx_Byte3(Temp);
	
}


	
void track_zhixian(u8 Temp)
{
	u8 ansL = 0,ansH = 0;
	u8 flag = 1;
	s8 ans = 0;
	if(Temp!=0x00){		//正确数据
		for(u8 i=0;i<4;i++){		//计算低四位
			if(Temp&(0x01<<i)){
				if(flag){
					ansL = cal2[i];
					flag = 0;
				}else{
					ansL -= cal2[i];
				}
			}
		}
		flag = 1;
		for(u8 i=0,j=7;i<4;i++,j--){		//计算高四位
			if(Temp&(0x01<<j)){
				if(flag){
					ansH = cal2[i];
					flag = 0;
				}else{
					ansH -= cal2[i];
				}
			}
		}
		
		ans = ansH-ansL;
		
		left_right(ans);
		
		up_down(200);

	}
	
	
	
}
