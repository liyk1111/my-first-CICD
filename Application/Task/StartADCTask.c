#include "startadctask.h"
#include "usartx.h"
#include "retarget.h"
#include "dma.h"

uint16_t ADC_DMAValues[DMA_DATA_SIZE]; 			//ADC1 三个通道值在这
uint16_t ADC2_DMAValues[DMA_DATA_SIZE];    	   //ADC2 三个通道值在这
uint32_t ADCDMAValue[DMA_DATA_SIZE];	  	  //ADC6t通道所有数据在这
uint16_t adcchannel[5][DMA_DATA_SIZE];	  	 //6个通道分别放在这打印
uint8_t testcout = 0;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
//	testcout++;
	HAL_ADC_Stop(&hadc2);
	HAL_ADC_Stop_DMA(&hadc1);
//	HAL_ADCEx_MultiModeStop_DMA(&hadc1);
//	Usart1Printf("testcout:%d\r\n",testcout);
	osSemaphoreRelease(adc6semHandle);
}


void startadcTask(void const * argument)
{

//	int16_t index,channel,maxValue = -1000, minValue = 1000;
	//校准ADC
	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_ADCEx_Calibration_Start(&hadc2);
	
	osDelay(500);
	
	Usart1Printf("Start adc6  task:\r\n");
	
	HAL_ADC_Start(&hadc2);

	HAL_ADCEx_MultiModeStart_DMA(&hadc1,ADCDMAValue,DMA_DATA_SIZE); //多重DMA模式
	
	for(;;)
	{
		osSemaphoreWait(adc6semHandle, osWaitForever);
		testcout = 0;
//		for(index = 0,channel = 0;index < DMA_DATA_SIZE;index = index+3,channel++)
//		{
//			ADC_DMAValues[channel] = ADCDMAValue[index]&0xffff;
//			ADC_DMAValues[channel+DMA_DATA_SIZE/3] = ADCDMAValue[index+1]&0xffff;
//			ADC_DMAValues[channel+DMA_DATA_SIZE*2/3] = ADCDMAValue[index+2]&0xffff;
//			
//			ADC2_DMAValues[channel] = (ADCDMAValue[index]&0xffff0000) >> 16;
//			ADC2_DMAValues[channel+DMA_DATA_SIZE/3] = (ADCDMAValue[index+1]&0xffff0000) >> 16;
//			ADC2_DMAValues[channel+DMA_DATA_SIZE*2/3] = (ADCDMAValue[index+2]&0xffff0000) >> 16;
//			Usart1Printf("%d\r\n",ADC2_DMAValues[channel]);
//		}
		
		CaptureSixADC();
		
		osDelay(1000);
		osDelay(1000);
		osDelay(1000);
		osDelay(1000);
		osDelay(1000);
		
		repeatADC();
	}
}


void CaptureSixADC()
{
	uint16_t channel = 0;
	int16_t index,cout,maxValue = -1000, minValue = 1000;
	
	//1.分别保存6个通道数据
	for(index = 0,cout = 0; index < DMA_DATA_SIZE;index = index+3 , cout++)
	{
		adcchannel[channel][cout] 	=   ADCDMAValue[index]&0xffff; //ch10
		adcchannel[channel+1][cout] = 	ADCDMAValue[index+1]&0xffff; //ch11
		adcchannel[channel+2][cout] = 	ADCDMAValue[index+2]&0xffff; //ch12
		
		adcchannel[channel+3][cout] = 	(ADCDMAValue[index]&0xffff0000) >> 16; 	  //ch13
		adcchannel[channel+4][cout] = 	(ADCDMAValue[index+1]&0xffff0000) >> 16; //ch14
		adcchannel[channel+5][cout] = 	(ADCDMAValue[index+2]&0xffff0000) >> 16; //ch15
		
	}
	
	for(index = 0;index < 500;index++)
	{
		Usart1Printf("%d\r\n",adcchannel[1][index]);
	}
	
//	for(index = 0,channel = 0;index < DMA_DATA_SIZE;index = index+3,channel++)
//	{
//			ADC_DMAValues[channel] = ADC_DMAValues[index]&0xffff;
//			ADC_DMAValues[channel+DMA_DATA_SIZE/3] = ADCDMAValue[index+1]&0xffff;
//			ADC_DMAValues[channel+DMA_DATA_SIZE*2/3] = ADCDMAValue[index+2]&0xffff;
//			
//			ADC2_DMAValues[channel] = (ADCDMAValue[index]&0xffff0000) >> 16;
//			ADC2_DMAValues[channel+DMA_DATA_SIZE/3] = (ADCDMAValue[index+1]&0xffff0000) >> 16;
//			ADC2_DMAValues[channel+DMA_DATA_SIZE*2/3] = (ADCDMAValue[index+2]&0xffff0000) >> 16;
//	}
		
//		Usart1_Send_Data((uint8_t *)&ADC_DMAValues,ADC_SEND);
//		Usart1_Send_Data((uint8_t *)&ADC2_DMAValues,ADC_SEND);
}
void repeatADC()
{
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_ADC2_Init();
	HAL_ADC_Start(&hadc2);
	HAL_ADCEx_MultiModeStart_DMA(&hadc1,ADCDMAValue,DMA_DATA_SIZE); //多重DMA模式
}
