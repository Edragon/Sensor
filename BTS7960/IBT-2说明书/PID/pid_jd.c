
/*====================================================================================================
    ���Ǵ�����������һ���Ƚϵ��͵�PID���������ʹ�õ�Ƭ����Ϊ����cpuʱ���������򻯣������PID
���������ɾ������ͨ��ʵ��ȷ�������ڵ�Ƭ���Ĵ����ٶȺ�ram��Դ�����ƣ�һ�㲻���ø��������㣬
�������в���ȫ�������������㵽����ٳ���һ��2��N�η����ݣ��൱����λ���������ƶ��������㣬��
�����������ٶȣ����ݿ��ƾ��ȵĲ�ͬҪ�󣬵�����Ҫ��ܸ�ʱ��ע�Ᵽ����λ����ġ���������������
���������������ֻ��һ�㳣��pid�㷨�Ļ����ܹ���û�а���������������֡�
=====================================================================================================*/
#include<reg52.h>
#include <string.h>
#include <stdio.h>
/*====================================================================================================
    PID Function
    
    The PID (���������֡�΢��) function is used in mainly
    control applications. PIDCalc performs one iteration of the PID
    algorithm.

    While the PID function works, main is just a dummy program showing
    a typical usage.
=====================================================================================================*/

typedef struct PID {

        unsigned int  SetPoint;           //  �趨Ŀ�� Desired Value

        unsigned int  Proportion;         //  �������� Proportional Const
        unsigned int  Integral;           //  ���ֳ��� Integral Const
        unsigned int  Derivative;         //  ΢�ֳ��� Derivative Const

        unsigned int  LastError;          //  Error[-1]
        unsigned int  PrevError;          //  Error[-2]
        unsigned int  SumError;           //  Sums of Errors

} PID;

/*====================================================================================================
   PID���㲿��
=====================================================================================================*/

unsigned int PIDCalc( PID *pp, unsigned int NextPoint )
{
    unsigned int  dError,
            Error;

        Error = pp->SetPoint -  NextPoint;          // ƫ��
        pp->SumError += Error;                      // ����
        dError = pp->LastError - pp->PrevError;     // ��ǰ΢��
        pp->PrevError = pp->LastError;
        pp->LastError = Error;
        return (pp->Proportion * Error              // ������
            +   pp->Integral * pp->SumError         // ������
            +   pp->Derivative * dError             // ΢����
        );
}

/*====================================================================================================
   Initialize PID Structure
=====================================================================================================*/

void PIDInit (PID *pp)
{
    memset ( pp,0,sizeof(PID));
}

/*====================================================================================================
    Main Program
=====================================================================================================*/

unsigned int sensor (void)                    //  Dummy Sensor Function
{
    return 95.0;
}

void actuator(unsigned int rDelta)            //  Dummy Actuator Function
{}

void main(void)
{
    PID         sPID;                   //  PID Control Structure
    unsigned int      rOut;                   //  PID Response (Output)
    unsigned int      rIn;                    //  PID Feedback (Input)

    PIDInit ( &sPID );                  //  Initialize Structure
    sPID.Proportion = 0.5;              //  Set PID Coefficients
    sPID.Integral   = 0.5;
    sPID.Derivative = 0.0;
    sPID.SetPoint   = 100.0;            //  Set PID Setpoint

    for (;;) {                          //  Mock Up of PID Processing

        rIn = sensor ();                //  Read Input
        rOut = PIDCalc ( &sPID,rIn );   //  Perform PID Interation
        actuator ( rOut );              //  Effect Needed Changes
    }
}

 
