#ifndef __EXTI_H__
#define __EXTI_H__

#include "stdint.h"

#define TRUE 1
#define FALSE 0

void exti_init(void);
uint8_t get_buttonA_flag(void);
void set_buttonA_flag(uint8_t value);
uint8_t get_buttonB_flag(void);
void set_buttonB_flag(uint8_t value);
uint8_t vibrate_flag_get(void);
void set_vibrate_flag(uint8_t value);


#endif
