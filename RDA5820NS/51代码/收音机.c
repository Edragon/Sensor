#include "stc12c5608.h"	
////////////////////////////////////////////////
#define read_add 0x23
#define write_add 0x22
 
////////////////////////////////////////////////在这里写地址
sbit en=P1^7;
sbit rw=P2^0;
sbit rs=P2^1;
sbit d0=P1^6;
sbit d1=P1^5;
sbit d2=P1^4;
sbit d3=P1^3;
sbit d4=P1^2;
sbit d5=P1^1;
sbit d6=P1^0;
sbit d7=P3^7;
///////////////////////////////////////////////////////////////
sbit scl=P2^4;
sbit sda=P2^5;
sbit led=P3^5;

sbit s1=P2^2;
sbit s2=P2^3;
sbit s3=P3^2;

////////////////////////////////////////////////////////////////
unsigned char code dis_table_num[]="0123456789";
unsigned char code dis_table_rx[]="  FM RECEIVER   ";
unsigned char code dis_table_rxs[]=" -FM RECEIVER-  ";
unsigned char code dis_table_rx_autos[]="     -AUTO-     ";
unsigned char code dis_table_rx_auto[]="      AUTO      ";
unsigned char code dis_table_rx_manuals[]="    -MANUAL-    ";
unsigned char code dis_table_rx_manual[]="     MANUAL     ";
unsigned char code dis_table_tx[]=" FM TRANSMITTER ";
unsigned char code dis_table_txs[]="-FM TRANSMITTER-";
unsigned char code dis_table_start[]="    WELCOME     ";
unsigned char code dis_table_wait[]=" .............. ";
unsigned char code dis_table_power[]="   SET POWER    ";
/////////////////////////////////////////////////
void delay_1ms(unsigned int num)
{
  unsigned int i,c;
  for(i=num;i>0;i--)
         for(c=70;c>0;c--);
}
void delay_nop()
{;;;;}
///////////////////////////////////
/********************************************************
                     下面是1602
********************************************************/
unsigned code LevlChar[5][8] =
{{0x1F,0x11,0x0A,0x04,0x04,0x04,0x04,0x04},
{0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x1E},
{0x00,0x00,0x00,0x00,0x06,0x1E,0x1E,0x1E},     //显示型号强度的自定义字符
{0x00,0x00,0x06,0x1E,0x1E,0x1E,0x1E,0x1E},
{0x06,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E}};  
/////////////////////////////////////////////////

