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
#include "semphr.h"
#include "Encoder.h"
#include "Motor.h"
#include "OLED.h"
#include "PID.h"
#include "MPU6050.h"
#include "bluetoos.h"
#include "string.h"

extern TaskHandle_t sendTaskHandle;
extern TaskHandle_t receiveTaskHandle;
extern TaskHandle_t OLEDTaskHandle;
extern QueueHandle_t xQueue;
extern TaskHandle_t keyTaskHandle;
extern SemaphoreHandle_t xPIDMutex;
extern SemaphoreHandle_t xBluetoothSemaphore;

void FreeRTOS_Start(void);

#endif 
