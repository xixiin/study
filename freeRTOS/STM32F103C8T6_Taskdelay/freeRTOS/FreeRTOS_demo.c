#include "FreeRTOS_demo.h"



//Task1特定类型的函数
void Task1( void * arg );
void Task2( void * arg );
void start_task(void *arg);

//start_task的堆栈大小
#define START_TASK_STACK_SIZE 128
//start_task的优先级
#define START_TASK_PRIORITY 4
//Task3的堆栈大小
#define Task3_STACK_SIZE 128
//Task3的优先级
#define Task3_PRIORITY 3
//Task2的堆栈大小
#define Task2_STACK_SIZE 128
//Task2的优先级
#define Task2_PRIORITY 2
//Task1的堆栈大小
#define Task1_STACK_SIZE 128
//Task1的优先级
#define Task1_PRIORITY 1

TaskHandle_t Task1Handle = NULL;
TaskHandle_t Task2Handle = NULL;
TaskHandle_t StartTaskHandle = NULL;

void FreeRTOS_Start(void)
{
    // Create tasks, queues, and other FreeRTOS objects here
    xTaskCreate(start_task, "start_task", START_TASK_STACK_SIZE, NULL, START_TASK_PRIORITY, &StartTaskHandle);
    // Start the scheduler
    vTaskStartScheduler();

    // If all is well, the scheduler will now be running, and the following line will never be reached.
    // If the following line does execute, then there was insufficient FreeRTOS heap memory available
    // for the idle and/or timer tasks to be created.
    for(;;);
}

void start_task(void *arg)
{
    xTaskCreate(Task1, "TASK1", Task1_STACK_SIZE, NULL, Task1_PRIORITY, &Task1Handle);
    xTaskCreate(Task2, "TASK2", Task2_STACK_SIZE, NULL, Task2_PRIORITY, &Task2Handle);


    vTaskDelete(NULL); // Delete the start_task itself after creating other tasks
}
void Delay_us(uint32_t us)         
{
    uint32_t count = us*72/9;
    while(count--)
    {
        __NOP();
    }

}
void Task1( void * arg )
{
    while(1)
    {
        led_toggle1();
        //FreeRTOS提供的延时函数
        Delay_us(50000);
        vTaskDelay(500);

    }

}
void Task2( void * arg )
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while(1)
    {
        led_toggle2();
        Delay_us(50000);
        vTaskDelayUntil(&xLastWakeTime, 500);
    }

}

