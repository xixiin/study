#ifndef __FREERTOS_DEMO__H
#define __FREERTOS_DEMO__H

#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "delay.h"
#include "uart1.h"
#include "stdio.h"
#include "key.h"

extern TaskHandle_t Task1Handle;
extern TaskHandle_t Task2Handle;
extern TaskHandle_t Task3Handle;

void FreeRTOS_Start(void);

#endif 
