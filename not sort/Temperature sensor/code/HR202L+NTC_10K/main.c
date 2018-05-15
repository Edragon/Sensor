#include <reg51.h>
#include <intrins.h>
#include "StringData.h"

#define uchar unsigned char
#define uint  unsigned int
#define u8 unsigned char
#define u16  unsigned int
#define u32  unsigned long int

//* STC15W408AS寄存器补充 
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
sfr ADC_CONTR = 0xBC;	//带AD系列
sfr ADC_RES   = 0xBD;	//带AD系列
sfr ADC_RESL  = 0xBE;	//带AD系列
sfr P1ASF = 0x9D;
sfr PCON2   = 0x97;

//定时器2
sfr T2H=0xD6;
sfr T2L=0xD7;
sfr IE2=0xAF;

#define ADC_POWER	0x80//ADC电源控制位
#define ADC_FLAG	0x10//ADC完成标志
#define ADC_START	0x08//ADC起始控制位
#define ADC_SPEEDLL	0x00//540个时钟转换一次
#define ADC_SPEEDL	0x20//360时钟
#define ADC_SPEEDH	0x40//180时钟
#define ADC_SPEEDHH	0x60//90时钟


sbit JLX_CS=P3^3;
sbit JLX_RST=P3^6;
sbit JLX_RS=P3^7;
sbit JLX_SCK=P1^0;
sbit JLX_SDA=P1^1;

sbit Rh=P1^5;//接湿敏电阻
sbit Rf=P1^4;//51K参考电阻
//HR P1^3  Temp P1^2;

u8 lcd[10];//显示缓存
u16 adrtc=0;//表示热敏电阻RTC的AD
u16 adhr=0;//表示湿敏电阻的AD
u16 ad9=0;//第九通道bandgap的adc值
u16 cnt=0;//计数器
u32 R_Humi;//湿度的电阻
u16 Temprature;
u32 Humi;//定义温湿度变量 ，此变量为全局变量
bit fushu=0;//温度是否为负
bit IsShow=0;//是否显示数据
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

void DelayMs(unsigned int i)  //延时N毫秒，0~65535ms
{
   while(i--) Delay1ms();
}


/*****************液晶屏函数**********************/
 /*写指令到 LCD 模块*/
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

/*写数据到 LCD 模块*/
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

/*LCD 模块初始化*/
void JLX_InitLCD()
{
	JLX_CS=0;
	JLX_RST=0;	/*低电平复位*/
	DelayMs(2);
	JLX_RST=1;	/*复位完毕*/
	DelayMs(10);
	JLX_SendCommand(0xE2);	/*软复位*/
	DelayMs(1);
	JLX_SendCommand(0x2C); /*升压步聚 1*/
	JLX_SendCommand(0x2E); /*升压步聚 2*/
	JLX_SendCommand(0x2F); /*升压步聚 3*/
	DelayMs(1);
	JLX_SendCommand(0x23); /*粗调对比度，可设置范围 0x20～0x27*/
	JLX_SendCommand(0x81); /*微调对比度*/
	JLX_SendCommand(0x28); /*0x1a,微调对比度的值，可设置范围 0x00～0x3f*/
	JLX_SendCommand(0xA2); /*1/9 偏压比（bias）*/
	JLX_SendCommand(0xC8); /*行扫描顺序：从上到下*/
	JLX_SendCommand(0xA0); /*列扫描顺序：从左到右*/
	JLX_SendCommand(0x40); /*起始行：第一行开始*/
	JLX_SendCommand(0xAF); /*开显示*/
	JLX_CS=1;
}

/*设置液晶屏的显示地址*/
void JLX_LCDAddress(u8 Page,u8 Column)
{
	JLX_CS=0;
	Column=Column-1;	//我们平常所说的第 1 列，在 LCD 驱动 IC 里是第 0 列。所以在这里减去 1.
	Page=Page-1;
	JLX_SendCommand(0xB0+Page);	//设置页地址。每页是 8 行。一个画面的 64 行被分成 8 个页。
	                            //我们平常所说的第 1 页，在 LCD 驱动 IC 里是第 0 页，所以在这里减去 1
	JLX_SendCommand(((Column>>4)&0x0F)+0x10); //设置列地址的高 4 位 
	JLX_SendCommand(Column&0x0F);	//设置列地址的低 4 位
}

