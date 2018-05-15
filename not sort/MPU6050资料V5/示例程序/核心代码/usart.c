#define  UDRE     5
#define  FE       4
#define  PE       2
#define  DOR      3

unsigned char Re_buf[11],counter=0;
unsigned char sign;

void USART_INIT()
{
   DDRD|=0x02;    
   DDRD&=0xfe;          //IO端口设置
   PORTD|=0x01;
   
   UCSRA=0x00;
   UCSRB=0x98;          //使能接收、接受中断，发送使能
   UCSRC=0x86;          //异步、无奇偶校验、8字符、1停止位
   UBRRH=0;
   UBRRL=5;            //系统时钟11.0592MHz,波特率为115200bps
}

void USART_Transmit(char dat)
{
   while( !(UCSRA & (1 << UDRE)) );  //只有数据寄存器为空时才能发送数据
    UDR = dat;
}


interrupt [USART_RXC] void usart_rx_isr(void)        //USART串行接收中断
 { 
      Re_buf[counter]=UDR;
      if(counter==0&&Re_buf[0]!=0x55) return;      //第0号数据不是帧头
              
      counter++; 
      
      if(counter==11)             //接收到11个数据
      {    
         counter=0;               //重新赋值，准备下一帧数据的接收 
         sign=1;
      }
    
 }   
