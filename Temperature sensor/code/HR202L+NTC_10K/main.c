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

sbit Rh=P1^5;//��ʪ������
sbit Rf=P1^4;//51K�ο�����
//HR P1^3  Temp P1^2;

u8 lcd[10];//��ʾ����
u16 adrtc=0;//��ʾ��������RTC��AD
u16 adhr=0;//��ʾʪ�������AD
u16 ad9=0;//�ھ�ͨ��bandgap��adcֵ
u16 cnt=0;//������
u32 R_Humi;//ʪ�ȵĵ���
u16 Temprature;
u32 Humi;//������ʪ�ȱ��� ���˱���Ϊȫ�ֱ���
bit fushu=0;//�¶��Ƿ�Ϊ��
bit IsShow=0;//�Ƿ���ʾ����
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

	/*************************************
	NTC��ֵ���¶ȹ�ϵ��
	Vcc---NTC(��1��)---R(10K��1��)---GND
	                |
	              ADC����
	**************************************/
	unsigned int code NTCcode[] = {
	/*ADC����ֵ****�¶�ֵ*******����ֵ**********/   
	    0x033,  // -40.00       190.5562
	    0x034,  // -39.00       183.4132
	    0x037,  // -38.00       175.6740
	    0x039,  // -37.00       167.6467
	    0x03c,  // -36.00       159.5647
	    0x03f,  // -35.00       151.5975
	    0x042,  // -34.00       143.8624
	    0x045,  // -33.00       136.4361
	    0x049,  // -32.00       129.3641
	    0x04d,  // -31.00       122.6678
	    0x050,  // -30.00       116.3519
	    0x054,  // -29.00       110.4098
	    0x059,  // -28.00       104.8272
	    0x05d,  // -27.00       99.5847
	    0x061,  // -26.00       94.6608
	    0x066,  // -25.00       90.0326
	    0x06a,  // -24.00       85.6778
	    0x06f,  // -23.00       81.5747
	    0x074,  // -22.00       77.7031
	    0x079,  // -21.00       74.0442
	    0x07e,  // -20.00       70.5811
	    0x084,  // -19.00       67.2987
	    0x089,  // -18.00       64.1834
	    0x08f,  // -17.00       61.2233
	    0x095,  // -16.00       58.4080
	    0x09b,  // -15.00       55.7284
	    0x0a1,  // -14.00       53.1766
	    0x0a8,  // -13.00       50.7456
	    0x0af,  // -12.00       48.4294
	    0x0b5,  // -11.00       46.2224
	    0x0bd,  // -10.00       44.1201
	    0x0c4,  // -9.00        42.1180
	    0x0cb,  // -8.00        40.2121
	    0x0d3,  // -7.00        38.3988
	    0x0db,  // -6.00        36.6746
	    0x0e3,  // -5.00        35.0362
	    0x0eb,  // -4.00        33.4802
	    0x0f3,  // -3.00        32.0035
	    0x0fb,  // -2.00        30.6028
	    0x0104, // -1.00        29.2750
	    0x010d, // 0.00     28.0170
	    0x0115, // 1.00     26.8255
	    0x011e, // 2.00     25.6972
	    0x0127, // 3.00     24.6290
	    0x0130, // 4.00     23.6176
	    0x0139, // 5.00     22.6597
	    0x0142, // 6.00     21.7522
	    0x014b, // 7.00     20.8916
	    0x0154, // 8.00     20.0749
	    0x015d, // 9.00     19.2988
	    0x0166, // 10.00        18.5600
	    0x0167, // 11.00        18.4818
	    0x016b, // 12.00        18.1489
	    0x0172, // 13.00        17.6316
	    0x017b, // 14.00        16.9917
	    0x0185, // 15.00        16.2797
	    0x0190, // 16.00        15.5350
	    0x019c, // 17.00        14.7867
	    0x01a9, // 18.00        14.0551
	    0x01b6, // 19.00        13.3536
	    0x01c2, // 20.00        12.6900
	    0x01cf, // 21.00        12.0684
	    0x01dc, // 22.00        11.4900
	    0x01e8, // 23.00        10.9539
	    0x01f4, // 24.00        10.4582
	    0x01ff, // 25.00        10.0000
	    0x020a, // 26.00        9.5762
	    0x0215, // 27.00        9.1835
	    0x021f, // 28.00        8.8186
	    0x0229, // 29.00        8.4784
	    0x0233, // 30.00        8.1600
	    0x023c, // 31.00        7.8608
	    0x0245, // 32.00        7.5785
	    0x024e, // 33.00        7.3109
	    0x0257, // 34.00        7.0564
	    0x0260, // 35.00        6.8133
	    0x0268, // 36.00        6.5806
	    0x0271, // 37.00        6.3570
	    0x0279, // 38.00        6.1418
	    0x0282, // 39.00        5.9343
	    0x028a, // 40.00        5.7340
	    0x0292, // 41.00        5.5405
	    0x029a, // 42.00        5.3534
	    0x02a2, // 43.00        5.1725
	    0x02aa, // 44.00        4.9976
	    0x02b1, // 45.00        4.8286
	    0x02b9, // 46.00        4.6652
	    0x02c1, // 47.00        4.5073
	    0x02c8, // 48.00        4.3548
	    0x02d0, // 49.00        4.2075
	    0x02d7, // 50.00        4.0650
	    0x02de, // 51.00        3.9271
	    0x02e5, // 52.00        3.7936
	    0x02ec, // 53.00        3.6639
	    0x02f3, // 54.00        3.5377
	    0x02fa, // 55.00        3.4146
	    0x0301, // 56.00        3.2939
	    0x0308, // 57.00        3.1752
	    0x030f, // 58.00        3.0579
	    0x0316, // 59.00        2.9414
	    0x031d, // 60.00        2.8250
	    0x0320, // 61.00        2.7762
	    0x0324, // 62.00        2.7179
	    0x0328, // 63.00        2.6523
	    0x032d, // 64.00        2.5817
	    0x0331, // 65.00        2.5076
	    0x0336, // 66.00        2.4319
	    0x033b, // 67.00        2.3557
	    0x0341, // 68.00        2.2803
	    0x0346, // 69.00        2.2065
	    0x034b, // 70.00        2.1350
	    0x034f, // 71.00        2.0661
	    0x0354, // 72.00        2.0004
	    0x0358, // 73.00        1.9378
	    0x035d, // 74.00        1.8785
	    0x0361, // 75.00        1.8225
	    0x0365, // 76.00        1.7696
	    0x0368, // 77.00        1.7197
	    0x036c, // 78.00        1.6727
	    0x036f, // 79.00        1.6282
	    0x0372, // 80.00        1.5860
	    0x0376, // 81.00        1.5458
	    0x0378, // 82.00        1.5075
	    0x037b, // 83.00        1.4707
	    0x037e, // 84.00        1.4352
	    0x0381, // 85.00        1.4006
	    0x0383, // 86.00        1.3669
	    0x0386, // 87.00        1.3337
	    0x0389, // 88.00        1.3009
	    0x038b, // 89.00        1.2684
	    0x038e, // 90.00        1.2360
	    0x0391, // 91.00        1.2037
	    0x0393, // 92.00        1.1714
	    0x0396, // 93.00        1.1390
	    0x0399, // 94.00        1.1067
        0x039b, // 95.00        1.0744
	    0x039e, // 96.00        1.0422
	    0x03a1, // 97.00        1.0104
	    0x03a3, // 98.00        0.9789
	    0x03a6, // 99.00        0.9481
	    0x03a8, // 100.00       0.9180
	    0x03ab, // 101.00       0.8889
	    0x03ad, // 102.00       0.8610
	    0x03b0, // 103.00       0.8346
	    0x03b2, // 104.00       0.8099
	    0x03b4, // 105.00       0.7870
	    0x03b6, // 106.00       0.7665
	    0x03b7, // 107.00       0.7485
	    0x03b9, // 108.00       0.7334
	    0x03ba, // 109.00       0.7214
	    0x03ba  // 110.00       0.7130
	};

