#ifndef __PWM_H__
#define __PWM_H__

#include "sys.h"
void pwm_init(uint16_t ARR,uint16_t PSC);
void pwm_compare_set(uint16_t val);
void sg90_angle_set(uint16_t angle);
	
#endif
