#include "PackageManager.h"
#include "stdlib.h"

PACKAGEPARSER * parsers;
int32_t * commands;
int32_t parserCount = 0;		//已经注册的命令个数
int32_t parserSize =  0;		//进入这个的数组有多大
int32_t commandSizeStep = PACKAGE_MANAGER_LEN;

void RegisterPackage(int32_t command, PACKAGEPARSER parser)
{
	if(parserCount >= parserSize)
	{
		PACKAGEPARSER * tmpparsers = malloc(sizeof(PACKAGEPARSER)*(parserSize + commandSizeStep));
		int32_t * tmpcommands = malloc(sizeof(int32_t)*(parserSize + commandSizeStep));
		for(int32_t n=0;n<parserSize;n++){
				tmpparsers[n] = parsers[n];
				tmpcommands[n] = commands[n];
		}
		if(parserSize > 0){			//防止内存泄露
				free(tmpparsers);
				free(commands);
		}
		parsers = tmpparsers;
		commands = tmpcommands;
		parserSize += commandSizeStep;
	}
	parsers[parserCount] = parser;
	commands[parserCount] = command;
	parserCount++;
}


uint32_t Parse(uint8_t dest[])
{
	//包解析，byte流转换成包
	uint32_t command;
	uint32_t packageBodySize;
	uint32_t sequenceid;

	int offset = 0;
	offset = read_uint32((uint8_t *)dest, &command, offset);
	offset = read_uint32((uint8_t *)dest, &packageBodySize, offset);
	offset = read_uint32((uint8_t *)dest, &sequenceid, offset);
	for(int n=0;n<parserCount;n++){
			if(commands[n] == command){
					return parsers[n](dest);
			}
	}

	printf("can not find the command:%x\r\n",command);
	return NULL;
}
