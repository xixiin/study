#ifndef __LED_H__
#define __LED_H__

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "cJSON.h"
#define LED_RED_ON (0x01<<0)
#define LED_GREEN_ON (0x01<<1)
#define LED_BLUE_ON (0x01<<2)

#define LED_RED_OFF (0x01<<4)
#define LED_GREEN_OFF (0x01<<3)
#define LED_BLUE_OFF (0x01<<4)
int led_init(void);
void state_led_set(rt_uint32_t state);

#endif // __LED_H__
