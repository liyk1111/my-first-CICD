#include "DeviceStatusRespPackage.h"
#include "DeviceStatusPackage.h"
#include "DeviceSetRespPackage.h"
#include "DeviceSetPackage.h"
#include "ArrayUtil.h"
#include "ProcessesRegister.h"
#include "ProcessesManager.h"
#include "PackageRegister.h"
#include "PackageManager.h"
#include "Communicate.h"

DeviceStatusPackage G_DeviceStatusPackage;
DeviceSetPackage G_DeviceSetPackage;
uint8_t sendBuf[100];					//一次性定义最大,发送数组使用，协议包内不定义大数据量数组，小的无所谓
uint8_t dataSourceBuf[100];		//大数据量结构体数据转换到此处，直接拷贝到发送数组；若没有就无需传递

uint32_t DeviceStatusPackage_PACKAGEPARSER(uint8_t dest[])
{
	PackageProperty *pPackageProperty = NULL;				//该指针使用自加操作，不影响释放内存位置
	Package * resp = CreateDeviceStatusPackage();		//注意释放内存
	uint32_t offsetPos = PACKAGE_START_OFFSET;
	uint32_t i = 0, respCommand;
	
	//协议包内数组需要定义的变量
	int32_t valueItemCount;
	uint8_t *pValueBYTE = NULL;											//注意释放内存
	uint8_t *pValueBYTETemp = NULL;									//该指针使用自加操作，不影响释放内存位置

	printf("DeviceStatusPackage_PACKAGEPARSER\r\n");
	
	respCommand = resp->command;
	pPackageProperty = resp->properties;
	
	offsetPos = read_uint32(dest,pPackageProperty->value,offsetPos);
	G_DeviceStatusPackage.sequenceid = *(uint8_t*)pPackageProperty->value;
	pPackageProperty++;
	
	offsetPos = read_uint8(dest,pPackageProperty->value,offsetPos);
	G_DeviceStatusPackage.dbValue1 = *(uint8_t*)pPackageProperty->value;
	pPackageProperty++;
	
	offsetPos = read_uint16(dest,pPackageProperty->value,offsetPos);
	G_DeviceStatusPackage.dbValue2 = *(uint16_t*)pPackageProperty->value;
	pPackageProperty++;
	
	offsetPos = read_int32(dest, &valueItemCount, offsetPos);
	pPackageProperty->valueItemCount = valueItemCount;
	G_DeviceStatusPackage.dataLen = valueItemCount;
	pValueBYTE = (uint8_t *)malloc( sizeof(uint8_t)*valueItemCount );
	pValueBYTETemp = pValueBYTE;
	
	for (i = 0; i < valueItemCount; i++)
	{
		offsetPos = read_uint8(dest, pValueBYTETemp, offsetPos);
		G_DeviceStatusPackage.tmpBuf[i] = *(uint8_t*)pValueBYTETemp;
		pValueBYTETemp++;
	}
	free(pValueBYTE);
	

	printf("sequenceid:%d, dbValue1:%d, dbValue2:%d, dataLen:%d, 1:%d, 2:%d, 3:%d, command:0x%x\r\n", \
			G_DeviceStatusPackage.sequenceid, G_DeviceStatusPackage.dbValue1, \
			G_DeviceStatusPackage.dbValue2, G_DeviceStatusPackage.dataLen, G_DeviceStatusPackage.tmpBuf[0], \
			G_DeviceStatusPackage.tmpBuf[1], G_DeviceStatusPackage.tmpBuf[2], respCommand);
	
	freePackage(resp);
	
	return respCommand;
}

void DeviceStatus_PROCESSLISTEN(void)
{
	uint32_t sendSize, i;
	DeviceStatusRespPackage resp;
	printf("DeviceStatus Process\r\n");
	
	resp.sequenceid = 2;
	for(i = 0;i < 40;i++)
		dataSourceBuf[i] = i;
	resp.result = 1;
	
	DeviceStatusRespSetActivityDataNumAndLength(4, 10);
	sendSize = CodecEncodeDeviceStatusRespPackage(&resp, sendBuf, dataSourceBuf);
	arrayutil_printBYTEArray(sendBuf, sendSize);
	//发送出去

}


