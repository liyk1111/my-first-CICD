/*
协议公共定义：
1.所有要添加协议包的功能码
2.所有要添加协议包的数据长度
*/
#ifndef PACKAGECONFIG_H
#define PACKAGECONFIG_H

//公共用到的一些定义
#define PACKAGE_START_OFFSET	8		//实际数据域开始位置，包含序列号
#define PACKAGE_COMMAND_LEN		4		//功能码长度
#define PACKAGE_LENGTH_LEN		4		//数据长度
#define PACKAGE_STARTBYTE_LEN 2		//包头长度
#define PACKAGE_ENDBYTE_LEN 	2		//包尾长度
#define PACKAGE_HEAD_FF				0xFF
#define PACKAGE_HEAD_FA				0xFA
#define PACKAGE_END_FF				0xFF
#define PACKAGE_END_FE				0xFE

//根据预估最大发送长度定义
#define SENDBUF_MAXLENGTH			145000	//字节

//协议包功能码长度定义 - 按需自行添加
#define DEVICESTATUS_COMMAND							0xEA				//设备状态功能码
//#define DEVICESTATUS_LENGTH								6						//设备状态包长度 - 通信设备发送，无需定义
#define DEVICESTATUS_PROPERTY_NUM					5						//设备状态包有多少个参数，数组也只算一个 
#define DEVICESTATUS_PROPERTY_BUFLEN			100					//设备状态包属性数组长度
#define DEVICESTATUS_COMMAND_RESP					0x080000EA	//设备状态返回包功能码
#define DEVICESTATUS_LENGTH_RESP					9						//设备状态返回包固定长度
																											//另外还需添加活动长度，例如脉冲个数（随机），1个脉冲100个字节 
#define DEVICESTATUS_PROPERTY_BUFLEN_RESP	100					//设备状态返回包属性数组长度-TODO是否需要？？？？

#define DEVICESET_COMMAND									0x300				//功能码
//#define DEVICESET_LENGTH									6						//包长度
#define DEVICESET_PROPERTY_NUM						5						//有多少个参数，数组也只算一个 
//#define DEVICESET_PROPERTY_BUFLEN					100					//属性数组长度
#define DEVICESET_COMMAND_RESP						0x08000300	//返回包功能码
#define DEVICESET_LENGTH_RESP							5						//返回包固定长度

//数据返回包
#define DEVICEPULSEDATA_COMMAND_RESP			0x08000301	//返回包功能码
#define DEVICEPULSEDATA_LENGTH_RESP				50					//返回包固定长度

#endif
