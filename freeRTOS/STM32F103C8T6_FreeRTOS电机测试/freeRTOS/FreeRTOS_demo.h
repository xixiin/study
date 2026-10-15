#ifndef __FREERTOS_DEMO__H
#define __FREERTOS_DEMO__H

#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "delay.h"
#include "uart1.h"
#include "stdio.h"
#include "Key.h"
#include "queue.h"
#include "Encoder.h"
#include "RT.h"
#include "Motor.h"
#include "OLED.h"

extern TaskHandle_t sendTaskHandle;
extern TaskHandle_t receiveTaskHandle;
extern TaskHandle_t OLEDTaskHandle;
extern QueueHandle_t xQueue;
extern TaskHandle_t keyTaskHandle;

void FreeRTOS_Start(void);

#endif 