void get_HUMI_vule(void)
{
  //�ֶμ���ʪ��ֵ 
  u8 temp_vule;
  R_Humi=R_Humi/1000;
  if(R_Humi>=2600)  				 Humi=(146000-10*R_Humi)*10/4800; //20-25
   if(1300<=R_Humi&R_Humi<2600)  Humi=(91000-10*R_Humi)*10/2600; //25-30
   if(630<=R_Humi&R_Humi<1300)   Humi=(53200-10*R_Humi)*10/1340; //30-35
   if(310<=R_Humi&R_Humi<630)    Humi=(28700-10*R_Humi)*10/640; //35-40  
   if(160<=R_Humi&R_Humi<310)    Humi=(15100-10*R_Humi)*10/300; //40-45 
   if(87<=R_Humi&R_Humi<160)     Humi=(8170-10*R_Humi)*10/146; //45-50
   if(49<=R_Humi&R_Humi<87)      Humi=(4670-10*R_Humi)*10/76; //50-55
   if(31<=R_Humi&R_Humi<49)      Humi=(2470-10*R_Humi)*10/36; //55-60     
   if(20<=R_Humi&R_Humi<31)      Humi=(1630-10*R_Humi)*10/22; //60-65  
   if(13<=R_Humi&R_Humi<20)      Humi=(1110-10*R_Humi)*10/14; //65-70
   if(8<=R_Humi&R_Humi<13)       Humi=(7740-100*R_Humi)*10/92;//70-75
   if(6<=R_Humi&R_Humi<8)        Humi=(4890-100*R_Humi)*10/54;//75-80
 //if(5<=R_Humi&R_Humi<6)        Humi=(2010-100*R_Humi)*10/18;//80-85
   if(R_Humi<6)                  Humi=(388-10*R_Humi)*10/4;//85-90
  
  //�¶Ȳ���0-60��
  if(fushu==0)
  {
	  temp_vule=Temprature/10;
	  if(temp_vule<=25)  Humi= Humi+((25-temp_vule)*2);   //С��25,һ�ȼ�0.2
	  if((25<temp_vule)&(temp_vule<=40))  Humi= Humi-((temp_vule-25)*2);   //����25,һ�ȼ�0.2
	  if(Humi>=950)  Humi=950;  //����950
	  if(Humi<200)Humi =200;
  }
}

