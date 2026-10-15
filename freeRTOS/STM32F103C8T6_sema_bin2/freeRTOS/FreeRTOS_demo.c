#include "FreeRTOS_demo.h"
#include "delay.h"
#include "uart1.h"
#include <string.h>

void Delay_us(uint32_t us);  
//Task1特定类型的函数
// void receiveTask( void * arg );
void timeTask( void * arg );

void printf1(void *arg);
#define printf1_STACK_SIZE 128
#define printf1_PRIORITY 3
TaskHandle_t printf1Handle = NULL;

void printf2(void *arg);
#define printf2_STACK_SIZE 128
#define printf2_PRIORITY 3
TaskHandle_t printf2Handle = NULL;

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
    sem = xSemaphoreCreateBinary(); // Create a mutex semaphore
    //二值信号量的初始状态是“空”，所以我们需要在创建后立即释放它，以便任务可以获取它。
    xSemaphoreGive(sem); // Release the mutex semaphore to make it available
    xQueue = xQueueCreate(10, sizeof(uint8_t)); // Create a queue to hold 10 uint8_t items
    // Create tasks, queues, and other FreeRTOS objects here
    // xTaskCreate(receiveTask, "RECEIVE", receiveTask_STACK_SIZE, NULL, receiveTask_PRIORITY, &receiveTaskHandle);
    // xTaskCreate(timeTask, "TIME", timeTask_STACK_SIZE, NULL, timeTask_PRIORITY, &timeTaskHandle);
    xTaskCreate(printf1, "PRINTF1", printf1_STACK_SIZE, NULL, printf1_PRIORITY, &printf1Handle);
    xTaskCreate(printf2, "PRINTF2", printf2_STACK_SIZE, NULL, printf2_PRIORITY, &printf2Handle);
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

void printf1(void *arg)
{
    while(1)
    {
        //打印前获取信号量，确保互斥访问
        xSemaphoreTake(sem, portMAX_DELAY); // Wait for the semaphore to be available
        printf("This is printf1 task.========================\r\n");
        //打印后释放信号量，允许其他任务访问
        xSemaphoreGive(sem); // Release the semaphore
        vTaskDelay(1000);

    }
}

void printf2(void *arg)
{
    while(1)
    {
        //打印前获取信号量，确保互斥访问
        xSemaphoreTake(sem, portMAX_DELAY); // Wait for the semaphore to be available
        printf("This is printf2 task.++++++++++++++++++++++++++\r\n");
        //打印后释放信号量，允许其他任务访问
        xSemaphoreGive(sem); // Release the semaphore
        vTaskDelay(1000);
    }
}
