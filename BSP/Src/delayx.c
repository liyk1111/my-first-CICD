/*
注意事项:
1.如需改动，由组内统一讨论更新

*/
#include "delayx.h"
#include "stdio.h"

void ADDelay_ms(uint32_t mdelay)
{
    HAL_Delay(mdelay);
}

void ADDelay_us(uint32_t udelay)
{
    uint32_t cnt = 0;
	
	  if(udelay > 900)
		{
		    printf("ADDelay_us延时长度不能大于900us.\r\n");
		}
	
    //__HAL_TIM_ENABLE(&htim2);
    __HAL_TIM_SetCounter(&htim2, 0);
	
	  while( cnt < udelay)
		{
       cnt = __HAL_TIM_GetCounter(&htim2);
    }
   
	  //__HAL_TIM_DISABLE(&htim2);


	/* 非常不准
    uint32_t startval,tickn,delays,wait;

    startval = SysTick->VAL;
    tickn = HAL_GetTick();
    delays = udelay * STMSYS_CLOCK; //sysc / 1000 * udelay;
	
    if(delays > startval)
    {
        while(HAL_GetTick() == tickn);
        wait = STMSYS_CLOCK_INDEX + startval - delays;
        while(wait < SysTick->VAL);
    }
    else
    {
        wait = startval - delays;
        while(wait < SysTick->VAL && HAL_GetTick() == tickn);
    }
	*/
}
