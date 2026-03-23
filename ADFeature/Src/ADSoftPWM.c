#include "ADSoftPWM.h"
#include "stdlib.h"
#include "stdio.h"

#ifndef ADSoftPWMMalloc 
	#define ADSoftPWMMalloc malloc
#endif


#ifndef ADSoftPWMFree
	#define ADSoftPWMFree free
#endif
/*创建软件定时器*/
ADSoftPWM *ADSoftPWMCreat(void)
{
	ADSoftPWM *xADSoftPWM;
	xADSoftPWM = (ADSoftPWM*)ADSoftPWMMalloc(sizeof(ADSoftPWM));
	return xADSoftPWM;
}

/*初始化*/
/*
	xADSoftPWM:PWM
	outPutH:输出高电平函数
	outPutL:输出低电平函数
	TimerPeriod_us:修改定时器的中断间隔函数
	TimerPeriodStart：定时器开始。
	TimerPeriodStop：定时器结束
*/
void ADSoftPWMInit(ADSoftPWM *xADSoftPWM,void *outPutH,void *outPutL,void *TimerPeriod_us,void *TimerPeriodStart, void *TimerPeriodStop)
{
	xADSoftPWM->outPutH 			=  	(void (*)(void))outPutH;
	xADSoftPWM->outPutL 			= 	(void (*)(void))outPutL;
	xADSoftPWM->TimerPeriod_us 		= 	(void (*)(uint32_t))TimerPeriod_us;
	xADSoftPWM->TimerPeriodStop 	= 	(void (*)(void))TimerPeriodStop;
	xADSoftPWM->TimerPeriodStart 	= 	(void (*)(void))TimerPeriodStart;
	
	xADSoftPWM->polarity = 1;					/*极性默认为1*/
	xADSoftPWM->dutyCycle = 50;					/*占空比默认为50%*/
	xADSoftPWM->frequency = 0;					/*频率默认为0*/
}

/*设置极性*/
void ADSoftPWMSetPolarity(ADSoftPWM *xADSoftPWM,float polarity)
{
	xADSoftPWM->polarity = polarity;
}

/*设置占空比*/
void ADSoftPWMSetCompare(ADSoftPWM *xADSoftPWM,float dutyCycle)
{
	float tempf = 0,tempf1=0;
	uint8_t nowPolarity;
	xADSoftPWM->dutyCycle = dutyCycle;
	nowPolarity =  xADSoftPWM->polarity;
	tempf = 1000000/xADSoftPWM->frequency;						/*获取周期*/
	tempf1 = tempf*xADSoftPWM->dutyCycle/100;					/*计算极性为xADSoftPWM->polarity时的时间*/
	xADSoftPWM->PolarityTime[nowPolarity] = tempf1;				/*该极性对应的持续时间*/
	xADSoftPWM->PolarityTime[!nowPolarity] = tempf-tempf1;		/*该极性对应的持续时间*/
}

/*设置频率*/
void ADSoftPWMSetFrequency(ADSoftPWM *xADSoftPWM,uint32_t frequency)
{
	float tempf = 0,tempf1=0;
	uint8_t nowPolarity;
	xADSoftPWM->frequency = frequency;
	
	nowPolarity =  xADSoftPWM->polarity;
	tempf = 1000000/xADSoftPWM->frequency;				/*获取周期*/
	tempf1 = tempf*xADSoftPWM->dutyCycle/100;			/*计算极性为xADSoftPWM->polarity时的时间*/
	xADSoftPWM->PolarityTime[nowPolarity] = tempf1;				/*该极性对应的持续时间*/
	xADSoftPWM->PolarityTime[!nowPolarity] = tempf-tempf1;		/*该极性对应的持续时间*/
}
/*设置频率和占空比*/
void ADSoftPWMSetFrequencyAndCompare(ADSoftPWM *xADSoftPWM,uint32_t frequency,float dutyCycle)
{
	float tempf = 0,tempf1=0;
	uint8_t nowPolarity;
	xADSoftPWM->dutyCycle = dutyCycle;
	xADSoftPWM->frequency = frequency;
	
	nowPolarity =  xADSoftPWM->polarity;
	tempf = 1000000/xADSoftPWM->frequency;				/*获取周期*/
	tempf1 = tempf*xADSoftPWM->dutyCycle/100;			/*计算极性为xADSoftPWM->polarity时的时间*/
	xADSoftPWM->PolarityTime[nowPolarity] = tempf1;				/*该极性对应的持续时间*/
	xADSoftPWM->PolarityTime[!nowPolarity] = tempf-tempf1;		/*该极性对应的持续时间*/
}

/*开启PWM输出*/
void ADSoftPWMStart(ADSoftPWM *xADSoftPWM)
{
	if(xADSoftPWM->frequency!=0)
	{	
		/*根据极性输出*/
		if(xADSoftPWM->polarity == 1)
			xADSoftPWM->outPutH();
		else 
			xADSoftPWM->outPutL();		
		
		xADSoftPWM->nowPolarity = xADSoftPWM->polarity;			
		xADSoftPWM->TimerPeriod_us(xADSoftPWM->PolarityTime[xADSoftPWM->nowPolarity]);
		xADSoftPWM->TimerPeriodStart();
	}
}
/*关闭PWM输出*/
void ADSoftPWMStop(ADSoftPWM *xADSoftPWM)
{
	xADSoftPWM->TimerPeriodStop();
} 
/*运行PWM*/
void ADSoftPWMRun(ADSoftPWM *xADSoftPWM)
{
	xADSoftPWM->nowPolarity = !xADSoftPWM->nowPolarity ;	
	if(xADSoftPWM->nowPolarity == 1)
	xADSoftPWM->outPutH();
	else 
	xADSoftPWM->outPutL();
	xADSoftPWM->TimerPeriod_us(xADSoftPWM->PolarityTime[xADSoftPWM->nowPolarity]);	
}
