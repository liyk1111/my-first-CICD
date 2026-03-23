/*
使用说明：
    1.

功能介绍：
    1.

更新记录：
    2023-11-20：TODO：需要下载到开发板重复验证
*/

#ifndef __IISX_H
#define __IISX_H
#include "stdint.h"
//设置SAIA的采样率(@MCKEN)
//samplerate:采样率,单位:Hz
//返回值:0,设置成功;1,无法设置.
uint8_t I2S2_SampleRate_Set(uint32_t samplerate);
//I2S开始播放
void I2S_Play_Start(void);
//关闭I2S播放
void I2S_Play_Stop(void);

uint8_t GetI2SState(void);

uint8_t I2S_Send_Data(uint16_t *data,uint16_t lenght);
uint8_t I2S2Init(void);
/*开启DMA双缓冲*/
//uint8_t HAL_I2S_Transmit_DMAEx(I2S_HandleTypeDef *hi2s, uint16_t *FirstBuffer, uint16_t *SecondBuffer, uint16_t Size);
#endif