u16 Get_ADC10bitResult(u8 channel) //��ȡ10λ���ȵ�ADCֵ
{
    u16	adc;
	if(channel > 7)	return	1024;	//����,����0, ADCͨ����0~7�����ܳ���7	
	ADC_RES = 0; //��λADC����
	ADC_RESL = 0;//��λADC����

	ADC_CONTR = ADC_POWER | ADC_SPEEDLL| ADC_START | channel; //��ָ��ͨ����ADCת��
	_nop_();
	_nop_();
	_nop_();
	_nop_(); //��ADC_CONTR������Ҫ4T֮����ܷ���
							
	while (!(ADC_CONTR & ADC_FLAG));//�ȴ�ADCת�����
	ADC_CONTR &= ~ADC_FLAG;	//ADC_FLAG�ֹ���0 ,�ر� ADC

    adc=ADC_RES;      //�õ���8λ
    adc<<=2;
    adc+=ADC_RESL;     //�õ���2λ

	return	adc;
}
//
//void mp_()    //ð�ݷ����� �����ѵ�ֱ�Ӹ��Ƶġ�
//{
//	u8 i,j;
//	u16 m;
//	for(i=1;i<10;i++)
//	{
//		for(j=0; j<10-i; j++)
//		{
//			if(ad[j]>ad[j+1])
//			{
//				m=ad[j];
//				ad[j]=ad[j+1];
//				ad[j+1]=m;
//			}
//		}
//	}
//} 

void Timer0Init(void)		//500΢��@12.000MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x90;		//���ö�ʱ��ֵ
	TH0 = 0xE8;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0=1;
	EA=1;
}

