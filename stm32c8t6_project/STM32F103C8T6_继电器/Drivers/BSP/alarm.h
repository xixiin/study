#ifndef __ALARM_H__
#define __ALARM_H__

#include "stdint.h"

#define alarm_status_on 0
#define alarm_statue_off 1


void alarm_init(void);
void alarm_on(void);
void alarm_off(void);
uint8_t get_alarm_status(void);


#endif
