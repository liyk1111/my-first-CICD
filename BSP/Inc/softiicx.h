/*
使用说明：
    1.

功能介绍：
    1.

更新记录：
    2023-11-20：TODO：需要下载到开发板重复验证
*/

#ifndef __SOFTIICX__
#define __SOFTIICX__

#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include "delayx.h"

#define SOFTIIC_NO1 1 //使能第一个软件模拟IIC
#define SOFTIIC_NO2 0 //使能第二个软件模拟IIC

//第一个软件IIC所需定义与接口
#if(SOFTIIC_NO1 == 1)
#define SCL_Type1    GPIOB
#define SDA_Type1    GPIOB
#define SCL_GPIO1    GPIO_PIN_0
#define SDA_GPIO1    GPIO_PIN_1
//设置输出高低电平模式
#define SDA1_OUT(X)   if(X) \
                          HAL_GPIO_WritePin(SDA_Type1, SDA_GPIO1, GPIO_PIN_SET); \
                      else  \
                          HAL_GPIO_WritePin(SDA_Type1, SDA_GPIO1, GPIO_PIN_RESET);
 
#define SCL1_OUT(X)   if(X) \
                          HAL_GPIO_WritePin(SCL_Type1, SCL_GPIO1, GPIO_PIN_SET); \
                      else  \
                          HAL_GPIO_WritePin(SCL_Type1, SCL_GPIO1, GPIO_PIN_RESET);    
#define SDA1_IN       HAL_GPIO_ReadPin(SDA_Type1,SDA_GPIO1)   //只有输入模式才能读取电平状态

#define ONEWIRESDA_IN       HAL_GPIO_ReadPin(SCL_Type1,SCL_GPIO1)   //只有输入模式才能读取电平状态											

//IIC1所有操作函数
void IIC1_Init(void);                //初始化IIC的IO口                 
void IIC1_Start(void);               //发送IIC开始信号
void IIC1_Stop(void);                //发送IIC停止信号
void IIC1_Send_Byte(uint8_t data);    //IIC发送一个字节
uint8_t IIC1_Read_Byte(uint8_t ack); //IIC读取一个字节
uint8_t IIC1_Wait_Ack(void);         //IIC等待ACK信号
void IIC1_Ack(void);                 //IIC发送ACK信号
void IIC1_NAck(void);                //IIC不发送ACK信号
#endif


//第二个软件IIC所需定义与接口
#if(SOFTIIC_NO2 == 1)

#endif














#endif
