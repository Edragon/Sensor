//12M

#include <reg51.h>
#include <intrins.h>
#include "StringData.h"

#define uchar unsigned char
#define uint  unsigned int
#define u8 unsigned char
#define u16  unsigned int
#define u32  unsigned long int

//* STC15W408AS�Ĵ������� 
sfr AUXR1 = 0XA2;
sfr AUXR = 0X8E;
sfr TH2 = 0XD6;
sfr TL2 = 0XD7;
sfr P4 = 0xc0;
sfr P5 = 0xc8;

sfr SPSTAT      =   0xCD;   //
sfr SPCTL       =   0xCE;   //
sfr SPDAT       =   0xCF;   //
sfr P1M1=0x91;
sfr P1M0=0x92;
sfr P2M1=0x95;
sfr P2M0=0x96;
sfr P3M1=0xB1;
sfr P3M0=0xB2;
sfr P5M1=0xC9;
sfr P5M0=0xCA; 

//ADC
sfr ADC_CONTR = 0xBC;	//��ADϵ��
sfr ADC_RES   = 0xBD;	//��ADϵ��
sfr ADC_RESL  = 0xBE;	//��ADϵ��
sfr P1ASF = 0x9D;
sfr PCON2   = 0x97;

//��ʱ��2
sfr T2H=0xD6;
sfr T2L=0xD7;
sfr IE2=0xAF;

#define ID_ADDR_ROM 0x1ff7//8K����ռ��MCU

#define ADC_POWER	0x80//ADC��Դ����λ
#define ADC_FLAG	0x10//ADC��ɱ�־
#define ADC_START	0x08//ADC��ʼ����λ
#define ADC_SPEEDLL	0x00//540��ʱ��ת��һ��
#define ADC_SPEEDL	0x20//360ʱ��
#define ADC_SPEEDH	0x40//180ʱ��
#define ADC_SPEEDHH	0x60//90ʱ��


sbit JLX_CS=P3^3;
sbit JLX_RST=P3^6;
sbit JLX_RS=P3^7;
sbit JLX_SCK=P1^0;
sbit JLX_SDA=P1^1;

sbit DHT12_SDA=P1^5;

u8 lcd[10];//��ʾ����

u16 Temprature,Humi;//������ʪ�ȱ��� ���˱���Ϊȫ�ֱ���
bit fushu=0;//�¶��Ƿ�Ϊ��
u8 Sensor_AnswerFlag=0;//���崫������Ӧ��־
u8 Sensor_ErrorFlag;  //�����ȡ�����������־

void Delay1ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 12;
	j = 169;
	do
	{
		while (--j);
	} while (--i);
}

void DelayMs(unsigned int i)  //��ʱN���룬0~65535ms
{
   while(i--) Delay1ms();
}


 
void delay_us(unsigned char n)
{
    while (n--)
    {
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    }
}


/*****************Һ��������**********************/
 /*дָ� LCD ģ��*/
void JLX_SendCommand(u8 Data)
{
	u8 i;
	JLX_CS=0;
	JLX_RS=0;
	for(i=0;i<8;i++)
	{
		JLX_SCK=0;
		if(Data&0x80) JLX_SDA=1;
		else JLX_SDA=0;
		JLX_SCK=1;
		Data=Data<<=1;
	}
}

/*д���ݵ� LCD ģ��*/
void JLX_SendData(u8 Data)
{
	u8 i;
	JLX_CS=0;
	JLX_RS=1;
	for(i=0;i<8;i++)
	{
		JLX_SCK=0;
		if(Data&0x80) JLX_SDA=1;
		else JLX_SDA=0;
		JLX_SCK=1;
		Data=Data<<=1;
	}
}

