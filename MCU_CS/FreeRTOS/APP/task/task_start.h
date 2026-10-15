#ifndef __TASK_START__H
#define __TASK_START__H

#include "SWM320.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "task_lisen.h"
#include "task_host.h"
#include "task_ctrl.h"
#include "stdarg.h"

void creat_task(void);
void cslock_init(void);
void cslock_get(void);
void cslock_free(void);
void myprintf( char* format,...);

#endif
