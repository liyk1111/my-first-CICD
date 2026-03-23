#include "usartx.h"
#include "string.h"
#include "stdarg.h"


//公共全局
uint32_t Usart1Baud = DEFAULT_BAUD;
uint32_t Usart2Baud = DEFAULT_BAUD;
uint32_t Usart3Baud = DEFAULT_BAUD;
uint32_t Usart4Baud = DEFAULT_BAUD;
uint32_t Usart5Baud = DEFAULT_BAUD;
uint32_t Usart6Baud = DEFAULT_BAUD;

//==================================各个独立函数================================================
#if ENLPUSART1 == 1//低功耗串口1 - 未处理
	uint8_t LPUsart1_Rx_Buff[LPUsart1_Rx_Max_Size];
	uint16_t LPUsart1_Rx_Len = 0;
	uint8_t LPU1TempData[LPUsart1_Rx_Max_Size];
	//初始化
	void LPUsart1Init(void)
	{				
		MX_LPUART1_UART_Init();
		//开启接收
		while(HAL_UARTEx_ReceiveToIdle_IT(&hlpuart1, LPU1TempData, LPUsart1_Rx_Max_Size)!=HAL_OK);
	}
	//释放初始化
	void LPUsart1DeInit(void)
	{
		HAL_UART_DeInit(&hlpuart1);
	}
	//开启串口接收
	uint32_t LPUsart1ReceiveStart(void)
	{
		return HAL_UARTEx_ReceiveToIdle_IT(&hlpuart1, LPU1TempData, LPUsart1_Rx_Max_Size);
	}
	//获取串口状态
	uint32_t LPUsart1GetState(void)
	{
		return HAL_UART_GetState(&hlpuart1);
	}
	//低功耗串口1发送
	uint8_t LPUsart1_Send_Data(uint8_t *data, uint16_t len)
	{
		HAL_StatusTypeDef Result;
		Result = HAL_UART_Transmit(&hlpuart1, data, len, 5000);
		if( Result == HAL_TIMEOUT)								//如果发送超时，就需要重新开启接收，HAL库就是这样的
		{
			LPUsart1ReceiveStart();
		}
		return Result;
	}
	//低功耗串口1打印
	uint8_t LPUsart1Printf(char* fmt, ...)
	{
		uint8_t TempBuff[100];
		uint8_t length;
		va_list ap;
		va_start(ap, fmt);
		vsprintf((char *)TempBuff, fmt, ap);
		va_end(ap);
		length = strlen((const char *)TempBuff);
		return LPUsart1_Send_Data( TempBuff, length);
	}
	__weak void LPUsart1_Rx_Finish_IRQHandler(void)
	{

	}
#endif

#if ENUSART1 == 1		//串口1
	uint8_t Usart1_Rx_Buff[Usart1_Rx_Buff_Len];
	uint16_t Usart1_Rx_Len = 0;
	uint8_t U1TempData[Usart1_Rx_Buff_Len];
	uint8_t usart1ing = 0;
	
//串口1发送 - 这个不能统一
uint8_t Usart1_Send_Data(uint8_t *data, uint16_t len)
{
	HAL_StatusTypeDef Result;
	
	while(usart1ing == 1)
	{
		//USARTDELAY(1);
	}	
	
	usart1ing = 1;
	Result = HAL_UART_Transmit(&huart1, data, len, 5000);
	if( Result == HAL_TIMEOUT)								//如果发送超时，就需要重新开启接收，HAL库就是这样的
	{
		UsartReceiveStart(ADUart1);
	}
	
	usart1ing = 0;
	return Result;
}

//串口1打印 - 这个不能统一
uint8_t Usart1Printf(char* fmt, ...)
{
	uint8_t TempBuff[100];
	uint8_t length;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char *)TempBuff, fmt, ap);
	va_end(ap);
	length = strlen((const char *)TempBuff);
	return Usart1_Send_Data( TempBuff, length);
}
	
