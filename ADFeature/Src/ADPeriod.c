#include "ADPeriod.h"
#include "stdio.h"
 /*获取系统节拍*/
/*
	V1.0.0:第一版。
	V1.0.1:修改ADPeriodInit接口
	*/
typedef struct
{
	uint32_t DelayTime;
	uint32_t StartTime;
	uint8_t isUsed;
}
PeriodTimeStruct;

PeriodTimeStruct PeriodTime[MaxPeriodNum];




void ADPeriodInitWithStartTime(uint32_t StartTime)
{
	uint8_t ii = 0;
	for(ii = 0;ii<MaxPeriodNum;ii++)
	{
		PeriodTime[ii].StartTime = StartTime;
		PeriodTime[ii].isUsed = 0;
		PeriodTime[ii].DelayTime = 1;
	}
}
void ADPeriodInit(void)
{
	uint8_t ii = 0;
	for(ii = 0;ii<MaxPeriodNum;ii++)
	{
		PeriodTime[ii].StartTime = ADPeriod_GetTick();
		PeriodTime[ii].isUsed = 0;
		PeriodTime[ii].DelayTime = 1;
	}
}
void ADPeriodPrintf(void)
{
	uint8_t ii = 0;
	for(ii = 0;ii<MaxPeriodNum;ii++)
	{
		if(PeriodTime[ii].isUsed == 1)
		{
			printf("ii=%d\r\n",ii);
			printf("PeriodTime[ii].StartTime=%d\r\n",PeriodTime[ii].StartTime);
			printf("PeriodTime[ii].StartTime=%d\r\n",PeriodTime[ii].DelayTime);
		}		
	}
}
uint8_t  ADPeriodWaitTimeOut(uint8_t id,uint32_t Period)
{
	uint32_t NowTime;
	NowTime = ADPeriod_GetTick();	
	PeriodTime[id].DelayTime = Period;
	PeriodTime[id].isUsed = 1;
	if(NowTime>(PeriodTime[id].StartTime+PeriodTime[id].DelayTime))
	{
		PeriodTime[id].StartTime = NowTime;
		return 1;
	}
	else return 0;
}

uint32_t ADPeriodGetMin(void)
{
	uint8_t ii = 0;
	uint32_t NowTime;
	uint32_t MinPeriod=0xffffffff;
	NowTime = ADPeriod_GetTick();
	for(ii = 0;ii<MaxPeriodNum;ii++)
	{
		if(PeriodTime[ii].isUsed==1)
		{
			if(NowTime>(PeriodTime[ii].StartTime+PeriodTime[ii].DelayTime))
			{
				return 0;
			}
			else 
			{								
				if(MinPeriod > PeriodTime[ii].DelayTime-(NowTime - PeriodTime[ii].StartTime))
				MinPeriod = PeriodTime[ii].DelayTime-(NowTime - PeriodTime[ii].StartTime);			
			}
		}		
	}
	return MinPeriod;
}