void write_com_1602(unsigned char com)
{ 
  unsigned char temp;
  temp=com;
  rs=0;
  temp=temp<<1;
  d7=CY;
  temp=temp<<1;
  d6=CY;
  temp=temp<<1;
  d5=CY;
  temp=temp<<1;
  d4=CY;
  temp=temp<<1;
  d3=CY;
  temp=temp<<1;
  d2=CY;
  temp=temp<<1;
  d1=CY;
  temp=temp<<1;
  d0=CY;
  delay_1ms(5);
  en=1;
  delay_1ms(5);
  en=0;
}
void write_data_1602(unsigned char date)
{ 
   unsigned char temp;
   temp=date;
   rs=1;
  temp=temp<<1;
  d7=CY;
  temp=temp<<1;
  d6=CY;
  temp=temp<<1;
  d5=CY;
  temp=temp<<1;
  d4=CY;
  temp=temp<<1;
  d3=CY;
  temp=temp<<1;
  d2=CY;
  temp=temp<<1;
  d1=CY;
  temp=temp<<1;
  d0=CY;
   delay_1ms(5);
   en=1;
   delay_1ms(5);
   en=0;
}
void init_1602()
{  unsigned char i,j;
   unsigned char CGRamAddr = 0x40;
   en=0;
   rw=0;
   rs=0;
   write_com_1602(0x38);
   write_com_1602(0x0c);
   write_com_1602(0x06);
   write_com_1602(0x01); 
     for(j=0;j<5;j++)
	 {
  write_com_1602(CGRamAddr + 8*j); //建立自定义字符 (显示信号强度的)
     for(i=0;i<8;i++) 
  {
  write_data_1602(LevlChar[j][i]);
   
  }
}
}
/********************************************************
                   以下是i2c
*********************************************************/
/////////////////////////////////////////////////////延时
void init_i2c()
{
   sda=1;
   delay_nop();
   scl=1;
   delay_nop();
}
void start_i2c()
{
   scl=1;
   delay_nop();
   sda=1;
   delay_nop();
   sda=0;
   delay_nop();
   
   
   
}
void stop_i2c()
{
  sda=0;
  delay_nop();
  scl=1;
  delay_nop();
  sda=1;
  delay_nop();
}
void ac_i2c()
{ 
  scl=0;
  delay_nop(); 
  sda=0;
  delay_nop();
  delay_nop();
  scl=1;
  delay_nop();
  scl=0;
 
}
void no_ac_i2c()
{ 
  scl=0;
  delay_nop(); 
  sda=1;
  delay_nop();
  delay_nop();
  scl=1;
  delay_nop();
  scl=0;
 
}
void respons_i2c()
{
  unsigned char i;
  sda=1;
  delay_nop();
  scl=1;
  delay_nop();
  while((sda==1)&&(i<250))
  {i++;}
  scl=0;
  delay_nop();

}
void write_byte_i2c(unsigned char dt)
{
  unsigned char temp,i;
  temp=dt;
  for(i=0;i<8;i++)
  {
       temp=temp<<1;
	   scl=0;
	   delay_nop();
	   sda=CY;
	   delay_nop();
	   scl=1;
	   delay_nop();

  }
  scl=0;
  delay_nop();
  sda=1;
  delay_nop();

}
unsigned char read_byte_i2c()
{
    unsigned int i,dt;
	scl=0;
	delay_nop();
	sda=1;
	delay_nop();
	for(i=0;i<8;i++)
	{
	  scl=1;
	  delay_nop();
	  dt=(dt<<1)|sda;
	  scl=0;
	  delay_nop();
	}
    delay_nop();
	delay_nop();

	return dt;
}
void write_i2c(unsigned char add,int dt)
{	  
      int temp;
      temp=dt;
	  temp=temp>>8;
      start_i2c();
	  write_byte_i2c(write_add);
	  respons_i2c();
	  write_byte_i2c(add);
	  respons_i2c();
	  write_byte_i2c((unsigned char)(temp));
	  respons_i2c();
	  write_byte_i2c((unsigned char)(dt));
	  respons_i2c();
	  stop_i2c();
}
unsigned int read_i2c(unsigned char add)
{
      unsigned int temp1,temp2,dt;
	  temp1=0;
	  temp2=0;
      start_i2c();
	  write_byte_i2c(write_add);
	  respons_i2c();
	  write_byte_i2c(add);
	  respons_i2c();
	  ///////////////
	  delay_1ms(5);
	  start_i2c();
	  write_byte_i2c(read_add);
	  respons_i2c();
	  temp1=read_byte_i2c();
	  ac_i2c();
	  temp2=read_byte_i2c();
	  no_ac_i2c(); 
	  stop_i2c();
	  temp1=(temp1<<8);
	  dt=(temp1|temp2);
	  return dt;
     
}
 
