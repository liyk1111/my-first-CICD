#include "ADLinkList.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
/*
	V1.0.1 2023-10-17 第一版。

*/
/*第一个元素不使用*/
void *ADListLinkCreat(uint8_t ItemSize)
{
	ADLinkList *xADLinkList=NULL;
	xADLinkList = ADLinkList_Malloc(sizeof(ADLinkList));
	if(xADLinkList!=NULL)
	{
		xADLinkList->LinkListHead 		= 	NULL;
		xADLinkList->ItemSize		=	ItemSize;
	}
	return xADLinkList;
}

void ADLinkListPrintf(void *xADLinkList,uint8_t type)
{
	ADLinkListElm *p;
	uint16_t ii = 1;
	
	if(xADLinkList==NULL)
	return ;
	
	printf ("打印链表:\r\n");
	p = ((ADLinkList*)xADLinkList)->LinkListHead;
	
	while(p!=NULL)
	{
		switch(type)
		{
			case ADLinkList_uint8_t:	printf("%d,%d\r\n",ii,*((uint8_t*)p->LinkListBody));									break;
			case ADLinkList_uint16_t:	printf("%d,%d\r\n",ii,*((uint16_t*)p->LinkListBody));									break;
			case ADLinkList_uint32_t:	printf("%d,%d\r\n",ii,*((uint32_t*)p->LinkListBody));									break;
			case ADLinkList_float:		printf("%d,%f\r\n",ii,*((float*)p->LinkListBody));										break;
			case ADLinkList_double:		printf("%d,%f\r\n",ii,*((double*)p->LinkListBody));										break;
			case ADLinkList_char:		printf("%d,%c\r\n",ii,*((char*)p->LinkListBody));										break;
			case ADLinkList_string:		printf("%d,%.*s\r\n",ii,((ADLinkList*)xADLinkList)->ItemSize,((char*)p->LinkListBody));	break;			
		}	
		ii++;
		p=p->next;
	}
	
	printf("\r\n");
}
uint8_t ADLinkListInsert(void *xADLinkList,uint16_t n,void *e)
{
	 uint16_t ii = 1;
	 ADLinkListElm *p,*newElm;
	
	if(xADLinkList==NULL)
	return ADLinkList_ERR;
	
	p = ((ADLinkList*)xADLinkList)->LinkListHead;
	
	/*找到第N个元素*/
	if(n == 1)
	{
		/*创建并插入新元素*/
		newElm =  ADLinkList_Malloc(sizeof(ADLinkListElm));
		
		if(newElm!=NULL)
		{
			newElm->LinkListBody =  ADLinkList_Malloc(((ADLinkList*)xADLinkList)->ItemSize);			
			if(newElm->LinkListBody!=NULL)
			{
				
				memcpy(newElm->LinkListBody,e,((ADLinkList*)xADLinkList)->ItemSize);
			
				newElm->next = ((ADLinkList*)xADLinkList)->LinkListHead;
				((ADLinkList*)xADLinkList)->LinkListHead = newElm;
				return ADLinkList_OK;
			}
			else 
			{
				ADLinkList_Free(newElm);
				printf("堆不足\r\n");
				return ADLinkList_ERR;
			}
		}
		else 
		{
			printf("堆不足\r\n");
			return ADLinkList_ERR;
		}
	}
	else 
	{
		while(1)
		{
			if(p == NULL||ii>=(n-1))
				break;
			else 
			{
				p = p->next;
				ii++;	
			}
		}
		//不存在这个元素
		if(p == NULL)				
		{
			printf("不存在这个元素\r\n");
			return ADLinkList_ERR;
		}
		/*创建并插入新元素*/
		newElm =  ADLinkList_Malloc(sizeof(ADLinkListElm));
	
		if(newElm!=NULL)
		{
			newElm->LinkListBody =  ADLinkList_Malloc(((ADLinkList*)xADLinkList)->ItemSize);
		
			if(newElm->LinkListBody!=NULL)
			{
				memcpy(newElm->LinkListBody,e,((ADLinkList*)xADLinkList)->ItemSize);
			
				newElm->next = p->next;
				p->next = newElm;
				return ADLinkList_OK;
			}
			else 
			{
				ADLinkList_Free(newElm);
				printf("堆不足\r\n");
				return ADLinkList_ERR;
			}
		}
		else 
		{
			printf("堆不足\r\n");
			return ADLinkList_ERR;
		}
	}
		
}
/*删除并获取元素*/
uint8_t ADLinkListDelete(void *xADLinkList,uint16_t n,void *e)
{
	 uint16_t ii = 1;
	 ADLinkListElm *p,*q;
	
	if(xADLinkList==NULL)
	return ADLinkList_ERR;
	
	p = ((ADLinkList*)xADLinkList)->LinkListHead;
	
	if(n == 1)
	{
		if(p!=NULL)
		{
			((ADLinkList*)xADLinkList)->LinkListHead = p->next;
			memcpy(e,p->LinkListBody,((ADLinkList*)xADLinkList)->ItemSize);
			ADLinkList_Free(p->LinkListBody);
			ADLinkList_Free(p);
			return ADLinkList_OK;
		}
		else 
		{
			printf("不存在这个元素\r\n");
			return ADLinkList_ERR;
		}
	}
	else 
	{
	
		while(1)
		{
			if(p == NULL||ii>=n-1)
				break;
			else 
			{
				p = p->next;
				ii++;	
			}
		}
		//不存在这个元素
		if(p == NULL)				
		{
			printf("不存在这个元素\r\n");
			return ADLinkList_ERR;
		}
		else 
		{
			q = p->next;
			
			p->next = q->next;
			
			memcpy(e,q->LinkListBody,((ADLinkList*)xADLinkList)->ItemSize);
			
			ADLinkList_Free(q->LinkListBody);
			ADLinkList_Free(q);
			return ADLinkList_OK;
		}
	}
	
}
/*获取链表长度*/
uint16_t ADLinkListGetLenght(void *xADLinkList)
{
	 uint16_t Lenght = 0;
	 ADLinkListElm *p;
	
	if(xADLinkList==NULL)
	return 0;
	
	p = ((ADLinkList*)xADLinkList)->LinkListHead;
	while(p==NULL)
	{
		Lenght++;
		p = p->next;
	}
	return Lenght;
}
/*链表求和*/
uint8_t  ADLinkListSum(void *xADLinkList,uint8_t type,void *Sum)
{
	 uint16_t Lenght = 0;
	 ADLinkListElm *p;
	uint32_t temp32 = 0;
	float tempfloat = 0;
	double tempdouble = 0;
	if(xADLinkList==NULL)
	return ADLinkList_ERR;
	
	p = ((ADLinkList*)xADLinkList)->LinkListHead;
	while(p!=NULL)
	{
		switch(type)
		{
			case ADLinkList_uint8_t:	
			{						
				temp32 = temp32 +	*((uint8_t*)p->LinkListBody);			
			}
			break;
			case ADLinkList_uint16_t:	
			{
				temp32 = temp32 +	*((uint16_t*)p->LinkListBody);				
			}
			break;
			case ADLinkList_uint32_t:	
			{
				temp32 = temp32 +	*((uint32_t*)p->LinkListBody);						
			}break;
			case ADLinkList_float:		
			{				
				tempfloat = tempfloat +	*((float*)p->LinkListBody);		
			}
			break;
			case ADLinkList_double:
			{
				tempdouble = tempdouble +	*((double*)p->LinkListBody);			
			}break;				
		}
		Lenght++;
		p = p->next;
	}
	printf ("tempfloat=%f\r\n",tempfloat);
	switch(type)
	{
		case ADLinkList_uint8_t:	
		{
			*(uint32_t*)Sum = temp32;
		}
		break;
		case ADLinkList_uint16_t:	
		{
			*(uint32_t*)Sum = temp32;			
		}
		break;
		case ADLinkList_uint32_t:	
		{
			*(uint32_t*)Sum = temp32;					
		}break;
		case ADLinkList_float:		
		{
			*(float*)Sum = tempfloat;	
		}
		break;
		case ADLinkList_double:
		{
			*(double*)Sum = tempdouble;	
		}break;				
	}
	return ADLinkList_OK;
}