uint32_t DeviceSetPackage_PACKAGEPARSER(uint8_t dest[])
{
	PackageProperty *pPackageProperty = NULL;				//该指针使用自加操作，不影响释放内存位置
	Package * resp = CreateDeviceSetPackage();		//注意释放内存
	uint32_t offsetPos = PACKAGE_START_OFFSET;
	uint32_t i = 0, respCommand;
	
	printf("DeviceSetPackage_PACKAGEPARSER\r\n");
	
	respCommand = resp->command;
	pPackageProperty = resp->properties;
	
	offsetPos = read_uint32(dest,pPackageProperty->value,offsetPos);
	G_DeviceSetPackage.sequenceid = *(uint32_t*)pPackageProperty->value;
	pPackageProperty++;
	
	offsetPos = read_double(dest,pPackageProperty->value,offsetPos);
	G_DeviceSetPackage.pulseWidth = *(double*)pPackageProperty->value;
	pPackageProperty++;
	
	offsetPos = read_double(dest,pPackageProperty->value,offsetPos);
	G_DeviceSetPackage.triggerLevel = *(double*)pPackageProperty->value;
	pPackageProperty++;
	
	offsetPos = read_float(dest,pPackageProperty->value,offsetPos);
	G_DeviceSetPackage.powerFreq = *(float*)pPackageProperty->value;
	pPackageProperty++;
	
	offsetPos = read_float(dest,pPackageProperty->value,offsetPos);
	G_DeviceSetPackage.preTrigger = *(float*)pPackageProperty->value;
	pPackageProperty++;
	
	printf("sequenceid:%d, pulseWidth:%lf, triggerLevel:%lf, powerFreq:%.2f, preTrigger:%.2f\r\n", \
			G_DeviceSetPackage.sequenceid, G_DeviceSetPackage.pulseWidth, \
			G_DeviceSetPackage.triggerLevel, G_DeviceSetPackage.powerFreq, G_DeviceSetPackage.preTrigger);
	
	freePackage(resp);
	
	return respCommand;
}

void DeviceSet_PROCESSLISTEN(void)
{
	uint32_t sendSize, i;
	DeviceSetRespPackage resp;
	printf("DeviceSet Process\r\n");
	
	resp.sequenceid = 2;
	resp.result = 1;

	sendSize = CodecEncodeDeviceSetRespPackage(&resp, sendBuf, dataSourceBuf);
	arrayutil_printBYTEArray(sendBuf, sendSize);
	//发送出去

}


void testTask()
{
	uint8_t destBuf[ADPROTOCOL_BUFF_LENGTH], resp;
	uint32_t respCommand;
	
	//首先要注册回调函数
	AllPackageRegister();
	AllProcessesRegister();
	
	while(1)
	{
		osSemaphoreWait(SemHandle, osWaitForever);
		if(GetADWifiRxLenght()!= 0)
		{
			receiveLen = GetRxLenght();							     /*获取指令长度*/		
			osDelay(30);
			if(receiveLen != GetRxLenght())
			{
				receiveLen = GetRxLenght();
			}
			
			memcpy(destBuf, GetRxData(), receiveLen);  /*获取指令内容*/

			ClearRxLenght();											/*清除指令*/
			
			printf("receiveLen:%d\r\n", receiveLen);
			PutBytes(destBuf, wifiReceiveLen);
		}
		
		resp = Decode(destBuf);
		if(resp == DECODE_REMAIN)
		{
			printf("============Remain===============\r\n");
			
			respCommand = Parse(destBuf);
			Process(respCommand);
			
			osSemaphoreRelease(SemHandle);
		}
		else if(resp == DECODE_OK)
		{
			printf("============Package===============\r\n");
			
			respCommand = Parse(destBuf);
			Process(respCommand);
		}
	}
}




