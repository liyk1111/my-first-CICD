#include "iicx.h"
#include "driverPort.h"
 #include "bspPort.h"

uint8_t I2C2Init(void)
{
	/*初始化I2C*/
	MX_I2C2_Init();
	return YES;
}
uint8_t I2C2DeInit(void)
{
	HAL_I2C_DeInit(&hi2c2);
	return YES;
}

/*IIC2发送*/
uint8_t I2C2_Send_Data(uint16_t DevAddress,uint8_t *data,uint16_t len)
{
	HAL_StatusTypeDef err;
	err = HAL_I2C_Master_Transmit(&hi2c2,DevAddress,data,len,20);
	
	if(err==HAL_OK)
		return YES;
	else 
		return NO;
	
}

//uint8_t I2C1Init(void)
//{
//	/*初始化I2C*/
//	MX_I2C1_Init();
//	return YES;
//}
///*IIC1发送*/
//uint8_t I2C1_Send_Data(uint16_t DevAddress,uint8_t *data,uint16_t len)
//{
//	HAL_StatusTypeDef err;
//	err = HAL_I2C_Master_Transmit(&hi2c1,DevAddress,data,len,20);
//	
//	if(err==HAL_OK)
//		return YES;
//	else 
//		return NO;
//	
//}


//uint8_t I2C2Init(void)
//{
//	MX_I2C2_Init();
//	return YES;
//}
///*IIC1接收*/
//uint8_t IIC2_Read_Data(uint8_t addr,uint8_t reg,uint8_t *data,uint8_t dataLenght)
//{
//	return HAL_I2C_Mem_Read(&hi2c2, addr<<1, reg, I2C_MEMADD_SIZE_8BIT,data , dataLenght, 10);
//}
