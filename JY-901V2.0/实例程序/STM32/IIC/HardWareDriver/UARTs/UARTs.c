/* UARTs.C file
STM32-SDK �������������
��д�ߣ�lisn3188
��ַ��www.chiplab7.com
����E-mail��lisn3188@163.com
���뻷����MDK-Lite  Version: 4.23
����ʱ��: 2012-02-28
���ԣ� ���������ڵ���ʵ���ҵ�STM32-SDK����ɲ���
���ܣ�ʵ��	STM32-SDK �������ϵ� UART1-RS232 �ӿڲ���

---------Ӳ���ϵ���������:----------
RS232�ӿڣ�
RS232TXD  -->  PA9  (UART1-TXD)
RS232RXD  -->  PA10 (UART1-RXD)
------------------------------------
 */

#include "UARTs.h"
#include "IOI2C.h"

u8 TxBuffer[258];
u8 TxCounter=0;
u8 count=0; 

void NVIC_Configuration(void)
{
        NVIC_InitTypeDef NVIC_InitStructure; 
          /* Enable the USART1 Interrupt */
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void Initial_UART1(u32 baudrate)
*��������:		��ʼ��STM32-SDK�������ϵ�RS232�ӿ�
���������
		u32 baudrate   ����RS232���ڵĲ�����
���������û��	
*******************************************************************************/
void Initial_UART1(u32 baudrate)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* ʹ�� UART1 ģ���ʱ��  ʹ�� UART1��Ӧ�����Ŷ˿�PA��ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

  	 /* ����UART1 �ķ�������
	 ����PA9 Ϊ�������  ˢ��Ƶ��50MHz
	  */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);    
  	/* 
	  ����UART1 �Ľ�������
	  ����PA10Ϊ�������� 
	 */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* 
	  UART1������:
	  1.������Ϊ���ó���ָ�������� baudrate;
	  2. 8λ����			  USART_WordLength_8b;
	  3.һ��ֹͣλ			  USART_StopBits_1;
	  4. ����żЧ��			  USART_Parity_No ;
	  5.��ʹ��Ӳ��������	  USART_HardwareFlowControl_None;
	  6.ʹ�ܷ��ͺͽ��չ���	  USART_Mode_Rx | USART_Mode_Tx;
	 */
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//Ӧ�����õ�UART1
	USART_Init(USART1, &USART_InitStructure); 
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);  
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);        
    USART_ClearFlag(USART1,USART_FLAG_TC);
	//����UART1
  	USART_Cmd(USART1, ENABLE);
	NVIC_Configuration();
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void UART1_Put_Char(unsigned char DataToSend)
*��������:		RS232����һ���ֽ�
���������
		unsigned char DataToSend   Ҫ���͵��ֽ�����
���������û��	
*******************************************************************************/
void UART1_Put_Char(unsigned char DataToSend)
{
	//��Ҫ���͵��ֽ�д��UART1�ķ��ͻ�����
	//USART_SendData(USART1, (unsigned char) DataToSend);
	//�ȴ��������
  	//while (!(USART1->SR & USART_FLAG_TXE));

	TxBuffer[count++] = DataToSend;  
    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  
}


void UART1_Put_String(unsigned char *Str)
{
	//�ж�Strָ��������Ƿ���Ч.
	while(*Str){
	//�Ƿ��ǻس��ַ� �����,������Ӧ�Ļس� 0x0d 0x0a
	if(*Str=='\r')UART1_Put_Char(0x0d);
		else if(*Str=='\n')UART1_Put_Char(0x0a);
			else UART1_Put_Char(*Str);
	Str++;
	}
}


unsigned char ucRxBuffer[250];
//------------------------------------------------------

	u8 chrTemp[250];
