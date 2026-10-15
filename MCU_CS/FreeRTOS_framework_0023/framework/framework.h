#ifndef __framework__H
#define __framework__H


#include "SWM320.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stdarg.h"
#include "string.h"
#include "stdlib.h"
#include "list.h"
#include "semphr.h"

#define NetRST 	56
#define NetUart _e_uart1

#define IO_HC_DAT 36
#define IO_HC_CLK 35
#define IO_HC_UD  37
	
#define MaxSock	2	//有2个插座
#define SockLed	7

#define IO_FUSE  21



#define IO_HLW0  22
#define IO_HLW1  20

#define IO_ELC0  26
#define IO_ELC1  3

#include "fk_typedef.h"

#include "task_lisen.h"
#include "task_host.h"
#include "task_ctrl.h"


extern void myprintf( char* format,...);

#endif
