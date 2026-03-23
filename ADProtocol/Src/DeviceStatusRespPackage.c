#include "DeviceStatusRespPackage.h"
#include "stdio.h"
#include "PackagePublic.h"

static uint32_t dataNum;
static uint32_t dataBufLength;

static uint32_t CalcPackageSize()
{
	return DEVICESTATUS_LENGTH_RESP + dataBufLength*dataNum;
}

void DeviceStatusRespSetActivityDataNumAndLength(uint32_t tmpNum, uint32_t tmpLength)
{
	dataNum = tmpNum;
	dataBufLength = tmpLength;
}

uint32_t CodecEncodeDeviceStatusRespPackage(DeviceStatusRespPackage *tmpPackage, uint8_t *result, uint8_t *dataSource)
{
	#if 0
	PackageProperty* pPackageProperty = NULL;
	Package *pPackage = createPackage(DEVICESTATUS_PROPERTY_NUM_RESP);
	printf("Resp Package:0x%x\r\n", (uint32_t*)pPackage);
	pPackage->command = DEVICESTATUS_COMMAND_RESP;

	pPackageProperty = pPackage->properties;

	setPackagePropertyValueForINT(pPackageProperty++, tmpPackage->sequenceid);
	//setPackagePropertyValueForBYTEArray(pPackageProperty++, tmpPackage->dataBuf, DEVICESTATUS_PROPERTY_BUFLEN_RESP);
	setPackagePropertyValueForSHORT(pPackageProperty++, tmpPackage->result);

	codec_encode(pPackage, result);
	#endif
	
	uint32_t offset = 0, i, j;
	//起始符
	offset = write_uint8(result, PACKAGE_HEAD_FF, offset);
  offset = write_uint8(result, PACKAGE_HEAD_FA, offset);

  //功能码
  offset = write_int32(result, DEVICESTATUS_COMMAND_RESP, offset);

	//数据包长度
	offset = write_int32(result, CalcPackageSize(), offset);

	//数据包本身
	//序列号
	offset = write_int32(result, tmpPackage->sequenceid, offset);
	offset = write_uint8(result, tmpPackage->result, offset);
	offset = write_int32(result, tmpPackage->dataNum, offset);
	
	for(i = 0;i < dataBufLength*dataNum;i++)
	{
		offset = write_uint8(result, dataSource[i], offset);
	}

	//结束符
	offset = write_uint8(result, PACKAGE_END_FF, offset);
  offset = write_uint8(result, PACKAGE_END_FE, offset);

	return offset;
	
	
	//freePackage(pPackage);
}





