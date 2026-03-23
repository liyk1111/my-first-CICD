#include "retarget.h"
#include "stdio.h"
#include "usartx.h"

#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)


#else
	struct __FILE 
	{ 
		int handle; 
	}; 
#endif
	
FILE __stdout;       

void _sys_exit(int x) 
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)
{ 	
	Usart1_Send_Data((uint8_t *)&ch,1);
	return ch;
}
