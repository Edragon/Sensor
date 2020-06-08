
	
#define read_add 0xa1
#define write_add 0xa0
////////////////////////////////////////////////在这里写地址
sbit scl=P1^1;
sbit sda=P1^2;
//////////////////////////////////////////////////
void delay_nop()
{;;}
/////////////////////////////////////////////////延时
void init()
{
   sda=1;
   delay_nop();
   scl=1;
   delay_nop();
}
void start()
{
   sda=1;
   delay_nop();
   scl=1;
   delay_nop();
   sda=0;
   delay_nop();
   
}
void stop()
{
  sda=0;
  delay_nop();
  scl=1;
  delay_nop();
  sda=1;
  delay_nop();
}
void respons()
{
  unsigned char i;
  sda=1;
  delay_nop();
  scl=1;
  delay_nop();
  while(sda==1&&i<250)
  {i++;}
  scl=0;
  delay_nop();

}
void write_byte(unsigned char dt)
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
unsigned char read_byte()
{
    unsigned char i,dt;
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
	return dt;
}
void write(unsigned char add,unsigned char dt)
{
      start();
	  write_byte(write_add);
	  respons();
	  write_byte(add);
	  respons();
	  write_byte(dt);
	  respons();
	  stop();
}
unsigned char read(unsigned char add)
{
      unsigned char dt;
      start();
	  write_byte(write_add);
	  respons();
	  write_byte(add);
	  respons();
	  ///////////////
	  start();
	  write_byte(read_add);
	  respons();
	  dt=read_byte();
	  stop();
	  return dt;
     
}
//////////////////////////////////////////////////模块函数