//接收中断
__weak void Usart1_Rx_Finish_IRQHandler(void)
{

}
#endif

#if ENUSART2 == 1//串口2
	uint8_t Usart2_Rx_Buff[Usart2_Rx_Buff_Len];
	uint16_t Usart2_Rx_Len = 0;
	uint8_t U2TempData[Usart2_Rx_Buff_Len];
	
	//串口2发送
	uint8_t Usart2_Send_Data(uint8_t *data, uint16_t len)
	{
		HAL_StatusTypeDef Result;
		Result = HAL_UART_Transmit(&huart2, data, len, 5000);
		if( Result == HAL_TIMEOUT)								//如果发送超时，就需要重新开启接收，HAL库就是这样的
		{
			UsartReceiveStart(ADUart2);
		}
		return Result;
	}
	//串口2打印
	uint8_t Usart2Printf(char* fmt, ...)
	{
		uint8_t TempBuff[100];
		uint8_t length;
		va_list ap;
		va_start(ap, fmt);
		vsprintf((char *)TempBuff, fmt, ap);
		va_end(ap);
		length = strlen((const char *)TempBuff);
		return Usart2_Send_Data( TempBuff, length);
	}
	
	__weak void Usart2_Rx_Finish_IRQHandler(void)
	{

	}
#endif

#if ENUSART3 == 1//串口3
	uint8_t Usart3_Rx_Buff[Usart3_Rx_Buff_Len];
	uint16_t Usart3_Rx_Len = 0;
	uint8_t U3TempData[Usart3_Rx_Buff_Len];
	
	//串口3发送
	uint8_t Usart3_Send_Data(uint8_t *data, uint16_t len)
	{
		HAL_StatusTypeDef Result;
		Result = HAL_UART_Transmit(&huart3, data, len, 5000);
		if( Result == HAL_TIMEOUT)								//如果发送超时，就需要重新开启接收，HAL库就是这样的
		{
			UsartReceiveStart(ADUart3);
		}
		return Result;
	}
	//串口3打印
	uint8_t Usart3Printf(char* fmt, ...)
	{
		uint8_t TempBuff[100];
		uint8_t length;
		va_list ap;
		va_start(ap, fmt);
		vsprintf((char *)TempBuff, fmt, ap);
		va_end(ap);
		length = strlen((const char *)TempBuff);
		 return Usart3_Send_Data( TempBuff, length);
	}
	
	__weak void Usart3_Rx_Finish_IRQHandler(void)
	{

	}
#endif

#if ENUSART4 == 1//串口
	uint8_t Usart4_Rx_Buff[Usart4_Rx_Buff_Len];
	uint16_t Usart4_Rx_Len = 0;
	uint8_t U4TempData[Usart4_Rx_Buff_Len];
	
	//串口发送
	uint8_t Usart4_Send_Data(uint8_t *data, uint16_t len)
	{
		HAL_StatusTypeDef Result;
		Result = HAL_UART_Transmit(&huart4, data, len, 5000);
		if( Result == HAL_TIMEOUT)								//如果发送超时，就需要重新开启接收，HAL库就是这样的
		{
			UsartReceiveStart(ADUart4);
		}
		return Result;
	}
	//串口打印
	uint8_t Usart4Printf(char* fmt, ...)
	{
		uint8_t TempBuff[100];
		uint8_t length;
		va_list ap;
		va_start(ap, fmt);
		vsprintf((char *)TempBuff, fmt, ap);
		va_end(ap);
		length = strlen((const char *)TempBuff);
		 return Usart4_Send_Data( TempBuff, length);
	}
	__weak void Usart4_Rx_Finish_IRQHandler(void)
	{

	}
#endif


