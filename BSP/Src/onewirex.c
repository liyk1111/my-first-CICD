/*
注意事项:
1.如需改动，由组内统一讨论更新

*/
#include "onewirex.h"

//第一个单线传输所需定义与接口
#if(ONEWIRE_NO1 == 1)
void OneWire1IOSetOutput(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = ONEWIRE_GPIO1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ONEWIRE_Type1, &GPIO_InitStruct);
}

void OneWire1IOSetInput(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = ONEWIRE_GPIO1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(ONEWIRE_Type1, &GPIO_InitStruct);
}
#endif


//第二个单线传输所需定义与接口
#if(ONEWIRE_NO2 == 1)

#endif
