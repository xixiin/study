#include "FreeRTOS_demo.h"
#include "delay.h"
#include "uart1.h"
#include <string.h>

void Delay_us(uint32_t us);  
//Task1特定类型的函数
// void receiveTask( void * arg );
void timeTask( void * arg );

void hightask(void *arg);
#define hightask_STACK_SIZE 128
#define hightask_PRIORITY 3
TaskHandle_t hightaskHandle = NULL;

void lowtask(void *arg);
#define lowtask_STACK_SIZE 128
#define lowtask_PRIORITY 1
TaskHandle_t lowtaskHandle = NULL;

void middletask(void *arg);
#define middletask_STACK_SIZE 128
#define middletask_PRIORITY 2
TaskHandle_t middletaskHandle = NULL;

//timeTask的堆栈大小
#define timeTask_STACK_SIZE 128
//timeTask的优先级
#define timeTask_PRIORITY 3
//receiveTask的堆栈大小
#define receiveTask_STACK_SIZE 128
//receiveTask的优先级
#define receiveTask_PRIORITY 3



TaskHandle_t receiveTaskHandle = NULL;
TaskHandle_t timeTaskHandle = NULL;
QueueHandle_t xQueue ;
QueueHandle_t item;
QueueHandle_t sem;
void FreeRTOS_Start(void)
{
    item = xSemaphoreCreateBinary(); // Create a binary semaphore
    // sem = xSemaphoreCreateBinary(); // Create a mutex semaphore
    sem = xSemaphoreCreateMutex(); // Create a mutex semaphore
    //二值信号量的初始状态是“空”，所以我们需要在创建后立即释放它，以便任务可以获取它。
    xSemaphoreGive(sem); // Release the mutex semaphore to make it available
    xQueue = xQueueCreate(10, sizeof(uint8_t)); // Create a queue to hold 10 uint8_t items
    // Create tasks, queues, and other FreeRTOS objects here
    // xTaskCreate(receiveTask, "RECEIVE", receiveTask_STACK_SIZE, NULL, receiveTask_PRIORITY, &receiveTaskHandle);
    // xTaskCreate(timeTask, "TIME", timeTask_STACK_SIZE, NULL, timeTask_PRIORITY, &timeTaskHandle);
    xTaskCreate(hightask, "HIGHTASK", hightask_STACK_SIZE, NULL, hightask_PRIORITY, &hightaskHandle);
    xTaskCreate(lowtask, "LOWTASK", lowtask_STACK_SIZE, NULL, lowtask_PRIORITY, &lowtaskHandle);
    xTaskCreate(middletask, "MIDDLETASK", middletask_STACK_SIZE, NULL, middletask_PRIORITY, &middletaskHandle);

    // Start the scheduler
    vTaskStartScheduler();

    // If all is well, the scheduler will now be running, and the following line will never be reached.
    // If the following line does execute, then there was insufficient FreeRTOS heap memory available
    // for the idle and/or timer tasks to be created.
    for(;;);
}
typedef enum
{
    key_NONE = 0,
    key1 = 1,
    key2 = 2,
    key3 = 3
} key_number;
uint8_t key_value=0;

// void receiveTask( void * arg )
// {

//     while(1)
//     {
//         /* 等待信号量：ISR 收到完整一帧数据后会释放 */
//         BaseType_t result = xSemaphoreTake(item, portMAX_DELAY);
//         if(result == pdTRUE)
//         {
//             /* 处理接收到的数据（uart1_rx_buf 在 ISR 中已填充并添加了 '\0'） */
//             printf("Received: %s\r\n", (char*)uart1_rx_buf);
//             /* 清空缓冲区，准备下一次接收 */
//             uart1_rx_clear();
//         }
//     }
        

// }
void Delay_us(uint32_t us)         
{
    uint32_t count = us*72/9;
    while(count--)
    {
        __NOP();
    }

}
uint8_t tasktime[128];
void timeTask( void * arg )
{
    while(1)
    {
        taskENTER_CRITICAL();
        vTaskGetRunTimeStatistics((char*)tasktime, sizeof(tasktime));
        printf("Task run time statistics:\r\n%s\r\n", tasktime);
        taskEXIT_CRITICAL();
        vTaskDelay(2000);
    }

}

void hightask(void *arg)
{
    vTaskDelay(50);
    xSemaphoreTake(sem, portMAX_DELAY); // Take the mutex semaphore
    for(uint8_t i = 0; i < 10; i++)
    {
        printf("High Task is running: %d\r\n", i);
    }
    xSemaphoreGive(sem); // Release the mutex semaphore
    vTaskDelete(NULL); // Delete the task when done
}

void lowtask(void *arg)
{
    xSemaphoreTake(sem, portMAX_DELAY); // Take the mutex semaphore
    for(uint8_t i = 0; i < 10; i++)
    {
        printf("Low Task is running: %d\r\n", i);
        Delay_us(100000);
    }

    xSemaphoreGive(sem); // Release the mutex semaphore
    vTaskDelete(NULL); // Delete the task when done
}

void middletask(void *arg)
{
    vTaskDelay(10);
    for(uint8_t i = 0; i < 10; i++)
    {
        led_toggle1();
        Delay_us(500000);
    }
    vTaskDelete(NULL); // Delete the task when done
}