/*LCD ģ���ʼ��*/
void JLX_InitLCD()
{
	JLX_CS=0;
	JLX_RST=0;	/*�͵�ƽ��λ*/
	DelayMs(2);
	JLX_RST=1;	/*��λ���*/
	DelayMs(10);
	JLX_SendCommand(0xE2);	/*��λ*/
	DelayMs(1);
	JLX_SendCommand(0x2C); /*��ѹ���� 1*/
	JLX_SendCommand(0x2E); /*��ѹ���� 2*/
	JLX_SendCommand(0x2F); /*��ѹ���� 3*/
	DelayMs(1);
	JLX_SendCommand(0x23); /*�ֵ��Աȶȣ������÷�Χ 0x20��0x27*/
	JLX_SendCommand(0x81); /*΢���Աȶ�*/
	JLX_SendCommand(0x28); /*0x1a,΢���Աȶȵ�ֵ�������÷�Χ 0x00��0x3f*/
	JLX_SendCommand(0xA2); /*1/9 ƫѹ�ȣ�bias��*/
	JLX_SendCommand(0xC8); /*��ɨ��˳�򣺴��ϵ���*/
	JLX_SendCommand(0xA0); /*��ɨ��˳�򣺴�����*/
	JLX_SendCommand(0x40); /*��ʼ�У���һ�п�ʼ*/
	JLX_SendCommand(0xAF); /*����ʾ*/
	JLX_CS=1;
}

/*����Һ��������ʾ��ַ*/
void JLX_LCDAddress(u8 Page,u8 Column)
{
	JLX_CS=0;
	Column=Column-1;	//����ƽ����˵�ĵ� 1 �У��� LCD ���� IC ���ǵ� 0 �С������������ȥ 1.
	Page=Page-1;
	JLX_SendCommand(0xB0+Page);	//����ҳ��ַ��ÿҳ�� 8 �С�һ������� 64 �б��ֳ� 8 ��ҳ��
	                            //����ƽ����˵�ĵ� 1 ҳ���� LCD ���� IC ���ǵ� 0 ҳ�������������ȥ 1
	JLX_SendCommand(((Column>>4)&0x0F)+0x10); //�����е�ַ�ĸ� 4 λ 
	JLX_SendCommand(Column&0x0F);	//�����е�ַ�ĵ� 4 λ
}

/*ȫ������*/
void JLX_ClearScreen()
{
	u8 i,j;
	JLX_CS=0;
	for(i=0;i<9;i++)
	{
		JLX_LCDAddress(1+i,1);
		for(j=0;j<132;j++)
		{
			JLX_SendData(0x00);	/*д���ݵ� LCD,ÿд��һ�� 8 λ�����ݺ��е�ַ�Զ��� 1*/
		}
	}
	JLX_CS=1;
}


//ȫ����ʾ�ڵ� 128*64 dots
//void JLX_FullScreenDots()
//{
//	u8 i,j;
//    JLX_CS=0;
//	for(i=0;i<8;i++)
//	{
//		JLX_LCDAddress(i+1,1);
//		for(j=0;j<128;j++)
//		{
//			JLX_SendData(0xFF);/*д���ݵ� LCD,ÿд��һ�� 8 λ�����ݺ��е�ַ�Զ��� 1*/
//		}
//	}
//	JLX_CS=1;
//}


/*��ʾ 12864 ����ͼ��*/

//void JLX_DisplayPicture128x64(u8 *dp)
//{
//	u8 i,j;
//	JLX_CS=0;
//	
//	for(j=0;j<8;j++)
//	{
//		JLX_LCDAddress(j+1,1);
//		for (i=0;i<128;i++)
//		{
//			JLX_SendData(*dp);	/*д���ݵ� LCD,ÿд��һ�� 8 λ�����ݺ��е�ַ�Զ��� 1*/
//			dp++;
//		}
//	}
//	JLX_CS=1;
//}

/*��ʾ 32x32 ����ͼ�񡢺��֡���Ƨ�ֻ� 32x32 ���������ͼ��*/
//void JLX_DisplayPicture32x32(u8 Page,u8 Column,u8 *dp)
//{
//	u8 i,j;
//	JLX_CS=0;
//	for(j=0;j<4;j++)
//	{
//		JLX_LCDAddress(Page+j,Column);
//		for (i=0;i<31;i++)
//		{
//			JLX_SendData(*dp);	/*д���ݵ� LCD,ÿд��һ�� 8 λ�����ݺ��е�ַ�Զ��� 1*/
//			dp++;
//		}
//	}
//	JLX_CS=1;
//}

