#include "Main.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "UART1.h"
#include "delay.h"
#include "DIO.h"
#include "DMREG.h"

void UART1_Put_StringL(unsigned char *Str,unsigned char len)
{
	unsigned char i = 0;
	for (i=0;i<len;i++) UART1_Put_Char(*(Str + i));
}

void UART_DM_ReportData(unsigned char data[])
{		
	UART1_Put_StringL(data,10);     //��ӡ���ȸ�������ӡ�����鳤�����ģ���������5���ٶ�״̬��10�������10��        
}

//**********�����鼰���ֶ�����ٶȽǶ����ö��ڡ�DMREG.h��ͷ�ļ�����******//
//**********ֱ�Ӱ��������Ʒ���UART_DM_ReportData()��������Ϳ����ˣ�����UART_DM_ReportData(DM0_Speed1_Position_90)********//

int main(void)
{  		

	SysTick_init(72,10);
	Initial_UART1(9600);            //���ڲ�����9600
	delay_ms(1000);
	
	while(1)
	{	
		UART_DM_ReportData(DM0_Speed1_Position_90);     //����ͨ��0���ٶ�=1��λ��=90�� 
		delay_ms(2000);                                 //��ʱ2S    
		UART_DM_ReportData(DM0_Speed3_Position_0);      //����ͨ��0���ٶ�=3��λ��=0��
		delay_ms(2000);
		UART_DM_ReportData(DM_Action0);            //���ö�����0
		delay_ms(2000);
//		UART_DM_ReportData(DM_Action1);          //���ö�����1
//		UART_DM_ReportData(DM_Action2);          //���ö�����2
//		UART_DM_ReportData(DM_Action3);          //���ö�����3
//		UART_DM_ReportData(DM_Action4);          //���ö�����4
//		UART_DM_ReportData(DM_Action5);          //���ö�����5
//		UART_DM_ReportData(DM_Action6);          //���ö�����6
//		UART_DM_ReportData(DM_Action7);          //���ö�����7        
//		UART_DM_ReportData(DM_Action8);          //���ö�����8         
//		UART_DM_ReportData(DM_Action9);          //���ö�����9
//		UART_DM_ReportData(DM_Action10);         //���ö�����10
//		UART_DM_ReportData(DM_Action11);         //���ö�����11
//		UART_DM_ReportData(DM_Action12);         //���ö�����12
//		UART_DM_ReportData(DM_Action13);         //���ö�����13
//		UART_DM_ReportData(DM_Action14);         //���ö�����14
//		UART_DM_ReportData(DM_Action15);         //���ö�����15
		
	}			
}










//		switch (i)
//		{
//			case 0:
//				if ( j == 1 )
//				UART_DM_ReportData(DM0_Speed10_Position_0);   //ͨ��0���ٶ�=10��0��
//				else
//				UART_DM_ReportData(DM0_Speed10_Position_90);	//ͨ��0���ٶ�=10��90��
//			break;                                                
//			case 1:
//        if ( j == 1 )				
//				UART_DM_ReportData(DM1_Speed10_Position_0);   //ͨ��1���ٶ�=10��0��
//				else
//				UART_DM_ReportData(DM1_Speed10_Position_90);
//				break;                                              
//			case 2:
//        if ( j == 1 )				
//				UART_DM_ReportData(DM2_Speed10_Position_0);   //ͨ��2���ٶ�=10��0��
//				else
//				UART_DM_ReportData(DM2_Speed10_Position_90);				
//				break;                                              
//			case 3: 
//        if ( j == 1 )				
//				UART_DM_ReportData(DM3_Speed10_Position_0);   //ͨ��3���ٶ�=10��0��
//				else
//				UART_DM_ReportData(DM3_Speed10_Position_90);break;
//			case 4:
//				if ( j == 1 )
//				UART_DM_ReportData(DM4_Speed10_Position_0);   //ͨ��4���ٶ�=10��0��
//				else
//				UART_DM_ReportData(DM4_Speed10_Position_90);break;                                               
//			case 5:        
//        if ( j == 1 )				
//				UART_DM_ReportData(DM5_Speed10_Position_0);   //ͨ��5���ٶ�=10��0��
//				else
//				UART_DM_ReportData(DM5_Speed10_Position_90);break;                                               
//			case 6:
//        if ( j == 1 )				
//				UART_DM_ReportData(DM6_Speed10_Position_0);   //ͨ��6���ٶ�=10��0��
//				else
//				UART_DM_ReportData(DM6_Speed10_Position_90);break;                                               
//			case 7:
//        if ( j == 1 )				
//				UART_DM_ReportData(DM7_Speed10_Position_0);   //ͨ��7���ٶ�=10��0��
//				else
//				UART_DM_ReportData(DM7_Speed10_Position_90);break;
//			case 8:
//				if ( j == 1 )
//				UART_DM_ReportData(DM8_Speed10_Position_0);   //ͨ��8���ٶ�=10��0��
//				else
//				UART_DM_ReportData(DM8_Speed10_Position_90);break;                                              
//			case 9:
//        if ( j == 1 )				
//				UART_DM_ReportData(DM9_Speed10_Position_0);   //ͨ��9���ٶ�=10��0��
//				else
//				UART_DM_ReportData(DM9_Speed10_Position_90);break;                                              
//			case 10:      
//        if ( j == 1 )				
//				UART_DM_ReportData(DM10_Speed10_Position_0);  //ͨ��10���ٶ�=10��0��;
//				else
//				UART_DM_ReportData(DM10_Speed10_Position_90);break;                                                
//			case 11:       
//        if ( j == 1 )				
//				UART_DM_ReportData(DM11_Speed10_Position_0);   //ͨ��11���ٶ�=10��0��;
//				else
//				UART_DM_ReportData(DM11_Speed10_Position_90);break;                                                
//			case 12:       
//        if ( j == 1 )				
//				UART_DM_ReportData(DM12_Speed10_Position_0);   //ͨ��12���ٶ�=10��0��;
//				else
//				UART_DM_ReportData(DM12_Speed10_Position_90);break;                                                
//			case 13:       
//        if ( j == 1 )				
//				UART_DM_ReportData(DM13_Speed10_Position_0);   //ͨ��13���ٶ�=10��0��;
//				else
//				UART_DM_ReportData(DM13_Speed10_Position_90);break;                                                
//			case 14:       
//        if ( j == 1 )				
//				UART_DM_ReportData(DM14_Speed10_Position_0);   //ͨ��14���ٶ�=10��0��;
//				else
//				UART_DM_ReportData(DM14_Speed10_Position_90);break;                                                
//			case 15:       
//        if ( j == 1 )				
//				UART_DM_ReportData(DM15_Speed10_Position_0);   //ͨ��15���ٶ�=10��0��;
//				else
//			  UART_DM_ReportData(DM15_Speed10_Position_90);
//				i = -1;
//			  j = -j;
//				break;
//		}			
//		i++;
