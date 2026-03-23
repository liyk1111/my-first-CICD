#include "timerx.h"
#include "stdio.h"
/****************************************定时器部分***********************************************/

/*定时器初始化 - 根据需求更改*/
void TimInit(TIM_HandleTypeDef *htim)
{	

#if defined(__Tim1ClockFreq)
	if(htim == &htim1)
		MX_TIM1_Init();	
#endif

#if defined(__Tim2ClockFreq)
	if(htim == &htim2)
		MX_TIM2_Init();	
#endif

#if defined(__Tim3ClockFreq)
	if(htim == &htim3)
		MX_TIM3_Init();	
#endif

#if defined(__Tim4ClockFreq)
	if(htim == &htim4)
		MX_TIM4_Init();	
#endif

#if defined(__Tim5ClockFreq)
	if(htim == &htim5)
		MX_TIM5_Init();	
#endif

#if defined(__Tim6ClockFreq)
	if(htim == &htim6)
		MX_TIM6_Init();	
#endif

#if defined(__Tim7ClockFreq)
	if(htim == &htim7)
		MX_TIM7_Init();	
#endif

#if defined(__Tim8ClockFreq)
	if(htim == &htim8)
		MX_TIM8_Init();	
#endif

#if defined(__Tim9ClockFreq)
	if(htim == &htim9)
		MX_TIM9_Init();	
#endif

#if defined(__Tim10ClockFreq)
	if(htim == &htim10)
		MX_TIM10_Init();	
#endif

#if defined(__Tim11ClockFreq)
	if(htim == &htim11)
		MX_TIM11_Init();	
#endif

#if defined(__Tim12ClockFreq)
	if(htim == &htim12)
		MX_TIM12_Init();	
#endif

#if defined(__Tim13ClockFreq)
	if(htim == &htim13)
		MX_TIM13_Init();	
#endif

#if defined(__Tim14ClockFreq)
	if(htim == &htim14)
		MX_TIM14_Init();	
#endif
}

void TimDeInit(TIM_HandleTypeDef *htim)
{
	HAL_TIM_Base_MspDeInit(htim);
	HAL_TIM_Base_DeInit(htim);
}

uint32_t TimGetClock(TIM_HandleTypeDef *htim)
{
#if defined(__Tim1ClockFreq)
	if(htim->Instance == TIM1)
		return __Tim1ClockFreq;	
#endif

#if defined(__Tim2ClockFreq)
	if(htim->Instance == TIM2)
		return __Tim2ClockFreq;
#endif

#if defined(__Tim3ClockFreq)
	if(htim->Instance == TIM3)
		return __Tim3ClockFreq;
#endif

#if defined(__Tim4ClockFreq)
	if(htim->Instance == TIM4)
		return __Tim4ClockFreq;
#endif

#if defined(__Tim5ClockFreq)
	if(htim->Instance == TIM5)
		return __Tim5ClockFreq;
#endif

#if defined(__Tim6ClockFreq)
	if(htim->Instance == TIM6)
		return __Tim6ClockFreq;
#endif

#if defined(__Tim7ClockFreq)
	if(htim->Instance == TIM7)
		return __Tim7ClockFreq;
#endif

#if defined(__Tim8ClockFreq)
	if(htim->Instance == TIM8)
		return __Tim8ClockFreq;
#endif

#if defined(__Tim9ClockFreq)
	if(htim->Instance == TIM9)
		return __Tim9ClockFreq;
#endif

#if defined(__Tim10ClockFreq)
	if(htim->Instance == TIM10)
		return __Tim10ClockFreq;
#endif

#if defined(__Tim11ClockFreq)
	if(htim->Instance == TIM11)
		return __Tim11ClockFreq;
#endif

#if defined(__Tim12ClockFreq)
	if(htim->Instance == TIM12)
		return __Tim12ClockFreq;
#endif

#if defined(__Tim13ClockFreq)
	if(htim->Instance == TIM13)
		return __Tim13ClockFreq;
#endif

#if defined(__Tim14ClockFreq)
	if(htim->Instance == TIM14)
		return __Tim14ClockFreq;	
#endif
}

/*
	初始化定时器
	同时设置定时的时间.单位微秒。
*/
void TimInitWithSetPeriod_us(TIM_HandleTypeDef *htim, uint32_t us)
{	
	TimInit(htim);	

	htim->Init.Prescaler = (TimGetClock(htim) / 1000000.0f) - 1;		
	if (HAL_TIM_Base_Init(htim) != HAL_OK)
	{
		Error_Handler();
	}
	TimSetPeriod_us(htim, us);	
}

/*
	初始化定时器
	同时设置定时的时间.单位毫秒。
*/
void TimInitWithSetPeriod_ms(TIM_HandleTypeDef *htim, uint32_t ms)
{	
	TimInit(htim);

	htim->Init.Prescaler = (TimGetClock(htim) / 10000.0f) - 1;		
	if (HAL_TIM_Base_Init(htim) != HAL_OK)
	{
		Error_Handler();
	}
	TimSetPeriod_ms(htim, ms);	
}

/*设置定时器中断时间间隙
	单位：微秒
*/
void TimSetPeriod_us(TIM_HandleTypeDef *htim, uint32_t us)
{
	uint32_t ClockDivision = 0;
	uint32_t ClockFreq = 0;	
	ClockDivision = htim->Init.Prescaler + 1;
	ClockFreq = TimGetClock(htim) / ClockDivision;
	htim->Instance->ARR = us / 1000.0f * ClockFreq / 1000.0f - 1;
	printf("arr:%u\r\n",htim->Instance->ARR);
}