/*链表求平均*/
uint8_t  ADLinkListAverage(void *xADLinkList,uint8_t type,void *average)
{
	  uint16_t Lenght = 0;
	 ADLinkListElm *p;
	uint32_t temp32 = 0;
	float tempfloat = 0;
	double tempdouble = 0;
	if(xADLinkList==NULL)
	return ADLinkList_ERR;
	
	p = ((ADLinkList*)xADLinkList)->LinkListHead;
	while(p!=NULL)
	{
		switch(type)
		{
			case ADLinkList_uint8_t:	
			{						
				temp32 = temp32 +	*((uint8_t*)p->LinkListBody);			
			}
			break;
			case ADLinkList_uint16_t:	
			{
				temp32 = temp32 +	*((uint16_t*)p->LinkListBody);				
			}
			break;
			case ADLinkList_uint32_t:	
			{
				temp32 = temp32 +	*((uint32_t*)p->LinkListBody);						
			}break;
			case ADLinkList_float:		
			{				
				tempfloat = tempfloat +	*((float*)p->LinkListBody);		
			}
			break;
			case ADLinkList_double:
			{
				tempdouble = tempdouble +	*((double*)p->LinkListBody);			
			}break;				
		}
		Lenght++;
		p = p->next;
	}
	switch(type)
	{
		case ADLinkList_uint8_t:	
		{
			*(uint32_t*)average = temp32/((ADLinkList*)xADLinkList)->ItemSize;
		}
		break;
		case ADLinkList_uint16_t:	
		{
			*(uint32_t*)average = temp32/((ADLinkList*)xADLinkList)->ItemSize;;			
		}
		break;
		case ADLinkList_uint32_t:	
		{
			*(uint32_t*)average = temp32/((ADLinkList*)xADLinkList)->ItemSize;;					
		}break;
		case ADLinkList_float:		
		{
			*(float*)average = tempfloat/((ADLinkList*)xADLinkList)->ItemSize;;	
		}
		break;
		case ADLinkList_double:
		{
			*(double*)average = tempdouble/((ADLinkList*)xADLinkList)->ItemSize;
		}break;				
	}
	return ADLinkList_OK;
}