/*全屏清屏*/
void JLX_ClearScreen()
{
	u8 i,j;
	JLX_CS=0;
	for(i=0;i<9;i++)
	{
		JLX_LCDAddress(1+i,1);
		for(j=0;j<132;j++)
		{
			JLX_SendData(0x00);	/*写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1*/
		}
	}
	JLX_CS=1;
}


//全屏显示黑点 128*64 dots
//void JLX_FullScreenDots()
//{
//	u8 i,j;
//    JLX_CS=0;
//	for(i=0;i<8;i++)
//	{
//		JLX_LCDAddress(i+1,1);
//		for(j=0;j<128;j++)
//		{
//			JLX_SendData(0xFF);/*写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1*/
//		}
//	}
//	JLX_CS=1;
//}


/*显示 12864 点阵图像*/

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
//			JLX_SendData(*dp);	/*写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1*/
//			dp++;
//		}
//	}
//	JLX_CS=1;
//}

/*显示 32x32 点阵图像、汉字、生僻字或 32x32 点阵的其他图标*/
//void JLX_DisplayPicture32x32(u8 Page,u8 Column,u8 *dp)
//{
//	u8 i,j;
//	JLX_CS=0;
//	for(j=0;j<4;j++)
//	{
//		JLX_LCDAddress(Page+j,Column);
//		for (i=0;i<31;i++)
//		{
//			JLX_SendData(*dp);	/*写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1*/
//			dp++;
//		}
//	}
//	JLX_CS=1;
//}

/*显示 16x16 点阵图像、汉字、生僻字或 16x16 点阵的其他图标*/
void JLX_DisplayPicture16x16(u8 Page,u8 Column,u8 *dp)
{
	u8 i,j;
	JLX_CS=0;
	for(j=0;j<2;j++)
	{
		JLX_LCDAddress(Page+j,Column);
		for (i=0;i<16;i++)
		{
			JLX_SendData(*dp);	/*写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1*/
			dp++;
		}
	}
	JLX_CS=1;
}

/*显示 8x16 点阵图像、ASCII, 或 8x16 点阵的自造字符、其他图标*/
//void JLX_DisplayPicture8x16(u8 Page,u8 Column,u8 *dp)
//{
//	u8 i,j;
//	JLX_CS=0;
//	for(j=0;j<2;j++)
//	{
//		JLX_LCDAddress(Page+j,Column);
//		for (i=0;i<8;i++)
//		{
//			JLX_SendData(*dp);	/*写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1*/
//			dp++;
//		}
//	}
//	JLX_CS=1;
//}

