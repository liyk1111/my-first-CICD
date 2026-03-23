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

#define ONEWIRE_NO1 1 //使能第一个单线传输
#define ONEWIRE_NO2 0 //使能第二个单线传输

//第一个单线传输所需定义与接口
#if(ONEWIRE_NO1 == 1)
#define ONEWIRE_Type1    AM2322_SDA_GPIO_Port
#define ONEWIRE_GPIO1    AM2322_SDA_Pin
//设置输出高低电平模式
#define ONEWIRE1_OUT(X)   if(X) \
                              HAL_GPIO_WritePin(ONEWIRE_Type1, ONEWIRE_GPIO1, GPIO_PIN_SET); \
                          else  \
                              HAL_GPIO_WritePin(ONEWIRE_Type1, ONEWIRE_GPIO1, GPIO_PIN_RESET);
 
#define ONEWIRE1_IN       HAL_GPIO_ReadPin(ONEWIRE_Type1,ONEWIRE_GPIO1)   //只有输入模式才能读取电平状态
void OneWire1IOSetOutput(void);
void OneWire1IOSetInput(void);
#endif


//第二个单线传输所需定义与接口
#if(ONEWIRE_NO2 == 1)

#endif














#endif
