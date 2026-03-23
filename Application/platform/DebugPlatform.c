#include "DebugPlatform.h"
#include "ADBSPPublicHF.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"


#define __Debug_Rx_Cnt 				Usart1_Rx_Len				/*Debug接收长度*/
#define __Debug_Rx_Buff 			Usart1_Rx_Buff				/*Debug接收的数据*/
#define __Debug_Send_Data 			Usart1_Send_Data			/*Debug发送数据*/

/*获取接收长度*/
uint32_t GetDebugRxLenght(void)
{
	return __Debug_Rx_Cnt;
}
/*清空接收长度*/
void ClearDebugRxLenght(void)
{
	__Debug_Rx_Cnt = 0;
}
uint8_t *GetDebugRxData(void)
{
	return __Debug_Rx_Buff;
}
/*获取接收的数据*/
uint32_t GetDebugRxBuff(uint8_t *data)
{
	memcpy(data,__Debug_Rx_Buff,__Debug_Rx_Cnt);
	return __Debug_Rx_Cnt;
}
/*Debug发送数据*/
uint8_t DebugSend(uint8_t *data, uint16_t len)
{
	return __Debug_Send_Data(data,len);
}

void DebugDriverInit(void)
{
	UsartInit(ADUart1, DEFAULT_BAUD);
}

void DebugDriverDeInit(void)
{
	UsartDeInit(ADUart1);
}
