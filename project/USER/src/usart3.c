#include "main.h"
/*һ��.c��������*/
u8 Rx_Buf3[Rx_Max3];
u16 Rx_End3,Rx_Len3,Rx_Tm3;

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
int _sys_exit(int x) 
{ 
	x = x; 
	return 0;
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      

	while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
	USART3->DR = (u8) ch;      

	return ch;
}
#endif 

/************************************************************
��������void usart3_init(u32 baud);
��  �Σ�u32 baud:������
����ֵ����
��  �ܣ�����3��ʼ��
��  �ߣ�meetwit
ʱ  �䣺2017��11��17��19:29:38
*************************************************************/
void usart3_init(u32 baud){
	/*
	1.��GPIOʱ��
	2.��ʼ��PB10(����	��������)  PB11(����	��������)
	3.������3ʱ��
	4.�ֳ���1����ʼλ��8������λ��1��ֹͣλ
	5.����ʹ��
	6.������
	7.����ʹ��
	*/
	float div,div_frr;
	u16 div_man;
	
	RCC ->APB2ENR |= (0x1<<3);//GPIOBʹ��
	GPIOB ->CRH &=~(0xF<<8);
	GPIOB ->CRH |=(0xB<<8);//��������
	
	GPIOB ->CRH &=~(0Xf<<12);
	GPIOB ->CRH |=(0X8<<12);//��������
	
	RCC ->APB1ENR |=(0x1<<18);//����3ʱ��ʹ��
	USART3->CR1 &=~(0x1<<12);//һ����ʼλ��8������λ
	USART3->CR2 &=~(0X3<<12);//һ��ֹͣλ
	USART3->CR1 |=(0x1<<3);//����ʹ��
	
	USART3->CR1|=(0x1<<2);    //����ʹ��
	USART3->CR1|=(1<<5);    //��USART_SR�е�ORE����RXNEΪ��1��ʱ������USART�ж�
	
	div=(float) 36000000/(baud * 16);
	div_man =div;
	div_frr=div-div_man;
	div_man <<= 4;
	
	USART3->BRR = div_man + (div_frr*16);
	
	/*�������ȼ�*/
	NVIC_SetPriority(USART3_IRQn,NVIC_EncodePriority(7-2,2,3));
	/*ʹ���ж�*/
	NVIC_EnableIRQ(USART3_IRQn);//NVIC_EnableIRQ������ʹ���ⲿ�жϵ�
	
	USART3->CR1 |=(0x1<<13);//����ʹ��	
}


/************************************************************
��������PcTx_Byte3(u8 Tx_data)
��  �Σ�u8 Tx_data
����ֵ����
��  �ܣ�����3����һ���ֽ�
��  �ߣ�meetwit
ʱ  �䣺2017��11��17��19:47:54
*************************************************************/
void PcTx_Byte3(u8 Tx_data){
	while(!(USART3->SR &(0x1<<7)));
	USART3 ->DR = Tx_data;
	
}

/************************************************************
��������PcTx_String3(u8 *str)
��  �Σ�u8 *str �ַ�����ַ
����ֵ����
��  �ܣ�����3�����ַ���
��  �ߣ�meetwit
ʱ  �䣺2017��11��17��19:47:38
*************************************************************/
void PcTx_String3(u8 *str){
		while(*str !='\0'){
			PcTx_Byte3(*(str++));
		}
}

/************************************************************
��������USART3_IRQHandler()
��  �Σ���
����ֵ����
��  �ܣ�����3�ж�
��  �ߣ�meetwit
ʱ  �䣺2017��11��17��19:47:27
*************************************************************/
	void USART3_IRQHandler(void)
{
	u8 tempdata;
	if(USART3->SR&(0x1<<5))	//���յ�����
	{ 
		tempdata=USART3->DR;
		if((Rx_End3==0)&&(Rx_Len3<Rx_Max3)){
				Rx_Buf3[Rx_Len3++]=tempdata;
				Rx_Tm3=5;//ÿ�����¸�ֵ5
		}
	}		
}

	u8 findFirstChar(u8 * a,char a2,u8 len){
	for(u8 i=0;i<len;i++){
		if(a[i]==a2){
			return i;
		}
	}
		return len;
}
	u8 findLastChar(u8 * a,char a2,u8 len){
	for(u8 i=len;i>0;i--){
		if(a[i-1]==a2){
			return i-1;
		}
	}
		return len;
}
/************************************************************
��������Task_Pc3()
��  �Σ���
����ֵ����
��  �ܣ�����3������ɣ���ӡ
��  �ߣ�meetwit
ʱ  �䣺2017��11��17��19:47:12
*************************************************************/
void Task_Pc3()
{
	
	/*
	p=65536,i=65536,d=65536
	01234567890123456789012345
	*/
	u32 mwData[10];
	char separator[]={'p','i','d'};
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
	
	memset(mwData,0,sizeof(mwData));		//�����ʱ��ֵ
	for(u8 i=0;i<sizeof(separator);i++){
		/*ȡ�ÿ�ʼ������ַ*/
		mwAddr[i][0] = findFirstChar(Rx_Buf3,separator[i],Rx_Len3);
		mwAddr[i][1] = findLastChar(Rx_Buf3,separator[i],Rx_Len3);
		
		/*����x number x��ʽ*/
		if(mwAddr[i][0]!=mwAddr[i][1])
		for(u8 j=0,len=mwAddr[i][1]-mwAddr[i][0]-1;j<len;j++){
			temp = (Rx_Buf3[mwAddr[i][1]-1-j]-48)*longNum[j];
			mwData[i] += temp;
		}	
	}
	
	printf("p=%d\r\n",mwData[0]);
	printf("i=%d\r\n",mwData[1]);
	printf("d=%d\r\n",mwData[2]);
	
		Rx_End3=0;
		Rx_Len3=0;
		memset(Rx_Buf3,0,sizeof(Rx_Buf3));
}
