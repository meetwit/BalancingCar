#include "xnumx.h"
#include "main.h"

	
u8 findFirstChar(char * a,char a2,char len){
	for(u8 i=0;i<len;i++){
		if(a[i]==a2){
			return i;
		}
	}
		return len;
}
u8 findLastChar(char * a,char a2,char len){
	for(u8 i=len;i>0;i--){
		if(a[i-1]==a2){
			return i-1;
		}
	}
		return len;
}


void xnumx(char * rx,char * separator,u32 * mwData)
{
	
	/*
	p=65536,i=65536,d=65536
	01234567890123456789012345
	*/
	
	u8 mwAddr[10][2];
	u32 temp=0;
	u32 longNum[]={
	1,
	10,
	100,
	1000,
	10000,
	100000,
	1000000,
	10000000,
	100000000,
	1000000000
	};
	
	//memset(mwData,0,3);		//�����ʱ��ֵ
	mwData[0]=0;
	mwData[1]=0;
	mwData[2]=0;
	
	//printf("sizeof(mwData)=%d\r\n",sizeof(m));
	
	for(u8 i=0;i<3;i++){
		/*ȡ�ÿ�ʼ������ַ*/
		mwAddr[i][0] = findFirstChar(rx,separator[i],26);
		mwAddr[i][1] = findLastChar(rx,separator[i],26);
		
		/*����x number x��ʽ*/
		if(mwAddr[i][0]!=mwAddr[i][1])
		for(u8 j=0,len=mwAddr[i][1]-mwAddr[i][0]-1;j<len;j++){
			temp = (rx[mwAddr[i][1]-1-j]-48)*longNum[j];
			mwData[i] += temp;
		}	
	}
	
	
}
