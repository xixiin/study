#ifndef __RELAY__H
#define __RELAY__H

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "cJSON.h"


void relay_send(char *msg);
int relay_init(void);

#endif // !__RELAY__H
