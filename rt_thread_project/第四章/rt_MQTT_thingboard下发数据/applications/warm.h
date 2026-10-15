#ifndef __WARM__H
#define __WARM__H

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "cJSON.h"

#define TEMP_WARM  (0x01<<0)
#define HUMI_WARM  (0x01<<1)
#define CURRENT_WARM  (0x01<<2)
#define VOLTAGE_WARM  (0x01<<3)

int warm_init(void);
void warm_set(rt_uint32_t state);

#endif // !__WARM__H