/*设置定时器中断时间间隙
	单位：毫秒
*/
void TimSetPeriod_ms(TIM_HandleTypeDef *htim, uint32_t ms)
{
	uint32_t ClockDivision = 0;
	uint32_t ClockFreq = 0;	
	ClockDivision = htim->Init.Prescaler+1;
	ClockFreq = TimGetClock(htim) / ClockDivision;
	htim->Instance->ARR = ms * ClockFreq / 1000 - 1;
}

/*开始定时*/
void TimStart(TIM_HandleTypeDef *htim)
{
	HAL_TIM_Base_Start_IT(htim);
}

/*停止计时*/
void TimStop(TIM_HandleTypeDef *htim)
{
	HAL_TIM_Base_Stop_IT(htim);
}

/*获取定时器值*/
uint32_t TimGetCnt(TIM_HandleTypeDef *htim)
{
	return __HAL_TIM_GetCounter(htim);
}

/*设置定时器值*/
void TimSetCnt(TIM_HandleTypeDef *htim, uint32_t counter)
{
	__HAL_TIM_SetCounter(htim, counter);
}

void TimSetPrescaler(TIM_HandleTypeDef *htim, uint32_t tmpPsc)
{
	__HAL_TIM_SET_PRESCALER(htim, tmpPsc);
}

void TimSetAutoReload(TIM_HandleTypeDef *htim, uint32_t tmpArr)
{
	__HAL_TIM_SET_AUTORELOAD(htim, tmpArr);
}

//此函数为刷新状态，让定时器的参数更改马上生效
void TimGenerateEvent(TIM_HandleTypeDef *htim)
{
	HAL_TIM_GenerateEvent(htim, TIM_EVENTSOURCE_UPDATE);
}

/*定时器中断，__weak声明，可以重写*/
__weak void TimIRQHandler(TIM_HandleTypeDef *htim)
{
	
}

/****************************************PWM部分***********************************************/
/*初始化PWM*/
void PWMInit(TIM_HandleTypeDef *htim)
{
	TimInit(htim);
}

/*初始化PWM并设置频率*/
/*ClockFrequency:时钟频率。例如STM32F4为168MHZ STM32F1为72MHZ*/
/*PWMFrequency:PWM的频率。单位HZ*/
uint8_t PWMInitWithSetFrequency(TIM_HandleTypeDef *htim, uint32_t ClockFrequency, uint32_t PWMFrequency)
{	
	uint32_t Period = 0;
	uint16_t Prescaler = 1;
	TimInit(htim);
	
	Period = ClockFrequency/PWMFrequency;	
	while(Period>=65535)
	{
		Prescaler ++;
		Period = (ClockFrequency/Prescaler)/PWMFrequency;
	}
	
	htim->Instance = TIM1;
	htim->Init.Prescaler = Prescaler-1;
	htim->Init.CounterMode = TIM_COUNTERMODE_UP;
	htim->Init.Period = Period-1;
	htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim->Init.RepetitionCounter = 0;
	htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_PWM_Init(htim) != HAL_OK)
	{
		Error_Handler();
	}
	return 0;
}

void PWMDeInit(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_MspDeInit(htim);
	HAL_TIM_PWM_DeInit(htim);
}

/*开始输出PWM*/
void PWMStartOutput(TIM_HandleTypeDef *htim, uint32_t xChannel)
{
	HAL_TIM_PWM_Start(htim, xChannel);
}

/*停止输出输出PWM*/
void PWMStopOutput(TIM_HandleTypeDef *htim, uint32_t xChannel)
{
	HAL_TIM_PWM_Stop(htim, xChannel);
}

/*设置占空比*/
/*Compare:占空比（单位：%）*/
uint8_t PWMSetCompare(TIM_HandleTypeDef *htim, uint32_t xChannel,float Compare)
{
	uint32_t Period = 0;
	float PulseCnt = 0;
	Period = htim->Instance->ARR;
	PulseCnt = Compare*Period / 100.0f;
	__HAL_TIM_SET_COMPARE(htim, xChannel, (uint32_t)PulseCnt);
	
	return 0;
}

/*设置极性*/
uint8_t PWMSetPolarity(TIM_HandleTypeDef *htim, uint32_t xChannel, uint32_t Polarity)
{
	TIM_OC_InitTypeDef sConfigOC = {0};
	PWMStopOutput(htim, xChannel);
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse =  __HAL_TIM_GET_COMPARE(htim,xChannel);;
	if(Polarity == 1)
		sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	else 
		sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;

	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;

	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	PWMStartOutput(htim, xChannel);
	return 0;
}

/****************************************捕获部分***********************************************/
/*定时器初始化*/
void TimCaptureInit(TIM_HandleTypeDef *htim)
{	
	TimInit(htim);
}

void TimCaptureDeInit(TIM_HandleTypeDef *htim)
{
	HAL_TIM_Base_MspDeInit(htim);
	HAL_TIM_Base_DeInit(htim);
}

/*开始捕获*/
void TimCaptureStart(TIM_HandleTypeDef *htim, uint32_t timCH)
{
	HAL_TIM_IC_Start_IT(htim, timCH);
}

/*停止捕获,定时器也会同时停止*/
void TimCaptureStop(TIM_HandleTypeDef *htim, uint32_t timCH)
{
	HAL_TIM_IC_Stop_IT(htim, timCH);
}

/*捕获中断，__weak声明，需要重写*/
__weak void TimCaptureIRQHandler(TIM_HandleTypeDef *htim)
{
	
}
