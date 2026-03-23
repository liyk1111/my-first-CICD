#include "appPublic.h"
#include "bspPort.h"
#include "stdlib.h"

//全局变量定义


//此函数内不要添加全局变量
uint8_t SendMsgToAppQMsg(void *QMsg,uint8_t MsgType,uint32_t Value,void *ptr,uint32_t ptrlen)
{
	uint8_t res = YES;
    MsgApp *appmsg = NULL;
    osStatus err;

    appmsg = malloc(sizeof(MsgApp));
    if(appmsg == NULL)
    {
        printf("消息内存分配失败 消息类型 %d\r\n",MsgType);
        if (ptrlen>0) free(ptr);

        res = NO;
    }
    else
    {
        appmsg->type  = MsgType;
        appmsg->value = Value;
        appmsg->ptr = ptr;
        appmsg->ptrlen = ptrlen;
		 //printf("appmsg:%p,%d\r\n",appmsg,MsgType);
        err = osMessagePut(QMsg, (uint32_t)appmsg, 1000);
       
        //err=OSQPost(QMsg,appmsg);	//发送队列
        if(err!=osOK) 	    //发送失败
        {
            printf("消息发送失败 消息类型 %d\r\n",MsgType);
			printf("消息发送失败 err= %d\r\n",err);
            free(appmsg);	//释放内存
            if (ptrlen>0) free(ptr);
            res = NO;
        }
        appmsg = NULL;
    }
    return res;
}


void PrintInfo()
{

}

uint8_t DetectPowerKey()
{
	return GPIOReadInputPinLevel(POWERKEY_GPIO_PORT, POWERKEY_PIN);
}

void WatiPowerKeyRelease()
{
	while(1)
	{
		HAL_Delay(30);
		if(DetectPowerKey() == POWERKEY_RELEASE)
			break;
	}
	
	printf("PowerKeyRelease!\r\n");
	
	//开机检测为0
	if(osSemaphoreGetCount(myShutDownSemHandle) == 1)
	{
		printf("ShutDown:1.\r\n");
	}
	else
		printf("ShutDown:0.\r\n");
}

