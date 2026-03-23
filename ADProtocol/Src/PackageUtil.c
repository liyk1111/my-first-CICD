
#include <stdio.h>
#include "PackagePublic.h"
#include "PackageUtil.h"

/*根据协议包属性的类型，写入到数组内*/
int32_t writePackagePropertyToByteArray(Package *package, PackageProperty *packageProperty, uint8_t* packageBytes, int32_t offset)
{
	uint8_t *pValueBYTE = NULL;
	int16_t *pValueSHORT = NULL;
	int32_t *pValueINT = NULL;
	double *pValueDouble = NULL;
	float *pValueFloat = NULL;
	int32_t dataLength = 0;

	int32_t i = 0;
	if (packageProperty->type == TYPE_BYTE)
	{
		pValueBYTE = (uint8_t*)packageProperty->value;
		offset = write_uint8(packageBytes, *pValueBYTE, offset);
		return offset;
	}
	else if (packageProperty->type == TYPE_SHORT)
	{
		pValueSHORT = (int16_t*)packageProperty->value;
		offset = write_int16(packageBytes, *pValueSHORT, offset);
		return offset;
	}
	else if (packageProperty->type == TYPE_INT)
	{
		pValueINT = (int32_t*)packageProperty->value;
		offset = write_int32(packageBytes, *pValueINT, offset);
		return offset;
	}
	else if (packageProperty->type == TYPE_Double)
	{
		pValueDouble = (double*)packageProperty->value;
		//printf("pValueDouble %0.2lf\r\n",*pValueDouble);
		offset = write_double(packageBytes, *pValueDouble, offset);
		return offset;
	}
	else if (packageProperty->type == TYPE_Float)
	{
		pValueFloat = (float*)packageProperty->value;
		offset = write_float(packageBytes, *pValueFloat, offset);
		return offset;
	}
	else if (packageProperty->type == TYPE_BYTEArray)
	{
		pValueBYTE = (uint8_t*)packageProperty->value;

		dataLength = (int32_t)packageProperty->valueItemCount;
		offset = write_int32(packageBytes, dataLength, offset);
		for (i = 0; i < dataLength; i++)
		{
			offset = write_uint8(packageBytes, pValueBYTE[i], offset);
		}

		return offset;
	}
	else if (packageProperty->type == TYPE_SHORTArray)
	{
		pValueSHORT = (int16_t*)packageProperty->value;

		dataLength = (int32_t)packageProperty->valueItemCount;
		offset = write_int16(packageBytes, dataLength, offset);
		for (i = 0; i < dataLength; i++)
		{
			offset = write_int16(packageBytes, pValueSHORT[i], offset);
		}

		return offset;
	}
	else if (packageProperty->type == TYPE_INTArray)
	{
		pValueINT = (int32_t*)packageProperty->value;

		dataLength = (int32_t)packageProperty->valueItemCount;
		offset = write_int32(packageBytes, dataLength, offset);
		for (i = 0; i < dataLength; i++)
		{
			offset = write_int32(packageBytes, pValueINT[i], offset);
		}

		return offset;
	}
	else if (packageProperty->type == TYPE_DoubleArray)
	{
		pValueDouble = (double*)packageProperty->value;
		dataLength = packageProperty->valueItemCount;
		offset = write_int32(packageBytes, dataLength, offset);
		for (i = 0; i < dataLength; i++)
		{
			offset = write_double(packageBytes, pValueDouble[i], offset);
		}

		return offset;
	}
	else if (packageProperty->type == TYPE_FloatArray)
	{
		pValueFloat = (float*)packageProperty->value;
		dataLength = packageProperty->valueItemCount;
		offset = write_int32(packageBytes, dataLength, offset);
		for (i = 0; i < dataLength; i++)
		{
			offset = write_float(packageBytes, pValueFloat[i], offset);
		}

		return offset;
	}
	else
	{
		printf("unsupport %d",packageProperty->type);
		return offset;
	}
}

//计算整个协议包大小，循环计算每一个属性
int32_t calcPackageByteSize(Package *package)
{
	int32_t length = package->propertyCount;
	int32_t idx = 0;
	int32_t size = 0;
	PackageProperty *pProperty = package->properties;
	printf("calcPackageByteSize:%d\r\n",length);
	for(size = 0;idx < length;idx++)
	{
		size += calcPackagePropertyByteSize(pProperty++);
	}
	return size;
}

