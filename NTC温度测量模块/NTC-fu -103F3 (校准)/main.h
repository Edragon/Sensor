#ifndef __MAIN_H
#define __MAIN_H

#define uint unsigned int
#define uchar unsigned char

#define DP_ENABLE 1
#define DP_DISENABLE 0

#define AD_MIN 39
#define AD_MAX 858

#define T_OVERFLOW 1500//随便定义一个超出温度检测范围的值

//#define TEMPERATURE_10MAX 1100

#define SEG_W1 1
#define SEG_W2 2
#define SEG_W3 3

#define SEG_A_LOW  GPIO_WriteLow(GPIOC,GPIO_PIN_6)
#define SEG_B_LOW  GPIO_WriteLow(GPIOD,GPIO_PIN_5)
#define SEG_C_LOW  GPIO_WriteLow(GPIOB,GPIO_PIN_4)
#define SEG_D_LOW  GPIO_WriteLow(GPIOC,GPIO_PIN_7)
#define SEG_E_LOW  GPIO_WriteLow(GPIOD,GPIO_PIN_3)
#define SEG_F_LOW  GPIO_WriteLow(GPIOC,GPIO_PIN_4)
#define SEG_G_LOW  GPIO_WriteLow(GPIOB,GPIO_PIN_5)
#define SEG_DP_LOW GPIO_WriteLow(GPIOC,GPIO_PIN_5)

#define SEG_A_HIGH  GPIO_WriteHigh(GPIOC,GPIO_PIN_6)
#define SEG_B_HIGH  GPIO_WriteHigh(GPIOD,GPIO_PIN_5)
#define SEG_C_HIGH  GPIO_WriteHigh(GPIOB,GPIO_PIN_4)
#define SEG_D_HIGH  GPIO_WriteHigh(GPIOC,GPIO_PIN_7)
#define SEG_E_HIGH  GPIO_WriteHigh(GPIOD,GPIO_PIN_3)
#define SEG_F_HIGH  GPIO_WriteHigh(GPIOC,GPIO_PIN_4)
#define SEG_G_HIGH  GPIO_WriteHigh(GPIOB,GPIO_PIN_5)
#define SEG_DP_HIGH GPIO_WriteHigh(GPIOC,GPIO_PIN_5)

#define SEG_W1_ENABLE GPIO_WriteHigh(GPIOD,GPIO_PIN_2)
#define SEG_W2_ENABLE GPIO_WriteHigh(GPIOC,GPIO_PIN_3)
#define SEG_W3_ENABLE GPIO_WriteHigh(GPIOD,GPIO_PIN_4)

#define SEG_W1_DISENABLE GPIO_WriteLow(GPIOD,GPIO_PIN_2)
#define SEG_W2_DISENABLE GPIO_WriteLow(GPIOC,GPIO_PIN_3)
#define SEG_W3_DISENABLE GPIO_WriteLow(GPIOD,GPIO_PIN_4)

void delay_ms(uint nms);
void MY_ADC1_Init(void);
//uint ADC1_GetValue(void);
uint ADC1_GetValue_Average(void);

void SEG_GPIO_Init(void);
void Display_Open(void);
void Send_NumberCode(uchar num,uchar dp_flag);
void Display_Temperature(int temp);
int GetTemperatureValue_X10(uint advol);
void Display_SingleNumber(uchar num,uchar wei,uchar dp_flag);

#endif