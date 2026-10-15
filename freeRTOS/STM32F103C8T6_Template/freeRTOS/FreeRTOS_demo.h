#ifndef __FREERTOS_DEMO__H
#define __FREERTOS_DEMO__H

#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "delay.h"
#include "uart1.h"
#include "stdio.h"

extern TaskHandle_t Task1Handle;
extern TaskHandle_t Task2Handle;

void FreeRTOS_Start(void);

#endif 
