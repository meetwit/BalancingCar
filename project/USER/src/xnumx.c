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


void xnumx(char * rx,char rxLen,char * separator,float * mwData,char mwDataLen)
{
	
	/*
	p=65536,i=65536,d=65536
	01234567890123456789012345
	*/
	
	u8 mwAddr[mwDataLen][2];
	u32 temp=0;
	s32 longNum[]={
	1.0,
	10.0,
	100.0,
	1000.0,
	10000.0,
	100000.0,
	1000000.0,
	10000000.0,
	100000000.0,
	1000000000.0
	};
	
	//memset(mwData,0,3);		//清除临时数值
	for(char i = 0; i < mwDataLen; i++)
	{
		mwData[i]=0;
	}
	
	
	//printf("sizeof(mwData)=%d\r\n",sizeof(m));
	
	for(u8 k=0,i=0;i<mwDataLen;i++,k=0){
		/*取得开始结束地址*/
		mwAddr[i][0] = findFirstChar(rx,separator[i],rxLen);
		mwAddr[i][1] = findLastChar(rx,separator[i],rxLen);
		
		/*满足x number x形式*/
		if(mwAddr[i][0]!=mwAddr[i][1])
		for(u8 j=0,len=mwAddr[i][1]-mwAddr[i][0]-1;j<len;j++){
			if(rx[mwAddr[i][1]-1-j]=='.'){
				k=j;
				continue;
			}
			if(rx[mwAddr[i][1]-1-j]=='-'){
				mwData[i] = 0 - mwData[i];
				break;
			}
			if(0==k){
				temp = (rx[mwAddr[i][1]-1-j]-48)*longNum[j];
			}else{
				temp = (rx[mwAddr[i][1]-1-j]-48)*longNum[j-1];
			}
			mwData[i] += temp;
			
		}	
			mwData[i] = mwData[i]/longNum[k];
	}
	
	
}
