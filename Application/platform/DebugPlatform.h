#ifndef __DEBUGPLATFORM_H
#define __DEBUGPLATFORM_H
#include "stdint.h"

#define Debug_Rx_IRQHandler			Usart1_Rx_Finish_IRQHandler	

/*获取接收长度*/
uint32_t GetDebugRxLenght(void);

/*清空接收长度*/
void ClearDebugRxLenght(void);

uint8_t *GetDebugRxData(void);
/*获取接收的数据*/
uint32_t GetDebugRxBuff(uint8_t *data);

/*Debug发送数据*/
uint8_t DebugSend(uint8_t *data, uint16_t len);
/*初始化驱动*/
void DebugDriverInit(void);
void DebugDriverDeInit(void);
#endif

