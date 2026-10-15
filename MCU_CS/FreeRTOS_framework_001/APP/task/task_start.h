#ifndef __TASK_START__H
#define __TASK_START__H


#include "framework.h"

void creat_task(void);
void cslock_init(void);
void cslock_get(void);
void cslock_free(void);
void myprintf( char* format,...);

#endif
