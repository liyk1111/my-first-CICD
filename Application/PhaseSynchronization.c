#include "PhaseSynchronization.h"

static uint32_t G_Counter = 0;
static uint32_t G_Period = 0;

void CounterAdd()
{
	G_Counter++;
}

void ClearCounter()
{
	G_Counter = 0;
}

/*unit: us*/
uint32_t CaleCapturePeriod(TIM_HandleTypeDef *htim)
{
	uint32_t tmpCnt = 0;
	tmpCnt = __HAL_TIM_GetCounter(htim);
	
	G_Period = G_Counter * 10 + tmpCnt;
	
	__HAL_TIM_SetCounter(htim, 0);
	ClearCounter();
	
	return G_Period;
}

uint32_t GetCounter()
{
	return G_Counter;
}

uint32_t GetPeriod()
{
	return G_Period;
}

float CalcFreq()
{
	return 1 / (G_Period * 0.000001);
}

/*浮点绝对值转换*/
double MyFabs(double tmpSource1, double tmpSource2)
{
	double tmpDouble;

	tmpDouble = tmpSource1 - tmpSource2;

	if(tmpDouble >= 0)
		return tmpDouble;
	else
		return (tmpDouble*-1.0);
}

#define PRESCALER_MAX		72		//主频
#define MAX_ARR					65000

uint8_t CalcTimerPscAndArr(float tmpFreq, uint32_t *tmpPsc, uint32_t *tmpArr, float *tmpAccuracy)
{
	static float hisFreq = 0;
	float curPeriod = 0, compPeriod = 0, tmpFloat = 0;
	uint32_t TimArr = 0,TimPsc = 0;
	
	//TODO：这里的定时器，10us进入一次，CPU占用率太高了，可以改用自适应调节，当频率低时，精度降一下
	if(MyFabs(tmpFreq, hisFreq) > (0.02*tmpFreq))	//判断前后两次频率差的绝对值，大于2%Hz就更换一次计数
	{
		hisFreq = tmpFreq;
		
		//判断精度
		if(tmpFreq >= 0.1 && tmpFreq <= 1)
			*tmpAccuracy = 0.1;
		else if(tmpFreq > 1 && tmpFreq <= 250)
			*tmpAccuracy = 1;
		else if(tmpFreq > 250 && tmpFreq <= 500)
			*tmpAccuracy = 2;
		else if(tmpFreq > 500 && tmpFreq <= 750)
			*tmpAccuracy = 3;
		else if(tmpFreq > 750 && tmpFreq <= 4000)
			*tmpAccuracy = 4;
		else if(tmpFreq > 4000)
		{
			*tmpAccuracy = 4;
			tmpFreq = 4000;
			hisFreq = 4000;
			printf("频率大于4KHz,不支持!\r\n");
		}
		
		curPeriod = (1.0*1000000.0)/(tmpFreq*360.0/(*tmpAccuracy));	//计算出当前周期，精度为1度时的时间，us

		for(TimPsc = 0; TimPsc < PRESCALER_MAX; TimPsc++)
		{
			//每一个分频都使用最大的计数去判断，符合要求再计算
			compPeriod = (float)MAX_ARR*(TimPsc+1)/PRESCALER_MAX;								//unit:us
			if(compPeriod > curPeriod)																					//符合，向下计算误差最小的值，符合误差就使用
			{
				TimArr = curPeriod * PRESCALER_MAX / (TimPsc+1) - 0.5;						//直接通过实际周期反算计算重装值
				compPeriod = (float)(TimArr+1)*(TimPsc+1)/PRESCALER_MAX;					//计算当前分频的重装值周期
				tmpFloat = MyFabs(curPeriod, compPeriod)*(360.0/(*tmpAccuracy));	//一个周期总差值
				if(tmpFloat < curPeriod*2)																				//若符合一个周期的误差若大于2度就算通过,300Hz时得出1度多一些
					break;
			}
		}
		
		*tmpPsc = TimPsc;
		*tmpArr = TimArr;

		printf("频率:%.2fHz, 精度:%.2f度, 精度周期:%.5fus, 计算周期:%.5fus, 周期误差:%.2fus, TimArr:%d, TimPsc:%d\r\n", \
						tmpFreq, *tmpAccuracy, curPeriod, compPeriod, tmpFloat, TimArr, TimPsc);
		
		return 0;
	}
	
	return 1;
}


