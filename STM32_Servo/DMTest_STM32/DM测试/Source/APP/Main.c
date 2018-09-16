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
	UART1_Put_StringL(data,10);     //打印长度根据所打印的数组长度来改，动作组是5，速度状态是10。最好填10。        
}

//**********动作组及各种舵机的速度角度配置都在“DMREG.h”头文件里面******//
//**********直接把数组名称放在UART_DM_ReportData()函数里面就可以了，例如UART_DM_ReportData(DM0_Speed1_Position_90)********//

int main(void)
{  		

	SysTick_init(72,10);
	Initial_UART1(9600);            //串口波特率9600
	delay_ms(1000);
	
	while(1)
	{	
		UART_DM_ReportData(DM0_Speed1_Position_90);     //设置通道0，速度=1，位置=90度 
		delay_ms(2000);                                 //延时2S    
		UART_DM_ReportData(DM0_Speed3_Position_0);      //设置通道0，速度=3，位置=0度
		delay_ms(2000);
		UART_DM_ReportData(DM_Action0);            //设置动作组0
		delay_ms(2000);
//		UART_DM_ReportData(DM_Action1);          //设置动作组1
//		UART_DM_ReportData(DM_Action2);          //设置动作组2
//		UART_DM_ReportData(DM_Action3);          //设置动作组3
//		UART_DM_ReportData(DM_Action4);          //设置动作组4
//		UART_DM_ReportData(DM_Action5);          //设置动作组5
//		UART_DM_ReportData(DM_Action6);          //设置动作组6
//		UART_DM_ReportData(DM_Action7);          //设置动作组7        
//		UART_DM_ReportData(DM_Action8);          //设置动作组8         
//		UART_DM_ReportData(DM_Action9);          //设置动作组9
//		UART_DM_ReportData(DM_Action10);         //设置动作组10
//		UART_DM_ReportData(DM_Action11);         //设置动作组11
//		UART_DM_ReportData(DM_Action12);         //设置动作组12
//		UART_DM_ReportData(DM_Action13);         //设置动作组13
//		UART_DM_ReportData(DM_Action14);         //设置动作组14
//		UART_DM_ReportData(DM_Action15);         //设置动作组15
		
	}			
}










//		switch (i)
//		{
//			case 0:
//				if ( j == 1 )
//				UART_DM_ReportData(DM0_Speed10_Position_0);   //通道0，速度=10，0度
//				else
//				UART_DM_ReportData(DM0_Speed10_Position_90);	//通道0，速度=10，90度
//			break;                                                
//			case 1:
//        if ( j == 1 )				
//				UART_DM_ReportData(DM1_Speed10_Position_0);   //通道1，速度=10，0度
//				else
//				UART_DM_ReportData(DM1_Speed10_Position_90);
//				break;                                              
//			case 2:
//        if ( j == 1 )				
//				UART_DM_ReportData(DM2_Speed10_Position_0);   //通道2，速度=10，0度
//				else
//				UART_DM_ReportData(DM2_Speed10_Position_90);				
//				break;                                              
//			case 3: 
//        if ( j == 1 )				
//				UART_DM_ReportData(DM3_Speed10_Position_0);   //通道3，速度=10，0度
//				else
//				UART_DM_ReportData(DM3_Speed10_Position_90);break;
//			case 4:
//				if ( j == 1 )
//				UART_DM_ReportData(DM4_Speed10_Position_0);   //通道4，速度=10，0度
//				else
//				UART_DM_ReportData(DM4_Speed10_Position_90);break;                                               
//			case 5:        
//        if ( j == 1 )				
//				UART_DM_ReportData(DM5_Speed10_Position_0);   //通道5，速度=10，0度
//				else
//				UART_DM_ReportData(DM5_Speed10_Position_90);break;                                               
//			case 6:
//        if ( j == 1 )				
//				UART_DM_ReportData(DM6_Speed10_Position_0);   //通道6，速度=10，0度
//				else
//				UART_DM_ReportData(DM6_Speed10_Position_90);break;                                               
//			case 7:
//        if ( j == 1 )				
//				UART_DM_ReportData(DM7_Speed10_Position_0);   //通道7，速度=10，0度
//				else
//				UART_DM_ReportData(DM7_Speed10_Position_90);break;
//			case 8:
//				if ( j == 1 )
//				UART_DM_ReportData(DM8_Speed10_Position_0);   //通道8，速度=10，0度
//				else
//				UART_DM_ReportData(DM8_Speed10_Position_90);break;                                              
//			case 9:
//        if ( j == 1 )				
//				UART_DM_ReportData(DM9_Speed10_Position_0);   //通道9，速度=10，0度
//				else
//				UART_DM_ReportData(DM9_Speed10_Position_90);break;                                              
//			case 10:      
//        if ( j == 1 )				
//				UART_DM_ReportData(DM10_Speed10_Position_0);  //通道10，速度=10，0度;
//				else
//				UART_DM_ReportData(DM10_Speed10_Position_90);break;                                                
//			case 11:       
//        if ( j == 1 )				
//				UART_DM_ReportData(DM11_Speed10_Position_0);   //通道11，速度=10，0度;
//				else
//				UART_DM_ReportData(DM11_Speed10_Position_90);break;                                                
//			case 12:       
//        if ( j == 1 )				
//				UART_DM_ReportData(DM12_Speed10_Position_0);   //通道12，速度=10，0度;
//				else
//				UART_DM_ReportData(DM12_Speed10_Position_90);break;                                                
//			case 13:       
//        if ( j == 1 )				
//				UART_DM_ReportData(DM13_Speed10_Position_0);   //通道13，速度=10，0度;
//				else
//				UART_DM_ReportData(DM13_Speed10_Position_90);break;                                                
//			case 14:       
//        if ( j == 1 )				
//				UART_DM_ReportData(DM14_Speed10_Position_0);   //通道14，速度=10，0度;
//				else
//				UART_DM_ReportData(DM14_Speed10_Position_90);break;                                                
//			case 15:       
//        if ( j == 1 )				
//				UART_DM_ReportData(DM15_Speed10_Position_0);   //通道15，速度=10，0度;
//				else
//			  UART_DM_ReportData(DM15_Speed10_Position_90);
//				i = -1;
//			  j = -j;
//				break;
//		}			
//		i++;
