#ifndef __FREERTOS_DEMO__H
#define __FREERTOS_DEMO__H

#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "delay.h"
#include "uart1.h"
#include "stdio.h"
#include "key.h"
#include "queue.h"

extern TaskHandle_t sendTaskHandle;
extern TaskHandle_t receiveTaskHandle;
extern TaskHandle_t timeTaskHandle;
extern QueueHandle_t xQueue;

void FreeRTOS_Start(void);

#endif 
