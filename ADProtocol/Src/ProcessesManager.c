#include "ProcessesManager.h"
#include "stdlib.h"

//暂时不用修改

PROCESSESLISTENER * listeners;
int * listenerCommands;
int listenerCount = 0;	//已经注册的监听器命令个数
int listenerSize =  0;  //存放监听器命令数组的长度
int listenerCommandSizeStep = PROCESSES_MANAGER_LEN;

void RegisterProcesses(int32_t command, PROCESSESLISTENER parser)
{
	if(listenerCount >= listenerSize)
	{
		PROCESSESLISTENER * tmpparsers = malloc(sizeof(PROCESSESLISTENER)*(listenerSize + listenerCommandSizeStep));
		int32_t * tmpcommands = malloc(sizeof(int32_t)*(listenerSize + listenerCommandSizeStep));
		for(int32_t n=0;n<listenerSize;n++){
				tmpparsers[n] = listeners[n];
				tmpcommands[n] = listenerCommands[n];
		}
		if(listenerSize > 0){			//防止内存泄露
				free(tmpparsers);
				free(listenerCommands);
		}
		listeners = tmpparsers;
		listenerCommands = tmpcommands;
		listenerSize += listenerCommandSizeStep;
	}
	listeners[listenerCount] = parser;
	listenerCommands[listenerCount] = command;
	listenerCount++;
}


void Process(uint32_t tmpCommand)
{
	//包解析，byte流转换成包
	uint32_t command = tmpCommand;

	for(int n=0;n<listenerCount;n++){
			if(listenerCommands[n] == command){
					listeners[n]();
					return;
			}
	}

	printf("can not find the command:%x\r\n",command);
}
