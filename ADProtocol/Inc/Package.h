/*

 */

#ifndef PACKAGE_H_
#define PACKAGE_H_

#include "stdint.h"

//package property type
#define TYPE_BYTE 				1
#define TYPE_SHORT 				2
#define TYPE_INT 					3
#define TYPE_Double 			4
#define TYPE_Float 				5
#define TYPE_BYTEArray 		6
#define TYPE_SHORTArray 	7
#define TYPE_INTArray 		8
#define TYPE_DoubleArray 	9
#define TYPE_FloatArray 	10

//基础类型的字节长度
#define BYTE_LENGTH_BYTE 		1 //uint8_t 的字节长度
#define BYTE_LENGTH_SHORT 	2 //int16_t 的字节长度
#define BYTE_LENGTH_INT 		4 //int32_t 的字节长度
#define BYTE_LENGTH_Double 	8 //Double 的字节长度
#define BYTE_LENGTH_Float 	4 //Float 的字节长度

/**
 * 定义一个字节数组的结构体
 */
typedef struct{
	uint8_t *bytes;
	int32_t  bytesLength;
}BYTEArray;

/**
 * 数据包的属性
 */
typedef struct{
	/*
	 * 属性类型，类型分两种，分为数组型和非数组型。
	 * 如果是数组型，valueItemCount有效；
	 *
	 * 非数组型属性对应的类型：
	 * TYPE_BYTE,TYPE_SHORT,TYPE_INT,TYPE_Double,TYPE_Float
	 * 数组型属性对应的类型
	 * TYPE_BYTEArray,TYPE_SHORTArray,TYPE_INTArray,TYPE_DoubleArray,TYPE_FloatArray
	 */
	int32_t type;
	/**
	 * 属性的value编码为字节码后的字节长度
	 */
	int32_t typeByteSize;
	/*
	 * 数组型属性的原始个数,
	 * 非数组型个数为0
	 */
	int32_t valueItemCount;
	/**
	 * 属性值，使用void指针指向，能够对所有类型的值进行引用
	 * 如果是数组型，那么指针类型可能为：uint8_t* or int16_t* or int32_t* or double* or float*
	 */
	void *value;
}PackageProperty;

/**
 * 数据包
 */
typedef struct{
	int32_t command;
	int32_t propertyCount;
	PackageProperty* properties;
}Package;

/**
 * 包创建器,由业务自行实现
 */
typedef Package * (*PFPackageCreator)();

//[start] package
Package *createPackage(int32_t propertyCount);
void freePackage(Package *pPackage);
//[end] package

//[start] packageProperty
void setPackagePropertyValue(PackageProperty *pPackageProperty, int32_t propertyType, int32_t valueItemCount, void *value, int32_t typeByteSize);
void setPackagePropertyValueForBYTE(PackageProperty *pPackageProperty, uint8_t value);
void setPackagePropertyValueForSHORT(PackageProperty *pPackageProperty, int16_t value);
void setPackagePropertyValueForINT(PackageProperty *pPackageProperty, int32_t value);
void setPackagePropertyValueForDouble(PackageProperty *pPackageProperty, double value);
void setPackagePropertyValueForFloat(PackageProperty *pPackageProperty, float value);

uint8_t* copyPackagePropertyValueForBYTEArray(uint8_t *value, int32_t valueItemCount);
void setPackagePropertyValueForBYTEArray(PackageProperty *pPackageProperty, uint8_t *value, int32_t valueItemCount);

int16_t* copyPackagePropertyValueForSHORTArray(int16_t *value, int32_t valueItemCount);
void setPackagePropertyValueForSHORTArray(PackageProperty *pPackageProperty, int16_t *value, int32_t valueItemCount);

int32_t* copyPackagePropertyValueForINTArray(int32_t *value, int32_t valueItemCount);
void setPackagePropertyValueForINTArray(PackageProperty *pPackageProperty, int32_t *value, int32_t valueItemCount);

double* copyPackagePropertyValueForDoubleArray(double *value, int32_t valueItemCount);
void setPackagePropertyValueForDoubleArray(PackageProperty *pPackageProperty, double *value, int32_t valueItemCount);

float* copyPackagePropertyValueForFloatArray(float *value, int32_t valueItemCount);
void setPackagePropertyValueForFloatArray(PackageProperty *pPackageProperty, float *value, int32_t valueItemCount);

#endif /* PACKAGE_H */
