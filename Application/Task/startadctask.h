#ifndef  _STARTADCTASK_H_
#define	 _STARTADCTASK_H_

#include "main.h"
#include "string.h"
#include "appPublic.h"

extern DMA_HandleTypeDef hdma_adc1;


//宏定义变量定义
#define DMA_DATA_SIZE	1500 										 //每个通道500个点
#define ADC_SEND		DMA_DATA_SIZE*2								//单字节发送乘2
#define	VREF_VOL		3300									   //单位为mV
#define ADCMAXValue		4095									  //最大转换值
#define PRE_POINTS 		1000									 //前1000个点
#define POST_POINTS 	1000									//后1000个点
	
	
//变量定义


//方法调用
extern void repeatADC(void);
extern void CaptureSixADC(void);


#endif
