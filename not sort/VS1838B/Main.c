/*
 * VS1838B�������ģ����Գ���
 * 
 * ��;��VS1838B�������ģ����Գ���
 * 
 * ����					����				��ע
 * Huafeng Lin			2012/08/23			����
 * Huafeng Lin			2012/08/23			�޸�
 * 
 */

#include <reg52.h>
#define uchar unsigned char
#define uint8 unsigned char
 
sbit P2_0 = P2^0;
sbit P2_1 = P2^1;
sbit P2_2 = P2^2;
sbit P2_3 = P2^3;

sbit P3_3=P3^3;		//���������ս�

uchar buf[66];		//����66λ����������洢���յ�ʱ�����

uchar DisBuff[4];

uchar code dis_code[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x84,0x8e};

void delay(uchar i)
{
	uchar j,k; 
	for(j=i;j>0;j--)
		for(k=125;k>0;k--);
}

void delay1(void)	//��5��ָ��
{
	unsigned char i=13;
	while(i)
		i--;
}

void display()
{
	P1=dis_code[DisBuff[3]];
	P2_0 = 0;
	delay(5);
	P2_0 = 1;
	
	P1=dis_code[DisBuff[2]];
	P2_1 = 0;
	delay(5);
	P2_1 = 1;
	
	P1=dis_code[DisBuff[1]];
	P2_2 = 0;
	delay(5);
	P2_2 = 1;
	
	P1=dis_code[DisBuff[0]];
	P2_3 = 0;
	delay(5);
	P2_3 = 1;   
}

void ir_recv(void) interrupt 2	//�ⲿ�ж�1��INT1��P3^3�����Ӻ����߽���IC���ݽ�
{
	unsigned char i,j,tmp;

	EX1=0;
	j=33;						//����һ��������������1λ����ַ��8λ�ӷ���8λ��ָ����8λ�ӷ���8λ���ܹ�33λ
	i=0;						//�ӵ�һά���鿪ʼ
	tmp=100;					//�Ӱ���ʱ��
	while(tmp){
	tmp--;
	}
	if(P3_3){					//Ȼ���ټ����߽��ս������������գ��м�����û�����˳�
		EX1=1;
		return;
	}	
	while(j){					//ѭ������33λ���ݣ�Ϊ�������õ�66λ���飬���ǿ��Բ����ոߵ�ƽʱ�䳣����ֻ�ӵ͵�ƽ������
								//�����ж�1��0�ˣ����������Ƕ����գ�Ϊ���ڵ���������ռ��㲨��
								//����һ��Ҫ֪�������ղ����Ƿ�����û�н���ʱ�˿�Ϊ�ߵ�ƽ
		tmp=0;
		buf[i]=1;				//ʱ������1��ʼ
		while(!P3_3){			//���ߵ͵�ƽ�ı仯
			buf[i]++;			//û�������1
			delay1();			//������ʱ������Ϊ���ǲ���8λ�������洢�����������ʱ��ʱ����������
			tmp++;				//�˱���Ϊ��ֹ���Ŵ�������ѭ��
			if(tmp==250)
				break;
		}
		i++;
		tmp=0;
		buf[i]=1;				//ʱ������1��ʼ
		while(P3_3){			//���ߵ͵�ƽ�ı仯
			buf[i]++;			//û�������1	
			delay1();			//ͬ��
			tmp++;
			if(tmp==250)
				break;
		}	
		i++;		
		j--;
	}
	i=255;						//����ѭ����ʱ��������
	while(i){
	tmp=255;
	while(tmp){
	tmp--;
	}
	i--;			
	}
	tmp=0;
	for(i=3;i<19;i++,i++){		//�����ַλ���Ե͵�ƽʱ�����ݵ�������������ֻ�����ַ���ָ���룬������ͷ������Ǿͺ���
		tmp>>=1;				//����һλ�����յ͵�ƽ��ǰ
		if(buf[i]>30)			//���͵�ƽʱ�䳬��30��ȷ��Ϊ1
			tmp+=0x80;
	}	
	DisBuff[3]=tmp/16;			//�ֿ�2λ��16������ʾ
	DisBuff[2]=tmp%16;
	tmp=0;
	for(i=35;i<51;i++,i++){		//ͬ�ϣ����ﴦ��ָ��λ
		tmp>>=1;
		if(buf[i]>30)
			tmp+=0x80;
	}
	DisBuff[1]=tmp/16;
	DisBuff[0]=tmp%16;	
	EX1=1;	
}

void main(void)
{
	unsigned int  a;

	EA=1;		//���ȿ������ж�
	EX1=1;		//�����ⲿ�ж� 1
	IT1=1;		//���ó��½��ش�����ʽ
   
	while(1)
	{
		for(a=100;a>0;a--)
		{
			display();
		} 
	}
}