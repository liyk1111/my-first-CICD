#include "PackagePublic.h"
#include "stdio.h"

/* 下位机发送赋值函数，不支持重载 */
int32_t write_uint8(uint8_t *buf, uint8_t value, int32_t offset)
{
  buf[offset] = value;
	return (offset + 1);
}

int32_t write_int8(uint8_t *buf, int8_t value, int32_t offset)
{
  buf[offset] = value;
	return (offset + 1);
}

int32_t write_uint16(uint8_t *buf, uint16_t value, int32_t offset)
{
  buf[offset]   = value >> 8;
  buf[offset+1] = value;
	return (offset + 2);
}

int32_t write_int16(uint8_t *buf, int16_t value, int32_t offset)
{
  buf[offset]   = value >> 8;
  buf[offset+1] = value;
	return (offset + 2);
}

int32_t write_uint32(uint8_t *buf, uint32_t value, int32_t offset)
{
  buf[offset]   = value >> 24;
  buf[offset+1] = value >> 16;
  buf[offset+2] = value >> 8;
  buf[offset+3] = value;
	return (offset + 4);
}

int32_t write_int32(uint8_t *buf, int32_t value, int32_t offset)
{
  buf[offset]   = value >> 24;
  buf[offset+1] = value >> 16;
  buf[offset+2] = value >> 8;
  buf[offset+3] = value;
	return (offset + 4);
}

int32_t write_uint64(uint8_t *buf, uint64_t value, int32_t offset)
{
  buf[offset]   = value >> 56;
  buf[offset+1] = value >> 48;
  buf[offset+2] = value >> 40;
  buf[offset+3] = value >> 32;
	buf[offset+4] = value >> 24;
  buf[offset+5] = value >> 16;
  buf[offset+6] = value >> 8;
  buf[offset+7] = value;
	return (offset + 8);
}

int32_t write_int64(uint8_t *buf, int64_t value, int32_t offset)
{
  buf[offset]   = value >> 56;
  buf[offset+1] = value >> 48;
  buf[offset+2] = value >> 40;
  buf[offset+3] = value >> 32;
	buf[offset+4] = value >> 24;
  buf[offset+5] = value >> 16;
  buf[offset+6] = value >> 8;
  buf[offset+7] = value;
	return (offset + 8);
}

int32_t write_float(uint8_t *buf, float value, int32_t offset)
{
	uint8_t n;
	uint8_t * i = (uint8_t *)(& value);
	for(n = 0; n < 4; n++){
		buf[offset + n] = i[3-n];
	}
	return (offset + 4);
}

int32_t write_double(uint8_t *buf, double value, int32_t offset)
{
	uint8_t n;
	uint8_t * i = (uint8_t *)(& value);
	for(n = 0; n < 8; n++){
		buf[offset + n] = i[7-n];
	}
	return (offset + 8);
}


/* 下位机解析读取函数，不支持重载 */
int32_t read_uint8(uint8_t *buf, uint8_t *value, int32_t offset)
{
  *value = buf[offset];
	printf("uint8:%d\r\n",*value);
	return (offset + 1);
}

int32_t read_int8(uint8_t *buf, int8_t *value, int32_t offset)
{
  *value = buf[offset];
	//printf("uint8:%d\r\n",*value);
	return (offset + 1);
}

int32_t read_uint16(uint8_t *buf, uint16_t *value, int32_t offset)
{
  *value = buf[offset]*0x100 + buf[offset+1];
	return (offset + 2);
}

int32_t read_int16(uint8_t *buf, int16_t *value, int32_t offset)
{
  *value = buf[offset]*0x100 + buf[offset+1];
	return (offset + 2);
}

int32_t read_uint32(uint8_t *buf, uint32_t *value, int32_t offset)
{
  *value = buf[offset] * 0x1000000
         + buf[offset+1] * 0x10000
         + buf[offset+2] * 0x100
         + buf[offset+3];
	return (offset + 4);
}

int32_t read_int32(uint8_t *buf, int32_t *value, int32_t offset)
{
  *value = buf[offset] * 0x1000000
         + buf[offset+1] * 0x10000
         + buf[offset+2] * 0x100
         + buf[offset+3];
	return (offset + 4);
}

int32_t read_uint64(uint8_t *buf, uint64_t *value, int32_t offset)
{
  *value = buf[offset]   * 0x100000000000000
         + buf[offset+1] * 0x1000000000000
         + buf[offset+2] * 0x10000000000
         + buf[offset+3] * 0x100000000
		     + buf[offset+4] * 0x1000000
		     + buf[offset+5] * 0x10000
		     + buf[offset+6] * 0x100
		     + buf[offset+7];
	return (offset + 8);
}

int32_t read_int64(uint8_t *buf, int64_t *value, int32_t offset)
{
  *value = buf[offset]   * 0x100000000000000
         + buf[offset+1] * 0x1000000000000
         + buf[offset+2] * 0x10000000000
         + buf[offset+3] * 0x100000000
		     + buf[offset+4] * 0x1000000
		     + buf[offset+5] * 0x10000
		     + buf[offset+6] * 0x100
		     + buf[offset+7];
	return (offset + 8);
}

int32_t read_float(uint8_t *buf, float *value, int32_t offset)
{
	uint8_t n;
	uint8_t *ptr = 0;
	ptr = (uint8_t*)value;

	for(n = 0; n < 4; n++){
		//printf("0x%x ",buf[offset + n]);
		ptr[3-n] =	buf[offset + n];
	}
	//printf("\r\n float:%f\r\n",*value);

	return (offset + 4);
}

int32_t read_double(uint8_t *buf, double *value, int32_t offset)
{
	uint8_t n;
	uint8_t *ptr = 0;
	ptr = (uint8_t*)value;
	
	for(n = 0; n < 8; n++){
		//printf("0x%x ",buf[offset + n]);
		ptr[7-n] =	buf[offset + n];
		//ptr[n] =	buf[offset + n];
	}
	
	//printf("\r\n double:%lf\r\n",*value);
	
//	for(n = 0; n < 8; n++){
//		printf("0x%x ",ptr[n]);
//	}
//	
//	printf("\r\n ptr:%lf\r\n",(double)*ptr);	
//	
	return (offset + 8);
}


