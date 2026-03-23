#include <stdio.h>
#include "ArrayUtil.h"

//复制字节数组
int32_t arrayutil_copyBYTEArray(uint8_t* dest, uint8_t* source, int32_t offset, int32_t length)
{
	int32_t copyNum = 0;
	int32_t i;
	for (i = offset; i < length; i++)
	{
		dest[i] = source[i];
		copyNum++;
	}
	return copyNum;
}

//打印ByteArray
void arrayutil_printBYTEArray(uint8_t *byteBuf, uint32_t bytesLength)
{
	int32_t i;
	printf("ByteArray Len:%d, Hex:\r\n", bytesLength);
	for(i = 0;i < bytesLength;i++)
		printf("0x%x ", byteBuf[i]);
	printf("\r\n");
}
