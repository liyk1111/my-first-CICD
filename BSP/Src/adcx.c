#include "adcx.h"
#include "stdio.h"

/*
2023-08-24:添加采集ADC后，关闭ADC。
2023-09-08:函数StartADC1Average采集到的ADC后部转换成电压
2023-09-13:修复StartADC1Average函数由于任务切换可能卡死的问题。
*/

uint8_t ADC1Init(void)
{
	MX_ADC1_Init();
	return 1;
}

uint8_t ADC1DeInit(void)
{
	HAL_ADC_DeInit(&hadc1);
	return 1;
}

uint8_t SampleIng = 0;
void StartADC1Sample(uint32_t xChannel,uint16_t* data,uint32_t len)
{
	  //uint32_t time0,time1=0;
	  ADC_ChannelConfTypeDef sConfig = {0};
	  /** Configure Regular Channel
	  */
	  while(SampleIng == 1)
	  {
			//osDelay(10);
			//printf("SampleIng=%d\r\n",SampleIng);
			HAL_Delay(1);
	  }
	  SampleIng = 1;
	  sConfig.Channel = xChannel;
	  sConfig.Rank = 1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
//	  sConfig.Offset = 0;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
		  printf("HAL_ADC_ConfigChannel Err\r\n");
	      Error_Handler();
	  }
	  
	  for(uint16_t ii = 0;ii<len;ii++)
	  {
		  HAL_ADC_Start(&hadc1);
		  HAL_ADC_PollForConversion(&hadc1, 100);
		  data[ii] = (uint16_t)HAL_ADC_GetValue(&hadc1);//
	  }
	  HAL_ADC_Stop(&hadc1);
	  SampleIng = 0;
}

uint8_t AverageSampleIng = 0;
uint16_t StartADC1Average(uint32_t xChannel, uint32_t times)
{
	uint32_t sum = 0;
	uint16_t data = 0;

	if(0 == times)
	return 0;
	
	while(AverageSampleIng == 1)
	HAL_Delay(1);
	
	ADC_ChannelConfTypeDef sConfig = {0};
	AverageSampleIng = 1;
	SampleIng = 1;
	sConfig.Channel = xChannel;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
//	sConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		printf("HAL_ADC_ConfigChannel Err\r\n");
		Error_Handler();
	}

	for(uint16_t i = 0;i < times;i++)
	{
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 100);
		sum = sum + (uint16_t)HAL_ADC_GetValue(&hadc1);//
	}
	HAL_ADC_Stop(&hadc1);
	data = sum / times;
	AverageSampleIng = 0;
	return data;
}

