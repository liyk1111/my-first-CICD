#include "systick.h"
#include "driverPort.h"

extern TIM_HandleTypeDef        htim1;
#define htim htim1

uint32_t HAL_GetTick(void)
{
	if(osKernelRunning() == 1)
	{	
		/*系统开始运行了*/
		return osKernelSysTick()*portTICK_PERIOD_MS;
	}
	else
	{
		return uwTick;
	}
}

void HAL_Delay( uint32_t Delay)
{
	if(osKernelRunning() == 1)
	{	
		/*系统开始运行了*/
		osDelay(Delay);		
	}
	else
	{
		uint32_t tickstart = HAL_GetTick();
		uint32_t wait = Delay;

	  /* Add a freq to guarantee minimum wait */
	  if (wait < HAL_MAX_DELAY)
	  {
		wait += (uint32_t)(uwTickFreq);
	  }

	  while ((HAL_GetTick() - tickstart) < wait)
	  {
	  }
	}
}

HAL_StatusTypeDef sysTimStart(uint32_t ms)
{
	HAL_StatusTypeDef status;
	uint32_t uwTimclock =0;
	uint32_t            uwPrescalerValue;
	uwTimclock = HAL_RCC_GetPCLK2Freq();
	uwPrescalerValue = uwTimclock/(1000*2)-1;
	//printf("sysTimStart:%d,%d\r\n",uwPrescalerValue,uwTimclock);
	 /* Initialize TIM4 */


	/* Initialize TIMx peripheral as follow:
	+ Period = [(TIM4CLK/1000) - 1]. to have a (1/1000) s time base.
	+ Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
	+ ClockDivision = 0
	+ Counter direction = Up
	*/
	htim.Init.Period = ms*2-1;
	htim.Init.Prescaler = uwPrescalerValue;

	
	status = HAL_TIM_Base_Init(&htim);
	if (status == HAL_OK)
	{
		__HAL_TIM_CLEAR_FLAG(&htim,TIM_FLAG_UPDATE);//清除中断标志，避免立刻进入中断
		status = HAL_TIM_Base_Start_IT(&htim);
	}
	else 
	{
		//printf("定时器开启失败\r\n");
	}
	return status;
}

void sysTimStop(void)
{	
	HAL_TIM_Base_Stop_IT(&htim);
	htim.Instance->CNT = 0;
}
/*返回计时时间，单位：ms*/
uint32_t sysTimCnt(void)
{
	return htim.Instance->CNT/2;
}
