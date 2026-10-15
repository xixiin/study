#include "FreeRTOS_demo.h"



//Task1特定类型的函数
void Task1( void * arg );
void Task2( void * arg );
void Task3( void * arg );
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
TaskHandle_t Task3Handle = NULL;
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
    xTaskCreate(Task3, "TASK3", Task3_STACK_SIZE, NULL, Task3_PRIORITY, &Task3Handle);

    vTaskDelete(NULL); // Delete the start_task itself after creating other tasks
}

void Task1( void * arg )
{
    while(1)
    {
        led_toggle1();
        //FreeRTOS提供的延时函数
        vTaskDelay(500);

    }

}
void Task2( void * arg )
{
    while(1)
    {
        led_toggle2();
        vTaskDelay(1000);
    }

}

uint8_t key1_flag = 0;
// void Task3( void * arg )
// {
//     while(1)
//     {
//         key1_flag = key_scand();
//         if(key1_flag == 1)
//         {
//             //获取优先级
//             UBaseType_t priority1 = uxTaskPriorityGet(Task1Handle);
//             taskENTER_CRITICAL();
//             printf("Task1 优先级: %d\r\n", priority1);
//             taskEXIT_CRITICAL();
//             UBaseType_t priority2 = uxTaskPriorityGet(Task2Handle);
//             taskENTER_CRITICAL();
//             printf("Task2 优先级: %d\r\n", priority2);
//             taskEXIT_CRITICAL();
//              UBaseType_t priority3 = uxTaskPriorityGet(Task3Handle);
//             taskENTER_CRITICAL();
//             printf("Task3 优先级: %d\r\n", priority3);
//             taskEXIT_CRITICAL();
//         }
//         else if(key1_flag == 2)
//         {
//             vTaskPrioritySet(Task1Handle, 3);
//         }
//             vTaskDelay(10);
//     }
// }
void Task3( void * arg )
{
        UBaseType_t numTasks = uxTaskGetNumberOfTasks();
        taskENTER_CRITICAL();
        printf("当前任务数量: %lu\r\n", (unsigned long)numTasks);
        taskEXIT_CRITICAL();
    while(1)
    {
        key1_flag = key_scand();
        if(key1_flag == 1)
        {
           UBaseType_t numTasks = uxTaskGetNumberOfTasks();
           taskENTER_CRITICAL();
           printf("当前任务数量: %lu\r\n", (unsigned long)numTasks);
            taskEXIT_CRITICAL();
        }
        if(key1_flag == 2)
        {
        //获取当前任务句柄
            TaskHandle_t currentTaskHandle = xTaskGetCurrentTaskHandle();
        //获取任务句柄
            TaskHandle_t task_Handle = xTaskGetHandle("TASK1");
            vTaskDelete(task_Handle); // 删除任务1
        }

      
        vTaskDelay(10);
    }
}