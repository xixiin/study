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
#define Task2_PRIORITY 3
//Task1的堆栈大小
#define Task1_STACK_SIZE 128
//Task1的优先级
#define Task1_PRIORITY 3

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
        taskENTER_CRITICAL();
        printf("Task1 running...\r\n");
        taskEXIT_CRITICAL();
        vTaskDelay(500);

    }

}
void Task2( void * arg )
{
    while(1)
    {
        led_toggle2();
        taskENTER_CRITICAL();
        printf("Task2 running...\r\n");
        taskEXIT_CRITICAL();
        vTaskDelay(1000);
    }

}

uint8_t key1_flag = 0;
uint8_t led1_flag = 0;
uint8_t led2_flag = 0;
void Task3( void * arg )
{
    
    while(1)
    {
        key1_flag = key_scand();
        if(key1_flag == 1)
        {
            led1_flag = !led1_flag;
            if(led1_flag)
            {
                vTaskResume(Task1Handle);
            }
            else
            {
                vTaskSuspend(Task1Handle);
            }
        }
        else if(key1_flag == 2)
        {
            led2_flag = !led2_flag;
            if(led2_flag)
            {
                vTaskResume(Task2Handle);
            }
            else
            {
                vTaskSuspend(Task2Handle);
            }
        }
            vTaskDelay(10);
        

    }

}