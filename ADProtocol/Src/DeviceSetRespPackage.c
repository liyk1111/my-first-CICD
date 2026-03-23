#include "DeviceSetRespPackage.h"
#include "stdio.h"
#include "PackagePublic.h"

static uint32_t dataNum = 0;
static uint32_t dataBufLength = 0;

static uint32_t CalcPackageSize()
{
	return DEVICESET_LENGTH_RESP + dataBufLength*dataNum;
}

uint32_t CodecEncodeDeviceSetRespPackage(DeviceSetRespPackage *tmpPackage, uint8_t *result, uint8_t *dataSource)
{
	uint32_t offset = 0, i, j;
	//起始符
	offset = write_uint8(result, PACKAGE_HEAD_FF, offset);
  offset = write_uint8(result, PACKAGE_HEAD_FA, offset);

  //功能码
  offset = write_int32(result, DEVICESET_COMMAND_RESP, offset);

	//数据包长度
	offset = write_int32(result, CalcPackageSize(), offset);

	//数据包本身
	//序列号
	offset = write_int32(result, tmpPackage->sequenceid, offset);
	offset = write_uint8(result, tmpPackage->result, offset);

	//结束符
	offset = write_uint8(result, PACKAGE_END_FF, offset);
  offset = write_uint8(result, PACKAGE_END_FE, offset);

	return offset;
}