/*************************************************************/
//unsigned int get_adc()
//{				   	 
//     unsigned int result;
//     ADC_CONTR=0xe0;
//     delay_1ms(10);
//     ADC_CONTR=ADC_CONTR|0x08;
//	 delay_1ms(10);
//	 while(!(ADC_CONTR&0x10));
//	 result=ADC_DATA;
//	 result=result<<2;
//	 result=result|ADC_LOW2;
//	 return result;
//}
unsigned char scan_key() //扫描按键
{	 
//     unsigned int result;
     unsigned char key_num=0;
//     result=get_adc();
//	 if(result<0x0199) //按键1按下
//	 { 
//	   key_num=1;
//	   led=1;	  
//	   while(get_adc()<0x332); 
//	   led=0;
//	 }
//	 if(result>0x0199&&result<0x0232) //按键2按下
//	 {
//	   key_num=2;
//	   led=1;
//	   while(get_adc()<0x332); 
//	   led=0;
//	 }
//	 if(result>0x0232&&result<0x332) //按键3按下
//	 {
//	   key_num=3;
//	   led=1; 
//	    while(get_adc()<0x332);
//	   led=0;
//	 }

	if(s1==0)
	{
		delay_1ms(10);
		if(s1==0)
		{
			while(!s1);
			key_num=1;
		}
	}
	if(s2==0)
	{
		delay_1ms(10);
		if(s2==0)
		{
			while(!s2);
			key_num=2;
		}
	}
	if(s3==0)
	{
		delay_1ms(10);
		if(s3==0)
		{
			while(!s3);
			key_num=3;
		}
	}

	 return key_num;

}
void dis_clear(bit num)	 //清屏
{
   unsigned char i;	 
   if(num==1)
   { write_com_1602(0x80);
     for(i=0;i<16;i++)
	 {write_data_1602(' ');}
	 write_com_1602(0xc0);
     for(i=0;i<16;i++)
	 {write_data_1602(' ');} 
   }
   else
   {
      write_com_1602(0x80);
     for(i=0;i<5;i++)
	 {write_data_1602(' ');}
   } 
}
void dis_wait()
{
    unsigned char i;
	dis_clear(1);
    write_com_1602(0xc0);
	for(i=0;i<16;i++)
	{
	   write_data_1602(dis_table_wait[i]);
	   delay_1ms(300);
	}
}
unsigned int auto_seek(unsigned int fm_fre,bit up_dowm)	   //自动搜索返回当前频率
{		 	
     unsigned int temp=0,mode=0;
     bit stc=0;
	 dis_wait();
	 fm_fre=fm_fre<<6;	
     write_i2c(0x03,(0x0010|fm_fre));
	 if(up_dowm==1)
	   {mode=0xd381;}
	 else
	   {mode=0xd181; }
     do
      { 
	   delay_1ms(10);
	   write_i2c(0x02,mode); 		   		
	   temp=read_i2c(0x0a);
	   temp=temp<<2;
	   stc=CY;
	   }while(stc==0);
	  fm_fre=(read_i2c(0x0a)&0x03ff);
	  write_i2c(0x02,0xd281);
	  return fm_fre;
			
}
void dis_fm_fre(bit num)	   //显示频率
{	
    unsigned int fm_fre=0;
    
	if(num==0)
	{
	fm_fre=(read_i2c(0x0a)&0x03ff);
    fm_fre=fm_fre+870;
    write_com_1602(0xc3);
	write_data_1602('F');
	write_data_1602('M');
	write_data_1602(':');
	write_data_1602(dis_table_num[fm_fre/1000]);
	write_data_1602(dis_table_num[(fm_fre%1000)/100]);
	write_data_1602(dis_table_num[((fm_fre%100)/10)]);
	write_data_1602('.');
	write_data_1602(dis_table_num[fm_fre%10]);
	}
	else
	{
//	fm_fre=(read_i2c(0x0a)&0x03ff);
    fm_fre=(read_i2c(0x03)>>6);
    fm_fre=fm_fre+760;
    write_com_1602(0xc3);
	write_data_1602('F');
	write_data_1602('M');
	write_data_1602(':');
	write_data_1602(dis_table_num[fm_fre/1000]);
	write_data_1602(dis_table_num[(fm_fre%1000)/100]);
	write_data_1602(dis_table_num[((fm_fre%100)/10)]);
	write_data_1602('.');
	write_data_1602(dis_table_num[fm_fre%10]);
	}
}

