static float Vk=1.88;//分压电阻算出的值
static float Ik=0.25;//根据采样电阻(4mR)算出的值
static uint32_t VParam=0;
static uint32_t VReg=0;
static float V=0;
static uint32_t IParam=0;
static uint32_t IReg=0;
static float I=0;

static uint32_t PParam=0;
static uint32_t PReg=0;
static float P=0;
static float PV=0;
static float PF=0;
static float PBuf[20];
static float PMin=0;
static uint8_t Windex=0;

void Widget::WLW8032_DecodProtocol(uint8_t Data)
{
    static uint8_t status=0;
    static uint8_t index=0;
    static uint8_t sum=0;
    static uint8_t tempBuf[24];
    static uint8_t i;
    switch (status)
    {
    case 0:
        if(Data==0x55)
        {
            tempBuf[0]=Data;
            status=1;
        }
        break;
    case 1:
        if(Data==0x5A)
        {
            tempBuf[1]=Data;
            status=2;
        }
        else status=0;
        index=2;
        sum=0;
        break;
    case 2:

        tempBuf[index]=Data;
        index++;
        if(index==24)
        {
            if(sum==Data)
            {
                //Tools_Printf("check sum OK\r\n");
                if(tempBuf[20]&(0x03<<4))
                {
                    VParam=(tempBuf[2]<<24)|(tempBuf[3]<<16)|tempBuf[4];
                    VReg=(tempBuf[5]<<24)|(tempBuf[6]<<16)|tempBuf[7];
                    IParam=(tempBuf[8]<<24)|(tempBuf[9]<<16)|tempBuf[10];
                    IReg=(tempBuf[11]<<24)|(tempBuf[12]<<16)|tempBuf[13];
                    PParam=(tempBuf[14]<<24)|(tempBuf[15]<<16)|tempBuf[16];
                    PReg=(tempBuf[17]<<24)|(tempBuf[18]<<16)|tempBuf[19];

                    V=(float)VParam/(float)VReg*Vk;
                    I=(float)IParam/(float)IReg*Ik;
                    P=(float)PParam/(float)PReg*Vk*Ik;
                    PV=V*I;
                    PF=P/PV;
                    //Tools_Printf("V:%f\tI:%f\tP:%f\tPV:%f\r\n",V,I,P,PV);
                    qDebug()<<"V:"<<V<<"I:"<<I<<"P:"<<P<<"PV:"<<PV<<"PF:"<<PF;
                }
            }
            //else Tools_Printf("check sum Fail\r\n");
            status=0;
        }
        else
        {
            sum+=Data;
        }
        break;
    default:
        break;
    }
}