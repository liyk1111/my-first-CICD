#ifndef PHASESYNCHRONIZATION_H
#define PHASESYNCHRONIZATION_H

#include <stdint.h>
#include "tim.h"
#include <stdio.h>

/*
说明：自动适配频率相位简单计算
精度	频率范围设置
0.1		[0.1~1]Hz
1			(1~250]Hz
2			(250~500]Hz
3			(500~750]Hz
4			(750~1000]Hz
*/


void CounterAdd(void);
void ClearCounter(void);
uint32_t CaleCapturePeriod(TIM_HandleTypeDef *htim);
uint32_t GetCounter(void);
uint32_t GetPeriod(void);
float CalcFreq(void);
uint8_t CalcTimerPscAndArr(float tmpFreq, uint32_t *tmpPsc, uint32_t *tmpArr, float *tmpAccuracy);

#endif