void dis_volume() //显示声音
{   
	unsigned char volume;
	volume=read_i2c(0x05);
	volume=(volume&0x000f);
	write_com_1602(0x87);
	write_data_1602('V');
	write_data_1602(':');
	write_data_1602(dis_table_num[(volume/10)]);
	write_data_1602(dis_table_num[(volume%10)]);

}
void dis_mode(bit num)		//显示模式参数为1时发射，为0时接收
{
     write_com_1602(0x8c);
	 if(num==0)
	 {
	   write_data_1602('R');
	   write_data_1602('X');
	 }
	 else
	 {
	   write_data_1602('T');
	   write_data_1602('X');
	 }
}
void dis_power_tx(bit num)
{
    unsigned int power;
	unsigned char i;
   	power=(read_i2c(0x42)&0x003f);
	if(num==0)
	{
	write_com_1602(0x80);
	for(i=0;i<16;i++)
	{write_data_1602(dis_table_power[i]);}
	write_com_1602(0xc6);
	if(power<0x0f)
	{//write_data_1602(0x00);
	write_data_1602(0x01);}
	else if(power<0x20)
	{//write_data_1602(0x00);
	write_data_1602(0x01);
	write_data_1602(0x02);}
	else if(power<0x30)
    {//write_data_1602(0x00);
	write_data_1602(0x01);
	write_data_1602(0x02);
	write_data_1602(0x03);}
	else if(power<0x40)
	{//write_data_1602(0x00);
	write_data_1602(0x01);
	write_data_1602(0x02);
	write_data_1602(0x03);
    write_data_1602(0x04);}
	}
	else
	{
	write_com_1602(0x80);
	if(power<0x0f)
	{//write_data_1602(0x00);
	write_data_1602(0x01);}
	else if(power<0x20)
	{//write_data_1602(0x00);
	write_data_1602(0x01);
	write_data_1602(0x02);}
	else if(power<0x30)
    {//write_data_1602(0x00);
	write_data_1602(0x01);
	write_data_1602(0x02);
	write_data_1602(0x03);}
	else if(power<0x40)
	{//write_data_1602(0x00);
	write_data_1602(0x01);
	write_data_1602(0x02);
	write_data_1602(0x03);
    write_data_1602(0x04);}
	}

}

void dis_rssi()		//显示信号
{	
    unsigned int rssi;
    dis_clear(0);
	rssi=read_i2c(0x0b);
	rssi=rssi>>10; 
   	write_com_1602(0x80);
	if(rssi<0x07)
	{//write_data_1602(0x00);
	write_data_1602(0x01);}
	else if(rssi<0x0e)
	{//write_data_1602(0x00);
	write_data_1602(0x01);
	write_data_1602(0x02);}
	else if(rssi<0x15)
    {//write_data_1602(0x00);
	write_data_1602(0x01);
	write_data_1602(0x02);
	write_data_1602(0x03);}
	else if(rssi<0x1c)
	{//write_data_1602(0x00);
	write_data_1602(0x01);
	write_data_1602(0x02);
	write_data_1602(0x03);
    write_data_1602(0x04);}
}
void set_volume(unsigned int num)
{
   write_i2c(0x05,(num|0x83a0));
   
}
void set_power_tx(unsigned int power)
 
{
   write_i2c(0x42,(0x0700|power));    
}
void set_fm_fre_rx(unsigned int fm_fre)
{
    fm_fre=fm_fre<<6;
	write_i2c(0x02,0xd281);
	write_i2c(0x03,(0x0010|fm_fre));
	delay_1ms(20);
}
void set_fm_fre_tx(unsigned int fm_fre)
{
    fm_fre=fm_fre<<6;
	write_i2c(0x02,0xd281);
	write_i2c(0x03,(0x0018|fm_fre));
	delay_1ms(20);
}

void start()//显示开机字母
{
    unsigned char i; 
	write_com_1602(0x80);
	for(i=0;i<16;i++)
	{
	   write_data_1602(dis_table_start[i]);
	   delay_1ms(1000);
	}
	write_com_1602(0xc0);
	for(i=0;i<16;i++)
	{
	   write_data_1602(dis_table_wait[i]);
	   delay_1ms(800);
	}
}