//显示8*16的字符串
void JLX_DisplayString8x16(u8 Page,u8 Column,u8 *text)
{
	u8 i=0,j,k,n;
	JLX_CS=0;
	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<=0x7E)) //32~126 ASCII码表的字符串数值之内
		{
			j=text[i]-0x20;//ASCII码值-32，刚好是该字符的ascii_table_8x16数组中点阵开始地址
			for(n=0;n<2;n++)
			{
				JLX_LCDAddress(Page+n,Column);
				for(k=0;k<8;k++)
				{
					JLX_SendData(ascii_table_8x16[j][k+8*n]);/*显示 5x7 的 ASCII 字到 LCD 上，y 为页地址，x 为列地址，最后为数据*/
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


//显示5*7的字符。用的是6*8的数据，基本是全部的液晶都是这样。
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
//				JLX_SendData(ascii_table_5x7[j][k]);/*显示 5x7 的 ASCII 字到 LCD 上，y 为页地址，x 为列地址，最后为数据*/
//			}
//			i++;
//			Column+=6;
//		}
//		else
//		i++;
//	}
//}

/***********************液晶屏函数结束****************/

	/*************************************
	NTC阻值与温度关系表
	Vcc---NTC(±1％)---R(10K±1％)---GND
	                |
	              ADC端子
	**************************************/
	unsigned int code NTCcode[] = {
	/*ADC采样值****温度值*******电阻值**********/   
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
  //分段计算湿度值 
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
  
  //温度补偿0-60℃
  if(fushu==0)
  {
	  temp_vule=Temprature/10;
	  if(temp_vule<=25)  Humi= Humi+((25-temp_vule)*2);   //小于25,一度加0.2
	  if((25<temp_vule)&(temp_vule<=40))  Humi= Humi-((temp_vule-25)*2);   //大于25,一度减0.2
	  if(Humi>=950)  Humi=950;  //大于950
	  if(Humi<200)Humi =200;
  }
}

u16 Get_ADC10bitResult(u8 channel) //获取10位精度的ADC值
{
    u16	adc;
	if(channel > 7)	return	1024;	//错误,返回0, ADC通道号0~7，不能超过7	
	ADC_RES = 0; //高位ADC清零
	ADC_RESL = 0;//低位ADC清零

	ADC_CONTR = ADC_POWER | ADC_SPEEDLL| ADC_START | channel; //打开指定通道的ADC转换
	_nop_();
	_nop_();
	_nop_();
	_nop_(); //对ADC_CONTR操作后要4T之后才能访问
							
	while (!(ADC_CONTR & ADC_FLAG));//等待ADC转换完成
	ADC_CONTR &= ~ADC_FLAG;	//ADC_FLAG手工清0 ,关闭 ADC

    adc=ADC_RES;      //得到高8位
    adc<<=2;
    adc+=ADC_RESL;     //得到低2位

	return	adc;
}
//
//void mp_()    //冒泡法排序 网上搜的直接复制的。
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

void Timer0Init(void)		//500微秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x90;		//设置定时初值
	TH0 = 0xE8;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0=1;
	EA=1;
}

void main(void)
{
	u8 i;
	DelayMs(100);//延时100ms等待稳定运行
	P1ASF=0x0C;//ADC的2.3通道进行ADC测量
	P1M0=0x30;//P1^4,P1^5强推挽。

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
	
	Timer0Init();//定时器开始计时
	while(1)
	{
		if(IsShow==1)
		{
			IsShow=0;
			i=0;
			//计算温度
			while(adrtc>NTCcode[i])i++;//查表得到最近的温度值的adc
			//插值法计算温度，将数据全部扩大10倍，使小数变为整数，+5是四舍五入
			Temprature=i*10 +( (adrtc - NTCcode[i-1])*10 +5 )/( NTCcode[i] - NTCcode[i-1] );
		    //Temprature=Temprature-27;//修正温度，比正常高2.7度(扩大10倍为27)
		    if(Temprature>400)    //正温度(大于0度)
		    {
		        fushu=0;
				Temprature=Temprature-400;    //取得0°以上温度
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
		    else    //负温度(小于0度)
		    {
		        fushu=1;
				Temprature=410-Temprature;    //取得0°以下温度
		        
	  			lcd[0]='-';	//显示负号-
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

			JLX_DisplayString8x16(5,41,lcd);//显示温度
			JLX_DisplayPicture16x16(5,97,sheshidu);
			//计算湿度 Rf参考电阻 Rh湿敏电阻
			R_Humi=(51000*1024)/adhr-51000;//Rf=51K，(Rh+Rf)/Rf=Vcc/Vh; 	 Rh=(Rf*Vcc/Vh)-R2
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

//定时器0
void Timer0() interrupt 1  //500微秒@12.000MHz
{
	if(Rh==1)//生产1KHz的方波信号，高低脉冲3：1。Rh和Rf的电平高低相反。
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
	if(cnt>=2000&&Rh==1)//1秒
	{
		//Rh电压为高时，测量ADC值。
		Get_ADC10bitResult(2);//测量第2通道adc值 因为我RTC焊在负极，和对照表相反了。
		Get_ADC10bitResult(2);//测量第2通道adc值 因为我RTC焊在负极，和对照表相反了。
		adrtc=1023-Get_ADC10bitResult(2);//测量第2通道adc值 因为我RTC焊在负极，和对照表相反了。
		Get_ADC10bitResult(3);//测量湿敏电阻的adc值 
		Get_ADC10bitResult(3);//测量湿敏电阻的adc值 
		adhr=Get_ADC10bitResult(3);//测量湿敏电阻的adc值 
		cnt=0;
		IsShow=1;
	}
}

