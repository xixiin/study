#ifndef __FREERTOS_DEMO__H
#define __FREERTOS_DEMO__H

/* ============================================================
 *  FreeRTOS_demo.h : application interface
 * ============================================================ */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "timers.h"

/* Create all objects/tasks and start the scheduler. */
void FreeRTOS_Start(void);

/* Called from the key EXTI ISR (see README step 3). */
void FreeRTOS_KeyISR(void);

#endif /* __FREERTOS_DEMO__H */
