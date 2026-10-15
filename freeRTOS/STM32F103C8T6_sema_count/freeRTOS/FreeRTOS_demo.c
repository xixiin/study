#include "FreeRTOS_demo.h"
#include "delay.h"
#include "uart1.h"
#include <string.h>

void Delay_us(uint32_t us);  
//Task1特定类型的函数
// void receiveTask( void * arg );
void timeTask( void * arg );

void key(void *arg);
#define key_STACK_SIZE 128
#define key_PRIORITY 3
TaskHandle_t keytaskHandle = NULL;



void led(void *arg);
#define led_STACK_SIZE 128
#define led_PRIORITY 3
TaskHandle_t ledtaskHandle = NULL;

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
#define countMAX 3
void FreeRTOS_Start(void)
{
    item = xSemaphoreCreateBinary(); // Create a binary semaphore
    // sem = xSemaphoreCreateBinary(); // Create a mutex semaphore
    sem = xSemaphoreCreateCounting(countMAX, 0); // Create a counting semaphore with a maximum count of 10 and an initial count of 0
    
    xQueue = xQueueCreate(10, sizeof(uint8_t)); // Create a queue to hold 10 uint8_t items
    // Create tasks, queues, and other FreeRTOS objects here
    // xTaskCreate(receiveTask, "RECEIVE", receiveTask_STACK_SIZE, NULL, receiveTask_PRIORITY, &receiveTaskHandle);
    // xTaskCreate(timeTask, "TIME", timeTask_STACK_SIZE, NULL, timeTask_PRIORITY, &timeTaskHandle);
    xTaskCreate(key, "KEY", key_STACK_SIZE, NULL, key_PRIORITY, &keytaskHandle);
    xTaskCreate(led, "LED", led_STACK_SIZE, NULL, led_PRIORITY, &ledtaskHandle);

    // Start the scheduler
    vTaskStartScheduler();

    // If all is well, the scheduler will now be running, and the following line will never be reached.
    // If the following line does execute, then there was insufficient FreeRTOS heap memory available
    // for the idle and/or timer tasks to be created.
    for(;;);
}


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

uint8_t key_value=0;
void key(void *arg)
{
    while(1)
    {
        key_value = key_scand();
        if(key_value == 1)
        {
            //累计的数值打到最大值后就会变了
            xSemaphoreGive(sem); // Take the mutex semaphore
        }
    }
}

void led(void *arg)
{
    while(1)
    {
        //获取信号量的值
        uint32_t sem_count = uxSemaphoreGetCount(sem);
        if(sem_count == countMAX)
        {
                led_toggle1();
                vTaskDelay(1000);
                led_toggle1();
                vTaskDelay(1000);
            for(int i = 0; i < countMAX; i++)
            {
                xSemaphoreTake(sem, portMAX_DELAY); // Take the counting semaphore
            }
        }

        
    }
}


