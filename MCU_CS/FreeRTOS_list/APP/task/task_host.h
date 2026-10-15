#ifndef __TASK_HOST__H
#define __TASK_HOST__H

#include "SWM320.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "task_start.h"

void host_task(void);

#endif
