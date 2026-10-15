#ifndef __FREERTOS_DEMO__H
#define __FREERTOS_DEMO__H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "led.h"
#include "delay.h"
#include "uart1.h"
#include "stdio.h"
#include "key.h"
#include "queue.h"
#include "string.h"
#include "event_groups.h"

extern uint8_t uart1_rx_buf[];
extern TaskHandle_t sendTaskHandle;
extern TaskHandle_t receiveTaskHandle;
extern TaskHandle_t timeTaskHandle;
extern QueueHandle_t xQueue;
extern QueueHandle_t uartQueue;
extern QueueHandle_t item;  /* 来自FreeRTOS_demo.c的二值信号量 */
void FreeRTOS_Start(void);

#endif 
