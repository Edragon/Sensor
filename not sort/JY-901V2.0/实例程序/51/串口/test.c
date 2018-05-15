//51��Ƭ����ȡ����MPU6050ģ���ʵ������
//            BY:�����ǿ�
//   http://RobotControl.taobao.com
//����ƽ̨��
//		51��Ƭ��������YL-39,оƬSTC89C52
//ע��:
//    1.51��Ƭ�������ش���ֻ��һ����ͬʱ����Ҫ��MPU6050����������ص�ʱ����Ҫ����MPU6050��TX�ߣ��ȳ������سɹ��Ժ��ٲ��ϡ�
//���߷�����
//     51��Ƭ��		  JY901ģ��
//		+5V		----	VCC
//		TX		(����)	RX
//		RX		----	TX
//		GND		----	GND
///////////////////////////////////////////////////////
/*Ԥ��������*/
#include <reg52.h> 		  //������Ƭ���Ĵ�����ͷ�ļ�
#include <stdio.h>
#include "JY901.h"

void delay_ms(unsigned short i)
{
unsigned short k;
	while(i--)
	for (k=0;k<100;k++); 
}

void main(void)
{
	unsigned char i=0;
	TMOD=0x20;		   //�ö�ʱ�����ô��ڲ�����	   9600 
	TH1=0xfd;
	TL1=0xfd;
	TR1=1;
	TI=1;
	REN=1;          //���ڳ�ʼ��
	SM0=0;
	SM1=1;
	EA=1;           //�������ж�
	ES=1;
	printf("STC89S52 Read JY901 module demo\r\n");
	printf("-------------BY:JYZK-------------\r\n");
	printf("---http://RobotControl.taobao.com---\r\n");
	while(1)
	{
		delay_ms(10);
		printf("Time:20%d-%d-%d %d:%d:%.3f\r\n",(short)stcTime.ucYear,(short)stcTime.ucMonth,
				(short)stcTime.ucDay,(short)stcTime.ucHour,(short)stcTime.ucMinute,(float)stcTime.ucSecond+(float)stcTime.usMiliSecond/1000);

		printf("Acc:%.3f %.3f %.3f\r\n",(float)stcAcc.a[0]/32768*16,(float)stcAcc.a[1]/32768*16,(float)stcAcc.a[2]/32768*16);

		printf("Gyro:%.3f %.3f %.3f\r\n",(float)stcGyro.w[0]/32768*2000,(float)stcGyro.w[1]/32768*2000,(float)stcGyro.w[2]/32768*2000);

		printf("Angle:%.3f %.3f %.3f\r\n",(float)stcAngle.Angle[0]/32768*180,(float)stcAngle.Angle[1]/32768*180,(float)stcAngle.Angle[2]/32768*180);

		printf("Mag:%d %d %d\r\n",stcMag.h[0],stcMag.h[1],stcMag.h[2]);

		printf("Pressure:%lx Height%.2f\r\n",stcPress.lPressure,(float)stcPress.lAltitude/100);

		printf("DStatus:%d %d %d %d\r\n",stcDStatus.sDStatus[0],stcDStatus.sDStatus[1],stcDStatus.sDStatus[2],stcDStatus.sDStatus[3]);

		printf("Longitude:%ldDeg%.5fm Lattitude:%ldDeg%.5fm\r\n",stcLonLat.lLon/10000000,(double)(stcLonLat.lLon % 10000000)/1e5,stcLonLat.lLat/10000000,(double)(stcLonLat.lLat % 10000000)/1e5);

		printf("GPSHeight:%.1fm GPSYaw:%.1fDeg GPSV:%.3fkm/h\r\n\r\n",(float)stcGPSV.sGPSHeight/10,(float)stcGPSV.sGPSYaw/10,(float)stcGPSV.lGPSVelocity/1000);
			
	}	
}
	  
void ser() interrupt 4
{
	if (RI)
	{	  
		RI=0;
		 CopeSerialData(SBUF);	    
	  }
	  
    
}