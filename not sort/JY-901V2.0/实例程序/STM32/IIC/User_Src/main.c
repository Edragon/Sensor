#include "stm32f10x.h"
#include "UARTs.h"
#include "IOI2C.h"
#include "delay.h"
#include "AHRSREG.h"


void SysTick_init(u8 SYSCLK,u16 nms);

void ShortToChar(short sData,unsigned char cData[])
{
	cData[0]=sData&0xff;
	cData[1]=sData>>8;
}
short CharToShort(unsigned char cData[])
{
	return ((short)cData[1]<<8)|cData[0];
}

void SetPWMOut()
{
  unsigned char chrTemp[2];
	//����D0ΪPWM���
	ShortToChar(DIO_MODE_DOPWM,chrTemp);//׼������
	IICwriteBytes(0x50, D0MODE, 2,&chrTemp[0]);		//IIC����

	//����D0ΪPWM�������
	ShortToChar(20000,chrTemp);//׼������
	IICwriteBytes(0x50, D0PWMT, 2,&chrTemp[0]);		//IIC����
	//����D0ΪPWM������Ǯ���
	ShortToChar(1500,chrTemp);//׼������
	IICwriteBytes(0x50, D0PWMH, 2,&chrTemp[0]);		//IIC����
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		int main(void)
*��������:		������
*******************************************************************************/
int main(void)
{
	int16_t Math_hz=0;
	short i=0;
  unsigned char chrTemp[30];
	unsigned char str[100];
	float a[3],w[3],h[3],Angle[3];
     
  SystemInit();	/* ����ϵͳʱ��Ϊ72M ʹ���ⲿ8M����+PLL*/ 
	SysTick_init(72,10);		//��ʱ��ʼ��
	Initial_UART1(9600);
	IIC_Init();	 //��ʼ��I2C�ӿ�
	delay_ms(1000);//�ȵ�JY-91��ʼ�����
	
	SetPWMOut();//��ʾ����D0�˿�ΪPWM�����
	while(1)
	{			
		IICreadBytes(0x50, AX, 24,&chrTemp[0]);
		a[0] = (float)CharToShort(&chrTemp[0])/32768*16;
		a[1] = (float)CharToShort(&chrTemp[2])/32768*16;
		a[2] = (float)CharToShort(&chrTemp[4])/32768*16;
		w[0] = (float)CharToShort(&chrTemp[6])/32768*2000;
		w[1] = (float)CharToShort(&chrTemp[8])/32768*2000;
		w[2] = (float)CharToShort(&chrTemp[10])/32768*2000;
		h[0] = CharToShort(&chrTemp[12]);
		h[1] = CharToShort(&chrTemp[14]);
		h[2] = CharToShort(&chrTemp[16]);
		Angle[0] = (float)CharToShort(&chrTemp[18])/32768*180;
		Angle[1] = (float)CharToShort(&chrTemp[20])/32768*180;
		Angle[2] = (float)CharToShort(&chrTemp[22])/32768*180;
		
		delay_ms(500);
		sprintf(str,"a:%.3f %.3f %.3f w:%.3f %.3f %.3f  h:%.0f %.0f %.0f  Angle:%.3f %.3f %.3f \r\n",a[0],a[1],a[2],w[0],w[1],w[2],h[0],h[1],h[2],Angle[0],Angle[1],Angle[2]);
		UART1_Put_String(str);		
	}//��ѭ�� end 

}
