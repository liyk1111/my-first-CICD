/*
使用说明：
    1.少用，暂时不处理

功能介绍：
    1.

更新记录：
    2023-11-20：
*/

#ifndef __IICX_H
#define __IICX_H
#include "stdint.h"


uint8_t I2C2Init(void);
/*IIC1初始化*/
uint8_t I2C1Init(void);
/*IIC1发送*/
uint8_t I2C1_Send_Data(uint16_t DevAddress,uint8_t *data,uint16_t len);

uint8_t I2C2Init(void);
/*IIC1接收*/
uint8_t IIC2_Read_Data(uint8_t addr,uint8_t reg,uint8_t *data,uint8_t dataLenght);
#endif
