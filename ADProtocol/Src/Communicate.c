#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "Communicate.h"
#include "PackagePublic.h"


#ifdef COMMUNICATE_DEBUG
	#define COMFILEDEBUG(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
	#define COMFILEDEBUG(fmt, ...)	
#endif


uint8_t ADProtocolBuff[ADPROTOCOL_BUFF_LENGTH];

static uint32_t wpos = 0;		//已写入数组位置
static uint32_t rpos = 0;		//已读取数组位置

static int32_t CacheRemaining(void);
static void ClearBuff(void);
static void ShiftBuff(void);
static void getBytes(uint8_t* buf, uint32_t len);
static uint32_t GetPackageHead(uint32_t* tmpCommand, uint32_t* tmpLength, uint32_t* tmpSequenceid);
static uint8_t doDecode(uint8_t *destBuf);

//查看数据剩余长度
static int32_t CacheRemaining()
{
	return (wpos - rpos);
}

//将数据写入到Buff中
void PutBytes(uint8_t* tmpBuff, uint32_t tmpLen)
{
	uint32_t size = tmpLen;
	if((wpos + size) > ADPROTOCOL_BUFF_LENGTH)	//限制写入长度，超过长度直接丢弃
		size = ADPROTOCOL_BUFF_LENGTH - wpos;
	
	memcpy(ADProtocolBuff+wpos, tmpBuff, size);
	
	wpos += size;
}

//打印数组，从0开始，调试用途
void PrintBytes(uint32_t tmpLen)
{
	uint32_t i = 0;
	COMFILEDEBUG("ADProtocol Source Buff:\r\n");
	for(i = 0;i < tmpLen;i++)
		COMFILEDEBUG("0x%x ", ADProtocolBuff[i]);
	COMFILEDEBUG("\r\n");
}

//移除正常解析以及判断异常数据
static void ShiftBuff()
{
	uint32_t i;
	
	for(i = 0;i < (wpos-rpos);i++)
		ADProtocolBuff[i] = ADProtocolBuff[rpos+i];
	
	wpos = wpos-rpos;
	rpos = 0;

	PrintBytes(wpos);
}

//清空数据
static void ClearBuff()
{
	COMFILEDEBUG("clear buff\r\n");
	wpos = 0;
	rpos = 0;
}

//读取字节，读位置偏移
static void getBytes(uint8_t* buf, uint32_t len) 
{
	uint32_t i;
	for(i = 0; i < len; i++) {
		buf[i] = ADProtocolBuff[rpos+i];
	}
	rpos += len;
}

//获取功能码，数据长度，序列号，不使用getBytes的原因是:出现异常数据的话，不想重新处理偏移的rpos
static uint32_t GetPackageHead(uint32_t* tmpCommand, uint32_t* tmpLength, uint32_t* tmpSequenceid)
{
	uint32_t offset = rpos;
	uint32_t ret = 0;
	
	offset = read_uint32(ADProtocolBuff, tmpCommand, offset);
	offset = read_uint32(ADProtocolBuff, tmpLength, offset);
	offset = read_uint32(ADProtocolBuff, tmpSequenceid, offset);
	
	return ret;
}

