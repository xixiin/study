#include "FreeRTOS_demo.h"
#include "delay.h"

void Delay_us(uint32_t us);  
//Task1特定类型的函数
void sendTask( void * arg );
void receiveTask( void * arg );
void timeTask( void * arg );

//timeTask的堆栈大小
#define timeTask_STACK_SIZE 128
//timeTask的优先级
#define timeTask_PRIORITY 3
//receiveTask的堆栈大小
#define receiveTask_STACK_SIZE 128
//receiveTask的优先级
#define receiveTask_PRIORITY 3
//sendTask的堆栈大小
#define sendTask_STACK_SIZE 128
//sendTask的优先级
#define sendTask_PRIORITY 3

TaskHandle_t sendTaskHandle = NULL;
TaskHandle_t receiveTaskHandle = NULL;
TaskHandle_t timeTaskHandle = NULL;
QueueHandle_t xQueue ;
void FreeRTOS_Start(void)
{
    xQueue = xQueueCreate(10, sizeof(uint8_t)); // Create a queue to hold 10 uint8_t items
    // Create tasks, queues, and other FreeRTOS objects here
    xTaskCreate(sendTask, "SEND", sendTask_STACK_SIZE, NULL, sendTask_PRIORITY, &sendTaskHandle);
    xTaskCreate(receiveTask, "RECEIVE", receiveTask_STACK_SIZE, NULL, receiveTask_PRIORITY, &receiveTaskHandle);
    xTaskCreate(timeTask, "TIME", timeTask_STACK_SIZE, NULL, timeTask_PRIORITY, &timeTaskHandle);
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
void sendTask( void * arg )
{
    while(1)
    {
        //使用轮询方式发送数据
        // taskENTER_CRITICAL();
        // if(len == 0)
        // {
        //     sendData[0] = 'a';
        //     len = 1;
        // }
        // taskEXIT_CRITICAL();
        //基本类型
        // uint8_t dataToSend = 10; // Example data to send
        // xQueueSend(xQueue, &dataToSend, portMAX_DELAY); // Send data to the queue
        key_value = key_scand();
        key_number key = (key_number)key_value;
       
        if (key != key_NONE) {
            
            xQueueSend(xQueue, &key, portMAX_DELAY); // Send data to the queue
        }

        vTaskDelay(10); 
    }

}
void receiveTask( void * arg )
{
    while(1)
    {
        // taskENTER_CRITICAL();
        // if(len == 1)
        // {
        //     printf("Received data: %c\n", sendData[0]);
        //     len = 0; // Reset length after processing
        // }
        // taskEXIT_CRITICAL();
        //基本类型
        // uint8_t receivedData=0;
        // BaseType_t result = xQueueReceive(xQueue, &receivedData, portMAX_DELAY); // Receive data from the queue
        // if (result == pdTRUE) {
        //     taskENTER_CRITICAL();
        //     printf("Received data: %d\n", receivedData);
        //     taskEXIT_CRITICAL();
        // }
        key_number receivedKey;
        BaseType_t result = xQueueReceive(xQueue, &receivedKey, portMAX_DELAY); // Receive data from the queue
        if (result == pdTRUE) {
            taskENTER_CRITICAL();
            switch (receivedKey) {
                case key1:
                    printf("Received key: key1\n");
                    break;
                case key2:
                    printf("Received key: key2\n");
                    break;
                case key3:
                    printf("Received key: key3\n");
                    break;
                default:
                    break;
            }
            taskEXIT_CRITICAL();
        }
    }

}
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
        printf("Task run time statistics:\n%s\n", tasktime);
        taskEXIT_CRITICAL();
        vTaskDelay(2000);
    }

}