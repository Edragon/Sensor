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

sbit IIC_SDA=P1^5;
sbit IIC_SCL=P1^3;

u8 lcd[10];//显示缓存

u16 Temprature,Humi;//定义温湿度变量 ，此变量为全局变量
bit fushu=0;//温度是否为负

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

void delay_IIC(void) //Delay10us()	@12.000MHz  
{//IIC总线限速延时函数 Delay10us。
	unsigned char i;
	_nop_();
	_nop_();
	i = 27;
	while (--i);
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


//产生IIC起始信号
void IIC_Start(void)
{
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_IIC();
 	IIC_SDA=0;//开始：当SCL为高时，数据从高到低变化
	delay_IIC();
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	IIC_SCL=0;
	IIC_SDA=0;//停止:当SCL为高时，数据从低到高变化
 	delay_IIC();
	IIC_SCL=1;
	delay_IIC();	 
	IIC_SDA=1;//发送I2C总线结束信号
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC_SCL=0;
	IIC_SDA=1;
//	SDA_IN();      //SDA设置为输入  
	delay_IIC();	   
	IIC_SCL=1;
	delay_IIC();	 
	while(IIC_SDA)//等到SDA变低
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
		delay_us(1);
	}
	IIC_SCL=0;//时钟输出0
	delay_IIC(); 
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	//SDA_OUT();
	IIC_SDA=0;
	delay_IIC();
	IIC_SCL=1;
	delay_IIC();
	IIC_SCL=0;
}
//产生非ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	//SDA_OUT();
	IIC_SDA=1;
	delay_IIC();
	IIC_SCL=1;
	delay_IIC();
	IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	//SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
		IIC_SCL=0;
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1; 	  
		delay_IIC();   //延时
		IIC_SCL=1;
		delay_IIC(); 	
    }
	IIC_SCL=0;	
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
		unsigned char i,receive=0;
	//SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_IIC();
		IIC_SCL=1;
		delay_IIC();
        receive<<=1;
        if(IIC_SDA)receive++;   
    }
        IIC_SCL=0;						 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}

unsigned char DHT12_IIC_ReadByte(unsigned char Waddr,unsigned char ack)
{//从任意地址读取一个字节数据函数
    unsigned char Data;//定义一个缓冲寄存器。
    IIC_Start();//产生起始信号
    IIC_Send_Byte(0xB8);//写入设备地址（写）
    IIC_Wait_Ack();//等待设备的应答
    IIC_Send_Byte(Waddr);//写入要操作的单元地址。
    IIC_Wait_Ack();//等待设备的应答。
    IIC_Stop();//产生停止符号。
    IIC_Start();//产生起始信号
    IIC_Send_Byte(0xB9);//写入设备地址（写）
    IIC_Wait_Ack();//等待设备的应答
    Data=IIC_Read_Byte(ack);//写入数据。
    IIC_Stop();//产生停止符号。
	delay_us(1000);
    //-------------------返回读取的数据--------------------
    return Data;//返回读取的一个字节数据。
}


/********************************************\
|* 功能：DHT12读取温湿度函数       *|
\********************************************/
//变量：Humi_H：湿度高位；Humi_L：湿度低位；Temp_H：温度高位；Temp_L：温度低位；Temp_CAL：校验位
//数据格式为：湿度高位（湿度整数）+湿度低位（湿度小数）+温度高位（温度整数）+温度低位（温度小数）+ 校验位
//校验：校验位=湿度高位+湿度低位+温度高位+温度低位

void Sensor_Read(void)
{	
	u8 Humi_H,Humi_L,Temp_H,Temp_L,Temp_CAL,temp;


	Humi_H=DHT12_IIC_ReadByte(0,0);   //读取湿度高位
	Humi_L=DHT12_IIC_ReadByte(1,0);   //读取湿度低位
	Temp_H=DHT12_IIC_ReadByte(2,0);   //读取温度高位
	Temp_L=DHT12_IIC_ReadByte(3,0);   //读取温度低位
	Temp_CAL=DHT12_IIC_ReadByte(4,0); //读取校验位

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

void main(void)
{
	DelayMs(200);//延时200ms等待稳定运行

	JLX_InitLCD();
	JLX_ClearScreen();

    JLX_DisplayString8x16(1,1,"    DHT12_IIC   ");

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


