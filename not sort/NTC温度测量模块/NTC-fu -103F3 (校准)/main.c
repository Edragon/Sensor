#include"stm8s.h"
#include"main.h"

main()
{
	uchar count;
	int TempVol_X10,ADC_vol;
	
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);//����ʱ�ӣ�ʹ���ڲ�16M����ʱ�ӣ�����Ƶ
	
	SEG_GPIO_Init();
	MY_ADC1_Init();
//	enableInterrupts();
	Display_Open();//������ʾ888
	
	while (1)
	{
		ADC_vol=ADC1_GetValue_Average();		//��ȡADCת�����
//		ADC_vol=850;
		TempVol_X10=GetTemperatureValue_X10(ADC_vol);//
		
		for(count=0;count<50;count++)//����ˢ�¹��죬������ʱ
		{
			Display_Temperature(TempVol_X10);//��ʾ�¶�ֵ
		}
		count=0;//���¼���
	}
}

void delay_ms(uint nms)//ms��ʱ����
{												
	uint x,y;						//��16M��ʱ���£���ʱ1ms
	for(x=nms;x>0;x--)
		for(y=1500;y>0;y--);
}

int GetTemperatureValue_X10(uint advol)//��ADֵת������Ӧ�¶�ֵ ��10��
{																			//10���������ݴ���	
	int T_vol;
	uchar remainder;
	
	if((advol>=AD_MIN)&&(advol<=AD_MAX))//�ж��Ƿ񳬳���ⷶΧ
	{
		if(advol>778&&advol<=858)       //-10~0�����ڷ����ԣ���11�δ���ÿ�ο�10��, 
			T_vol=(-100)+8+(int)((858-advol)*1.25);//��10����
		else if(advol>678&&advol<=778) //0~10
			T_vol=0+8+778-advol;
		else if(advol>457&&advol<=678) //10~20��,20~30����Ϊ���ƣ��ϲ�
			T_vol=100+4+(int)((678-advol)*0.91);
			
		else if(advol>356&&advol<=457)// 30~40
			T_vol=300+457-advol;
		else if(advol>270&&advol<=356) //40~50
			T_vol=400+(int)((356-advol)*1.16);//pass
			
		else if(advol>203&&advol<=270) //50~60
			T_vol=500-5+(int)((270-advol)*1.59);
		else if(advol>151&&advol<=203) //60~70
			T_vol=600-10+(int)((203-advol)*1.92);
		else if(advol>113&&advol<=151) //70~80
			T_vol=700-15+(int)((151-advol)*2.63);
		else if(advol>85&&advol<=113) //80~90
			T_vol=800-20+(int)((113-advol)*3.57);	
		else if(advol>65&&advol<=85) //90~100
			T_vol=900-20+(85-advol)*5;
		else if(advol>=49&&advol<=65) //pass  ����100�ȣ����� ������һ�㲻ͬ
			{														//100~110,ע������ĵȺ�
				T_vol=1000-20+(int)((65-advol)*6.5);
				
				remainder=((int)((65-advol)*6.5))%10;
				if(remainder>=5) T_vol=T_vol+10; //�������봦��
			}
		else if(advol>=39&&advol<49) //110~120,ע������ĵȺ�
			T_vol=1100-20+(49-advol)*10;  //pass
			
		return T_vol;		//-100 ~ 1200
	}
	else 
		return T_OVERFLOW;//������ⷶΧʱ������һ����ⷶΧ����¶�ֵ�������������ʾ
}


/***************ADCת������******************/
void MY_ADC1_Init(void)
{
	GPIO_Init(GPIOD,GPIO_PIN_6,GPIO_MODE_IN_FL_NO_IT);//��ʼ��ADC1ͨ������
  ADC1_DeInit();		//ADC1��λ
  ADC1_Init(ADC1_CONVERSIONMODE_SINGLE,ADC1_CHANNEL_6, ADC1_PRESSEL_FCPU_D18, //����ת��ģʽ��18��Ƶ
    ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL3, DISABLE);
  ADC1_Cmd(ENABLE);//ʹ��ADC1
}

