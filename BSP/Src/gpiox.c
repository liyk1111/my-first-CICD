/*
注意事项:
1.讨论是否有必要写多一层ADGPIO

*/
#include "gpiox.h"

void GPIOSetOutputPinLevel(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
}

uint16_t GPIOReadInputPinLevel(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
}