#if ENUSART5 == 1//串口5
	uint8_t Usart5_Rx_Buff[Usart5_Rx_Buff_Len];
	uint16_t Usart5_Rx_Len = 0;
	uint8_t U5TempData[Usart5_Rx_Buff_Len];
	
	//串口发送
	uint8_t Usart5_Send_Data(uint8_t *data, uint16_t len)
	{
		HAL_StatusTypeDef Result;
		Result = HAL_UART_Transmit(&huart5, data, len, 5000);
		if( Result == HAL_TIMEOUT)								//如果发送超时，就需要重新开启接收，HAL库就是这样的
		{
			UsartReceiveStart(ADUart5);
		}
		return Result;
	}
	//串口打印
	uint8_t Usart5Printf(char* fmt, ...)
	{
		uint8_t TempBuff[100];
		uint8_t length;
		va_list ap;
		va_start(ap, fmt);
		vsprintf((char *)TempBuff, fmt, ap);
		va_end(ap);
		length = strlen((const char *)TempBuff);
		 return Usart5_Send_Data( TempBuff, length);
	}
	__weak void Usart5_Rx_Finish_IRQHandler(void)
	{

	}
#endif

#if ENUSART6 == 1//串口6
	uint8_t Usart6_Rx_Buff[Usart6_Rx_Buff_Len];
	uint16_t Usart6_Rx_Len = 0;
	uint8_t U6TempData[Usart6_Rx_Buff_Len];
	
	//串口发送
	uint8_t Usart6_Send_Data(uint8_t *data, uint16_t len)
	{
		HAL_StatusTypeDef Result;
		Result = HAL_UART_Transmit(&huart6, data, len, 5000);
		if( Result == HAL_TIMEOUT)								//如果发送超时，就需要重新开启接收，HAL库就是这样的
		{
			UsartReceiveStart(ADUart6);
		}
		return Result;
	}
	//串口打印
	uint8_t Usart6Printf(char* fmt, ...)
	{
		uint8_t TempBuff[100];
		uint8_t length;
		va_list ap;
		va_start(ap, fmt);
		vsprintf((char *)TempBuff, fmt, ap);
		va_end(ap);
		length = strlen((const char *)TempBuff);
		 return Usart6_Send_Data( TempBuff, length);
	}
	__weak void Usart6_Rx_Finish_IRQHandler(void)
	{

	}
#endif

//=====================================中断处理函数=================================================
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	
#if ENLPUSART1 == 1	
	if(huart == &hlpuart1)
	{
		//避免溢出
		if(LPUsart1_Rx_Max_Size<LPUsart1_Rx_Len+Size)
		Size = LPUsart1_Rx_Max_Size - LPUsart1_Rx_Len;

		//复制数据
		memcpy(&LPUsart1_Rx_Buff[LPUsart1_Rx_Len],LPU1TempData,Size);

		//更新接收长度
		LPUsart1_Rx_Len = LPUsart1_Rx_Len+Size;

		//重新接收
		if(HAL_UARTEx_ReceiveToIdle_IT(&hlpuart1, LPU1TempData, LPUsart1_Rx_Max_Size)!=HAL_OK)
		{
			HAL_UART_AbortReceive_IT(&hlpuart1);
			if(HAL_UARTEx_ReceiveToIdle_IT(&hlpuart1, LPU1TempData, LPUsart1_Rx_Max_Size)!=HAL_OK)
			{
				LPUsart1DeInit();			
				LPUsart1Init();	
			}
					
		};

		//调用接收完成中断
		LPUsart1_Rx_Finish_IRQHandler();
		
		//检查串口是否正常
		if(HAL_UART_GetState(&hlpuart1)!=HAL_UART_STATE_BUSY_RX)
		{
			LPUsart1DeInit();			
			LPUsart1Init();					
		};
		return ;
	}
	 
