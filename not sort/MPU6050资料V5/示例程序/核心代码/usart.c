#define  UDRE     5
#define  FE       4
#define  PE       2
#define  DOR      3

unsigned char Re_buf[11],counter=0;
unsigned char sign;

void USART_INIT()
{
   DDRD|=0x02;    
   DDRD&=0xfe;          //IO�˿�����
   PORTD|=0x01;
   
   UCSRA=0x00;
   UCSRB=0x98;          //ʹ�ܽ��ա������жϣ�����ʹ��
   UCSRC=0x86;          //�첽������żУ�顢8�ַ���1ֹͣλ
   UBRRH=0;
   UBRRL=5;            //ϵͳʱ��11.0592MHz,������Ϊ115200bps
}

void USART_Transmit(char dat)
{
   while( !(UCSRA & (1 << UDRE)) );  //ֻ�����ݼĴ���Ϊ��ʱ���ܷ�������
    UDR = dat;
}


interrupt [USART_RXC] void usart_rx_isr(void)        //USART���н����ж�
 { 
      Re_buf[counter]=UDR;
      if(counter==0&&Re_buf[0]!=0x55) return;      //��0�����ݲ���֡ͷ
              
      counter++; 
      
      if(counter==11)             //���յ�11������
      {    
         counter=0;               //���¸�ֵ��׼����һ֡���ݵĽ��� 
         sign=1;
      }
    
 }   
