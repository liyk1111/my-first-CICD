
#include "flashx.h"
//读取指定地址的字(32位数据) 
//faddr:读地址 
//返回值:对应数据.
uint32_t STMFLASH_ReadWord(uint32_t faddr)
{
	return *(uint32_t*)faddr; 
}

//获取某个地址所在的flash扇区
//addr:flash地址
//返回值:0~11,即addr所在的扇区
uint8_t STMFLASH_GetFlashSector(uint32_t addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_SECTOR_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_SECTOR_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_SECTOR_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_SECTOR_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_SECTOR_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_SECTOR_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_SECTOR_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_SECTOR_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_SECTOR_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_SECTOR_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_SECTOR_10;
	else return FLASH_SECTOR_11;
//	else if(addr<ADDR_FLASH_SECTOR_12)return FLASH_SECTOR_11; 
//	else if(addr<ADDR_FLASH_SECTOR_13)return FLASH_SECTOR_12; 
//	else if(addr<ADDR_FLASH_SECTOR_14)return FLASH_SECTOR_13; 
//	else if(addr<ADDR_FLASH_SECTOR_15)return FLASH_SECTOR_14; 
//	else if(addr<ADDR_FLASH_SECTOR_16)return FLASH_SECTOR_15; 
//	else if(addr<ADDR_FLASH_SECTOR_17)return FLASH_SECTOR_16; 
//	else if(addr<ADDR_FLASH_SECTOR_18)return FLASH_SECTOR_17; 
//	else if(addr<ADDR_FLASH_SECTOR_19)return FLASH_SECTOR_18; 
//	else if(addr<ADDR_FLASH_SECTOR_20)return FLASH_SECTOR_19; 
//	else if(addr<ADDR_FLASH_SECTOR_21)return FLASH_SECTOR_20; 
//	else if(addr<ADDR_FLASH_SECTOR_22)return FLASH_SECTOR_21; 
//	else if(addr<ADDR_FLASH_SECTOR_23)return FLASH_SECTOR_22;  
//	return FLASH_SECTOR_23;	
//}
}

//void FlashWrite(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite)	
//{ 
//	FLASH_EraseInitTypeDef FlashEraseInit;
//	HAL_StatusTypeDef FlashStatus=HAL_OK;
//	uint32_t SectorError=0;
//	uint32_t addrx=0;
//	uint32_t endaddr=0;	
//	if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//非法地址
//    
//	HAL_FLASH_Unlock();             //解锁	
//	addrx=WriteAddr;				//写入的起始地址
//	endaddr=WriteAddr+NumToWrite*4;	//写入的结束地址
// 
//	if(addrx<0X1FFF0000)
//	{
//		while(addrx<endaddr)		//扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
//		{
//			 if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
//			{   
//				FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除 
//				FlashEraseInit.Sector=STMFLASH_GetFlashSector(addrx);   //要擦除的扇区
//				FlashEraseInit.NbSectors=1;                             //一次只擦除一个扇区
//				FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
//				if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK) 
//				{
//					break;//发生错误了	
//				}
//				}else addrx+=4;
//				FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成
//		}
//	}

//	FlashStatus=FLASH_WaitForLastOperation(FLASH_WAITETIME);            //等待上次操作完成
//	if(FlashStatus==HAL_OK)
//	{
//		 while(WriteAddr<endaddr)//写数据
//		 {
//			if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,WriteAddr,*pBuffer)!=HAL_OK)//写入数据
//			{ 
//				break;	//写入异常
//			}
//			WriteAddr+=4;
//			pBuffer++;
//		}  
//	}

//	HAL_FLASH_Lock();           //上锁
//	
//} 

/*写入地址必须4的倍数*/
uint8_t FlashWrite(uint32_t WriteAddr,uint8_t *pBuffer,uint32_t NumToWrite)	
{ 
	FLASH_EraseInitTypeDef FlashEraseInit;
	HAL_StatusTypeDef FlashStatus=HAL_OK;
	uint32_t SectorError=0;
	uint32_t addrx=0;
	uint32_t endaddr=0;
	if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)
	{
		//printf("flash err 0:0x%x\r\n",WriteAddr);
		return HAL_ERROR;	//非法地址
	}	
	addrx=WriteAddr;				//写入的起始地址
	endaddr=WriteAddr+NumToWrite;	//写入的结束地址
    
	if(WriteAddr>=STM32_FLASH_BASE&&endaddr<=ADDR_STM32_FLASH_END)
	{
		
		HAL_FLASH_Unlock();             //解锁
		FLASH_WaitForLastOperation(FLASH_WAITETIME);  		
		while(addrx<endaddr)		//扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
		{
			 if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
			{   
				FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除 
				FlashEraseInit.Sector=STMFLASH_GetFlashSector(addrx);   //要擦除的扇区
				FlashEraseInit.NbSectors=1;                             //一次只擦除一个扇区
				FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
				FlashEraseInit.Banks = FLASH_BANK_1;						
				if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK) 
				{	
					//printf("flash err 1:%d,%d\r\n",SectorError,FlashEraseInit.Sector);	
					HAL_FLASH_Lock();           //上锁						
					return HAL_ERROR;
					//break;//发生错误了	
				}
			}else addrx+=4;
			FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成
		}
		FlashStatus=FLASH_WaitForLastOperation(FLASH_WAITETIME);            //等待上次操作完成
		if(FlashStatus==HAL_OK)
		{
			 while(WriteAddr<endaddr)//写数据
			 {
				if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,WriteAddr,*(uint32_t*)pBuffer)!=HAL_OK)//写入数据
				{ 
					//printf("flash err 2\r\n");
					HAL_FLASH_Lock();           //上锁
					return HAL_ERROR;	//写入异常
				}
				WriteAddr+=4;
				pBuffer+=4;
			}  
		}
		HAL_FLASH_Lock();           //上锁		
	}
	else
	{
		//printf("flash err 3\r\n");
		return HAL_ERROR; 
	}		
	return HAL_OK;
} 

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToRead:字(32位)数
void FlashRead(uint32_t ReadAddr,uint8_t *pBuffer,uint32_t NumToRead)   	
{
	uint32_t ReadWordLen = NumToRead>>2;
	uint32_t ReadByteLen = NumToRead%4;
	uint32_t ii = 0;
	uint32_t *data;


	data = (uint32_t*)pBuffer;

	for(ii = 0;ii<ReadWordLen;ii++)
	{
		data[ii] = *(uint32_t*)ReadAddr;
		ReadAddr = ReadAddr + 4;
	}

	if(ReadByteLen!=0)
	{
		uint8_t *p,*q;
		p = &pBuffer[ReadWordLen*4];
		q = (uint8_t*)ReadAddr;
		for(ii = 0;ii<ReadByteLen;ii++)
		{
			p[ii] = q[ii];
		}
	}
}

//////////////////////////////////////////测试用///////////////////////////////////////////

