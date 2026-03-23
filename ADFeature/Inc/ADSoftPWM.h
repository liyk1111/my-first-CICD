#ifndef __ADSOFTPWM_H
#define __ADSOFTPWM_H
#include "stdint.h"



typedef struct
{
	void (*outPutH)(void);
	void (*outPutL)(void);
	void (*TimerPeriod_us)(uint32_t);
	void (*TimerPeriodStart)(void);
	void (*TimerPeriodStop)(void);
	uint8_t polarity;		/*极性*/
	float dutyCycle;		/*占空比*/
	uint32_t frequency;		/*频率*/	
	uint8_t nowPolarity;	/*当前极性*/	
	uint32_t PolarityTime[2];
}
ADSoftPWM;


/*创建软件定时器*/
ADSoftPWM *ADSoftPWMCreat(void);
/*初始化*/
void ADSoftPWMInit(ADSoftPWM *xADSoftPWM,void *outPutH,void *outPutL,void *TimerPeriod_us,void *TimerPeriodStart, void *TimerPeriodStop);
/*设置极性*/
void ADSoftPWMSetPolarity(ADSoftPWM *xADSoftPWM,float polarity);
/*设置占空比*/
void ADSoftPWMSetCompare(ADSoftPWM *xADSoftPWM,float dutyCycle);
/*设置频率*/
void ADSoftPWMSetFrequency(ADSoftPWM *xADSoftPWM,uint32_t frequency);
/*设置频率和占空比*/
void ADSoftPWMSetFrequencyAndCompare(ADSoftPWM *xADSoftPWM,uint32_t frequency,float dutyCycle);
/*开启PWM输出*/
void ADSoftPWMStart(ADSoftPWM *xADSoftPWM);
/*关闭PWM输出*/
void ADSoftPWMStop(ADSoftPWM *xADSoftPWM); 
/*运行PWM*/
void ADSoftPWMRun(ADSoftPWM *xADSoftPWM);
#endif
