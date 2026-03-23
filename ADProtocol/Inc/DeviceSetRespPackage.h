//返回包，需要实现编码函数
#include "stdint.h"
#include "Package.h"
#include "PackageConfig.h"

//注意此处的数组会自动在数组前面加上长度，无需手动添加
//大数据这里如何指定长度？结构体如何在SRAM中定义
typedef struct{
	uint32_t command;			//
	uint32_t packageLen;	//
	uint32_t sequenceid;	//4
	uint8_t  result;			//1
	//数组无需定义
}DeviceSetRespPackage;

uint32_t CodecEncodeDeviceSetRespPackage(DeviceSetRespPackage *tmpPackage, uint8_t *result, uint8_t *dataSource);
