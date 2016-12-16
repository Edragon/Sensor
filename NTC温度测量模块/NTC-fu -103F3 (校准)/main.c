#include"stm8s.h"
#include"main.h"

main()
{
	uchar count;
	int TempVol_X10,ADC_vol;
	
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);//配置时钟，使用内部16M高速时钟，不分频
	
	SEG_GPIO_Init();
	MY_ADC1_Init();
//	enableInterrupts();
	Display_Open();//开机显示888
	
	while (1)
	{
		ADC_vol=ADC1_GetValue_Average();		//获取ADC转换结果
//		ADC_vol=850;
		TempVol_X10=GetTemperatureValue_X10(ADC_vol);//
		
		for(count=0;count<50;count++)//避免刷新过快，进行延时
		{
			Display_Temperature(TempVol_X10);//显示温度值
		}
		count=0;//重新计数
	}
}

void delay_ms(uint nms)//ms延时函数
{												
	uint x,y;						//在16M主时钟下，延时1ms
	for(x=nms;x>0;x--)
		for(y=1500;y>0;y--);
}

int GetTemperatureValue_X10(uint advol)//将AD值转换成相应温度值 的10倍
{																			//10倍便于数据处理	
	int T_vol;
	uchar remainder;
	
	if((advol>=AD_MIN)&&(advol<=AD_MAX))//判断是否超出检测范围
	{
		if(advol>778&&advol<=858)       //-10~0，由于非线性，分11段处理，每段跨10度, 
			T_vol=(-100)+8+(int)((858-advol)*1.25);//乘10处理
		else if(advol>678&&advol<=778) //0~10
			T_vol=0+8+778-advol;
		else if(advol>457&&advol<=678) //10~20度,20~30度因为相似，合并
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
		else if(advol>=49&&advol<=65) //pass  大于100度，处理 方法有一点不同
			{														//100~110,注意这里的等号
				T_vol=1000-20+(int)((65-advol)*6.5);
				
				remainder=((int)((65-advol)*6.5))%10;
				if(remainder>=5) T_vol=T_vol+10; //四舍五入处理
			}
		else if(advol>=39&&advol<49) //110~120,注意这里的等号
			T_vol=1100-20+(49-advol)*10;  //pass
			
		return T_vol;		//-100 ~ 1200
	}
	else 
		return T_OVERFLOW;//超出检测范围时，返回一个检测范围外的温度值，用于数码管显示
}


/***************ADC转换部分******************/
void MY_ADC1_Init(void)
{
	GPIO_Init(GPIOD,GPIO_PIN_6,GPIO_MODE_IN_FL_NO_IT);//初始化ADC1通道引脚
  ADC1_DeInit();		//ADC1复位
  ADC1_Init(ADC1_CONVERSIONMODE_SINGLE,ADC1_CHANNEL_6, ADC1_PRESSEL_FCPU_D18, //单次转换模式，18分频
    ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL3, DISABLE);
  ADC1_Cmd(ENABLE);//使能ADC1
}

uint ADC1_GetValue(void)//获取ADC1单次转换的结果
{
  uint adc1_vol;
  
  ADC1_StartConversion();//启动ADC1转换
  while(!(ADC1_GetFlagStatus(ADC1_FLAG_EOC)));//等待ADC1转换结束
	adc1_vol=ADC1_GetConversionValue(); //得到转换值
  ADC1_ClearFlag(ADC1_FLAG_EOC);//清除转换结束标志位
   
  return adc1_vol;//返回转换结果
}

uint ADC1_GetValue_Average(void)//连续10次得到ADC1的转换值，再去平均值
{																//提高转换准确度
  uint adc1_vol_average,sum=0,i=0;
  
  for(i=0;i<10;i++)
  {
    sum=sum+ADC1_GetValue();//对连续10次转换结果累加
  }
  adc1_vol_average=sum/10; //求平均值
	if((sum%10)>=5) adc1_vol_average++;//四舍五入处理
  
  return adc1_vol_average;//返回平均值
}
/***************ADC转换部分******************/

/**************数码管显示部分****************/
void SEG_GPIO_Init(void)//对数码管显示的GPIO进行初始化配置
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
			
		case '-':     //显示负号
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
			SEG_DP_HIGH;//不亮
	else 
			SEG_DP_LOW;//亮
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

void Display_Open(void)//开机时显示8.8.8.
{
	Send_NumberCode(8,DP_ENABLE);
	SEG_W2_ENABLE;
	SEG_W3_ENABLE;
	SEG_W1_ENABLE;//因为3个数码管显示相同，位选使能全部打开
	delay_ms(1500);
}

void Display_Temperature(int temp)
{
	uchar ge,shi,bai;
	
	if(temp>-100&&temp<0)
	{
		temp=-temp;//得到其绝对值
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
		temp=temp/10;//截掉最低位
		
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
	else			//超出检测范围
	{
		Send_NumberCode('-',DP_DISENABLE);
		SEG_W2_ENABLE;
		SEG_W3_ENABLE;
		SEG_W1_ENABLE;
		delay_ms(15);
	//	SEG_W2_DISENABLE;
	//	SEG_W3_DISENABLE;
	//	SEG_W1_DISENABLE;
	//	delay_ms(100);//显示"---"并闪烁
	}
}
/**************数码管显示部分****************/

/************TIM5定时器部分***************
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

************TIM5定时器部分***************/