#endif
#if ENUSART1 == 1
	 if(huart == &huart1)
	{
		
		//避免溢出
		if(Usart1_Rx_Buff_Len < Usart1_Rx_Len + Size)
			Size = Usart1_Rx_Buff_Len - Usart1_Rx_Len;

		//复制数据
		memcpy(&Usart1_Rx_Buff[Usart1_Rx_Len], U1TempData, Size);

		//更新接收长度
		Usart1_Rx_Len = Usart1_Rx_Len + Size;
		
		//重新接收		  
		if(HAL_UARTEx_ReceiveToIdle_IT(&huart1, U1TempData, Usart1_Rx_Buff_Len) != HAL_OK)
		{	
			HAL_UART_AbortReceive_IT(&huart1);
			if(HAL_UARTEx_ReceiveToIdle_IT(&huart1, U1TempData, Usart1_Rx_Buff_Len) != HAL_OK)
			{				
				UsartDeInit(ADUart1);
				UsartInit(ADUart1, Usart1Baud);				
			}						
		}
		
		//调用接收完成中断
		Usart1_Rx_Finish_IRQHandler();
	
		
		//检查串口是否正常
		if(HAL_UART_GetState(&huart1) != HAL_UART_STATE_BUSY_RX)
		{
			UsartDeInit(ADUart1);								
			UsartInit(ADUart1, Usart1Baud);
		}
		
		return;
	}
	
#endif

#if ENUSART2 == 1
	 if(huart == &huart2)
	{		
		//避免溢出
		if(Usart2_Rx_Buff_Len < Usart2_Rx_Len + Size)
			Size = Usart2_Rx_Buff_Len - Usart2_Rx_Len;

		//复制数据
		memcpy(&Usart2_Rx_Buff[Usart2_Rx_Len], U2TempData, Size);

		//更新接收长度
		Usart2_Rx_Len = Usart2_Rx_Len + Size;

		//重新接收
		if(HAL_UARTEx_ReceiveToIdle_IT(&huart2, U2TempData, Usart2_Rx_Buff_Len) != HAL_OK)
		{
			HAL_UART_AbortReceive_IT(&huart2);
			if(HAL_UARTEx_ReceiveToIdle_IT(&huart2, U2TempData, Usart2_Rx_Buff_Len) != HAL_OK)
			{
				UsartDeInit(ADUart2);								
				UsartInit(ADUart2, Usart2Baud);
			}			
		}
		//调用接收完成中断
		Usart2_Rx_Finish_IRQHandler();		
		
		//检查串口是否正常
		if(HAL_UART_GetState(&huart2) != HAL_UART_STATE_BUSY_RX)
		{
			UsartDeInit(ADUart2);								
			UsartInit(ADUart2, Usart2Baud);			
		}
		return ;
	}
	
#endif	

#if ENUSART3 == 1
	 if(huart == &huart3)
	{
		//避免溢出
		if(Usart3_Rx_Buff_Len < Usart3_Rx_Len + Size)
			Size = Usart3_Rx_Buff_Len - Usart3_Rx_Len;

		//复制数据
		memcpy(&Usart3_Rx_Buff[Usart3_Rx_Len], U3TempData, Size);

		//更新接收长度
		Usart3_Rx_Len = Usart3_Rx_Len + Size;

		//重新接收
		if(HAL_UARTEx_ReceiveToIdle_IT(&huart3, U3TempData, Usart3_Rx_Buff_Len) != HAL_OK)
		{	
			HAL_UART_AbortReceive_IT(&huart3);
			if(HAL_UARTEx_ReceiveToIdle_IT(&huart3, U3TempData, Usart3_Rx_Buff_Len) != HAL_OK)
			{
				UsartDeInit(ADUart3);								
				UsartInit(ADUart3, Usart3Baud);
			}					
		}
		
		//调用接收完成中断
		Usart3_Rx_Finish_IRQHandler();
		
		//检查串口是否正常
		if(HAL_UART_GetState(&huart3) != HAL_UART_STATE_BUSY_RX)
		{
			UsartDeInit(ADUart3);								
			UsartInit(ADUart3, Usart3Baud);		
		}
		return ;
	}
#endif

