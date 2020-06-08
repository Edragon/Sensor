#define uchar unsigned char
#define uint usigned int
////////////////////////////////////////////////////
sbit en=P3^2;
sbit rw=P3^1;
sbit rs=P3^0;
sbit d0=P3^3;
sbit d1=P3^4;
sbit d2=P3^5;
sbit d3=P1^7;
sbit d4=P1^6;
sbit d5=P1^5;
sbit d6=P1^4;
sbit d7=P1^3;
 
uchar code LevlChar[5][8] =
{{0x1F,0x11,0x0A,0x04,0x04,0x04,0x04,0x04},
{0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x1E},
{0x00,0x00,0x00,0x00,0x06,0x1E,0x1E,0x1E},     //显示型号强度的自定义字符
{0x00,0x00,0x06,0x1E,0x1E,0x1E,0x1E,0x1E},
{0x06,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E}};  

/////////////////////////////////////////////////

void write_com(uchar com)
{ 
  uchar temp;
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
  delay(5);
  en=1;
  delay(5);
  en=0;
}
void write_data(uchar date)
{ 
   uchar temp;
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
   delay(5);
   en=1;
   delay(5);
   en=0;
}
void start()
{  int i,j;
   uchar CGRamAddr = 0x40;
   en=0;
   rw=0;
   rs=0;
   write_com(0x38);
   write_com(0x0c);
   write_com(0x06);
   write_com(0x01); 
     for(j=0;j<5;j++)
	 {
  write_com(CGRamAddr + 8*j); //建立自定义字符 (显示信号强度的)
     for(i=0;i<8;i++) 
  {
  write_data(LevlChar[j][i]);
   delay(510);
  }
}
}
//////////////////////////////////////////////////