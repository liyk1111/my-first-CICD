//#include "iisx.h"
//#include "appPort.h"
//#include "bspPort.h"
////采样率计算公式:Fs=I2SxCLK/[256*(2*I2SDIV+ODD)]
////I2SxCLK=(HSE/pllm)*PLLI2SN/PLLI2SR
////一般HSE=8Mhz 
////pllm:在Sys_Clock_Set设置的时候确定，一般是8
////PLLI2SN:一般是192~432 
////PLLI2SR:2~7
////I2SDIV:2~255
////ODD:0/1
////I2S分频系数表@pllm=8,HSE=8Mhz,即vco输入频率为1Mhz
////表格式:采样率/10,PLLI2SN,PLLI2SR,I2SDIV,ODD
//const uint16_t I2S_PSC_TBL[][5]=
//{
//	{800 ,256,5,12,1},		//8Khz采样率
//	{1102,429,4,19,0},		//11.025Khz采样率 
//	{1600,213,2,13,0},		//16Khz采样率
//	{2205,429,4, 9,1},		//22.05Khz采样率
//	{3200,213,2, 6,1},		//32Khz采样率
//	{4410,271,2, 6,0},		//44.1Khz采样率
//	{4800,258,3, 3,1},		//48Khz采样率
//	{8820,316,2, 3,1},		//88.2Khz采样率
//	{9600,344,2, 3,1},  	//96Khz采样率
//	{17640,361,2,2,0},  	//176.4Khz采样率 
//	{19200,393,2,2,0},  	//192Khz采样率
//};

////开启I2S的DMA功能,HAL库没有提供此函数
////因此我们需要自己操作寄存器编写一个
////void I2S_DMA_Enable(void)
////{
////    uint32_t tempreg=0;
////    tempreg=SPI2->CR2;    	//先读出以前的设置			
////	tempreg|=1<<1;			//使能DMA
////	SPI2->CR2=tempreg;		//写入CR1寄存器中
////}

////设置SAIA的采样率(@MCKEN)
////samplerate:采样率,单位:Hz
////返回值:0,设置成功;1,无法设置.
//uint8_t I2S2_SampleRate_Set(uint32_t samplerate)
//{   
//    uint8_t i=0; 
//	uint32_t tempreg=0;
//    RCC_PeriphCLKInitTypeDef RCCI2S2_ClkInitSture;  
//	//printf("samplerate=%d\r\n",samplerate);
//	for(i=0;i<(sizeof(I2S_PSC_TBL)/10);i++)//看看改采样率是否可以支持
//	{
//		if((samplerate/10)==I2S_PSC_TBL[i][0])break;
//	}
//    if(i==(sizeof(I2S_PSC_TBL)/10))return 1;//搜遍了也找不到
//	
//    RCCI2S2_ClkInitSture.PeriphClockSelection=RCC_PERIPHCLK_I2S;	//外设时钟源选择 
//    RCCI2S2_ClkInitSture.PLLI2S.PLLI2SN=(uint32_t)I2S_PSC_TBL[i][1];    	//设置PLLI2SN
//    RCCI2S2_ClkInitSture.PLLI2S.PLLI2SR=(uint32_t)I2S_PSC_TBL[i][2];    	//设置PLLI2SR
//    HAL_RCCEx_PeriphCLKConfig(&RCCI2S2_ClkInitSture);             	//设置时钟
//	
//	RCC->CR|=1<<26;					//开启I2S时钟
//	while((RCC->CR&1<<27)==0);		//等待I2S时钟开启成功. 
//	tempreg=I2S_PSC_TBL[i][3]<<0;	//设置I2SDIV
//	tempreg|=I2S_PSC_TBL[i][4]<<8;	//设置ODD位
//	tempreg|=1<<9;					//使能MCKOE位,输出MCK
//	SPI2->I2SPR=tempreg;			//设置I2SPR寄存器 
//	
//	
//	return 0;
//} 

////I2S开始播放
//void I2S_Play_Start(void)
//{   	  
//	//__HAL_DMA_ENABLE(&hdma_spi2_tx);//开启DMA TX传输  			
//}
////关闭I2S播放
//void I2S_Play_Stop(void)
//{   
//	//__HAL_DMA_DISABLE(&hdma_spi2_tx);//结束播放;
// 	 
//} 
//extern uint8_t mp3transferend;
//void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
//{
//	//printf("HAL_I2S_TxCpltCallback\r\n");
//  /* Prevent unused argument(s) compilation warning */
//  mp3transferend = 1;

//  /* NOTE : This function Should not be modified, when the callback is needed,
//            the HAL_I2S_TxCpltCallback could be implemented in the user file
//   */
//	
//}

//uint8_t I2S2Init(void)
//{
//	MX_I2S2_Init();
//	return YES;
//}
//uint8_t GetI2SState(void)
//{
//	return HAL_I2S_GetState(&hi2s2);
//}

//uint8_t I2S_Send_Data(uint16_t *data,uint16_t lenght)
//{
//	return HAL_I2S_Transmit_DMA(&hi2s2,data,lenght);
//}
