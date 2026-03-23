#ifndef ARRAYUTIL_H
#define ARRAYUTIL_H

#include "Package.h"
/**
 * 复制字节数组
 */
int32_t arrayutil_copyBYTEArray(uint8_t* dest, uint8_t* source, int32_t offset, int32_t length);

void arrayutil_printBYTEArray(uint8_t *byteBuf, uint32_t bytesLength);

#endif /* ARRAYUTIL_H */
