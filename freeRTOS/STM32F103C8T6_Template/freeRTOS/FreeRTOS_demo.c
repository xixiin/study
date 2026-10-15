#include "FreeRTOS_demo.h"

TaskHandle_t Task1Handle;
#define TASK1_STACK_SIZE 128
#define TASK1_STACK_PRIORITY 1
void Task1(void *pvParameters);
StaticTask_t Task1Buffer;
StackType_t Task1Stack[TASK1_STACK_SIZE];

//空闲任务分配空间

#define IDLE_TASK_STACK_SIZE 128
StaticTask_t IdleTaskTCB;
StackType_t IdleTaskStack[IDLE_TASK_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
    *ppxIdleTaskTCBBuffer = &IdleTaskTCB;
    *ppxIdleTaskStackBuffer = IdleTaskStack;
    *pulIdleTaskStackSize = IDLE_TASK_STACK_SIZE;
}

void FreeRTOS_Start(void)
{
    // Create tasks, queues, and other FreeRTOS objects here
     Task1Handle = xTaskCreateStatic(Task1, "Task1", TASK1_STACK_SIZE, NULL, TASK1_STACK_PRIORITY, Task1Stack, &Task1Buffer);
    // Start the scheduler
    vTaskStartScheduler();

    // If all is well, the scheduler will now be running, and the following line will never be reached.
    // If the following line does execute, then there was insufficient FreeRTOS heap memory available
    // for the idle and/or timer tasks to be created.
    for(;;);
}


void Task1(void *pvParameters)
{
    for (;;)
    {
        // Task code goes here
        led_toggle1(); // Example function to toggle an LED
        vTaskDelay(1000);
        // For example, toggle an LED or send data to a queue
    }
}