void dis_menu(unsigned char num)//显示菜单
{
    
    unsigned char i;
	if(num==0)
	{
	 dis_clear(1); 
	 write_com_1602(0x80);
	 for(i=0;i<16;i++)
	 {
	   write_data_1602(dis_table_rxs[i]);
	   
	 }
	 write_com_1602(0xc0);
	 for(i=0;i<16;i++)
	 {
	   write_data_1602(dis_table_tx[i]);	   
	 }
	}
	if(num==1)
	{
	  dis_clear(1);
	  write_com_1602(0x80);
	 for(i=0;i<16;i++)
	 {
	  write_data_1602(dis_table_rx[i]);  
	 }
	 write_com_1602(0xc0);
	 for(i=0;i<16;i++)
	 {
	   write_data_1602(dis_table_txs[i]);

	 }
	}
	if(num==2)
	{
	  dis_clear(1);
	  write_com_1602(0x80);
	 for(i=0;i<16;i++)
	 {
	  write_data_1602(dis_table_rx_autos[i]);  
	 }
	 write_com_1602(0xc0);
	 for(i=0;i<16;i++)
	 {
	   write_data_1602(dis_table_rx_manual[i]);

	 }
	}
	if(num==3)
	{
	  dis_clear(1);
	  write_com_1602(0x80);
	 for(i=0;i<16;i++)
	 {
	  write_data_1602(dis_table_rx_auto[i]);  
	 }
	 write_com_1602(0xc0);
	 for(i=0;i<16;i++)
	 {
	   write_data_1602(dis_table_rx_manuals[i]);

	 }
	}
}
//void init_stc12c5608() //初始化单片机I0
//{
//    P1M0=0x07;
//	P1M1=0x06;
//	P3M0=0x00;
//	P3M1=0x80;
//}
void function() interrupt 1 
{
   TH0 = (65536 - 20000) / 256;//载入高8位初值
   TL0 = (65536 - 20000) % 256;//载入低8位初值  
   dis_rssi();
}
void main()
{	 
    unsigned int fm_fre=0x0000,power_tx=0x0000;
	unsigned char key_num=0;
	bit seek_mode=0,select=0,tf=1;
    TMOD = 0x01;//设置定时器0 工作方式0
    TH0 = (65536 - 20000) / 256;//载入高8位初值
    TL0 = (65536 - 20000) % 256;//载入低8位初值    
    ET0  =1;//开定时器中断。为0则表示关闭！
    TR0 =1; //启动定时器
	EA =0;//开总中断
	led=0;
	init_1602();
	start();
//    init_stc12c5608();	
	init_i2c();
	set_volume(0x0007);
	dis_clear(1);
	dis_menu(0);
	while(1)
	{ 
	     key_num=scan_key();
		 	  
	     if(key_num==1)//有确认按键按下
		 {           
		            if(select==0)//就为接受模式
		              { 
					      dis_clear(1);
	                      dis_menu(2);
						  write_i2c(0x40,0x0000);
						  tf=1;
						  while(tf)
						  	{
							    key_num=scan_key();
								if(key_num==1)
								{
								     if(seek_mode==0)	//自动搜台
									 { 
									  EA=0;
							          fm_fre=auto_seek(fm_fre,1);
									  dis_clear(1);
									  dis_volume();
									  dis_fm_fre(0);
									  dis_mode(0);
									  EA=1;
									  tf=1;
					                  while(tf)
		                               {
					                         key_num=scan_key();
								             if(key_num==1)		  //退回到上一级
								              { EA=0;
								                dis_clear(1);
								                dis_menu(0);
												select=0;
												seek_mode=0;
									            tf=0;
								              }

						                     if(key_num==2)	//向上搜索
						                      {
						                        EA=0;
							                    fm_fre=auto_seek(fm_fre,1);
									            dis_clear(1);
									            dis_volume();
									            dis_fm_fre(0);
												dis_mode(0);
									            EA=1;
						                      }
								             if(key_num==3)	 //向下搜索
						                      {
						                        EA=0;
							                    fm_fre=auto_seek(fm_fre,0);
									            dis_clear(1);
									            dis_volume();
									            dis_fm_fre(0);
												dis_mode(0);
									            EA=1;
						                      }
								  
				                       }
									}
									else	//否则为手动搜台
									{	
									    EA=0;
									    dis_wait();
										set_fm_fre_rx(fm_fre);
										dis_clear(1);
									    dis_volume();
									    dis_fm_fre(0);
										dis_mode(0);
									    EA=1;
										tf=1;
									    while(tf)
		                                {
									           key_num=scan_key();
											   if(key_num==1)		  //退回到上一级
								              { EA=0;
								                dis_clear(1);
								                dis_menu(0);
												select=0;
												seek_mode=0;
									            tf=0;												
								              }
											   if(key_num==2)
											   {
											     EA=0; 
												 	  		 
												 fm_fre+=10;
												 set_fm_fre_rx(fm_fre); 
												 if(fm_fre>=210)
												 {
												 fm_fre=0;
												 set_fm_fre_rx(fm_fre);
												 }  
												 dis_clear(1);
									             dis_volume();
									             dis_fm_fre(0);
												 dis_mode(0);
									             EA=1;
											   } 
											   if(key_num==3)
											   {
											     EA=0;
												 if((fm_fre%10)>=9)
												 {
												 fm_fre-=10;
												 set_fm_fre_rx(fm_fre);
												 }		
												 fm_fre+=1;				
												 set_fm_fre_rx(fm_fre); 						 	 
												 dis_clear(1);
									             dis_volume();
									             dis_fm_fre(0);
												 dis_mode(0);
									             EA=1;
											   } 
									    }
									}
								}
								if(key_num==2)
								{    
								     EA=0;
								     dis_clear(1); 
		                             seek_mode=0;
				                     dis_menu(2);
								}
								if(key_num==3)
								{    
								     EA=0;
								     dis_clear(1); 
		                             seek_mode=1;
				                     dis_menu(3);
								}
							 }	 
		             }	
				   if(select==1)//否则就是发射模式
					{		
							fm_fre=0x0000;
					        EA=0;
							tf=1;
					        dis_wait();	
							write_i2c(0x40,0x0001);		
							write_i2c(0x02,0xd281); 
							write_i2c(0x03,0x0018);	
							dis_clear(1);
							dis_power_tx(0);
							key_num=0;
							while(tf)
							{
							    key_num=scan_key();
								if(key_num==1)		  //退回到上一级
								{ 								               
									  dis_clear(1);
									  dis_power_tx(1);
							          dis_mode(1);
							          dis_fm_fre(1);
							          while(tf)
		                                   {
									           key_num=scan_key();
											   if(key_num==1)		  //退回到上一级
								               { 								               
												write_i2c(0x02,0x0000);
												select=0;
									            tf=0; 
												dis_clear(1);
	            				                dis_menu(0);							
								               }
											   if(key_num==2)
											   {  		 
												 fm_fre+=10;
												 set_fm_fre_tx(fm_fre); 
												  
											     if(fm_fre>=320)
												 {
												 fm_fre=0;
												 set_fm_fre_tx(fm_fre);
												 }  	
												 dis_clear(1);
									            // dis_volume(); 
												 dis_power_tx(1);									             
												 dis_mode(1);
												 dis_fm_fre(1);
									           
											   } 
											   if(key_num==3)
											   {
											       	
												  if((fm_fre%10)>=9)
												 {
												 fm_fre-=10;
												 set_fm_fre_tx(fm_fre);
												 }
												 fm_fre+=1;					
												 set_fm_fre_tx(fm_fre); 						 	 
												 dis_clear(1);
									             dis_power_tx(1);
												 dis_mode(1);
									              dis_fm_fre(1);
											    } 
				                            }
									}
									if(key_num==2)
									{
									   power_tx+=16;
									   set_power_tx(power_tx);
									   if(power_tx>=64|power_tx<=0)
									   {power_tx=0;}
									   dis_clear(1);
									   dis_power_tx(0);
									}
							 }
					}
		 }
		 if(key_num==2) 
		 { 
		         					
		        dis_clear(1); 
		        select=0;
				dis_menu(0);
				
			
		 }
		  if(key_num==3) 
		 {   
		         
				dis_clear(1); 
				select=1;
				dis_menu(1);
				 
		 }
		 key_num=0;	
		           
	 }
} 
	