/*��ʾ 16x16 ����ͼ�񡢺��֡���Ƨ�ֻ� 16x16 ���������ͼ��*/
void JLX_DisplayPicture16x16(u8 Page,u8 Column,u8 *dp)
{
	u8 i,j;
	JLX_CS=0;
	for(j=0;j<2;j++)
	{
		JLX_LCDAddress(Page+j,Column);
		for (i=0;i<16;i++)
		{
			JLX_SendData(*dp);	/*д���ݵ� LCD,ÿд��һ�� 8 λ�����ݺ��е�ַ�Զ��� 1*/
			dp++;
		}
	}
	JLX_CS=1;
}

/*��ʾ 8x16 ����ͼ��ASCII, �� 8x16 ����������ַ�������ͼ��*/
//void JLX_DisplayPicture8x16(u8 Page,u8 Column,u8 *dp)
//{
//	u8 i,j;
//	JLX_CS=0;
//	for(j=0;j<2;j++)
//	{
//		JLX_LCDAddress(Page+j,Column);
//		for (i=0;i<8;i++)
//		{
//			JLX_SendData(*dp);	/*д���ݵ� LCD,ÿд��һ�� 8 λ�����ݺ��е�ַ�Զ��� 1*/
//			dp++;
//		}
//	}
//	JLX_CS=1;
//}

//��ʾ8*16���ַ���
void JLX_DisplayString8x16(u8 Page,u8 Column,u8 *text)
{
	u8 i=0,j,k,n;
	JLX_CS=0;
	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<=0x7E)) //32~126 ASCII�����ַ�����ֵ֮��
		{
			j=text[i]-0x20;//ASCII��ֵ-32���պ��Ǹ��ַ���ascii_table_8x16�����е���ʼ��ַ
			for(n=0;n<2;n++)
			{
				JLX_LCDAddress(Page+n,Column);
				for(k=0;k<8;k++)
				{
					JLX_SendData(ascii_table_8x16[j][k+8*n]);/*��ʾ 5x7 �� ASCII �ֵ� LCD �ϣ�y Ϊҳ��ַ��x Ϊ�е�ַ�����Ϊ����*/
				}
			}
			i++;
			Column+=8;
		}
		else
		i++;
	}
//	JLX_CS=1;
}


//��ʾ5*7���ַ����õ���6*8�����ݣ�������ȫ����Һ������������
//void JLX_DisplayString5x7(u8 Page,u8 Column,u8 *text)
//{
//	u8 i=0,j,k;
//	JLX_CS=0;
//	while(text[i]>0x00)
//	{
//		if((text[i]>=0x20)&&(text[i]<0x7E))
//		{
//			j=text[i]-0x20;
//			JLX_LCDAddress(Page,Column);
//			for(k=0;k<5;k++)
//			{
//				JLX_SendData(ascii_table_5x7[j][k]);/*��ʾ 5x7 �� ASCII �ֵ� LCD �ϣ�y Ϊҳ��ַ��x Ϊ�е�ַ�����Ϊ����*/
//			}
//			i++;
//			Column+=6;
//		}
//		else
//		i++;
//	}
//}

/***********************Һ������������****************/

/********************************************\
|* ���ܣ� �����������͵ĵ����ֽ�	        *|
\********************************************/
u8 DHT12_Single_ReadByte(void)
{
	u8 i;
	u16 j;
	u8 Data=0;
	u8 Bit=0;
	
	for(i=0;i<8;i++)
	{
		while(!DHT12_SDA)//����ϴε͵�ƽ�Ƿ����
		{
			if(++j>=50000) //��ֹ������ѭ��
			{
				break;
			}
		}
		//��ʱMin=26us Max70us ��������"0" �ĸߵ�ƽ		 
		delay_us(30);

		//�жϴ�������������λ
		Bit=0;
		if(DHT12_SDA)Bit=1;
		j=0;
		while(DHT12_SDA)	//�ȴ��ߵ�ƽ����
		{
			if(++j>=50000) //��ֹ������ѭ��
			{
				break;
			}		
		}
		Data<<=1;
		Data|=Bit;
	}
	return Data;
}


