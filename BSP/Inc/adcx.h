/*
使用说明：
    1.

功能介绍：
    1.

更新记录：
    2023-11-20：TODO：需要下载到开发板重复验证
*/

#ifndef ADCX_H
#define ADCX_H
#include "stdint.h" 
#include "ADConfig.h"
#include "adc.h"

uint8_t ADC1Init(void);
uint8_t ADC1DeInit(void);
void StartADC1Sample(uint32_t xChannel,uint16_t* data,uint32_t len);
uint16_t StartADC1Average(uint32_t xChannel, uint32_t times);

#endif