uint ADC1_GetValue(void)//��ȡADC1����ת���Ľ��
{
  uint adc1_vol;
  
  ADC1_StartConversion();//����ADC1ת��
  while(!(ADC1_GetFlagStatus(ADC1_FLAG_EOC)));//�ȴ�ADC1ת������
	adc1_vol=ADC1_GetConversionValue(); //�õ�ת��ֵ
  ADC1_ClearFlag(ADC1_FLAG_EOC);//���ת��������־λ
   
  return adc1_vol;//����ת�����
}

uint ADC1_GetValue_Average(void)//����10�εõ�ADC1��ת��ֵ����ȥƽ��ֵ
{																//���ת��׼ȷ��
  uint adc1_vol_average,sum=0,i=0;
  
  for(i=0;i<10;i++)
  {
    sum=sum+ADC1_GetValue();//������10��ת������ۼ�
  }
  adc1_vol_average=sum/10; //��ƽ��ֵ
	if((sum%10)>=5) adc1_vol_average++;//�������봦��
  
  return adc1_vol_average;//����ƽ��ֵ
}
/***************ADCת������******************/

/**************�������ʾ����****************/
void SEG_GPIO_Init(void)//���������ʾ��GPIO���г�ʼ������
{												
	GPIO_Init(GPIOB,GPIO_PIN_4|GPIO_PIN_5,GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(GPIOC,GPIO_PIN_HNIB|GPIO_PIN_3,GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(GPIOD,GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2,GPIO_MODE_OUT_PP_LOW_FAST);
}

void Send_NumberCode(uchar num,uchar dp_flag)
{
	switch(num)
	{
		case 0: 
			SEG_A_LOW;    
      SEG_B_LOW;    
      SEG_C_LOW;    
      SEG_D_LOW;   
      SEG_E_LOW;   
      SEG_F_LOW;    
      SEG_G_HIGH;   
			break;
			
		case 1:
			SEG_A_HIGH;    
      SEG_B_LOW;    
      SEG_C_LOW;    
      SEG_D_HIGH;    
      SEG_E_HIGH;   
      SEG_F_HIGH;    
      SEG_G_HIGH;   
			break;
		case 2: 
			SEG_A_LOW;    
      SEG_B_LOW;    
      SEG_C_HIGH;    
      SEG_D_LOW;   
      SEG_E_LOW;   
      SEG_F_HIGH;    
      SEG_G_LOW;   
			break;
			
		case 3:
			SEG_A_LOW;    
      SEG_B_LOW;    
      SEG_C_LOW;    
      SEG_D_LOW;    
      SEG_E_HIGH;   
      SEG_F_HIGH;    
      SEG_G_LOW;   
			break;
		case 4: 
			SEG_A_HIGH;    
      SEG_B_LOW;    
      SEG_C_LOW;    
      SEG_D_HIGH;   
      SEG_E_HIGH;   
      SEG_F_LOW;    
      SEG_G_LOW;   
			break;
			
		case 5:
			SEG_A_LOW;    
      SEG_B_HIGH;    
      SEG_C_LOW;    
      SEG_D_LOW;    
      SEG_E_HIGH;   
      SEG_F_LOW;    
      SEG_G_LOW;   
			break;
		case 6: 
			SEG_A_LOW;    
      SEG_B_HIGH;    
      SEG_C_LOW;    
      SEG_D_LOW;   
      SEG_E_LOW;   
      SEG_F_LOW;    
      SEG_G_LOW;   
			break;
			
		case 7:
			SEG_A_LOW;    
      SEG_B_LOW;    
      SEG_C_LOW;    
      SEG_D_HIGH;    
      SEG_E_HIGH;   
      SEG_F_HIGH;    
      SEG_G_HIGH;   
			break;
		case 8: 
			SEG_A_LOW;    
      SEG_B_LOW;    
      SEG_C_LOW;    
      SEG_D_LOW;   
      SEG_E_LOW;   
      SEG_F_LOW;    
      SEG_G_LOW;   
			break;
			
		case 9:
			SEG_A_LOW;    
      SEG_B_LOW;    
      SEG_C_LOW;    
      SEG_D_LOW;    
      SEG_E_HIGH;   
      SEG_F_LOW;    
      SEG_G_LOW;   
			break;
			
		case '-':     //��ʾ����
			SEG_A_HIGH;    
			SEG_B_HIGH;    
			SEG_C_HIGH;    
			SEG_D_HIGH;    
			SEG_E_HIGH;   
			SEG_F_HIGH;    
			SEG_G_LOW; 
			SEG_DP_HIGH;	
			
		default:break;
	}
	if(dp_flag==DP_DISENABLE)	
			SEG_DP_HIGH;//����
	else 
			SEG_DP_LOW;//��
}

void Display_SingleNumber(uchar num,uchar wei,uchar dp_flag)
{
	Send_NumberCode(num,dp_flag);		
	
	switch(wei)
	{
		case SEG_W1:
			SEG_W2_DISENABLE;
			SEG_W3_DISENABLE;
			SEG_W1_ENABLE;
			break;
		case SEG_W2:
			SEG_W3_DISENABLE;
			SEG_W1_DISENABLE;
			SEG_W2_ENABLE;
			break;
		case SEG_W3:
			SEG_W1_DISENABLE;
			SEG_W2_DISENABLE;
			SEG_W3_ENABLE;
			break;
			
		default:break;	
	}
}

void Display_Open(void)//����ʱ��ʾ8.8.8.
{
	Send_NumberCode(8,DP_ENABLE);
	SEG_W2_ENABLE;
	SEG_W3_ENABLE;
	SEG_W1_ENABLE;//��Ϊ3���������ʾ��ͬ��λѡʹ��ȫ����
	delay_ms(1500);
}

void Display_Temperature(int temp)
{
	uchar ge,shi,bai;
	
	if(temp>-100&&temp<0)
	{
		temp=-temp;//�õ������ֵ
		shi=temp/10;
		ge=temp%10;
		
		Display_SingleNumber('-',SEG_W1,DP_DISENABLE);
		delay_ms(5);
		Display_SingleNumber(shi,SEG_W2,DP_ENABLE);
		delay_ms(5);
		Display_SingleNumber(ge,SEG_W3,DP_DISENABLE);
		delay_ms(5);
		
	}
	else if(temp>=0&&temp<1000)
	{
		bai=temp/100;
		shi=temp/10%10;
		ge=temp%10;
		
		Display_SingleNumber(bai,SEG_W1,DP_DISENABLE);
		delay_ms(5);
		Display_SingleNumber(shi,SEG_W2,DP_ENABLE);
		delay_ms(5);
		Display_SingleNumber(ge,SEG_W3,DP_DISENABLE);
		delay_ms(5);
	}
	else if(temp>=1000&&temp<=1200)
	{
		temp=temp/10;//�ص����λ
		
		bai=temp/100;
		shi=temp/10%10;
		ge=temp%10;
		
		Display_SingleNumber(bai,SEG_W1,DP_DISENABLE);
		delay_ms(5);
		Display_SingleNumber(shi,SEG_W2,DP_DISENABLE);
		delay_ms(5);
		Display_SingleNumber(ge,SEG_W3,DP_DISENABLE);
		delay_ms(5);
	}
	else			//������ⷶΧ
	{
		Send_NumberCode('-',DP_DISENABLE);
		SEG_W2_ENABLE;
		SEG_W3_ENABLE;
		SEG_W1_ENABLE;
		delay_ms(15);
	//	SEG_W2_DISENABLE;
	//	SEG_W3_DISENABLE;
	//	SEG_W1_DISENABLE;
	//	delay_ms(100);//��ʾ"---"����˸
	}
}
/**************�������ʾ����****************/

/************TIM5��ʱ������***************
void TIM5_Init(void)
{
	TIM5_DeInit();
	TIM5_TimeBaseInit(TIM5_PRESCALER_16,4999);
	TIM5_PrescalerConfig(TIM5_PRESCALER_16, TIM5_PSCRELOADMODE_IMMEDIATE);
	TIM5_ARRPreloadConfig(ENABLE);
	TIM5_ITConfig(TIM5_IT_UPDATE, ENABLE);
	TIM5_Cmd(ENABLE);
}

@far @interrupt void TIM5_UPDATE_Handler(void)
{
	if(TIM5_GetITStatus(TIM5_IT_UPDATE)==SET)
	{
		wei_counter++;
		if(wei_counter>=3) wei_counter=0;
		
		TIM5_ClearITPendingBit(TIM5_IT_UPDATE);
	}
}

************TIM5��ʱ������***************/