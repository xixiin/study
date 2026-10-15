#ifndef __LED_BOARD__H
#define __LED_BOARD__H

#include "framework.h"

typedef struct{
	void (*init)(void);
	void (*set_net)(int pwr);
	void (*set_fuse)(int pwr);
	void (*set_sock)(int road , int sta);
	void (*work)(uint32_t tick);
	
}led_board_opt_t;


extern led_board_opt_t* led_board_opt;

#endif
