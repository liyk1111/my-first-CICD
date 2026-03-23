/*
使用：
1.在字节数组中，读取或写入对应的数据类型
2.存储方式为大端

2023-11-01: V1.0，liyk
	1.基础功能实现，测试正常
*/
#ifndef PACKAGEPUBLIC_H
#define PACKAGEPUBLIC_H

#include "PackageConfig.h"
#include "stdint.h"

int32_t write_uint8(uint8_t *buf, uint8_t value, int32_t offset);
int32_t write_int8(uint8_t *buf, int8_t value, int32_t offset);
int32_t write_uint16(uint8_t *buf, uint16_t value, int32_t offset);
int32_t write_int16(uint8_t *buf, int16_t value, int32_t offset);
int32_t write_uint32(uint8_t *buf, uint32_t value, int32_t offset);
int32_t write_int32(uint8_t *buf, int32_t value, int32_t offset);
int32_t write_uint64(uint8_t *buf, uint64_t value, int32_t offset);
int32_t write_int64(uint8_t *buf, int64_t value, int32_t offset);
int32_t write_float(uint8_t *buf, float value, int32_t offset);
int32_t write_double(uint8_t *buf, double value, int32_t offset);

int32_t read_uint8(uint8_t *buf, uint8_t *value, int32_t offset);
int32_t read_int8(uint8_t *buf, int8_t *value, int32_t offset);
int32_t read_uint16(uint8_t *buf, uint16_t *value, int32_t offset);
int32_t read_int16(uint8_t *buf, int16_t *value, int32_t offset);
int32_t read_uint32(uint8_t *buf, uint32_t *value, int32_t offset);
int32_t read_int32(uint8_t *buf, int32_t *value, int32_t offset);
int32_t read_uint64(uint8_t *buf, uint64_t *value, int32_t offset);
int32_t read_int64(uint8_t *buf, int64_t *value, int32_t offset);
int32_t read_float(uint8_t *buf, float *value, int32_t offset);
int32_t read_double(uint8_t *buf, double *value, int32_t offset);

#endif
