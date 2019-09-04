/*====================================================================================================
    ���Ǵ�����������һ���Ƚϵ��͵�PID���������ʹ�õ�Ƭ����Ϊ����cpuʱ���������򻯣������PID
���������ɾ������ͨ��ʵ��ȷ�������ڵ�Ƭ���Ĵ����ٶȺ�ram��Դ�����ƣ�һ�㲻���ø��������㣬
�������в���ȫ�������������㵽����ٳ���һ��2��N�η����ݣ��൱����λ���������ƶ��������㣬��
�����������ٶȣ����ݿ��ƾ��ȵĲ�ͬҪ�󣬵�����Ҫ��ܸ�ʱ��ע�Ᵽ����λ����ġ���������������
���������������ֻ��һ�㳣��pid�㷨�Ļ����ܹ���û�а���������������֡�
=====================================================================================================*/
#include <string.h>
#include <stdio.h>
#include <math.h>
/*====================================================================================================
    PID Function
    
    The PID (���������֡�΢��) function is used in mainly
    control applications. PIDCalc performs one iteration of the PID
    algorithm.

    While the PID function works, main is just a dummy program showing
    a typical usage.
=====================================================================================================*/

typedef struct PID {

        double  SetPoint;           //  �趨Ŀ�� Desired Value

        double  Proportion;         //  �������� Proportional Const
        double  Integral;           //  ���ֳ��� Integral Const
        double  Derivative;         //  ΢�ֳ��� Derivative Const

        double  LastError;          //  Error[-1]
        double  PrevError;          //  Error[-2]
        double  SumError;           //  Sums of Errors

} PID;

/*====================================================================================================
   PID���㲿��
=====================================================================================================*/

double PIDCalc( PID *pp, double NextPoint )
{
    double  dError,
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

double sensor (void)                    //  Dummy Sensor Function
{
    return 100.0;
}

void actuator(double rDelta)            //  Dummy Actuator Function
{}

void main(void)
{
    PID         sPID;                   //  PID Control Structure
    double      rOut;                   //  PID Response (Output)
    double      rIn;                    //  PID Feedback (Input)

    PIDInit ( &sPID );                  //  Initialize Structure
    sPID.Proportion = 1;              //  Set PID Coefficients
    sPID.Integral   = 1;
    sPID.Derivative = 0.0;
    sPID.SetPoint   = 100.0;            //  Set PID Setpoint

    for (;;) {                          //  Mock Up of PID Processing

        rIn = sensor ();                //  Read Input
        rOut = PIDCalc ( &sPID,99.0 );   //  Perform PID Interation
        actuator ( rOut );              //  Effect Needed Changes
    }
}

