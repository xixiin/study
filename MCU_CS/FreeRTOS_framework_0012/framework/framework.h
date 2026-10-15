#ifndef __framework__H
#define __framework__H


#include "SWM320.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "task_lisen.h"
#include "task_host.h"
#include "task_ctrl.h"
#include "stdarg.h"
#include "string.h"
#include "list.h"
#include "semphr.h"

#define NetRST 	56
#define NetUart _e_uart1

#include "fk_typedef.h"


extern void myprintf( char* format,...);

#endif