#if ENUSART4 == 1
	 if(huart == &huart4)
	{
		//避免溢出
		if(Usart4_Rx_Buff_Len < Usart4_Rx_Len + Size)
			Size = Usart4_Rx_Buff_Len - Usart4_Rx_Len;

		//复制数据
		memcpy(&Usart4_Rx_Buff[Usart4_Rx_Len], U4TempData, Size);

		//更新接收长度
		Usart4_Rx_Len = Usart4_Rx_Len + Size;

		//重新接收
		if(HAL_UARTEx_ReceiveToIdle_IT(&huart4, U4TempData, Usart4_Rx_Buff_Len) != HAL_OK)
		{	
			HAL_UART_AbortReceive_IT(&huart4);
			if(HAL_UARTEx_ReceiveToIdle_IT(&huart4, U4TempData, Usart4_Rx_Buff_Len) != HAL_OK)
			{
				UsartDeInit(ADUart4);								
				UsartInit(ADUart4, Usart4Baud);
			}					
		}
		
		//调用接收完成中断
		Usart4_Rx_Finish_IRQHandler();
		
		//检查串口是否正常
		if(HAL_UART_GetState(&huart4) != HAL_UART_STATE_BUSY_RX)
		{
			UsartDeInit(ADUart4);								
			UsartInit(ADUart4, Usart4Baud);		
		}
		return ;
	}
#endif


#if ENUSART5 == 1
	 if(huart == &huart5)
	{
		//避免溢出
		if(Usart5_Rx_Buff_Len < Usart5_Rx_Len + Size)
			Size = Usart5_Rx_Buff_Len - Usart5_Rx_Len;

		//复制数据
		memcpy(&Usart5_Rx_Buff[Usart5_Rx_Len], U5TempData, Size);

		//更新接收长度
		Usart5_Rx_Len = Usart5_Rx_Len + Size;

		//重新接收
		if(HAL_UARTEx_ReceiveToIdle_IT(&huart5, U5TempData, Usart5_Rx_Buff_Len) != HAL_OK)
		{	
			HAL_UART_AbortReceive_IT(&huart5);
			if(HAL_UARTEx_ReceiveToIdle_IT(&huart5, U5TempData, Usart5_Rx_Buff_Len) != HAL_OK)
			{
				UsartDeInit(ADUart5);								
				UsartInit(ADUart5, Usart5Baud);
			}					
		}
		
		//调用接收完成中断
		Usart5_Rx_Finish_IRQHandler();
		
		//检查串口是否正常
		if(HAL_UART_GetState(&huart5) != HAL_UART_STATE_BUSY_RX)
		{
			UsartDeInit(ADUart5);								
			UsartInit(ADUart5, Usart5Baud);	
		}
		return ;
	}
#endif
	
#if ENUSART6 == 1
	 if(huart == &huart6)
	{
		//避免溢出
		if(Usart6_Rx_Buff_Len < Usart6_Rx_Len + Size)
			Size = Usart6_Rx_Buff_Len - Usart6_Rx_Len;

		//复制数据
		memcpy(&Usart6_Rx_Buff[Usart6_Rx_Len], U6TempData, Size);

		//更新接收长度
		Usart6_Rx_Len = Usart6_Rx_Len + Size;

		//重新接收
		if(HAL_UARTEx_ReceiveToIdle_IT(&huart6, U6TempData, Usart6_Rx_Buff_Len) != HAL_OK)
		{	
			HAL_UART_AbortReceive_IT(&huart6);
			if(HAL_UARTEx_ReceiveToIdle_IT(&huart6, U6TempData, Usart6_Rx_Buff_Len) != HAL_OK)
			{
				UsartDeInit(ADUart6);								
				UsartInit(ADUart6, Usart6Baud);
			}					
		}
		
		//调用接收完成中断
		Usart6_Rx_Finish_IRQHandler();
		
		//检查串口是否正常
		if(HAL_UART_GetState(&huart6)!=HAL_UART_STATE_BUSY_RX)
		{
			UsartDeInit(ADUart6);								
			UsartInit(ADUart6, Usart6Baud);	
		}
		return ;
	}