//计算每一次协议包属性大小
int32_t calcPackagePropertyByteSize(PackageProperty *pProperty)
{
	int32_t arrayByteSize = 0;
	printf("pProperty->type:%d\r\n", pProperty->type);
	if(
			pProperty->type == TYPE_BYTE
			|| pProperty->type == TYPE_SHORT
			|| pProperty->type == TYPE_INT
			|| pProperty->type == TYPE_Double
			|| pProperty->type == TYPE_Float
		)
	{
		return pProperty->typeByteSize;
	}
	else if(
			pProperty->type == TYPE_BYTEArray
			|| pProperty->type == TYPE_SHORTArray
			|| pProperty->type == TYPE_INTArray
			|| pProperty->type == TYPE_DoubleArray
			|| pProperty->type == TYPE_FloatArray
			)
	{
		arrayByteSize = BYTE_LENGTH_INT;
		return pProperty->typeByteSize * pProperty->valueItemCount + arrayByteSize;
	}
	else
	{
		return 0;
	}
}

void printPackageProperty(PackageProperty* pProperty)
{
	uint8_t *pValueBYTE = NULL;
	int16_t *pValueSHORT = NULL;
	int32_t *pValueINT = NULL;
	double *pValueDouble = NULL;
	float *pValueFloat = NULL;

	int32_t i = 0;

	if(pProperty->type == TYPE_BYTE){
		printf("TYPE_BYTE, value:\r\n");
		pValueBYTE = (uint8_t*)pProperty->value;
		printf("%c\r\n", *pValueBYTE);
	}else if(pProperty->type == TYPE_SHORT){
		printf("TYPE_SHORT, value:\r\n");
		pValueSHORT = (int16_t*)pProperty->value;
		printf("%d\r\n", *pValueSHORT);
	}else if(pProperty->type == TYPE_INT)
	{
		printf("TYPE_INT, value:\r\n");
		pValueINT = (int32_t*)pProperty->value;
		printf("%d\r\n", *pValueINT);
	}else if(pProperty->type == TYPE_Double)
	{
		printf("TYPE_Double, value:\r\n");
		pValueDouble = (double*)pProperty->value;
		printf("%f\r\n", *pValueDouble);
	}else if(pProperty->type == TYPE_Float)
	{
		printf("TYPE_Float, value:\r\n");
		pValueFloat = pProperty->value;
		printf("%f\r\n", *pValueFloat);
	}else if(pProperty->type == TYPE_BYTEArray)
	{
		printf("TYPE_BYTEArray, value:\r\n");
		pValueBYTE = pProperty->value;
		for(i=0;i<pProperty->valueItemCount;i++){
			printf("idx:%d,value:%c\r\n", i, *pValueBYTE++);
		}
	}
	else if(pProperty->type == TYPE_BYTEArray)
	{
		printf("TYPE_BYTEArray, value:\r\n");
		pValueBYTE = pProperty->value;
		for(i=0;i<pProperty->valueItemCount;i++){
			printf("idx:%d,value:%c\r\n", i, *pValueBYTE++);
		}
	}
	else if(pProperty->type == TYPE_SHORTArray)
	{
		printf("TYPE_SHORTArray, value:\r\n");
		pValueSHORT = pProperty->value;
		for(i=0;i<pProperty->valueItemCount;i++){
			printf("idx:%d,value:%d\r\n", i, *pValueSHORT++);
		}
	}
	else if(pProperty->type == TYPE_INTArray)
	{
		printf("TYPE_INTArray, value:\r\n");
		pValueINT = pProperty->value;
		for(i=0;i<pProperty->valueItemCount;i++){
			printf("idx:%d,value:%d\r\n", i, *pValueINT++);
		}
	}
	else if(pProperty->type == TYPE_DoubleArray)
	{
		printf("TYPE_DoubleArray, value:\r\n");
		pValueDouble = pProperty->value;
		for(i=0;i<pProperty->valueItemCount;i++){
			printf("idx:%d,value:%f\r\n", i, *pValueDouble++);
		}
	}else if(pProperty->type == TYPE_FloatArray)
	{
		printf("TYPE_FloatArray, value:\r\n");
		pValueFloat = pProperty->value;
		for(i=0;i<pProperty->valueItemCount;i++){
			printf("idx:%d,value:%f\r\n", i, *pValueFloat++);
		}
	}
	else{
		printf("not support value type\r\n");
	}
}


void printPackage(Package *package)
{
	PackageProperty *pPackageProperty = NULL;
	int32_t length = package->propertyCount;
	int32_t idx = 0;

	printf("package property count: %d\r\n", package->propertyCount);
	printf("package uint8_t size: %d\r\n", calcPackageByteSize(package));

	pPackageProperty = package->properties;
	for(idx = 0;idx < length;idx++){
		printPackageProperty(pPackageProperty++);
	}
}
