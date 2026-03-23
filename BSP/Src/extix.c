/*
注意事项:
1.如需改动，由组内统一讨论更新

*/
#include "extix.h"


__weak void BSPEXTI0_IRQHandler(void)
{

}

void EXTIn_IRQ_ENABLE(IRQn_Type IRQn)
{
    HAL_NVIC_EnableIRQ(IRQn);
}

void EXTIn_IRQ_DISABLE(IRQn_Type IRQn)
{
    HAL_NVIC_DisableIRQ(IRQn);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GPS_PPS_Pin)
    {
        BSPEXTI0_IRQHandler();
    }
}

