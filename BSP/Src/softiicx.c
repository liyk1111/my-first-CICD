/*
注意事项:
1.如需改动，由组内统一讨论更新

*/
#include "softiicx.h"

//第一个软件IIC所需定义与接口
#if(SOFTIIC_NO1 == 1)
void IIC1_Init(void)                //初始化IIC的IO口  
{
    //CUBE已自动初始化
    IIC1_Stop();
}               
void IIC1_Start(void)               //发送IIC开始信号
{
    SDA1_OUT(1);
    SCL1_OUT(1);
    ADDelay_us(4);
    SDA1_OUT(0);
    ADDelay_us(4);
    SCL1_OUT(0);
}
void IIC1_Stop(void)                //发送IIC停止信号
{
    SCL1_OUT(0);
    SDA1_OUT(0);
    ADDelay_us(4);
    SCL1_OUT(1);
    SDA1_OUT(1);
    ADDelay_us(4);
}
void IIC1_Send_Byte(uint8_t data)    //IIC发送一个字节
{
    uint8_t t;

    SCL1_OUT(0);                        //拉低时钟开始数据传输
    for (t = 0; t < 8; t++)
    {
        SDA1_OUT((data & 0x80) >> 7);
        data <<= 1;
        ADDelay_us(2);
        SCL1_OUT(1);
        ADDelay_us(2);
        SCL1_OUT(0);
        ADDelay_us(2);
    }
}
uint8_t IIC1_Read_Byte(uint8_t ack) //IIC读取一个字节
{
    uint8_t i, receive = 0;

    SDA1_OUT(1);            /// 没有这两行会读不到高字节
    SCL1_OUT(1);            /// 

    for (i = 0; i < 8; i++) {
        SCL1_OUT(0);
        ADDelay_us(2);
        SCL1_OUT(1);
        receive <<= 1;

        if (SDA1_IN)
            receive++;

        ADDelay_us(1);
    }

    if (!ack)
        IIC1_NAck(); //发送nACK
    else{ 
        IIC1_Ack(); //发送ACK     
    }

    return receive;
}
uint8_t IIC1_Wait_Ack(void)         //IIC等待ACK信号
{
    uint8_t ucErrTime = 0;

    SCL1_OUT(1);
    ADDelay_us(1);
    SCL1_OUT(1);
    ADDelay_us(1);

    while (SDA1_IN) {
        ucErrTime++;

        if (ucErrTime > 250) {
            printf("IIC1_Wait_Ack timeout\r\n");
            IIC1_Stop();
            return 1;
        }
    }

    SCL1_OUT(0);
    return 0;
}
void IIC1_Ack(void)                 //IIC发送ACK信号
{
    SCL1_OUT(0);
    SDA1_OUT(0);
    ADDelay_us(2);
    SCL1_OUT(1);
    ADDelay_us(2);
    SCL1_OUT(0);
}
void IIC1_NAck(void)                //IIC不发送ACK信号
{
    SCL1_OUT(0);
    SDA1_OUT(1);
    ADDelay_us(2);
    SCL1_OUT(1);
    ADDelay_us(2);
    SCL1_OUT(0);
}
#endif


//第二个软件IIC所需定义与接口
#if(SOFTIIC_NO2 == 1)

#endif