#endif
}

//=====================================公共函数=================================================
//获取对应串口设备指针
static void* GetUsartHandle(ADUartIndex UsartIndex)
{
	if(UsartIndex == ADUart1)
	{
#if ENUSART1 == 1
		return &huart1;
#endif
	}
	else if(UsartIndex == ADUart2)
	{
#if ENUSART2 == 1
		return &huart2;
#endif
	}
	else if(UsartIndex == ADUart3)
	{
#if ENUSART3 == 1
		return &huart3;
#endif
	}
	else if(UsartIndex == ADUart4)
	{
#if ENUSART4 == 1
		return &huart4;
#endif
	}
	else if(UsartIndex == ADUart5)
	{
#if ENUSART5 == 1
		return &huart5;
#endif
	}
	else if(UsartIndex == ADUart6)
	{
#if ENUSART6 == 1
		return &huart6;
#endif
	}
	
	return NULL;
}	

static void MX_USART_UART_Init_SetBaud(UART_HandleTypeDef *tmphuart, ADUartIndex UsartIndex, uint32_t tmpBaud)
{
	if(UsartIndex == ADUart1)
	{
		tmphuart->Instance = USART1;
		Usart1Baud = tmpBaud;
	}
	else if(UsartIndex == ADUart2)
	{
		tmphuart->Instance = USART2;
		Usart2Baud = tmpBaud;
	}
	else if(UsartIndex == ADUart3)
	{
		tmphuart->Instance = USART3;
		Usart3Baud = tmpBaud;
	}
	else if(UsartIndex == ADUart4)
	{
		tmphuart->Instance = UART4;
		Usart4Baud = tmpBaud;
	}
	else if(UsartIndex == ADUart5)
	{
		tmphuart->Instance = UART5;
		Usart5Baud = tmpBaud;
	}
//	else if(UsartIndex == ADUart6)
//	{
//		tmphuart->Instance = USART6;
//		Usart6Baud = tmpBaud;
//	}
//	
	tmphuart->Init.BaudRate = tmpBaud;
	tmphuart->Init.WordLength = UART_WORDLENGTH_8B;
	tmphuart->Init.StopBits = UART_STOPBITS_1;
	tmphuart->Init.Parity = UART_PARITY_NONE;
	tmphuart->Init.Mode = UART_MODE_TX_RX;
	tmphuart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	tmphuart->Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(tmphuart) != HAL_OK)
	{
		Error_Handler();
	}
}

//初始化
uint8_t UsartInit(ADUartIndex UsartIndex, uint32_t tmpBaud)
{
	UART_HandleTypeDef *tmpUsart = NULL;
	tmpUsart = GetUsartHandle(UsartIndex);
	if(tmpUsart == NULL)
		return HAL_ERROR;
	
	MX_USART_UART_Init_SetBaud(tmpUsart, UsartIndex, tmpBaud);
	
	if(UsartIndex == ADUart1)
	{
	#if ENUSART1 == 1
		//重新接收
		while(HAL_UARTEx_ReceiveToIdle_IT(&huart1, U1TempData, Usart1_Rx_Buff_Len) != HAL_OK)	;
	#endif
	}
	else if(UsartIndex == ADUart2)
	{
	#if ENUSART2 == 1
		//重新接收
		while(HAL_UARTEx_ReceiveToIdle_IT(&huart2, U2TempData, Usart2_Rx_Buff_Len) != HAL_OK)	;
	#endif
	}
	else if(UsartIndex == ADUart3)
	{
	#if ENUSART3 == 1
		//重新接收
		while(HAL_UARTEx_ReceiveToIdle_IT(&huart3, U3TempData, Usart3_Rx_Buff_Len) != HAL_OK)	;
	#endif
	}
	else if(UsartIndex == ADUart4)
	{
	#if ENUSART4 == 1
		//重新接收
		while(HAL_UARTEx_ReceiveToIdle_IT(&huart4, U4TempData, Usart4_Rx_Buff_Len) != HAL_OK)	;
	#endif
	}
	else if(UsartIndex == ADUart5)
	{
	#if ENUSART5 == 1
		//重新接收
		while(HAL_UARTEx_ReceiveToIdle_IT(&huart5, U5TempData, Usart5_Rx_Buff_Len) != HAL_OK)	;
	#endif
	}
	else if(UsartIndex == ADUart6)
	{
	#if ENUSART6 == 1
		//重新接收
		while(HAL_UARTEx_ReceiveToIdle_IT(&huart6, U6TempData, Usart6_Rx_Buff_Len) != HAL_OK)	;
	#endif
	}	
	
	return HAL_OK;
}