void main(void)
{
	u8 i;
	DelayMs(100);//��ʱ100ms�ȴ��ȶ�����
	P1ASF=0x0C;//ADC��2.3ͨ������ADC����
	P1M0=0x30;//P1^4,P1^5ǿ���졣

	JLX_InitLCD();
	JLX_ClearScreen();

    JLX_DisplayString8x16(1,9,"HR202L+NTC_10K");

	JLX_DisplayPicture16x16(3,1,shi);
	JLX_DisplayPicture16x16(3,17,du);
	JLX_DisplayString8x16(3,33,":");
	JLX_DisplayString8x16(3,41," ");

	JLX_DisplayPicture16x16(5,1,wen);
	JLX_DisplayPicture16x16(5,17,du);
	JLX_DisplayString8x16(5,33,":");
	//JLX_DisplayString8x16(5,41," ");

	JLX_DisplayString8x16(7,1,"                ");
	
	Timer0Init();//��ʱ����ʼ��ʱ
	while(1)
	{
		if(IsShow==1)
		{
			IsShow=0;
			i=0;
			//�����¶�
			while(adrtc>NTCcode[i])i++;//���õ�������¶�ֵ��adc
			//��ֵ�������¶ȣ�������ȫ������10����ʹС����Ϊ������+5����������
			Temprature=i*10 +( (adrtc - NTCcode[i-1])*10 +5 )/( NTCcode[i] - NTCcode[i-1] );
		    //Temprature=Temprature-27;//�����¶ȣ���������2.7��(����10��Ϊ27)
		    if(Temprature>400)    //���¶�(����0��)
		    {
		        fushu=0;
				Temprature=Temprature-400;    //ȡ��0�������¶�
				lcd[0]=' ';
			    lcd[1]=Temprature/1000+48;
				lcd[2]=Temprature%1000/100+48;
				lcd[3]=Temprature%100/10+48;	
				lcd[4]='.';	
				lcd[5]=Temprature%10+48;
				lcd[6]=' ';	
				lcd[7]=' ';
			    lcd[8]=' ';
				lcd[9]=' ';
				if(lcd[1]=='0')lcd[1]=' ';
		    }
		    else    //���¶�(С��0��)
		    {
		        fushu=1;
				Temprature=410-Temprature;    //ȡ��0�������¶�
		        
	  			lcd[0]='-';	//��ʾ����-
			    lcd[1]=Temprature/1000+48;
				lcd[2]=Temprature%1000/100+48;
				lcd[3]=Temprature%100/10+48;	
				lcd[4]='.';	
				lcd[5]=Temprature%10+48;
				lcd[6]=' ';	
				lcd[7]=' ';
			    lcd[8]=' ';
				lcd[9]=' ';
				if(lcd[1]=='0')lcd[1]=' ';
		    }

			JLX_DisplayString8x16(5,41,lcd);//��ʾ�¶�
			JLX_DisplayPicture16x16(5,97,sheshidu);
			//����ʪ�� Rf�ο����� Rhʪ������
			R_Humi=(51000*1024)/adhr-51000;//Rf=51K��(Rh+Rf)/Rf=Vcc/Vh; 	 Rh=(Rf*Vcc/Vh)-R2
		    get_HUMI_vule();

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
		}


	}
}

//��ʱ��0
void Timer0() interrupt 1  //500΢��@12.000MHz
{
	if(Rh==1)//����1KHz�ķ����źţ��ߵ�����3��1��Rh��Rf�ĵ�ƽ�ߵ��෴��
	{
		Rh=0;
		Rf=1;
	}
	else
	{
		Rh=1;
		Rf=0; 	
	}

	cnt++;
	if(cnt>=2000&&Rh==1)//1��
	{
		//Rh��ѹΪ��ʱ������ADCֵ��
		Get_ADC10bitResult(2);//������2ͨ��adcֵ ��Ϊ��RTC���ڸ������Ͷ��ձ��෴�ˡ�
		Get_ADC10bitResult(2);//������2ͨ��adcֵ ��Ϊ��RTC���ڸ������Ͷ��ձ��෴�ˡ�
		adrtc=1023-Get_ADC10bitResult(2);//������2ͨ��adcֵ ��Ϊ��RTC���ڸ������Ͷ��ձ��෴�ˡ�
		Get_ADC10bitResult(3);//����ʪ�������adcֵ 
		Get_ADC10bitResult(3);//����ʪ�������adcֵ 
		adhr=Get_ADC10bitResult(3);//����ʪ�������adcֵ 
		cnt=0;
		IsShow=1;
	}
}

