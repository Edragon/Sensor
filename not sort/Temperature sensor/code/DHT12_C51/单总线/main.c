//12M

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

#define ID_ADDR_ROM 0x1ff7//8K程序空间的MCU

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

sbit DHT12_SDA=P1^5;

u8 lcd[10];//显示缓存

u16 Temprature,Humi;//定义温湿度变量 ，此变量为全局变量
bit fushu=0;//温度是否为负
u8 Sensor_AnswerFlag=0;//定义传感器响应标志
u8 Sensor_ErrorFlag;  //定义读取传感器错误标志

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

/********************************************\
|* 功能： 读传感器发送的单个字节	        *|
\********************************************/
u8 DHT12_Single_ReadByte(void)
{
	u8 i;
	u16 j;
	u8 Data=0;
	u8 Bit=0;
	
	for(i=0;i<8;i++)
	{
		while(!DHT12_SDA)//检测上次低电平是否结束
		{
			if(++j>=50000) //防止进入死循环
			{
				break;
			}
		}
		//延时Min=26us Max70us 跳过数据"0" 的高电平		 
		delay_us(30);

		//判断传感器发送数据位
		Bit=0;
		if(DHT12_SDA)Bit=1;
		j=0;
		while(DHT12_SDA)	//等待高电平结束
		{
			if(++j>=50000) //防止进入死循环
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
	//主机发送起始信号
	DHT12_SDA=0;	//主机把数据总线（SDA）拉低
	DelayMs(20);//拉低一段时间（至少18ms）， 通知传感器准备数据
	DHT12_SDA=1;	 //释放总线
	delay_us(30);//延时30us

	Sensor_AnswerFlag=0;	//传感器响应标志
	//判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行	  
	if(DHT12_SDA==0)
	{
		Sensor_AnswerFlag=1;	//收到起始信号

		j=0;
		while(!DHT12_SDA) //判断从机发出 80us 的低电平响应信号是否结束	
		{
			if(++j>=500) //防止进入死循环
			{
				Sensor_ErrorFlag=1;
				break;
			}
		}

		j=0;
		while(DHT12_SDA)//判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
		{
			if(++j>=800) //防止进入死循环
			{
				Sensor_ErrorFlag=1;
				break;
			}		
		}
		//接收数据
		Humi_H=DHT12_Single_ReadByte();
		Humi_L=DHT12_Single_ReadByte();
		Temp_H=DHT12_Single_ReadByte();	
		Temp_L=DHT12_Single_ReadByte();
		Temp_CAL=DHT12_Single_ReadByte();


		temp = Humi_H+Humi_L+Temp_H+Temp_L;//只取低8位

		if(Temp_CAL==temp)//如果校验成功，往下运行
		{
			Humi=Humi_H*10+Humi_L; //湿度
			if(Temp_L&0x80)	//为负温度
			{
				fushu=1;
				Temprature =Temp_H*10+(Temp_L&0x7F);
			}
			else   //为正温度
			{
				fushu=0;
				Temprature=Temp_H*10+Temp_L;//为正温度
			} 
		
			//判断温湿度是否超过测量范围（温度：-20℃~60摄氏度；湿度：20%RH~95%RH）		
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
		else //校验失败
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
	DelayMs(200);//延时200ms等待稳定运行

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


