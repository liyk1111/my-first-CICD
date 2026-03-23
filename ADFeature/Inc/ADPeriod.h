#ifndef __ADPERIOD_H
#define __ADPERIOD_H
#include "stdint.h"

#define MaxPeriodNum 5
#define ADPeriod_GetTick bsp_delay_getTick

extern uint32_t bsp_delay_getTick(void);

void ADPeriodInit(void);
void ADPeriodPrintf(void);
uint8_t  ADPeriodWaitTimeOut(uint8_t id,uint32_t Period);
uint32_t ADPeriodGetMin(void);
#endif
