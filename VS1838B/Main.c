/*
 * VS1838B红外接收模块测试程序
 * 
 * 用途：VS1838B红外接收模块测试程序
 * 
 * 作者					日期				备注
 * Huafeng Lin			2012/08/23			新增
 * Huafeng Lin			2012/08/23			修改
 * 
 */

#include <reg52.h>
#define uchar unsigned char
#define uint8 unsigned char
 
sbit P2_0 = P2^0;
sbit P2_1 = P2^1;
sbit P2_2 = P2^2;
sbit P2_3 = P2^3;

sbit P3_3=P3^3;		//定义红外接收脚

uchar buf[66];		//定义66位数组变量来存储接收的时间参数

uchar DisBuff[4];

uchar code dis_code[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x84,0x8e};

void delay(uchar i)
{
	uchar j,k; 
	for(j=i;j>0;j--)
		for(k=125;k>0;k--);
}

void delay1(void)	//空5个指令
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

void ir_recv(void) interrupt 2	//外部中断1，INT1（P3^3）连接红外线接收IC数据脚
{
	unsigned char i,j,tmp;

	EX1=0;
	j=33;						//传送一组数包括引导码1位，地址码8位加反码8位，指令码8位加反码8位，总共33位
	i=0;						//从第一维数组开始
	tmp=100;					//加八延时，
	while(tmp){
	tmp--;
	}
	if(P3_3){					//然后再检测红线接收脚是有数据招收，有继续，没有则退出
		EX1=1;
		return;
	}	
	while(j){					//循环接收33位数据，为何我们用到66位数组，我们可以不接收高电平时间常数，只接低电平常数就
								//可以判断1或0了，在这里我们都接收，为串口调试软件接收计算波形
								//还有一点要知道，接收波形是反向，在没有接收时端口为高电平
		tmp=0;
		buf[i]=1;				//时间量从1开始
		while(!P3_3){			//检测高低电平的变化
			buf[i]++;			//没变继续加1
			delay1();			//加入延时，是因为我们采用8位二进来存储，如果不加延时，时间量将计满
			tmp++;				//此变量为防止干扰带来的死循环
			if(tmp==250)
				break;
		}
		i++;
		tmp=0;
		buf[i]=1;				//时间量从1开始
		while(P3_3){			//检测高低电平的变化
			buf[i]++;			//没变继续加1	
			delay1();			//同上
			tmp++;
			if(tmp==250)
				break;
		}	
		i++;		
		j--;
	}
	i=255;						//加入循环延时，抗干扰
	while(i){
	tmp=255;
	while(tmp){
	tmp--;
	}
	i--;			
	}
	tmp=0;
	for(i=3;i<19;i++,i++){		//处理地址位，对低电平时间数据的数理，这里我们只处理地址码和指令码，引导码和反码我们就忽略
		tmp>>=1;				//右移一位，接收低电平在前
		if(buf[i]>30)			//检测低电平时间超过30就确认为1
			tmp+=0x80;
	}	
	DisBuff[3]=tmp/16;			//分开2位以16进制显示
	DisBuff[2]=tmp%16;
	tmp=0;
	for(i=35;i<51;i++,i++){		//同上，这里处理指令位
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

	EA=1;		//首先开启总中断
	EX1=1;		//开启外部中断 1
	IT1=1;		//设置成下降沿触发方式
   
	while(1)
	{
		for(a=100;a>0;a--)
		{
			display();
		} 
	}
}