//解码具体操作
static uint8_t doDecode(uint8_t *destBuf)
{
	uint32_t i, tmpLen;
	uint32_t command, length, sequenceid;
	uint8_t headFF, headFA;
	uint8_t tailBuf[2];
	uint8_t resp;
	
	tmpLen = CacheRemaining();
	headFF = 0;
	for(i = 0;i < tmpLen;i++)
	{
		if (headFF != 0xFF)		//若找不到就继续找,找到为止
		{
			getBytes(&headFF, 1);	//累加rpos的位置
		}
		else	//找到就继续做判断
		{
			getBytes(&headFA, 1);
			if (headFA != 0xFA)		//若找不到就继续找,找到为止
			{
				headFF = headFA;
			}
			else  //找到开始包头
			{
				break;
			}
		}
	}
	COMFILEDEBUG("i:%d, tmpLen:%d\r\n", i, tmpLen);
	
	//无论是否找到包头，继续执行操作
	tmpLen = CacheRemaining();
	if(tmpLen > (PACKAGE_MINI_LEN-2))		//有数据可以继续处理
	{
		GetPackageHead(&command, &length, &sequenceid);
		COMFILEDEBUG("command:0x%x, 0x%x, 0x%x, tmpLen:%d\r\n", command, length, sequenceid, tmpLen);
		if(length == 0 || length > ADPROTOCOL_BUFF_LENGTH)		//数据长度异常，丢弃到包头位置
		{
			COMFILEDEBUG("Data Len Invalid:%d\r\n",length);
			resp = DECODE_INVALID;
		}
		else
		{
			COMFILEDEBUG("rpos+length+PACKAGE_COMMAND_LEN+PACKAGE_LENGTH_LEN:%d\r\n", rpos+length+PACKAGE_COMMAND_LEN+PACKAGE_LENGTH_LEN);
			if((rpos+length+PACKAGE_COMMAND_LEN+PACKAGE_LENGTH_LEN) > ADPROTOCOL_BUFF_LENGTH)//限制读取的长度不能大于最大值，否则丢弃
			{
				COMFILEDEBUG("Data Len Invalid:%d\r\n",length);
				resp = DECODE_INVALID;
			}
			else if((rpos+length+PACKAGE_COMMAND_LEN+PACKAGE_LENGTH_LEN + 2) > wpos)//读取后需要有包尾长度，否则就是长度不足，继续等待接收
			{
				COMFILEDEBUG("Data No Tail:%d, wpos:%d\r\n", rpos+length+PACKAGE_COMMAND_LEN+PACKAGE_LENGTH_LEN + 2, wpos);
				return DECODE_NOTAIL;
			}

			getBytes(destBuf, length+PACKAGE_COMMAND_LEN+PACKAGE_LENGTH_LEN);
			getBytes(tailBuf, 2);
			if (tailBuf[0] != 0xFF || tailBuf[1] != 0xFE)
      {
				COMFILEDEBUG("Fail, No Tail, Data Len Error! 0x%x, 0x%x\r\n", tailBuf[0], tailBuf[1]);
				rpos = rpos - 2;//防止第一个包最后丢1个字节，让下一个包也遭殃；但是故意将包尾写成FFFA的情况，无法处理
				resp = DECODE_INVALID;
      }
			else
				resp = DECODE_OK;
		}
		
		//最后需要判断是否还有剩余数据
		tmpLen = CacheRemaining();
		if(tmpLen > PACKAGE_MINI_LEN)
		{
			return DECODE_REMAIN;
		}
		else
			return resp;
	}
	else	//数据太少或为空，返回
	{
		COMFILEDEBUG("After Find FFFA Data Len Not Enougth:%d\r\n",tmpLen);
		rpos = rpos - 2;//这个操作，保留包头，也可以丢掉包头之前的异常数据
		return DECODE_EMPTY;
	}
}

//开始解码：destBuf为解析后取出包的实体，已去头去尾，可以直接放到Package中使用，识别功能码自动回调
uint8_t Decode(uint8_t *destBuf)
{
	uint32_t remainLen;
	uint8_t resp;
	uint32_t oldPosition = rpos;
	
	remainLen = CacheRemaining();
	if(remainLen > PACKAGE_MINI_LEN)
	{
		COMFILEDEBUG("Have Data Len:%d\r\n", remainLen);
		resp = doDecode(destBuf);
		COMFILEDEBUG("resp:%d\r\n", resp);
		if(resp == DECODE_NOTAIL)
		{
			if(rpos > wpos)
				ClearBuff();
			else
			{
				rpos = oldPosition;
			}
			return resp;
		}
		else if(resp == DECODE_EMPTY || resp == DECODE_INVALID || resp == DECODE_REMAIN || resp == DECODE_OK)
		{
			if(rpos >= wpos)	//排除异常操作
				ClearBuff();
			else							//移位操作
				ShiftBuff();
			return resp;
		}
	}

	COMFILEDEBUG("Data Not Enougth To Parse!\r\n");
	return DECODE_EMPTY;
}

//打印当前数组的读写位置，调试用途
void PrintPos()
{
	COMFILEDEBUG("PrintPos:wpos:%d, rpos:%d\r\n", wpos, rpos);
}
