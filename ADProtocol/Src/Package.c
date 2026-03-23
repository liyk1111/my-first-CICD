#include "Package.h"
#include <stdlib.h>
#include <stdio.h>

//创建协议包内存
Package *createPackage(int32_t propertyCount)
{
	Package *pPackage = (Package *)malloc(sizeof(Package));
	pPackage->propertyCount = propertyCount;
	pPackage->properties = (PackageProperty*)malloc( sizeof(PackageProperty) * propertyCount);
	return pPackage;
}

//释放协议包内存
void freePackage(Package *pPackage)
{
	int32_t i = 0;
	for(i = 0; i < pPackage->propertyCount; i++)
	{
		free(pPackage->properties[i].value);
	}

	free(pPackage->properties);
	free(pPackage);
}

void setPackagePropertyValue(PackageProperty *pPackageProperty, int32_t propertyType, int32_t valueItemCount, void *value, int32_t typeByteSize)
{
	pPackageProperty->type = propertyType;
	pPackageProperty->valueItemCount = valueItemCount;
	pPackageProperty->value = value;
	pPackageProperty->typeByteSize = typeByteSize;
}

void setPackagePropertyValueForBYTE(PackageProperty *pPackageProperty, uint8_t value)
{
	uint8_t *pValue = (uint8_t *)malloc( sizeof(uint8_t));
	*pValue = value;//拷贝值
	setPackagePropertyValue(pPackageProperty, TYPE_BYTE, 0, pValue, BYTE_LENGTH_BYTE);
}

void setPackagePropertyValueForSHORT(PackageProperty *pPackageProperty, int16_t value)
{
	int16_t *pValue = (int16_t *)malloc(sizeof(int16_t));
	*pValue = value;//拷贝值
	setPackagePropertyValue(pPackageProperty, TYPE_SHORT, 0, pValue, BYTE_LENGTH_SHORT);
}

void setPackagePropertyValueForINT(PackageProperty *pPackageProperty, int32_t value)
{
	int32_t *pValue = (int32_t *)malloc(sizeof(int32_t));
	*pValue = value;//拷贝值
	setPackagePropertyValue(pPackageProperty, TYPE_INT, 0, pValue, BYTE_LENGTH_INT);
}

void setPackagePropertyValueForDouble(PackageProperty *pPackageProperty, double value)
{
	double *pValue = (double *)malloc( sizeof(double));
	*pValue = value;//拷贝值
	setPackagePropertyValue(pPackageProperty, TYPE_Double, 0, pValue, BYTE_LENGTH_Double);
}

void setPackagePropertyValueForFloat(PackageProperty *pPackageProperty, float value)
{
	float *pValue = (float *)malloc( sizeof(float));
	*pValue = value;//拷贝值
	setPackagePropertyValue(pPackageProperty, TYPE_Float, 0, pValue, BYTE_LENGTH_Float);
}

uint8_t* copyPackagePropertyValueForBYTEArray(uint8_t *value, int32_t valueItemCount)
{
	uint8_t *pValue = value == NULL ? NULL : (uint8_t *)malloc( sizeof(uint8_t) * valueItemCount);
	int32_t i = 0;
	for(i = 0; i < valueItemCount; i++)
	{
		pValue[i] = value[i];//拷贝值
	}
	return pValue;
}

void setPackagePropertyValueForBYTEArray(PackageProperty *pPackageProperty, uint8_t *value, int32_t valueItemCount)
{
	uint8_t *pValue = copyPackagePropertyValueForBYTEArray(value, valueItemCount);
	
	//导致无法回收正常
	//uint8_t *pValue = value;//嵌入式内存太小，不拷贝数据了，直接引用?
	
	setPackagePropertyValue(pPackageProperty, TYPE_BYTEArray, valueItemCount, pValue, BYTE_LENGTH_BYTE);
}

int16_t* copyPackagePropertyValueForSHORTArray(int16_t *value, int32_t valueItemCount)
{
	int16_t *pValue = value == NULL ? NULL : (int16_t *)malloc(sizeof(int16_t)* valueItemCount);
	int32_t i = 0;
	for(i = 0; i < valueItemCount; i++)
	{
		pValue[i] = value[i];//拷贝值
	}
	return pValue;
}

void setPackagePropertyValueForSHORTArray(PackageProperty *pPackageProperty, int16_t *value, int32_t valueItemCount)
{
	int16_t *pValue = copyPackagePropertyValueForSHORTArray(value, valueItemCount);
	
	//导致无法回收正常
	//int16_t *pValue = value;

	setPackagePropertyValue(pPackageProperty, TYPE_SHORTArray, valueItemCount, pValue, BYTE_LENGTH_SHORT);
	
}

int32_t* copyPackagePropertyValueForINTArray(int32_t *value, int32_t valueItemCount)
{
	int32_t *pValue = value == NULL ? NULL : (int32_t *)malloc(sizeof(int32_t)* valueItemCount);
	int32_t i = 0;
	for(i = 0; i < valueItemCount; i++)
	{
		pValue[i] = value[i];//拷贝值
	}
	return pValue;
}

void setPackagePropertyValueForINTArray(PackageProperty *pPackageProperty, int32_t *value, int32_t valueItemCount)
{
	int32_t *pValue = copyPackagePropertyValueForINTArray(value, valueItemCount);

	//导致无法回收正常
	//int32_t *pValue = value;

	setPackagePropertyValue(pPackageProperty, TYPE_INTArray, valueItemCount, pValue, BYTE_LENGTH_INT);
}

double* copyPackagePropertyValueForDoubleArray(double *value, int32_t valueItemCount)
{
	double *pValue = value == NULL ? NULL : (double *)malloc( sizeof(double) * valueItemCount );
	int32_t i = 0;
	for(i = 0; i < valueItemCount; i++)
	{
		pValue[i] = value[i];//拷贝值
	}
	return pValue;
}

void setPackagePropertyValueForDoubleArray(PackageProperty *pPackageProperty, double *value, int32_t valueItemCount)
{
	double *pValue = copyPackagePropertyValueForDoubleArray(value, valueItemCount);

	//导致无法回收正常
	//double *pValue = value;

	setPackagePropertyValue(pPackageProperty, TYPE_DoubleArray, valueItemCount, pValue, BYTE_LENGTH_Double);
}

float* copyPackagePropertyValueForFloatArray(float *value, int32_t valueItemCount)
{
	float *pValue = value == NULL ? NULL : (float *)malloc( sizeof(float) * valueItemCount );
	int32_t i = 0;
	for(i = 0; i < valueItemCount; i++)
	{
		pValue[i] = value[i];//拷贝值
	}
	return pValue;
}

void setPackagePropertyValueForFloatArray(PackageProperty *pPackageProperty, float *value, int32_t valueItemCount)
{
	float *pValue = copyPackagePropertyValueForFloatArray(value, valueItemCount);

	//导致无法回收正常
	//float *pValue = value;

	setPackagePropertyValue(pPackageProperty, TYPE_FloatArray, valueItemCount, pValue, BYTE_LENGTH_Float);
}
