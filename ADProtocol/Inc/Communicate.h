/*
根据业务自行判断，固定数组长度，不考虑重新分配，因为单片机内存较小，连续分配较大内存可能会出现错误
自行修改地方：
1.ADPROTOCOL_BUFF_LENGTH长度根据业务自行定义，默认200；
2.业务处理，若是连包，返回DECODE_REMAIN时，需要再次调用Decode；
3.预留调试宏定义COMMUNICATE_DEBUG，后续优化可开启

使用：
1.首先将外部接收的字节数据写入到本地数组ADProtocolBuff
2.调用Decode解码出符合协议标准的字节返回到app

2023-11-01: V1.0，liyk
	1.基础功能实现，测试正常
*/
#ifndef COMMUNICATE_H
#define COMMUNICATE_H

#include "PackageConfig.h"
#include "stdint.h"

#define ADPROTOCOL_BUFF_LENGTH	200	
//#define COMMUNICATE_DEBUG	//调试定义，屏蔽就没有打印信息

#define DECODE_OK				0		
#define DECODE_NOTAIL		1		//断流数据
#define DECODE_INVALID	2		//无效数据：无起始位，满内存但无结束位
#define DECODE_REMAIN		3		//解析后还有剩余数据
#define DECODE_EMPTY		4		//数据不足或为空

#define PACKAGE_MINI_LEN		16	//数据至少要有这么多再处理，否则继续等待接收

void PutBytes(uint8_t* tmpBuff, uint32_t tmpLen);
uint8_t Decode(uint8_t *destBuf);
void PrintBytes(uint32_t tmpLen);
void PrintPos(void);

#endif
