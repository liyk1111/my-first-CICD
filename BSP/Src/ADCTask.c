#include "usartx.h"
#include "adc.h"

/*float VREF;
uint16_t vrefvoltage;
float voltage;*/
/*void GetAdcValue()
{
	 uint16_t adcValue;
   float zhuanhuanhou;
   float angle;

   // 启动ADC转换
   HAL_ADC_Start(&hadc1);
   HAL_ADC_PollForConversion(&hadc1, 100);
   adcValue = HAL_ADC_GetValue(&hadc1);
   HAL_ADC_Stop(&hadc1);
   // 计算电压
   voltage = (adcValue * VREF) / 4095.0;
   // 计算角度（电位器为5kΩ线性电位器角度为320+-5）
		//angle = (voltage / VREF) * 320.0;
		zhuanhuanhou = Calculate_VREFINT(voltage,vrefvoltage);
   // 打印结果（假设使用串口）
   Usart1Printf("ADC Value: %d, Voltage: %.3fV, Angle: %.3f degrees\r\n", adcValue, zhuanhuanhou, angle);

   HAL_Delay(2000);  // 每秒读取一次
}*/

//获取ADC1_17通道的参考电压通过读取到的ADC值和VREFINT的实际值（通常为1.2V）来计算实际的参考电压
/*void GetAdcVref()
{
		uint16_t adcValue = 0;
		ADC_ChannelConfTypeDef sConfig = {0};
		// 配置ADC通道
    sConfig.Channel = ADC_CHANNEL_VREFINT;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        // 配置通道错误处理
        Error_Handler();
    }

    // Start ADC conversion
    if (HAL_ADC_Start(&hadc1) != HAL_OK) {
        // Start Error
        Error_Handler();
    }

    // Wait for conversion to complete
    if (HAL_ADC_PollForConversion(&hadc1, 100) != HAL_OK) {
        // Conversion Error
        Error_Handler();
    }
		
    // Read the converted value
    adcValue = HAL_ADC_GetValue(&hadc1);//获取ADC1_17adc未转换的值
		Usart1Printf("17通道是:%d\r\n",adcValue);
		VREF = Calculate_VREFINT(adcValue);
		Usart1Printf("VREF：%.2f",VREF);
		HAL_Delay(1000);
		//Usart1Printf("Voltage: %.3fV\r\n", vrefvoltage);//打印内部参考电压通过内部参考电压校准外部通道
    // Stop ADC conversion
    HAL_ADC_Stop(&hadc1);
}


void Get_adc10()
{
	uint16_t adcValue;
   float adcvalue1;
   float angle;
	float VREF1;
		ADC_ChannelConfTypeDef sConfig = {0};
		// 配置ADC通道
    sConfig.Channel = ADC_CHANNEL_10;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        // 配置通道错误处理
        Error_Handler();
    }
   // 启动ADC转换
   HAL_ADC_Start(&hadc1);
   HAL_ADC_PollForConversion(&hadc1, 100);
   adcValue = HAL_ADC_GetValue(&hadc1);//电位器读取到的原始数值
   HAL_ADC_Stop(&hadc1);
	VREF1 = VREF;
	adcvalue1 = (adcValue/4095.0)*VREF1;//转换电压
	angle = (adcvalue1 / VREF1) * 320.0;//角度转换
	Usart1Printf("ADC Value: %d, Voltage: %.3f, Angle: %.3f degrees\r\n", adcValue, adcvalue1, angle);
	
}

//计算Vref参照电压
float Calculate_VREFINT(uint16_t advalue) {
    float vrefint = 1.2f; // 参照电压为1.2v
    float vref = vrefint/(advalue/4096.0);//得出ref参考电压值
		Usart1Printf("得到的ref是:%.2f\r\n",vref);
    return vref;
}*/



