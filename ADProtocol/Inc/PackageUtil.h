
#ifndef PACKAGEUTIL_H
#define PACKAGEUTIL_H

#include "stdint.h"
#include "Package.h"

/**
 * 计算包属性所需的字节大小
 */
int32_t calcPackagePropertyByteSize(PackageProperty *pProperty);

/**
 * 把packageProperty写入到字节数组byteArray中
 */
int32_t writePackagePropertyToByteArray(Package *package, PackageProperty *packageProperty, uint8_t* byteArray, int32_t offset);

/**
 * 计算包所需的字节大小
 */
int32_t calcPackageByteSize(Package *package);

/**
 * 打印PackageProperty对象
 */
void printPackageProperty(PackageProperty* property);

/**
 * 打印Package对象
 */
void printPackage(Package *package);

#endif /* PACKAGEUTIL_H */