//释放初始化
uint8_t UsartDeInit(ADUartIndex UsartIndex)
{
	UART_HandleTypeDef *tmpUsart = NULL;
	tmpUsart = GetUsartHandle(UsartIndex);
	if(tmpUsart == NULL)
		return HAL_ERROR;

	HAL_UART_DeInit(tmpUsart);
}

//开启串口接收
uint32_t UsartReceiveStart(ADUartIndex UsartIndex)
{
	if(UsartIndex == ADUart1)
	{
#if ENUSART1 == 1
		return HAL_UARTEx_ReceiveToIdle_IT(&huart1, U1TempData, Usart1_Rx_Buff_Len);
#endif
	}
	else if(UsartIndex == ADUart2)
	{
#if ENUSART2 == 1
		return HAL_UARTEx_ReceiveToIdle_IT(&huart2, U2TempData, Usart2_Rx_Buff_Len);
#endif
	}
	else if(UsartIndex == ADUart3)
	{
#if ENUSART3 == 1
		return HAL_UARTEx_ReceiveToIdle_IT(&huart3, U3TempData, Usart3_Rx_Buff_Len);
#endif
	}
	else if(UsartIndex == ADUart4)
	{
#if ENUSART4 == 1
		return HAL_UARTEx_ReceiveToIdle_IT(&huart4, U4TempData, Usart4_Rx_Buff_Len);
#endif
	}
	else if(UsartIndex == ADUart5)
	{
#if ENUSART5 == 1
		return HAL_UARTEx_ReceiveToIdle_IT(&huart5, U5TempData, Usart5_Rx_Buff_Len);
#endif
	}
	else if(UsartIndex == ADUart6)
	{
#if ENUSART6 == 1
		return HAL_UARTEx_ReceiveToIdle_IT(&huart6, U6TempData, Usart6_Rx_Buff_Len);
#endif
	}
}

//获取串口状态
uint32_t UsartGetState(ADUartIndex UsartIndex)
{
	UART_HandleTypeDef *tmpUsart = NULL;
	tmpUsart = GetUsartHandle(UsartIndex);
	if(tmpUsart == NULL)
		return HAL_ERROR;
	
	return HAL_UART_GetState(tmpUsart);
}

//清空接收长度
void UsartClearRxLenght(ADUartIndex UsartIndex)
{
	if(UsartIndex == ADUart1)
	{
#if ENUSART1 == 1
		Usart1_Rx_Len = 0;
#endif
	}
	else if(UsartIndex == ADUart2)
	{
#if ENUSART2 == 1
		Usart2_Rx_Len = 0;
#endif
	}
	else if(UsartIndex == ADUart3)
	{
#if ENUSART3 == 1
		Usart3_Rx_Len = 0;
#endif
	}
	else if(UsartIndex == ADUart4)
	{
#if ENUSART4 == 1
		Usart4_Rx_Len = 0;
#endif
	}
	else if(UsartIndex == ADUart5)
	{
#if ENUSART5 == 1
		Usart5_Rx_Len = 0;
#endif
	}
	else if(UsartIndex == ADUart6)
	{
#if ENUSART6 == 1
		Usart6_Rx_Len = 0;
#endif
	}
}