void CopeSerialData(unsigned char ucData)
{
	static unsigned char ucRxCnt = 0;
	unsigned char i=0;
	ucRxBuffer[ucRxCnt++]=ucData;
	if (ucRxCnt==5)
	{
		if ((ucRxBuffer[0]==0xff)&(ucRxBuffer[1]==0xaa)) 
		{
			IICreadBytes(ucRxBuffer[2], ucRxBuffer[3], ucRxBuffer[4],&chrTemp[0]);		
			for(i=0;i<	ucRxBuffer[4];i++)
			{
				UART1_Put_Char(chrTemp[i]);
			}			
			ucRxCnt=0;	
		}
		if ((ucRxBuffer[0]==0xff)&(ucRxBuffer[1]==0xa0)) 
		{
			IICwriteBytes(0x50, ucRxBuffer[2], 2,&ucRxBuffer[3]);		
					
			ucRxCnt=0;	
		}
	}
	else if(ucRxCnt>250)
	{
			ucRxCnt=0;		
	}
}

void USART1_IRQHandler(void)
{
  
if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  {   
    USART_SendData(USART1, TxBuffer[TxCounter++]); 
    if(TxCounter == count) 
		{
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);// ȫ���������
		}
    USART_ClearITPendingBit(USART1, USART_IT_TXE); 
  }
	else if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
		CopeSerialData((unsigned char)USART1->DR);//��������
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  }
	USART_ClearITPendingBit(USART1,USART_IT_ORE);

}




//uart reicer flag
#define b_uart_head  0x80
#define b_rx_over    0x40

// USART Receiver buffer
#define RX_BUFFER_SIZE 100

u8 U2TxBuffer[258];
u8 U2TxCounter=0;
u8 U2count=0; 

void U2NVIC_Configuration(void)
{
        NVIC_InitTypeDef NVIC_InitStructure; 
          /* Enable the USART1 Interrupt */
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 8;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void Initial_UART2(u32 baudrate)
*��������:		��ʼ��STM32-SDK�������ϵ�RS232�ӿ�
���������
		u32 baudrate   ����RS232���ڵĲ�����
���������û��	
*******************************************************************************/
void Initial_UART2(u32 baudrate)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* ʹ�� UART2 ģ���ʱ��  ʹ�� UART2��Ӧ�����Ŷ˿�PA��ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB2Periph_GPIOA, ENABLE);

  	 /* ����UART2 �ķ�������
	 ����PA9 Ϊ�������  ˢ��Ƶ��50MHz
	  */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);    
  	/* 
	  ����UART2 �Ľ�������
	  ����PA10Ϊ�������� 
	 */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* 
	  UART2������:
	  1.������Ϊ���ó���ָ�������� baudrate;
	  2. 8λ����			  USART_WordLength_8b;
	  3.һ��ֹͣλ			  USART_StopBits_1;
	  4. ����żЧ��			  USART_Parity_No ;
	  5.��ʹ��Ӳ��������	  USART_HardwareFlowControl_None;
	  6.ʹ�ܷ��ͺͽ��չ���	  USART_Mode_Rx | USART_Mode_Tx;
	 */
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//Ӧ�����õ�UART2
	USART_Init(USART2, &USART_InitStructure); 
	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);        
    USART_ClearFlag(USART2,USART_FLAG_TC);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//ʹ�ܽ����ж�
	//����UART2
  	USART_Cmd(USART2, ENABLE);
	U2NVIC_Configuration();
}

volatile unsigned char RC_Flag;
//------------------------------------------------------
void USART2_IRQHandler(void)
{
  unsigned char data;
  if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
  {   
    /* Write one byte to the transmit data register */
    USART_SendData(USART2, U2TxBuffer[U2TxCounter++]);                    

    /* Clear the USART1 transmit interrupt */
    USART_ClearITPendingBit(USART2, USART_IT_TXE); 

    if(U2TxCounter == U2count)
    {
      /* Disable the USART1 Transmit interrupt */
      USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
    }    
  }else if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
		data=USART_ReceiveData(USART2);
 
  /* Clear the USART1 transmit interrupt */
  USART_ClearITPendingBit(USART2, USART_IT_RXNE);
  }
}
//------------------End of File----------------------------