void Sensor_Read(void)
{	
	u16 j;
	u8 Humi_H,Humi_L,Temp_H,Temp_L,Temp_CAL,temp;
	//����������ʼ�ź�
	DHT12_SDA=0;	//�������������ߣ�SDA������
	DelayMs(20);//����һ��ʱ�䣨����18ms���� ֪ͨ������׼������
	DHT12_SDA=1;	 //�ͷ�����
	delay_us(30);//��ʱ30us

	Sensor_AnswerFlag=0;	//��������Ӧ��־
	//�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������	  
	if(DHT12_SDA==0)
	{
		Sensor_AnswerFlag=1;	//�յ���ʼ�ź�

		j=0;
		while(!DHT12_SDA) //�жϴӻ����� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����	
		{
			if(++j>=500) //��ֹ������ѭ��
			{
				Sensor_ErrorFlag=1;
				break;
			}
		}

		j=0;
		while(DHT12_SDA)//�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
		{
			if(++j>=800) //��ֹ������ѭ��
			{
				Sensor_ErrorFlag=1;
				break;
			}		
		}
		//��������
		Humi_H=DHT12_Single_ReadByte();
		Humi_L=DHT12_Single_ReadByte();
		Temp_H=DHT12_Single_ReadByte();	
		Temp_L=DHT12_Single_ReadByte();
		Temp_CAL=DHT12_Single_ReadByte();


		temp = Humi_H+Humi_L+Temp_H+Temp_L;//ֻȡ��8λ

		if(Temp_CAL==temp)//���У��ɹ�����������
		{
			Humi=Humi_H*10+Humi_L; //ʪ��
			if(Temp_L&0x80)	//Ϊ���¶�
			{
				fushu=1;
				Temprature =Temp_H*10+(Temp_L&0x7F);
			}
			else   //Ϊ���¶�
			{
				fushu=0;
				Temprature=Temp_H*10+Temp_L;//Ϊ���¶�
			} 
		
			//�ж���ʪ���Ƿ񳬹�������Χ���¶ȣ�-20��~60���϶ȣ�ʪ�ȣ�20%RH~95%RH��		
			if(Humi>950) 
			{
			  Humi=950;
			}
			if(Humi<200)
			{
				Humi =200;
			}
			if(Temprature>600&&fushu==0)
			{
			  Temprature=600;
			}
			if(Temprature>200&&fushu==1)
			{
				Temprature=200;
			}

			lcd[0]=' ';
			lcd[1]=Humi%1000/100+48;
			lcd[2]=Humi%100/10+48;	
			lcd[3]='.';	
			lcd[4]=Humi%10+48;
			lcd[5]=' ';	
			lcd[6]=' ';
		    lcd[7]=' ';
			lcd[8]=' ';
			lcd[9]=' ';
	
			JLX_DisplayString8x16(3,49,lcd);
			JLX_DisplayPicture16x16(3,97,baifenbi);
	
	 		if(fushu==1)lcd[0]='-';
			else lcd[0]=' ';
			lcd[1]=Temprature%1000/100+48;
			lcd[2]=Temprature%100/10+48;
			lcd[3]='.';	
			lcd[4]=Temprature%10+48;
	
			JLX_DisplayString8x16(5,49,lcd);
			JLX_DisplayPicture16x16(5,97,sheshidu);
		}
		else //У��ʧ��
		{
			JLX_DisplayString8x16(7,1,"CRC Error       ");
		}
	}
	else
	{
		JLX_DisplayString8x16(7,1,"DTH12 Not Find  ");

	}
}

void main(void)
{
	DelayMs(200);//��ʱ200ms�ȴ��ȶ�����

	JLX_InitLCD();
	JLX_ClearScreen();

    JLX_DisplayString8x16(1,1," DHT12_One_Wire ");

	JLX_DisplayPicture16x16(3,1,shi);
	JLX_DisplayPicture16x16(3,17,du);
	JLX_DisplayString8x16(3,33,":");
	JLX_DisplayString8x16(3,41," ");

	JLX_DisplayPicture16x16(5,1,wen);
	JLX_DisplayPicture16x16(5,17,du);
	JLX_DisplayString8x16(5,33,":");
	JLX_DisplayString8x16(5,41," ");

	JLX_DisplayString8x16(7,1,"                ");

	while(1)
	{
	    DelayMs(1000);
	    Sensor_Read();
	}
}


