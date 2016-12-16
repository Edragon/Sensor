/*
��д�ߣ�Zhaowen
��ַ��http://RobotControl.taobao.com
����E-mail��zhaowenwin@139.com
���뻷����MDK-Lite  Version: 4.73
����ʱ��: 2015-1-1
���ԣ� STM32F103C8T6
���ܣ�����GY-901ģ�鷢�͵����ݣ����ô��ڴ�ӡ���������������ݣ�������9600
		�STM32F103����1��Rx��JY-901��Tx,���ڽ���JY-901ģ�������
			STM32F103����1��Rx�ӵ��Ե�USB-TTLģ���Rx����������Է��ͽ����Ժ�����ݡ����Զ�ͨ�����ڵ������ֲ鿴��������9600��ASCII�뷽ʽ��ʾ��
			���ݽ���ʹ�ýṹ��ķ������н������ṹ����JY901.h�ļ��ж���

---------Ӳ���ϵ���������:----------
TTL�ӿڣ�
PC(USB-TTL)			STM32F103						JY-901
RX			<-->	PA9(UART1-TXD)
							PA10(UART1-RXD)	<-->  	TX
------------------------------------
*/

#include "stm32f10x.h"
#include "UARTs.h"
#include "IOI2C.h"
#include "delay.h"
#include "JY901.h"
#include "string.h"

struct STime		stcTime;
struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SAngle 	stcAngle;
struct SMag 		stcMag;
struct SDStatus stcDStatus;
struct SPress 	stcPress;
struct SLonLat 	stcLonLat;
struct SGPSV 		stcGPSV;

//CopeSerialDataΪ�����жϵ��ú���������ÿ�յ�һ�����ݣ�����һ�����������
void CopeSerialData(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	
	
	ucRxBuffer[ucRxCnt++]=ucData;
	if (ucRxBuffer[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//���ݲ���11�����򷵻�
	else
	{
		switch(ucRxBuffer[1])
		{
			case 0x50:	memcpy(&stcTime,&ucRxBuffer[2],8);break;//memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݹ�ͬ�����棬�Ӷ�ʵ�����ݵĽ�����
			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;
			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
			case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8);break;
			case 0x54:	memcpy(&stcMag,&ucRxBuffer[2],8);break;
			case 0x55:	memcpy(&stcDStatus,&ucRxBuffer[2],8);break;
			case 0x56:	memcpy(&stcPress,&ucRxBuffer[2],8);break;
			case 0x57:	memcpy(&stcLonLat,&ucRxBuffer[2],8);break;
			case 0x58:	memcpy(&stcGPSV,&ucRxBuffer[2],8);break;
		}
		ucRxCnt=0;
	}
}

int main(void)
{
	char str[100];
	
  SystemInit();	/* ����ϵͳʱ��Ϊ72M ʹ���ⲿ8M����+PLL*/ 
	SysTick_init(72,10);		//��ʱ��ʼ��
	Initial_UART1(9600);
	delay_ms(1000);//�ȵ�JY-91��ʼ����ɣ
	while(1)
	{			
			delay_ms(500);
		sprintf(str,"Time:20%d-%d-%d %d:%d:%.3f\r\n",stcTime.ucYear,stcTime.ucMonth,stcTime.ucDay,stcTime.ucHour,stcTime.ucMinute,(float)stcTime.ucSecond+(float)stcTime.usMiliSecond/1000);
			UART1_Put_String(str);		
			delay_ms(10);//�ȴ��������
		sprintf(str,"Acc:%.3f %.3f %.3f\r\n",(float)stcAcc.a[0]/32768*16,(float)stcAcc.a[1]/32768*16,(float)stcAcc.a[2]/32768*16);
			UART1_Put_String(str);
			delay_ms(10);//�ȴ��������
		sprintf(str,"Gyro:%.3f %.3f %.3f\r\n",(float)stcGyro.w[0]/32768*2000,(float)stcGyro.w[1]/32768*2000,(float)stcGyro.w[2]/32768*2000);
			UART1_Put_String(str);
			delay_ms(10);//�ȴ��������
		sprintf(str,"Angle:%.3f %.3f %.3f\r\n",(float)stcAngle.Angle[0]/32768*180,(float)stcAngle.Angle[1]/32768*180,(float)stcAngle.Angle[2]/32768*180);
			UART1_Put_String(str);
			delay_ms(10);//�ȴ��������
		sprintf(str,"Mag:%d %d %d\r\n",stcMag.h[0],stcMag.h[1],stcMag.h[2]);
			UART1_Put_String(str);		
			delay_ms(10);//�ȴ��������
		sprintf(str,"Pressure:%ld Height%.2f\r\n",stcPress.lPressure,(float)stcPress.lAltitude/100);
			UART1_Put_String(str);
			delay_ms(10);//�ȴ��������
		sprintf(str,"DStatus:%d %d %d %d\r\n",stcDStatus.sDStatus[0],stcDStatus.sDStatus[1],stcDStatus.sDStatus[2],stcDStatus.sDStatus[3]);
			UART1_Put_String(str);
			delay_ms(10);//�ȴ��������
		sprintf(str,"Longitude:%ldDeg%.5fm Lattitude:%ldDeg%.5fm\r\n",stcLonLat.lLon/10000000,(double)(stcLonLat.lLon % 10000000)/1e5,stcLonLat.lLat/10000000,(double)(stcLonLat.lLat % 10000000)/1e5);
			UART1_Put_String(str);
			delay_ms(10);//�ȴ��������
		sprintf(str,"GPSHeight:%.1fm GPSYaw:%.1fDeg GPSV:%.3fkm/h\r\n\r\n",(float)stcGPSV.sGPSHeight/10,(float)stcGPSV.sGPSYaw/10,(float)stcGPSV.lGPSVelocity/1000);
			UART1_Put_String(str);
			delay_ms(10);//�ȴ��������
	}//��ѭ�� end 

}
